import pymodbus
import serial
from pymodbus.pdu import ModbusRequest
from pymodbus.client.sync import ModbusSerialClient as ModbusClient #initialize a serial RTU client instance
from pymodbus.transaction import ModbusRtuFramer
import time

import logging
logging.basicConfig()
log = logging.getLogger()
#log.setLevel(logging.DEBUG)

def test_slave_rs485_set_address(cln,addr,newAddr):
    print('Lecture etat electrovanne:')
    result= client.read_coils(0,1,unit= addr)
    print(result.bits[0])

    client.write_registers(0,[newAddr],unit= addr)


def test_slave_rs485(cln,addr=0):
    print('Lecture etat electrovanne. Doit etre OFF.')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==False)

    print('OUVERTURE ELECTROVANNE')
    result= client.write_coils(0,[True],unit= addr)
    
    print('Lecture etat electrovanne. Doit etre ON.')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==True)

    print('Lecture etat des niveaux. Doivent etre 0.')
    result= client.read_discrete_inputs(0,2,unit= addr)
    assert(result.bits[0]==False)
    assert(result.bits[1]==False)

    print('Attente...')
    time.sleep(5)
    
    print('FERMETURE ELECTROVANNE')
    result= client.write_coils(0,[False],unit= addr)
    
    print('Lecture etat electrovanne. Doit etre OFF.')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==False)   
    
def test_slave_rs485_wdg(cln,addr=0):
    print('Lecture etat electrovanne. Doit etre OFF.')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==False)

    print('Entretien de la comm avec vanne ON...')
    for i in range(0,10):
        result= client.write_coils(0,[True],unit= addr)
        print('.',end='')
        time.sleep(0.8)
        result= client.read_coils(0,1,unit= addr)
        assert(result.bits[0]==True)
    print()

    print('Quelques lectures toujours on mais plus de write coil !')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==True)
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==True)

    print('Attente...')
    time.sleep(3)
    print('La vanne doit maintenant être fermée')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==False)

    
def test_slave_rs485_asserv(cln,addr=0):
    print('Lecture etat electrovanne. Doit etre OFF.')
    result= client.read_coils(0,1,unit= addr)
    assert(result.bits[0]==False)

    while (True):
        result= client.read_discrete_inputs(0,2,unit= addr)
        level_low=result.bits[0]
        level_high=result.bits[1]
        print('Status %d %d' % (level_low,level_high) )

        if (level_low==False and level_high==False):
            client.write_coils(0,[True],unit= addr)
        elif (level_low==True and level_high==False):
            client.write_coils(0,[True],unit= addr)
        elif level_high==True:
            client.write_coils(0,[False],unit= addr)
            #break
        
    

client= ModbusClient(method = "rtu", port=r"\\.\COM14",stopbits = 1, bytesize = 8, parity='N',baudrate= 115200,timeout=2)

print('Connection:',end='')
res = client.connect()
if res==True:
    print('[OK]')
else:
    print('[NOK]')


time.sleep(4);

#test_slave_rs485_set_address(client,0,5)
#test_slave_rs485_wdg(client,5)
test_slave_rs485_asserv(client,5)


client.close()
