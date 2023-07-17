#!/usr/bin/python3
from jard_wemos_mqtt_mgr import RdApp
import json
import redis
import time

class RdWiioClient(RdApp):
    def __init__(self,ip,port=6379):
        super(RdWiioClient,self).__init__(ip,port)

    def setOn(self,flgOn,tm=None):
        if (flgOn==True):
            self.setSleep(False)
            
        self.set_app_var_bool('on',flgOn,tm)

    def setSleep(self,flgSleep):
        self.set_app_var_bool('sleep',flgSleep,None)        

    def setCmd(self,mname,flgOn,tm=None):
        self.set_mod_var(mname,'to_cmd',1 if flgOn==True else 0,tm)

    def getLvl(self,mname):
        n1=self.get_mod_var_bool(mname,'n1')
        n2=self.get_mod_var_bool(mname,'n2')
        n3=self.get_mod_var_bool(mname,'n3')

        lvl=None
        if n1==True:
            lvl=1
        if n2==True:
            lvl=2
        if n3==True:
            lvl=3

        return lvl

    def hasModule(self,name):
        res=self.r.smembers('%s.modules' % (self.kApp()))
        if res!=None:
            for s in res:
                if name==s:
                    return True

        return False        

    def getModuleJson(self,name):
        mod={
            'name':name,
            'cmd':self.get_mod_var_bool(name,'cmd'),
            'n1':self.get_mod_var_bool(name,'n1'),
            'n2':self.get_mod_var_bool(name,'n2'),
            'n3':self.get_mod_var_bool(name,'n3'),
            'rssi':self.get_mod_var_int(name,'rssi'),
            'pwr':self.get_mod_var_int(name,'pwr'),
            'valid':self.get_mod_var_bool(name,'valid'),
            'date':self.get_mod_var(name,'date')
            }
        
        return mod
    

    def getJson(self):
        ret={
            'on': True if self.get_app_var('on')=='1' else False,
            'alive': True if self.get_app_var('alive')=='1' else False,
            'modules':{}
            }
        
        res=self.r.smembers('%s.modules' % (self.kApp()))
        for name in res:
            mod=self.getModuleJson(name)
            ret['modules'][name]=mod

        return ret            


################################

class Rempli:
    def __init__(self,cln,name,pmp,lvl_tgt,lvl_src):
        self.cln=cln
        
        self.name=name;
        self.pmp=pmp
        self.lvl_tgt=lvl_tgt
        self.lvl_src=lvl_src
        self.on=False
        
        self.cons_tgt=0
        self.limit_src=0
        self.cmd=False

    def start(self,cons_tgt,limit_src=1):
        self.cons_tgt=cons_tgt
        self.limit_src=limit_src
        self.on=True
        self.cmd=False

    def stop(self):
        self.on=False
        self.cons_tgt=0
        self.limit_src=0
        self.cmd=False

    def run(self):
        if self.on==True:

            cmd=False

            # Si le niveau de la source est donne, on arrete qd trop bas !
            src_too_low=False
            if self.lvl_src!=None:
                mod_src=self.cln.getModuleJson(self.lvl_src)
                
                if mod_src['valid']==False:
                    src_too_low=True                    
                elif self.limit_src==1:
                    src_too_low=not mod_src['n1']
                elif self.limit_src==2:
                    src_too_low=not mod_src['n2']
                elif self.limit_src==3:
                    src_too_low=not mod_src['n3']
            print('src_too_low=%s' % (src_too_low))

            # Sinon, on cherche a atteindre la consigne
            if src_too_low==False:
                if self.lvl_tgt!=None:
                    mod_dst=self.cln.getModuleJson(self.lvl_tgt)
                    
                    if mod_dst['valid']==False:
                        cmd=False
                        
                    if self.cons_tgt==1:
                        cmd=not mod_dst['n1']
                    elif self.cons_tgt==2:
                        cmd=not mod_dst['n2']
                    elif self.cons_tgt==3:
                        cmd=not mod_dst['n3']
                    else:
                        cmd=False
                else:
                    cmd=True
                    
            else:
                cmd=False

            print('%s: %s (%s -> %s)' % (self.pmp,cmd,self.cons_tgt,self.cln.getLvl(self.lvl_tgt)))

            self.cln.setCmd(self.pmp,cmd,7)

    def __str__(self):
        return ""

class RempliApp():
    def get_app_bool(self,vname):
        val=self.r.get('remplis.%s' % vname)
        if val==None or val!='1':
            return False
        else:
            return True
        
    def get_rempli_bool(self,rname,vname):
        val=self.r.get('remplis.%s' % vname)
        if val==None or val!='1':
            return False
        else:
            return True
        
    def get_rempli_int(self,rname,vname):
        val=self.r.get('remplis.%s' % vname)
        if val!=None and val.isnumeric():
            return int(val,10)
        else:
            return None

    def __init__(self,ip='192.168.3.200',port=6379):
        self.r = redis.Redis(host=ip, port=port, db=0, decode_responses=True)
        
        self.remplis={}
        
        r1=Rempli(cln,"main_reduit","main","reduit","main")
        r2=Rempli(cln,"reduit_barbec","reduit","barbec","reduit")
        r3=Rempli(cln,"main_paul","paul","paul","main")
        lst=[r1,r2,r3]        
        for r in lst:
            self.remplis[r.name]=r

        ks=self.r.keys('remplis.*')
        p=self.r.pipeline()
        for k in ks:
            p.delete(k)
        p.execute()

        p=self.r.pipeline()
        p.set('on','0')
        for _,v in self.remplis.items():
            pref='remplis.%s' % v.name

            p.set('%s.on' % (pref),'0')
            p.set('%s.cons_src' % (pref),'3')
            p.set('%s.cons_dst' % (pref),'1')
            p.set('%s.src' % (pref),v.lvl_src)
            p.set('%s.dst' % (pref),v.lvl_tgt)

        p.execute()

    def loop(self):
        on=self.get_app_bool('on')
        
        for _,v in self.remplis.items():
            pref='remplis.%s' % v.name
            
            src=self.r.get('%s.src' % pref)
            dst=self.r.get('%s.dst' % pref)
            
            ron=self.get_rempli_bool(v.name,'on')
            rcons_src=self.get_rempli_int(v.name,'cons_src')
            rcons_dst=self.get_rempli_int(v.name,'cons_dst')

            print('%s (%s->%s): %s [%s->%s]' % (v.name,src,dst,ron,rcons_src,rcons_dst))

            v.on=ron
            v.cons_tgt=rcons_dst
            v.cons_src=rcons_src
            v.run()
        
        

      
if __name__=='__main__':
    cln=RdWiioClient('192.168.3.200')
    print(cln.getJson())
    print('_'*40)

    app=RempliApp()
    app.loop()


