#include <Key.h>
#include <Keypad.h>
#include <IRremote.hpp>
#define TVONOFF 0xF708FB04
#define BOXONOFF 0xBA45FF00
#define IRTRANSMITTER  3
#define IRRECEIVER  11
#define ASIIGNBUTTON_PIN 13
#define SENDBUTTON_PIN 12
#define RED_RGB 14
#define GREEN_RGB 15
#define BLUE_RGB 16

enum {
  K0 = 0,
  K1 = 1,
  K2 = 2,
  K3 = 3,
  K4 = 4,
  K5 = 5,
  K6 = 6,
  K7 = 7,
  K8 = 8,
  K9 = 9,
  KA = 10,
  KB = 11,
  KC = 12,
  KD = 13,
  KHASHTAG = 14,
  KSTAR = 15
};


uint32_t signals[16];

const byte rows = 4; //four rows
const byte cols = 4; //four columns
bool assignmode = false;
uint32_t signal = 0;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] = {10, 9, 8, 6};
byte colPins[cols] = {5, 4, 2, 1};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
char lastkeypressed;

void setup() {
  Serial.begin(9600);
  pinMode(RED_RGB, OUTPUT);
  pinMode(GREEN_RGB, OUTPUT);
  pinMode(BLUE_RGB, OUTPUT);
  pinMode(ASIIGNBUTTON_PIN, INPUT);
  pinMode(SENDBUTTON_PIN, INPUT);
  IrReceiver.begin(IRRECEIVER);
  IrSender.begin(IRTRANSMITTER);
  delay(100);
  Serial.println("Script Started!");
  digitalWrite(GREEN_RGB, HIGH);
  digitalWrite(RED_RGB, LOW);
  Serial.println("SEND");
}

void loop() {
  char rn = keypad.getKey();
  if (rn) {
    if (assignmode) {
      Serial.print("ASSIGN-Button: ");
    } else {
      Serial.print("SEND-Button: ");
    }
    Serial.println(rn);
    lastkeypressed = rn;
  }
  if (rn == '*') {
    assignmode = true;
    digitalWrite(RED_RGB, HIGH);
    digitalWrite(GREEN_RGB, LOW);
    Serial.println("ASSIGN");
    delay(500);
  }
  if (rn == '#') {    
    assignmode = false;
    digitalWrite(GREEN_RGB, HIGH);
    digitalWrite(RED_RGB, LOW);
    Serial.println("SEND");
    delay(500);
  }
  if (lastkeypressed) {
    if (assignmode == false) {
      Serial.print("SEND-Signal: ");
      switch (lastkeypressed) {
        case '0':
          IrSender.sendNECRaw(signals[K0], 1);
          Serial.println(signals[K0], HEX);
          break;
        case '1':
          IrSender.sendNECRaw(signals[K1], 1);
          Serial.println(signals[K1], HEX);
          break;
        case '2':
          IrSender.sendNECRaw(signals[K2], 1);
          Serial.println(signals[K2], HEX);
          break;
        case '3':
          IrSender.sendNECRaw(signals[K3], 1);
          Serial.println(signals[K3], HEX);
          break;
        case '4':
          IrSender.sendNECRaw(signals[K4], 1);
          Serial.println(signals[K4], HEX);
          break;
        case '5':
          IrSender.sendNECRaw(signals[K5], 1);
          Serial.println(signals[K5], HEX);
          break;
        case '6':
          IrSender.sendNECRaw(signals[K6], 1);
          Serial.println(signals[K6], HEX);
          break;
        case '7':
          IrSender.sendNECRaw(signals[K7], 1);
          Serial.println(signals[K7], HEX);
          break;
        case '8':
          IrSender.sendNECRaw(signals[K8], 1);
          Serial.println(signals[K8], HEX);
          break;
        case '9':
          IrSender.sendNECRaw(signals[K9], 1);
          Serial.println(signals[K9], HEX);
          break;
        case 'A':
          IrSender.sendNECRaw(signals[KA], 1);
          break;
        case 'B':
          IrSender.sendNECRaw(signals[KB], 1);
          break;
        case 'C':
          IrSender.sendNECRaw(signals[KC], 1);
          break;
        case 'D':
          IrSender.sendNECRaw(signals[KD], 1);
          break;
        default:
          break;
      }
      delay(100);
      lastkeypressed = 0;
    } else {
      if (IrReceiver.decode()) {
        signal = IrReceiver.decodedIRData.decodedRawData;
        switch (lastkeypressed) {
          case '0':
            signals[K0] = signal;
            break;
          case '1':
            signals[K1] = signal;
            break;
          case '2':
            signals[K2] = signal;
            break;
          case '3':
            signals[K3] = signal;
            break;
          case '4':
            signals[K4] = signal;
            break;
          case '5':
            signals[K5] = signal;
            break;
          case '6':
            signals[K6] = signal;
            break;
          case '7':
            signals[K7] = signal;
            break;
          case '8':
            signals[K8] = signal;
            break;
          case '9':
            signals[K9] = signal;
            break;
          case 'A':
            signals[KA] = signal;
            break;
          case 'B':
            signals[KB] = signal;
            break;
          case 'C':
            signals[KC] = signal;
            break;
          case 'D':
            signals[KD] = signal;
            break;
          default:
            break;
        }
        delay(100);
        Serial.print("ASSIGN-Signal: ");
        Serial.println(signal, HEX);
        lastkeypressed = 0;
        IrReceiver.resume();
      }
    }
  }
}

