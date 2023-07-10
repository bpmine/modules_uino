import paho.mqtt.client as mqtt
import redis
import time
import re
import json
import datetime

TMT=5
SIMU=False

modules=set()


class RdApp:
    def __init__(self,r):
        self.r=r

    def kApp(self):
        return 'wiio'

    def kMod(self,nmod):
        return '%s.%s' % (self.kApp(),nmod)

    def kModVar(self,nmod,nvar):
        return '%s.%s' % (self.kMod(nmod),nvar)

    def set_app_var(self,nvar,val,tmt):
        k='%s.%s' % (self.kApp(),nvar)
        self.r.set(k,val)
        if tmt!=None:
            self.r.expire(k,tmt)        

    def get_app_var(self,nvar):
        k='%s.%s' % (self.kApp(),nvar)
        return self.r.get(k)

    def set_mod_var(self,nmod,nvar,val,tmt):
        k=self.kModVar(nmod,nvar)
        self.r.set(k,val)
        if tmt!=None:
            self.r.expire(k,tmt)        

    def get_mod_var(self,nmod,nvar):
        k=self.kModVar(nmod,nvar)
        return self.r.get(k)

    def del_mod_var(self,nmod,nvar):
        k=self.kModVar(nmod,nvar)
        self.r.delete(k)

        

class RdWiioSrv(RdApp):
    def __init__(self,r):
        self.r=r
        self.client = mqtt.Client()
        self.modules=set()
        
        ks=self.r.keys('%s.*' % (self.kApp()))
        p=self.r.pipeline()
        for k in ks:
            p.delete(k)
        p.execute()

        self.set_app_var('alive',1,TMT)
        self.set_app_var('on',1,TMT)

        self.r.delete('wiio.modules')

    def on_connect(self,client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        client.subscribe("/wifiio/log/#")
        client.subscribe("/wifiio/data/#")

    def update_mod_var_bool(self,name,data,key):
        global r
        if key in data:
            self.set_mod_var(name,key,1 if data[key]==True else 0,None)
        else:
            self.del_mod_var(name,key)

        
    def update_data(self,name,data):
        print(data)
        self.update_mod_var_bool(name,data,'cmd')
        self.update_mod_var_bool(name,data,'n1')
        self.update_mod_var_bool(name,data,'n2')
        self.update_mod_var_bool(name,data,'n3')
                            
        self.set_mod_var(name,'valid',1,TMT)

        now=datetime.datetime.now()
        self.set_mod_var(name,'date',now.isoformat(),None)
        
        
    pTopic=re.compile(r'/wifiio/data/([a-z]+)')
    def on_message(self,client,userdata,msg):
        print(msg.topic+" "+str(msg.payload))
        
        m=RdWiioSrv.pTopic.match(msg.topic)
        if m!=None:
            name=m.group(1)
            js=json.loads(msg.payload)
            self.update_data(name,js)
            
            if name not in modules:
                try:
                    r.sadd('%s.modules' % (self.kApp()),name)
                    modules.add(name)
                    print('Add %s' % name)
                except Exception as ex:
                    print(ex)

    def start(self):
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

        self.client.connect("192.168.3.200", 1883, 60)

        self.client.loop_start()
    
        while True:
            time.sleep(1)

            on = self.get_app_var('on')
            if on==None:
                on=False
            else:
                on=True if on==1 else False

            #if on==False:
            #    continue
            
            for n in modules:
                v=self.get_mod_var(n,'to_cmd')
                if v!=None and v==1:
                    self.client.publish("/wifiio/cmd/%s" % n,"on");
                else:
                    self.client.publish("/wifiio/cmd/%s" % n,"off");

                val=self.get_mod_var(n,'valid')
                if val==None:
                    print("Loose %s!" % n)
                    self.set_mod_var(n,'valid',0,None)



class RdWiioClient(RdApp):
    def __init__(self,r):
        self.r=r

    def setCmd(self,mname,flgOn):
        self.set_mod_var(n,'to_cmd',1 if flgOn==True else 0,TMT)

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

    def getJson(self):
        ret={
            'on': True if self.get_app_var('on')==1 else False,
            'alive': True if self.get_app_var('alive')==1 else False,
            'modules':{}
            }
        
        res=r.smembers('%s.modules' % (self.kApp()))
        for name in res:
            mod={
                'name':name,
                'cmd':self.get_mod_var(name,'cmd'),
                'n1':self.get_mod_var(name,'n1'),
                'n2':self.get_mod_var(name,'n2'),
                'n3':self.get_mod_var(name,'n3'),
                'valid':self.get_mod_var(name,'valid')
                }
            ret['modules'][name]=mod

        return ret            
        

r = redis.Redis(host='192.168.3.200', port=6379, db=0)
srv=RdWiioSrv(r)
srv.start()

#cln=RdWiioClient(r)
#print(cln.getJson())




