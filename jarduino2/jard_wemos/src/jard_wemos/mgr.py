import pymodbus
from pymodbus.client.sync import ModbusTcpClient as ModbusClient
import time
from threading import Thread

class WifiIoMgr:
    def __init__(self,name,ip,port=502):
        self.name=name
        self.ip=ip
        self.port=port

        self.cmd=False
        self.N1=False
        self.N2=False
        self.N3=False
        self.RSSI=0
        self.Valid=False
        
        self.t = Thread(target=self.loop, args=[])
        self.t.start()

    def loop(self):
        print('Start %s...' % (self.name) )
        cln = ModbusClient(host=self.ip, port=self.port, auto_open=True, auto_close=True, timeout=10)
        while True:
            try:
                data = cln.read_coils(1, 2,unit=1)
                cmd=data.bits[1]

                data = cln.read_discrete_inputs(1, 3,unit=1)
                self.N1=data.bits[0]
                self.N2=data.bits[1]
                self.N3=data.bits[2]

                data = cln.read_input_registers(1, 1,unit=1)
                self.RSSI=data.registers[0]

                cln.write_coils(1,[True,self.cmd],unit=1)
                
                self.Valid=True

            except Exception as ex:
                print('%s: %s' % (self.name,ex))
                self.Valid=False

            time.sleep(0.3)
            
        cln.close()
                

 
pompe=WifiIoMgr('pompe','192.168.3.201')
cuve=WifiIoMgr('cuve','192.168.3.202')

mgrs=[pompe,cuve]


while True:
    print('_'*60)
    for mgr in mgrs:
        print('%s:' % (mgr.name) )
        print('  - RSSI    : %d' % (mgr.RSSI))
        print('  - Niveaux : %d %d %d' % (mgr.N1,mgr.N2,mgr.N3))
        print('  - Cmd     : %d' % (mgr.cmd))        

    if cuve.Valid==True:
        if cuve.N3==False:
            pompe.cmd=True
        else:
            pompe.cmd=False
    else:
        pompe.cmd=False
        

    time.sleep(5)
        
