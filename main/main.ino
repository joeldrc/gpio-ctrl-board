/**
 ******************************************************************************
  @file    BTMS mcu
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   gpio-ctrl-board
 ******************************************************************************
  Board:
    - Developped on teensy 4.1
    - CPU speed: 600MHz
    - Optimize: Faster
  Info programming:
    -
  References C++:
    -
  Scheme:
    -
 ******************************************************************************
*/


#include "defines.h"
#include <Bounce.h>
#include "src\TeensyThreads\TeensyThreads.h"
#include <SPI.h>
#include <fnet.h>
#include "src\NativeEthernet\NativeEthernet.h"


/*** Global variables **/
uint8_t boardSN = 0;
uint8_t mac[] = { 0xDA, 0xAD, 0xBE, 0xEF, 0xFE, 0xE0 };

uint8_t P1A_val = 0;
uint8_t P1B_val = 0;
uint8_t P2A_val = 0;
uint8_t P2B_val = 0;


// button object
Bounce pushbutton1 = Bounce(BTN1, 10);  // 10 ms debounce
Bounce pushbutton2 = Bounce(BTN2, 10);  // 10 ms debounce
Bounce pushbutton3 = Bounce(BTN3, 10);  // 10 ms debounce


volatile float cpuTemp = 0.0;

// mcu temperature
extern float tempmonGetTemp(void);


uint8_t readSettingSwitch(uint8_t pinA, uint8_t pinB) {
  // Read the current four-bit
  uint8_t b = 0;
  // The switch are active LOW
  if (digitalReadFast(pinA) == 0) b |= 1;
  if (digitalReadFast(pinB) == 0) b |= 2;
  //if (digitalReadFast(pinC) == 0) b |= 4;
  //if (digitalReadFast(pinD) == 0) b |= 8;
  return (b);
}


void ctrlEthernetThread() {
  while (1) {
    int linkStatusLed = ctrlConnection();
    switch (linkStatusLed) {
      case 1: {
          // connected (led green)
          digitalWriteFast(StsLedGr, LOW);
          digitalWriteFast(StsLedOr, HIGH);
        }
        break;
      case -1: {
          // unknown (led orange blinking)
          digitalWriteFast(StsLedGr, HIGH);
          digitalWriteFast(StsLedOr, LOW);
        }
        break;
      default: {
          // not connected (led off)
          digitalWriteFast(StsLedGr, HIGH);
          digitalWriteFast(StsLedOr, HIGH);
        }
        break;
    }
    ethernetConfig_thread();
    threads.delay(2000);
    threads.yield();
  }
}


String decodeData(String receivedData) {
  int valueReceived = -1;
  String returnString = "";

  char cmdReceived = receivedData.charAt(0);

  if (receivedData.length() < 5 && receivedData.length() > 1) {
    valueReceived = receivedData.substring(1, receivedData.length()).toInt();
  }

  switch (cmdReceived) {
    case 'a': {
        if (valueReceived >= 0 && valueReceived <= 255) {
          P1A_val = valueReceived;
        }
        returnString += "PORT1A:";
        returnString += P1A_val;
      }
      break;
    case 'b': {
        if (valueReceived >= 0 && valueReceived <= 255) {
          P1B_val = valueReceived;
        }
        returnString += "PORT1B:";
        returnString += P1B_val;
      }
      break;
    case 'c': {
        if (valueReceived >= 0 && valueReceived <= 255) {
          P2A_val = valueReceived;
        }
        returnString += "PORT2A:";
        returnString += P2A_val;
      }
      break;
    case 'd': {
        if (valueReceived >= 0 && valueReceived <= 255) {
          P2B_val = valueReceived;
        }
        returnString += "PORT2B:";
        returnString += P2B_val;
      }
      break;
  }

  // Write Ports
  for (uint8_t i = 0; i < 8; i++) {
    digitalWriteFast(P1A[i], bit_test(i, P1A_val));
    digitalWriteFast(P1B[i], bit_test(i, P1B_val));
    digitalWriteFast(P2A[i], bit_test(i, P2A_val));
    digitalWriteFast(P2B[i], bit_test(i, P2B_val));
  }

  return returnString;
}


void heartBeatThread() {
  while (1) {
    // read CPU temperature
    cpuTemp = tempmonGetTemp();

    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');
      Serial.println(decodeData(receivedData));
    }

    threads.delay(100);
    threads.yield();
  }
}


bool bit_test(uint8_t bit, uint8_t byte) {
  if ((byte & (1 << bit)) > 0) {
    return 0;
  }
  else {
    return 1;
  }
}


void setup() {
  // Buttons
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // Outputs LED
  pinMode(StsLedOr, OUTPUT);
  pinMode(StsLedGr, OUTPUT);
  // Set LEDs off
  digitalWriteFast(StsLedOr, LOW);
  digitalWriteFast(StsLedGr, LOW);

  // Digital outputs
  for (uint8_t i = 0; i < Pbit; i++) {
    pinMode(P1A[i], OUTPUT);
    pinMode(P1B[i], OUTPUT);
    pinMode(P2A[i], OUTPUT);
    pinMode(P2B[i], OUTPUT);

    digitalWriteFast(P1A[i], HIGH);
    digitalWriteFast(P1B[i], HIGH);
    digitalWriteFast(P2A[i], HIGH);
    digitalWriteFast(P2B[i], HIGH);
  }

  // Start serial
  Serial.begin(9600);
  Serial.println("BTMS mcu serial monitor");

  Serial1.begin(9600);

  // Read hardware switch
  Serial.print("Setting switch 1,2: ");
  Serial.print(readSettingSwitch(SW1, SW2));
  Serial.println();

  // Set board serial number
  boardSN = readSettingSwitch(SW1, SW2);

  // Set mac address
  mac[5] += boardSN;

  // Start thread
  threads.addThread(ctrlEthernetThread, 1);
  threads.addThread(heartBeatThread, 1);
}


void loop() {
  // Check buttons
  if (pushbutton1.update()) {
    if (pushbutton1.fallingEdge()) {

    }
  }
  if (pushbutton2.update()) {
    if (pushbutton2.fallingEdge()) {

    }
  }
  if (pushbutton3.update()) {
    if (pushbutton3.fallingEdge()) {

    }
  }

  // Handle webServer
  handleWebServer();
}
