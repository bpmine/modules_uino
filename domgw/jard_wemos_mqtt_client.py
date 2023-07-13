from jard_wemos_mqtt_mgr import RdApp
import json
import redis

class RdWiioClient(RdApp):
    def __init__(self,ip,port=6379):
        super(RdWiioClient,self).__init__(ip,port)

    def setCmd(self,mname,flgOn):
        self.set_mod_var(n,'to_cmd',1 if flgOn==True else 0)

    def getLvl(self,mname,flgOn):
        n1=self.get_mod_var(mname,'n1')
        n2=self.get_mod_var(mname,'n2')
        n3=self.get_mod_var(mname,'n3')

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
            'cmd':True if self.get_mod_var(name,'cmd')=='1' else False,
            'n1':True if self.get_mod_var(name,'n1')=='1' else False,
            'n2':True if self.get_mod_var(name,'n2')=='1' else False,
            'n3':True if self.get_mod_var(name,'n3')=='1' else False,
            'rssi':int(self.get_mod_var(name,'rssi')),
            'pwr':int(self.get_mod_var(name,'pwr')),
            'valid':True if self.get_mod_var(name,'valid')=='1' else False
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

      
if __name__=='__main__':
    cln=RdWiioClient('192.168.3.200')
    print(cln.getJson())
    print('_'*40)
    print(cln.getModuleJson('paul'))




