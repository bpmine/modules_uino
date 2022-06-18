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

def test_coils(cln):
    result= client.read_coils(0,13,unit= 4)
    for o in result.bits:
        print(o)

    result= client.write_coils(0,[True]*13,unit= 4)
    print(result)

    result= client.read_multiple_coils(0,13,unit= 4)
    for o in result.bits:
        print(o)

def test_registers(cln):
    
  
    print('Read Input Registers:',end='')
    result= cln.read_input_registers(0,5,unit= 4)
    assert len(result.registers)==5
    assert result.registers==[0xabcd,0xef00,0,0,0xacdc]
    print('[OK]')

    print('Write Holding Registers:',end='')
    result= cln.write_registers(0,[0xabcd,0,0,0,0xacdc],unit= 4)
    assert result.function_code==16
    print('[OK]')

    print('Read Holding Registers:',end='')
    result= cln.read_holding_registers(0,5,unit= 4)
    assert len(result.registers)==5
    assert result.registers==[0xabcd,0,0,0,0xacdc]
    print('[OK]')

    print('Write Holding Registers:',end='')
    result= cln.write_registers(0,[0,0xFFFF,0xAAAA,0,0],unit= 4)
    assert result.function_code==16
    print('[OK]')

    print('Read Holding Registers:',end='')
    result= cln.read_holding_registers(0,5,unit= 4)
    assert len(result.registers)==5
    assert result.registers==[0,0xFFFF,0xAAAA,0,0]
    print('[OK]')


def test_date_time(cln):
    
    print('Read Holding Registers:',end='')
    result= cln.read_holding_registers(0,6,unit= 4)
    assert len(result.registers)==6
    
    print('[OK]')
    print('%02d/%02d/%04d' % (
        int(result.registers[2]),
        int(result.registers[1]),
        int(result.registers[0])
        ))

    print('%02d:%02d:%02d' % (
        int(result.registers[3]),
        int(result.registers[4]),
        int(result.registers[5])
        ))
    
    
def test_jard_coils(cln):
    
    print('Read Holding Registers:',end='')
    result= cln.read_holding_registers(0,6,unit= 4)
    assert len(result.registers)==6
    
    print('[OK]')
    print('%02d/%02d/%04d' % (
        int(result.registers[2]),
        int(result.registers[1]),
        int(result.registers[0])
        ))

    print('%02d:%02d:%02d' % (
        int(result.registers[3]),
        int(result.registers[4]),
        int(result.registers[5])
        ))
    
    print('Read Coil status:',end='')
    result= cln.read_coils(10,3,unit= 4)
    print('[OK]')
    print(result.bits)

    print('Write Coil status:',end='')
    result= cln.write_coils(10,[True],unit= 4)
    print('[OK]')

    print('Read Coil status:',end='')
    result= cln.read_coils(10,3,unit= 4)
    print('[OK]')
    print(result.bits)

def test_readTempBatt(cln):
    print('Read Input Registers:',end='')
    result= cln.read_input_registers(0,4,unit= 4)
    assert len(result.registers)==4
    
    print('[OK]')
    print('Batt : %.1f V' % (result.registers[0]/10))
    print('Sun  : %.1f V' % (result.registers[1]/10))
    print('Temp : %d°C' % result.registers[2])
    print('Hum  : %d%%' % result.registers[3])

def test_enable_pompes(cln):
    print('Write Coil status:',end='')
    result= cln.write_coils(11,[True],unit= 4)
    print('[OK]')

    print('Write Coil status:',end='')
    result= cln.write_coils(21,[True],unit= 4)
    print('[OK]')


def test_statistics(cln):

    print('Read Input Registers / Statistics:',end='')
    result= cln.read_input_registers(100,6,unit= 4)
    assert len(result.registers)==6    
    print('[OK]')    
    print('Boots    : %d' % (result.registers[0]))
    print('Pompe  1 : %d H' % (result.registers[1]))
    print('Pompe  2 : %d H' % (result.registers[2]))
    print('Soleil   : %d H' % (result.registers[3]))
    print('Bouton 1 : %d' % (result.registers[4]))
    print('Bouton 2 : %d' % (result.registers[5]))

 

