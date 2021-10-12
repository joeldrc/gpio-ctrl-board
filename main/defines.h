

const char softwareVersion[] = "1.00";
const char softwareUpdate[] = "10.2021";

const uint8_t Pbit = 8;

// pin assignment
const uint8_t LcdRX = 0;
const uint8_t LcdTX = 1;

const uint8_t SW1 = 2;
const uint8_t SW2 = 3;
const uint8_t BTN1 = 21;
const uint8_t BTN2 = 22;
const uint8_t BTN3 = 23;

const uint8_t StsLedGr = 4;
const uint8_t StsLedOr = 5;

const uint8_t P1A[Pbit] = {32,  6, 31,  7, 30,  8, 29,  9};
const uint8_t P1B[Pbit] = {28, 10, 27, 11, 26, 12, 25, 24};
const uint8_t P2A[Pbit] = {20, 33, 19, 34, 18, 35, 17, 36};
const uint8_t P2B[Pbit] = {16, 37, 15, 38, 14, 39, 13, 40};
