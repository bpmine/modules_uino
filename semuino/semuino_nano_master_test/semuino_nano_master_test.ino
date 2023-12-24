#include <Wire.h>


void write_ctrl(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(1));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void write_level(unsigned char level)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(2));
  Wire.write(byte(level));
  Wire.endTransmission();   
}


void write_modeA(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(3));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void write_modeB(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(4));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

unsigned char do_alive(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(20));
  Wire.write(byte(val));
  Wire.endTransmission();  
  Wire.requestFrom(0xA, 1); 
  if (1 <= Wire.available())
    return Wire.read();
  else
    return 0;
}

void test_read_value(const char *name,unsigned char reg)
{
  Serial.print(name);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(reg));
  Wire.endTransmission();    
  Wire.requestFrom(0xA, 1);

  if (1 <= Wire.available())
  {
    Serial.println(Wire.read());
  }  
}

unsigned char read_eep(unsigned char addr)
{
  Serial.print("Read EEP @");
  Serial.print(addr);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(16));
  Wire.write(byte(addr));
  Wire.endTransmission();    
  Wire.requestFrom(0xA, 1);

  if (1 <= Wire.available())
  {
    return Wire.read();
  }  
  else
  {
    return 0;
  }
}

void write_eep(unsigned char addr,unsigned char val)
{
  Serial.print("Write EEP @");
  Serial.print(addr);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(5));
  Wire.write(byte(addr));
  Wire.write(byte(val));
  Wire.endTransmission();    
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(10000);

  Serial.println("Commandes à 0");
  write_ctrl(0);
  
  delay(1000);

  test_read_value("Read temp",11);
  test_read_value("Read hum",12);
  test_read_value("Read h1",13);
  test_read_value("Read h2",14);
  test_read_value("Read h3",15);
  
  //write_eep(8,0xDD);
  Serial.println(read_eep(8),HEX);
}

void loop() 
{
  delay(100);
  
  Serial.println("Commande P1 à ON");
  write_ctrl(0x10);    
  delay(2000);
  
  Serial.print("Alive:");
  Serial.println(do_alive(0x55),HEX);
  
  Serial.println("Commande P1 à OFF");
  write_ctrl(0x00);  
  delay(1000);

  Serial.print("Alive:");
  Serial.println(do_alive(0xAA),HEX);

  Serial.println("Commande RGB1 et 5V à ON");
  write_ctrl(0x0F);
  delay(4000);

  Serial.print("Alive:");
  Serial.println(do_alive(0x01),HEX);

  Serial.println("Commande RGB1 à OFF");
  write_ctrl(0x01);
  delay(4000);

  Serial.print("Alive:");
  Serial.println(do_alive(0x10),HEX);

  Serial.println("Commande 5V à OFF");
  write_ctrl(0x00);
}
