import serial
import time
import re
import threading

PORT='COM5'
SIMU=False

def calcCS(buff):
    cs=0
    for b in range(0,len(buff)):
        #print('%d' % ord(buff[b]))
        cs=cs+ord(buff[b])

    return cs % 256


def decodePong(msg):
    pPong=re.compile(r'\x0107y([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})\x02')
    print('Recv> %s' % msg)
    txt=str(msg,'ascii')
    m=pPong.match(txt)
    if m!=None:
        addr=int(m.group(1),16)
        val=int(m.group(2),16)
        cs=int(m.group(3),16)

        calc_cs=calcCS(txt[3:-3])
        if (calc_cs!=cs):
            print('ERREUR: CS Incorrect %02X recu au lieu de %02X attendu' % (calc_cs,cs))
            return None

        return {'addr':'%02X' % (addr),'val':'%02X' % val}

    return None    

def sendPing(ser,addr,val):
    cmd=''
    cmd+=chr(1)
    cmd+="07"
    cmd+='i'
    cmd+='%02X' % (addr)
    cmd+='%02X' % (val)
    dta=cmd[3:]
    cs=calcCS(dta)
    cmd+='%02X' % cs
    cmd+=chr(2)

    req=bytes(cmd,'ascii')
    print('Send> %s' % (req))
    ser.write(req)
    msg=ser.read_until(b'\x02')
    pong=decodePong(msg)
    print(pong)

def decodeInfo(msg):
    pCmdResp=re.compile(r'\x01([0-9A-F]{2})([po])([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{4})([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{4})([0-9A-F]{4})([0-9A-F]{0,4})([0-9A-F]{2})\x02')
    print('Recv> %s' % msg)
    txt=str(msg,'ascii')
    m=pCmdResp.match(txt)
    if m!=None:
        size=int(m.group(1),16)
        typ=m.group(2)

        obj={
            'type':typ,
            'addr':m.group(3),
            'status':m.group(4),
            'tick':int(m.group(5),16),
            'temp':int(m.group(6),16),
            'hum':int(m.group(7),16),
            'volt':(float)(int(m.group(8),16)/10),
            'time':int(m.group(9),16),
            'errs':int(m.group(10),16)
            }

        if typ=='p':
            obj['flow']=int(m.group(11),16)
            cs=int(m.group(12),16)
        elif typ=='o':
            cs=int(m.group(12),16)
        else:
            print('ERROR: Incorrect type %s' % (typ))
            return None

        calc_cs=calcCS(txt[3:-3])

        if (calc_cs!=cs):
            print('ERREUR: CS Incorrect %02X recu au lieu de %02X attendu' % (calc_cs,cs))
            return None

        return obj

    return None
    
def sendCmd(ser,cmds,addr):
    cmd=''
    cmd+=chr(1)
    cmd+="09"
    cmd+='m'
    cmd+='%04X' % (cmds)
    cmd+='%02X' % (addr)
    dta=cmd[3:]
    cs=calcCS(dta)
    cmd+='%02X' % cs
    cmd+=chr(2)

    req=bytes(cmd,'ascii')
    print('Send %s' % (req))
    ser.write(req)
    msg=ser.read_until(b'\x02')
    obj=decodeInfo(msg)
    print(obj)

def test_recv():
    ser= serial.Serial(port=r"\\.\%s" % PORT,stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=0.5)

    while True:
        res=ser.read(100)
        print(res)

    ser.close()
    
def test_one_slave():
    ser= serial.Serial(port=r"\\.\%s" % PORT,stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=0.5)

    time.sleep(2)
    res=ser.read(100)
    print(res)


    #sendPing(ser,1,254)
    sendCmd(ser,0x1,ord('S'))
    sendCmd(ser,0x1,ord('S'))
    sendCmd(ser,0x1,ord('S'))
    sendCmd(ser,0x1,ord('S'))
    sendCmd(ser,0x1,1)

    ser.close()

test_recv()
