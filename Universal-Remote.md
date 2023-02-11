# Universal Remot (Infrared-Signals-Cloner)

## the Idea

The actuall idea of this project was cloning Remotes. You can clone TV-Remotes, but also Music-Remotes or anything else that uses Infrared-Signals. The idea is to use a Keypad to send Infrared-Signals. The Keypad has 16 buttons, so it's possible to clone 16 different devices. The Infrared-Signals are stored in an array. The array is 16 long, because the Keypad has 16 buttons (0-9, A-D). These signals can be easily assigned, by switching into assign-mode. There are two modes: assign-/assining-mode and send-/sending-mode. When the Assign-Button is pressed, the LED turns red (to show it's in assign-mode), when the sending-button is clicked, it turns blue. The default mode is sending-mode. When the remote is in sending-mode, it will send the Infrared-Signal that is stored in the array at the index of the button that is pressed. When the remote is in assigning-mode, it will receive the Infrared-Signal and store it in the array at the index of the button that was pressed before. (So you have to press the button that you want to assign the signal to, and then press the button of the device that you want to clone. The signal will be stored in the array at the index of the button that you pressed before. So if you press the button "1" and then press the button of the device that you want to clone, the signal will be stored in the array at a certain index, depending which button was pressed (0-9, A-D).) Duriong the time, the signals is being sent/waited for to be assigned, there is a little green in the LED-color.
(An unnecesarry thing: The * and # cant be asigned to, so why KHASHTAG and KSTAR and why length 16 of List?)

## the Code

### includes/definitions

These includes are important.

- using the Keypad to make a remote with maximum 16 Buttons more easily. (Keypad.h)
- using the Infrared-Transmitter/Infrared-Receiver more easily. (IRremote.hpp)

```c++
#include <Key.h>
#include <Keypad.h>
#include <IRremote.hpp>
```

These definitions are only used to test the code. They're just some Infrared-Signals as hex of devices that I can test on, so they're not actually necessary for the final product.

```c++
#define TVONOFF 0xF708FB04
#define BOXONOFF 0xBA45FF00
```

These definitions are used to define the pins of the Infrared-Transmitter and Infrared-Receiver. The RGB-LEDs are used to show the current state of the remote (Assigning-Mode/Transmitting-Mode).
These definitions are not necessary, but important to make the code more readable.

```c++
#define IRTRANSMITTER 5
#define IRRECEIVER 6
#define RED_RGB 9
#define GREEN_RGB 10
#define BLUE_RGB 11
```

These definitions are the indeces of the signals-array (that stores all the Infrared-Signals as hex). They're used to assign/trasnmit the signals to the buttons.
These definitions are not necessary, but important to make the code more readable.
(It's signal[Kx], not signal[i] (x being a character from the Keypad; i being the index), so)

```c++
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
```

These consts are used to define the count of rows and columns of the Keypad.

```c++
const byte rows = 4;
const byte cols = 4;
```

This 2d-array is used to define the buttons of the Keypad. The first 2d-array is used to define the buttons. And their symbol.

```c++
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
```

This array is used to store the Infrared-Signals as hex. The array is 16 long, because the Keypad has 16 buttons. (0-9, #, *)

```c++
uint32_t signals[16];
```

This bool is used to define the current state of the remote. If it's true, the remote is in assigning-mode. If it's false, the remote is in transmitting-mode. It's initialized with false (transmitting-mode).

```c++
bool assignmode = false;
```

This uint32_t is used to temporarily store the Infrared-Signal as hex. It's used to store the signal that is received by the Infrared-Receiver.

```c++
uint32_t signal = 0;
```

These are the pins of the rows and columns of the Keypad. They're used to index through the 2d-array from above.

```c++
byte rowPins[rows] = {12, 13, 14, 15};
byte colPins[cols] = {16, 17, 18, 19};
```

This is the Keypad-object. It's used to make the Keypad work. It's initialized with the 2d-array from above, the rowPins, the colPins, the count of rows and the count of columns.

```c++
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
```

This is the variable that stores the last key that was pressed, so it can be used to asign the signal to a key.

```c++
char lastkeypressed;
```

### setup

This is the setup-function. It's used to initialize the Serial, the RGB-LED, the Infrared-Receiver and the Infrared-Transmitter. It also confirms the finished set-up-process to the Serial and turns on the RGB-LED as green. After the initialization, the remote is in send-mode (so "SEND" will be also in the Serial).

```c++
void setup() {
  Serial.begin(9600);
  pinMode(RED_RGB, OUTPUT);
  pinMode(GREEN_RGB, OUTPUT);
  pinMode(BLUE_RGB, OUTPUT);
  IrReceiver.begin(IRRECEIVER);
  IrSender.begin(IRTRANSMITTER);
  delay(100);
  Serial.println("Script Started!");
  digitalWrite(GREEN_RGB, HIGH);
  Serial.println("SEND");
}
```

### loop

This is the loop-function.
It's used to check if a button is pressed, if the button is a special button (assigning-mode (="#") or transmitting-mode (="*")), it will change the state of the remote and print it to the Serial. If the button is a normal button, it will print the button to the Serial and store it in the lastkeypressed-variable.
What also happens is the color change. Untill the singal was received/sent, the LED will be red or blue mixed with green (which means a button was pressed). If the signal was received, the LED will be green.

```c++
void loop() {
  char rn = keypad.getKey();
  if (rn && rn != '#' && rn != '*') {
    if (assignmode) {
      digitalWrite(BLUE_RGB, HIGH);
      Serial.print("ASSIGN-Button: ");
    } else {
      Serial.print("SEND-Button: ");
    }
    Serial.println(rn);
    lastkeypressed = rn;
  }
```

If the button is a special button (assigning-mode (="#") or transmitting-mode (="*")), it will change the state of the remote and print it to the Serial and change the color of the LED to either red (= assigning-mode) or green (= transmitting-mode). If the button is a normal button, it will print the button to the Serial and store it in the lastkeypressed-variable.

```c++
  if (rn == '#') {
    digitalWrite(RED_RGB, HIGH);
    digitalWrite(BLUE_RGB, LOW);
    digitalWrite(GREEN_RGB, LOW);
    assignmode = true;
    Serial.println("ASSIGN");
    delay(500);
  }

  if (rn == '*') {    
    assignmode = false;
    digitalWrite(GREEN_RGB, HIGH);
    digitalWrite(BLUE_RGB, LOW);
    digitalWrite(RED_RGB, LOW);
    Serial.println("SEND");
    delay(500);
  }
```

If the remote is in sending-mode, and a Key of the Keypad is pressed, it will send the Infared-Signal saved in signals through the Infrared-Transmitter. It will also print "SEND-Signal: " and the hex-code of it to the Serial.
If that signal isn't part of the Keypad, it will print "SEND-Signal: " and "INTERNAL ERROR" to the Serial, which means that something unknown and internal went wrong (which shoouldn't happen).

```c++
  if (lastkeypressed && lastkeypressed != '#' && lastkeypressed != '*') {
    if (!assignmode) {
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
          Serial.println(signals[KA], HEX);
          break;
        case 'B':
          IrSender.sendNECRaw(signals[KB], 1);
          Serial.println(signals[KB], HEX);
          break;
        case 'C':
          IrSender.sendNECRaw(signals[KC], 1);
          Serial.println(signals[KC], HEX);
          break;
        case 'D':
          IrSender.sendNECRaw(signals[KD], 1);
          Serial.println(signals[KD], HEX);
          break;
        default:
          Serial.println("INTERNAL ERROR");
          break;
      }
```

Next there's a delay of 100ms, so that the remote doesn't send the same signal multiple times and after that, the lastkeypressed-variable is set to 0 (= nothing), so that the remote doesn't send the same signal multiple times (because of the delay and because of the nothing it doesn't even get past the if-clause (*if (lastkeypressed)*)).

```c++
      delay(100);
      lastkeypressed = 0;
```

If the remote is in assigning-mode, it will wait for a signal to be received and then store it in the signal-variable. After that, it will print "RECEIVED-Signal: " and the hex-code of it to the Serial, set the lastbuttonpressed-variable to 0 and store the signal in the right place in the signals-array. It also turns the LED to red, so that the user knows that the remote is in assigning-mode waiting for a button to be pressed.

```c++
    } else if (IrReceiver.decode() && IrReceiver.decodedIRData.decodedRawData != 0) {
      signal = IrReceiver.decodedIRData.decodedRawData; 
      Serial.println("Inside");
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

      digitalWrite(RED_RGB, HIGH);
      digitalWrite(BLUE_RGB, LOW);
      digitalWrite(GREEN_RGB, LOW);
      delay(100);
      Serial.print("ASSIGN-Signal: ");
      Serial.println(signal, HEX);
      lastkeypressed = 0;
      IrReceiver.resume();
    }
  }
}
```
