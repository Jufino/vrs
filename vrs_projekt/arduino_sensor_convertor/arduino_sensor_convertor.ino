#include <OneWire.h>
OneWire  ds(4);

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  float celsius;

  byte addr[] = {0x28, 0x5E, 0x02, 0xF5, 0x00, 0x00, 0x00, 0x50};

  ds.reset();
  ds.select(addr);
  ds.write(0x44);

  delay(1000);
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);  

  for ( i = 0; i < 9; i++) {  
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7; 
    else if (cfg == 0x20) raw = raw & ~3; 
    else if (cfg == 0x40) raw = raw & ~1; 
  }
  celsius = (float)raw / 16.0;
  Serial.print(celsius);
  Serial.print("\r");
}

