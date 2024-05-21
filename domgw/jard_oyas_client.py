#!/usr/bin/python3
from jard_oyas_mqtt_mgr import RdApp
import json
import redis
import time

class RdOyasClient(RdApp):
    def __init__(self,ip,port=6379):
        super(RdOyasClient,self).__init__(ip,port)

    def setOn(self,flgOn,tm=None):
        if (flgOn==True):
            self.setSleep(False)
            
        self.set_app_var_bool('on',flgOn,tm)

    #def setSleep(self,flgSleep):
    #    self.set_app_var_bool('sleep',flgSleep,None)        

    def setCmd(self,mname,flgOn,tm=None):
        self.set_mod_var(mname,'to_cmd',1 if flgOn==True else 0,tm)

    def hasModule(self,name):
        res=self.r.smembers('%s.modules' % (self.kApp()))
        if res!=None:
            for s in res:
                if name==s:
                    return True

        return False

    def getSlaveJson(self,name,addr):
        slv={
            'addr':addr,
            'type':self.get_slave_var(name,addr,'type'),
            'comm_ok':self.get_slave_var_bool(name,addr,'comm_ok'),
            'on':self.get_slave_var_bool(name,addr,'on'),
            'cmd':self.get_slave_var_bool(name,addr,'cmd'),
            'temp':self.get_slave_var_int(name,addr,'temp'),
            'hum':self.get_slave_var_int(name,addr,'hum'),
            'voltage':self.get_slave_var_int(name,addr,'voltage'),
            'tick':self.get_slave_var_int(name,addr,'tick'),
            'total_on_s':self.get_slave_var_int(name,addr,'total_on_s'),            
            }

        if slv['type']=='oya':
            slv['low']=self.get_slave_var_bool(name,addr,'low')
            slv['high']=self.get_slave_var_bool(name,addr,'high')            
        elif slv['type']=='pump':
            slv['flow']=self.get_slave_var_int(name,addr,'flow')

        return slv

    def getModuleJson(self,name):
        mod={
            'name':name,
            'cmds':self.get_mod_var_int(name,'cmds'),
            'ctrl':self.get_mod_var_bool(name,'ctrl'),
            'ons':self.get_mod_var_int(name,'ons'),
            'comms_ok':self.get_mod_var_int(name,'comms_ok'),
            'lows':self.get_mod_var_int(name,'lows'),
            'highs':self.get_mod_var_int(name,'highs'),
            'config_slaves':self.get_mod_var_int(name,'config_slaves'),
            'date_mqtt':self.get_mod_var(name,'date_mqtt'),
            'slaves':[]
            }

        cfg=int(mod['config_slaves'])
        for addr in range(1,15):
            mask=1<<addr-1
            if (cfg&mask)==mask:
                jsSlave=self.getSlaveJson(name,addr)
                mod['slaves'].append(jsSlave)
            
        
        return mod

    def getJson(self):
        ret={
            'on': self.get_app_var_bool('on'),
            'alive': self.get_app_var_bool('alive'),
            #'sleep': self.get_app_var_bool('sleep'),
            'modules':{}
            }
        
        res=self.r.smembers('%s.modules' % (self.kApp()))
        for name in res:
            mod=self.getModuleJson(name)
            ret['modules'][name]=mod            


        return ret            

     
if __name__=='__main__':
    cln=RdOyasClient('192.168.3.200')
    print(cln.getJson())
    print('_'*40)


