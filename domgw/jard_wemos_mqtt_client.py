#!/usr/bin/python3
from jard_wemos_mqtt_mgr import RdApp
import json
import redis
import time

class RdWiioClient(RdApp):
    def __init__(self,ip,port=6379):
        super(RdWiioClient,self).__init__(ip,port)

    def setOn(self,flgOn,tm=None):
        self.set_app_var('on',1 if flgOn==True else 0,tm)

    def setCmd(self,mname,flgOn,tm=None):
        self.set_mod_var(mname,'to_cmd',1 if flgOn==True else 0,tm)

    def getLvl(self,mname,flgOn):
        n1=self.get_mod_var_bool(name,'n1')
        n2=self.get_mod_var_bool(name,'n2')
        n3=self.get_mod_var_bool(name,'n3')

        lvl=None
        if n1==True:
            lvl=1
        elif n2==True:
            lvl=2
        elif n3==True:
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
            'valid':self.get_mod_var_bool(name,'valid')
            }
        
        return mod
    

    def getJson(self):
        ret={
            'on': True if self.get_app_var('on')==1 else False,
            'alive': True if self.get_app_var('alive')==1 else False,
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
                mod_src=cln.getModuleJson(self.lvl_src)
                
                if mod_src['valid']==False:
                    src_too_low=True                    
                elif self.limit_src==1:
                    src_too_low=not mod_src['n1']
                elif self.limit_src==2:
                    src_too_low=not mod_src['n2']
                elif self.limit_src==3:
                    src_too_low=not mod_src['n3']

            # Sinon, on cherche a atteindre la consigne
            if src_too_low==False:
                if self.lvl_tgt!=None:
                    mod_dst=cln.getModuleJson(self.lvl_tgt)
                    
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

            self.cln.setCmd(self.pmp,cmd)

    def __str__(self):
        return ""

      
if __name__=='__main__':
    cln=RdWiioClient('192.168.3.200')
    print(cln.getJson())
    print('_'*40)
    print(cln.getModuleJson('paul'))
    print('_'*60)

    #print('Set on')
    #cln.setOn(True)
    #time.sleep(2)

    #print('Set off')
    #cln.setOn(False)
    #time.sleep(2)

    #print('Set on')
    #cln.setOn(True)

    #print('SetCmd')
    cln.setCmd('barbec',True,600)


