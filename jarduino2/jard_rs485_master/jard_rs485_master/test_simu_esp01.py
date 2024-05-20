import serial
import time
import re
import threading
import json

PORT='COM5'
SIMU=False
   
def sendCmd(ser,jsTxt):
    msg=''
    msg+=chr(1)
    msg+=jsTxt
    msg+=chr(2)

    req=bytes(msg,'ascii')
    print('Send %s' % (req))
    ser.write(req)
    msg=ser.read_until(b'\x02')
    if msg!=b'' and len(msg)>2:
        jsTxt=msg[1:-1]
        print(jsTxt)
        return json.loads(jsTxt)
    else:
        return None

def test():
    ser= serial.Serial(port=r"\\.\%s" % PORT,stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=2)
    time.sleep(0.1)
    res=ser.read(100)
    print(res)

    sendCmd(ser,'{"req":"test"}')

    sendCmd(ser,'{"req":"master"}')

    sendCmd(ser,'{"req":"pump"}')
    
    sendCmd(ser,'{"req":"oya","addr":"2"}')

    res=sendCmd(ser,'{"req":"cmds","cmds":255,"ctrl":true}')
    print(res)
    time.sleep(4)
    res=sendCmd(ser,'{"req":"cmds","cmds":0,"ctrl":false}')
    print(res)


test()

