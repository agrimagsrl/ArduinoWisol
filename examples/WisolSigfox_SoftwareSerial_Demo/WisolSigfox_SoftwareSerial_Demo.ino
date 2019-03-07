#include <SoftwareSerial.h>
#include <Wisol.h>


typedef struct {
  uint8_t counter;
} Payload;


SoftwareSerial stream(10, 11);
Wisol sigfox(&stream);


void setup() {
  Serial.begin(115200);
  delay(3000);
  stream.begin(9600);
}


void loop() {
  Payload p = { rand() * 100 };
  
  Serial.println(sigfox.dumpInfo());
  Serial.print("Temp: "); Serial.println(sigfox.getTemp());
  Serial.print("Send: "); Serial.println(sigfox.send(&p, sizeof(p)) ? "OK" : "ERR");
  delay(30000);
}