#include <IRremote.hpp>

#define TVONOFF 0xF708FB04
#define BOXONOFF 0xBA45FF00

const int IRRECEIVE  = 11;
const int IRTRANSMIT = 3;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IRRECEIVE, ENABLE_LED_FEEDBACK); // Start the receiver
  IrSender.begin(IRTRANSMIT);
}

void loop() {
  IrSender.sendNECRaw(TVONOFF, 1);
  if (IrReceiver.decode()) {
      IrReceiver.printIRResultShort(&Serial);
      delay(1000);
      IrReceiver.resume();
  }
}

