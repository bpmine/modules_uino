from flask import Flask
import json
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
        self.PWR=0
        self.valid=False
        
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

                data = cln.read_input_registers(1, 2,unit=1)
                self.RSSI=data.registers[0]
                #self.PWR=data.registers[1]

                cln.write_coils(1,[True,self.cmd],unit=1)
                
                self.valid=True

            except Exception as ex:
                print('%s: %s' % (self.name,ex))
                self.valid=False

            time.sleep(0.3)
            
        cln.close()


class Rempli:
    def __init__(self,pmp,lvl):
        self.pmp=pmp
        self.lvl=lvl
        self.on=False
        self.cons=0

    def start(self,cons):
        self.cons=cons
        self.on=True

    def stop(self):
        self.on=False
        self.cons=0
        self.pmp.cmd=False

    def run(self):
        if self.on==True:
            if self.cons==1:
                self.pmp=not self.lvl.N1
            elif self.cons==2:
                self.pmp=not self.lvl.N2
            elif self.cons==3:
                self.pmp=not self.lvl.N3
            else:
                self.pmp=False                


def test_autom():
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

        if cuve.valid==True:
            if cuve.N3==False:
                pompe.cmd=True
            else:
                pompe.cmd=False
        else:
            pompe.cmd=False
            

        time.sleep(5)


pompe=WifiIoMgr('Principal','192.168.3.201')
cuve=WifiIoMgr('Reduit','192.168.3.203')
mgrs=[pompe,cuve]
        
def test_autom2():
    print('Start tache de fond');
    r=Rempli(pompe,cuve)
    r.start(3)

    while True:
        time.sleep(2)
        r.run()
        
t2 = Thread(target=test_autom2, args=[])
t2.start()

app = Flask(__name__)

@app.route('/test', methods=['GET'])
def test():
    return 'ok'


@app.route('/states', methods=['GET'])
def states():
    ret={
        'name':'jarduino'
        }

    ret['modules']=[]
    for m in mgrs:
        ret['modules'].append({
                'name':m.name,
                'cmd':m.cmd,
                'N1':m.N1,
                'N2':m.N2,
                'N3':m.N3,
                'RSSI':m.RSSI,
                'PWR':m.PWR,
                'ip':m.ip,
                'valid':m.valid
            })            

    return json.dumps(ret);


if __name__=='__main__':
    #app.debug = True
    app.run(host='0.0.0.0',port=5000,debug=False)
