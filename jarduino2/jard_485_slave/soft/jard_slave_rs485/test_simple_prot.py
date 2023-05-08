import serial
import time
import re

def calcCS(buff):
    cs=0
    for b in range(1,len(buff)):
        cs=cs+ord(buff[b])

    return cs

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
        self.comm=False

class SlvPmp(Slv):
    def __init__(self,addr):
        super().__init__(addr,'1')
        print(self.addr)
        self.flow=None
        self.cmd=False

    def __str__(self):
        if self.comm==False:
            return 'Pompe %c: OFF' % (self.addr)
        else:
            return 'Pompe %c: cmd=%s flow=%s' % (self.addr,self.cmd,self.flow) 
        

class SlvOya(Slv):
    def __init__(self,addr):
        super().__init__(addr,'2')
        print(self.addr)
        self.levels=None
        self.cmd=False

    def __str__(self):
        if self.comm==False:
            return 'Pompe %c: OFF' % (self.addr)
        else:
            return 'Oya %c: cmd=%s Niv=%s' % (self.addr,self.cmd,self.levels) 


slaves=[
    SlvPmp('A'),
    SlvOya('B')
    ]


ser= serial.Serial(port=r"\\.\COM12",stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=0.5)
txt=ser.read_until('\n')
print(txt)
ser.flush()

while True:
    slaves[0].cmd=True
    slaves[1].cmd=True
    
    for s in slaves:    
        res=sendRequest(ser,s.addr,s.fct,s.cmd)
        if res!=None:
            s.comm=True
            
            if s.fct=='2':
                s.levels=res
            elif s.fct=='1':
                s.flow=res

        else:
            s.comm=False
        
        print(s)

    time.sleep(0.05)

ser.close()
