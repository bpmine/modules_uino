import serial
import time
import re
import threading

SIMU=False

def calcCS(buff):
    cs=0
    for b in range(1,len(buff)):
        #print('%d' % ord(buff[b]))
        cs=cs+ord(buff[b])

    return cs % 256

pResp=re.compile(r'\x01(.)(.)(.+)([0-9A-F]{2})\x02')
def decodeResp(txt):
    m=pResp.match(txt)
    if m!=None:
        addr=m.group(1)
        fct=m.group(2)
        datas=m.group(3)
        cs=m.group(4)

        return (addr,fct,datas)

    return None

def sendRequest(ser,addr,fct,val):
    cmd=''
    cmd+=chr(1)
    cmd+=addr
    cmd+=fct
    cmd+='%02X' % val
    cs=calcCS(cmd)
    cmd+='%02X' % cs
    cmd+=chr(2)

    req=bytes(cmd,'ascii')
    #print('Send %s' % (req))
    ser.write(req)
    resp=ser.read_until(chr(2))
    res=decodeResp(str(resp,'ascii'))
    if res!=None:
        (raddr,rfct,datas)=res
        if (raddr==addr) and (rfct==fct):
            return datas
    
    return None

class Slv:
    def __init__(self,addr,fct):
        self.addr=addr
        self.fct=fct
        self.enabled=None
        self.comm=False

pRespPmp=re.compile('^([0-9A-F]{2})([0-9A-F]{4})([0-9A-F]{2})([0-9A-F]{2}).*')
class SlvPmp(Slv):
    def __init__(self,addr):
        super().__init__(addr,'1')
        print('Pompe %s' % (self.addr))
        self.flow=None
        self.temp=None
        self.hum=None
        self.cmd=False
        self.st=None

    def parse_resp(self,resp):
        m=pRespPmp.match(resp)
        if m!=None:
            self.st=int(m.group(1),16)
            self.enable=True if (self.st&0x08==0x08) else False
            self.flow=int(m.group(2),16)
            self.temp=int(m.group(3),16)
            self.hum=int(m.group(4),16)
        else:
            self.st=None
            self.enable=None
            self.flow=None
            self.temp=None
            self.hum=None

    def __str__(self):
        if self.comm==False:
            return 'Pompe %c: OFF' % (self.addr)
        else:
            return 'Pompe %c: st=%02x cmd=%s flow=%sL/H temp=%s°C hum=%s%%' % (self.addr,self.st if self.st!=None else 0,self.cmd,self.flow,self.temp,self.hum) 
        

pRespOya=re.compile('^([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2}).*')
class SlvOya(Slv):
    def __init__(self,addr):
        super().__init__(addr,'2')
        print('Oya %s' % (self.addr))
        self.cmd=False
        self.simu_level=0
        self.high=False
        self.low=False
        self.st=None

    def parse_resp(self,resp):
        m=pRespOya.match(resp)
        if m!=None:
            self.st=int(m.group(1),16)
            self.enable=True if (self.st&0x08==0x08) else False
            self.temp=int(m.group(2),16)
            self.hum=int(m.group(3),16)
            self.low=True if self.st&0x01==0x01 else False
            self.high=True if self.st&0x02==0x02 else False
        else:
            self.st=None
            self.enable=None
            self.flow=None
            self.temp=None
            self.hum=None

    def __str__(self):
        if self.comm==False:
            return 'Oya %c: OFF' % (self.addr)
        else:
            if SIMU==True:
                simu_str="%s" %(self.simu_level)
            else:
                simu_str=""
                
            return 'Oya %c: st=%02x cmd=%s Low=%s High=%s Temp=%s Hum=%s %s' % (self.addr,self.st if self.st!=None else 0,self.cmd,self.low,self.high,self.temp,self.hum,simu_str) 


pompe=SlvPmp('A')

oyas=[
    SlvOya('B'),
    SlvOya('C')
    ]

if SIMU==False:
    ser= serial.Serial(port=r"\\.\COM10",stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=0.2)
    txt=ser.read_until('\n')
    print(txt)
    ser.flush()
    
quit=False

pompe.cmd=False
cyclesMiniOneOyaOn=0
cyclesMiniPompeOff=0

def thread_func():
    while quit==False:
        if SIMU==0:
            try:
                res=sendRequest(ser,pompe.addr,pompe.fct,1 if pompe.cmd==True else 0)
                if res!=None:
                    pompe.parse_resp(res)

                    pompe.comm=True
                else:
                    pompe.comm=False

                for s in oyas:
                    if s.comm==True and s.low==False and s.high==False:
                        s.cmd=False

                oneoya=False
                oneneed=False
                for s in oyas:
                    res=sendRequest(ser,s.addr,s.fct,1 if s.cmd==True else 0)
                    if res!=None:                        
                        s.parse_resp(res)
                        if s.st!=None and s.st&0x04==0x04:                            
                            if s.low==True or s.high==True:
                                oneneed=True
                                oneoya=True
                        
                        s.comm=True
                        
                    else:
                        s.comm=False

                if oneoya==False:
                    pompe.cmd=False
                else:
                    if oneneed==True:
                        pompe.cmd=True
                    else:
                        pompe.cmd=False
                
                time.sleep(0.05)
            except Exception as ex:
                print('Err comm: %s'%(ex))
                pass
        else:
            pompe.comm=True
            for s in oyas:
                s.comm=True
                if s.cmd==True and pompe.cmd==True:
                    s.simu_level+=1
                    if s.simu_level>12:
                        s.simu_level=12
                else:
                    s.simu_level-=1
                    if s.simu_level<0:
                        s.simu_level=0

                s.low=True if s.simu_level>4 else False
                s.high=True if s.simu_level>10 else False
                    
            time.sleep(1)

    ser.close()

time.sleep(2)
res=sendRequest(ser,pompe.addr,'1',0)
print(res)
res=sendRequest(ser,pompe.addr,'e',1)
print(res)

res=sendRequest(ser,'C','1',0)
print(res)
res=sendRequest(ser,'C','e',1)
print(res)

res=sendRequest(ser,'B','1',0)
print(res)
res=sendRequest(ser,'B','e',1)
print(res)

t=threading.Thread(target=thread_func)
t.start()

while True:
    print('_'*60)
    print('1) Statut')
    print('2) Pompe 1')
    print('3) Oya 1')
    print('4) Oya 2')
    print('0) Quitter')

    res=input('>')
    if res=='1':
        print(pompe)
        for s in oyas:
            print(s)
    elif res=='2':
        oy=False
        for s in oyas:
            if s.cmd==True:
                oy=True
                break

        if pompe.cmd==True:
            pompe.cmd=False
        elif oy==True:
        #else:
            pompe.cmd=True
            
        print('Pompe: %s' %pompe.cmd)
    elif res=='3':
        if len(oyas)>0:
            if oyas[0].cmd==False:
                oyas[0].cmd=True
            else:
                oyas[0].cmd=False
                
            print('Oya 1: %s' % (oyas[0].cmd))
            
        oy=False
        for s in oyas:
            if s.cmd==True:
                oy=True
                break
            
        if oy==False:
            pompe.cmd=False
    elif res=='4':
        if len(oyas)>1:
            if oyas[1].cmd==False:
                oyas[1].cmd=True
            else:
                oyas[1].cmd=False
                
            print('Oya 2: %s' % (oyas[0].cmd))
            
        oy=False
        for s in oyas:
            if s.cmd==True:
                oy=True
                break
            
        if oy==False:
            pompe.cmd=False
    elif res=='0':
        quit=True
        print('Arrêt en cours')
        time.sleep(2)
        exit(0)
    

