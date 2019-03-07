#include <HardwareSerial.h>
#include <Wisol.h>


typedef struct {
  uint8_t counter;
} Payload;


HardwareSerial stream(1);
Wisol sigfox(&stream);


void setup() {
  Serial.begin(115200);
  delay(3000);
  stream.begin(9600, SERIAL_8N1, 18, 19);
}


void loop() {
  Payload p = { rand() * 100 };
  
  Serial.println(sigfox.dumpInfo());
  Serial.print("Temp: "); Serial.println(sigfox.getTemp());
  Serial.print("Send: "); Serial.println(sigfox.send(&p, sizeof(p)) ? "OK" : "ERR");
  delay(30000);
}