def test_schedulers(cln):

    print('Write Holding Registers / Scheduler:',end='')
    result= cln.write_registers(10,[17,00,5,255],unit= 4)
    print('[OK]')

    print('Read Holding Registers / Scheduler:',end='')
    result= cln.read_holding_registers(10,4,unit= 4)
    assert len(result.registers)==4    
    print('[OK]')    
    print(result.registers)

    print('Write Coil status:',end='')
    result= cln.write_coils(11,[True,True],unit= 4)
    print('[OK]')    

    print('Write Coil status:',end='')
    result= cln.write_coils(10,[True],unit= 4)
    print('[OK]')


def config_serial_version(cln):
    print('Read Holding Registers / Version and serial:',end='')
    result= cln.read_input_registers(200,3,unit= 4)
    assert len(result.registers)==3    
    print('[OK]')
    
    print('Version MODBUS : %d' % (result.registers[0]) );
    print('Serial number  : %d' % (result.registers[1]) );
    print('Soft version   : %d' % (result.registers[2]) );


    ser=int(input('Serial>'))
	
    print('Write Holding Registers / Serial:',end='')
    result= cln.write_registers(201,[ser],unit= 4)
    print('[OK]')



def config_jarduino(cln):
    print('Read Coil status:',end='')
    result= cln.read_coils(20,3,unit= 4)
    print('[OK]')
    print(result.bits)    

    print('Read Input Registers / Version and serial:',end='')
    result= cln.read_input_registers(200,3,unit= 4)
    assert len(result.registers)==3   
    print('[OK]')    

    print('Version MODBUS : %d' % (result.registers[0]) );
    print('Serial number  : %d' % (result.registers[1]) );
    print('Soft version   : %d' % (result.registers[2]) );
##    print('Date           : %02d/%02d/%04d' % (
##        result.registers[3],
##        result.registers[4],
##        result.registers[5]        
##        ));

    print('Read Holding Registers / Heure:',end='')
    result= cln.read_holding_registers(0,6,unit= 4)
    assert len(result.registers)==6    
    print('[OK]')
    
    print('Date/Heure:')
    print('  - %02d/%02d/%04d' % (
        int(result.registers[2]),
        int(result.registers[1]),
        int(result.registers[0])
        ))

    print('  - %02d:%02d:%02d' % (
        int(result.registers[3]),
        int(result.registers[4]),
        int(result.registers[5])
        ))

    test_readTempBatt(cln)


    print('Write Holding Registers / Scheduler:',end='')
    result= cln.write_registers(10,[19,00,60,255],unit= 4)
    print('[OK]')

    print('Read Holding Registers / Scheduler:',end='')
    result= cln.read_holding_registers(10,4,unit= 4)
    assert len(result.registers)==4    
    print('[OK]')    
    print(result.registers)

    print('Write Coil status / Pompe 1 activee auto:',end='')
    result= cln.write_coils(10,[False,True,True],unit= 4)
    print('[OK]')

    print('Read Coil status:',end='')
    result= cln.read_coils(10,3,unit= 4)
    print('[OK]')
    print(result.bits)    

    print('Write Holding Registers / Scheduler:',end='')
    result= cln.write_registers(20,[19,00,60,255],unit= 4)
    print('[OK]')

    print('Read Holding Registers / Scheduler:',end='')
    result= cln.read_holding_registers(10,4,unit= 4)
    assert len(result.registers)==4    
    print('[OK]')    
    print(result.registers)

    print('Write Coil status / Pompe 2 desactivee:',end='')
    result= cln.write_coils(20,[False,True,True],unit= 4)
    print('[OK]')

    print('Read Coil status:',end='')
    result= cln.read_coils(20,3,unit= 4)
    print('[OK]')
    print(result.bits)    
    

client= ModbusClient(method = "rtu", port=r"\\.\COM13",stopbits = 1, bytesize = 8, parity='N',baudrate= 9600,timeout=2)

print('Connection:',end='')
res = client.connect()
if res==True:
    print('[OK]')
else:
    print('[NOK]')



time.sleep(4);

#test_registers(client)

#test_enable_pompes(client)  
#test_statistics(client)
#config_serial_version(client)
config_jarduino(client)

client.close()
