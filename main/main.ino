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

// 0: read only
// 1: simulate SCY and ECY
// 2: simulate SCY, INJ and ECY
// 3: simulate SCY, CALSTATR, CALSTOP, INJ and ECY
// 4: manual mode
int32_t operationMode = 0;
int32_t previousOperationMode = -1;


volatile float cpuTemp = 0.0;


// button object
Bounce pushbutton1 = Bounce(BTN1, 10);  // 10 ms debounce
Bounce pushbutton2 = Bounce(BTN2, 10);  // 10 ms debounce
Bounce pushbutton3 = Bounce(BTN3, 10);  // 10 ms debounce


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


void heartBeatThread() {
  while (1) {
    // read CPU temperature
    cpuTemp = tempmonGetTemp();

    threads.delay(240);
    threads.yield();
  }
}


void selectOperationMode() {
  // operationMode selection
  if (previousOperationMode != operationMode) {
    switch (operationMode) {
      case 1: {

        }
        break;
      case 2: {

        }
        break;
      case 3: {

        }
        break;
      case 4: {

        }
        break;
      default: {

          operationMode = 0;
        }
        break;
    }
    //Serial.print("Operation mode: ");
    //Serial.println(operationMode);
    previousOperationMode = operationMode;
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
  for (uint8_t i=0; i < Pbit; i++) {
    pinMode(P1_A[i], OUTPUT);
    pinMode(P1_B[i], OUTPUT);
    pinMode(P2_A[i], OUTPUT);
    pinMode(P2_A[i], OUTPUT);

    digitalWriteFast(P1_A[i], HIGH);
    digitalWriteFast(P1_B[i], HIGH);
    digitalWriteFast(P2_A[i], HIGH);
    digitalWriteFast(P2_A[i], HIGH);
  }

  // Start serial
  Serial.begin(9600);
  Serial.println("BTMS mcu serial monitor");

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

  //selectOperationMode();

  // Handle webServer
  handleWebServer();
}
