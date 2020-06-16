// Sapphire nixie tube driver via Arduino Mega, no shift registers
// Made for IN16 and K155 drivers
// 06/16/2020
/* !! WARNING !!
    Nixie tubes are high voltage. 
    Improper handling may cause serious harm or death, as
    well as destruction of any connected devices.
    Always check your schematic and power levels before
    powering your devices. 
*/ 

// Reference pinout of K155:
/*
0 1 5 4 GND 6 7 3
-----------------

o
-----------------
8 9 A D VCC B C 2
*/

// Reference pinout of IN16, counter clockwise from anode
// ANODE 1 7 3 LDP 4 5 6 2 RDP 8 9 0


// Suggested pins for driver connection, change as needed
#define DriverOneA 5
#define DriverOneB 6
#define DriverOneC 7
#define DriverOneD 8

#define DriverTwoA 10
#define DriverTwoB 11
#define DriverTwoC 12
#define DriverTwoD 13

#define DriverThreeA 31
#define DriverThreeB 33
#define DriverThreeC 35
#define DriverThreeD 37

#define DriverFourA 32
#define DriverFourB 34
#define DriverFourC 36
#define DriverFourD 38

#define DriverFiveA 41
#define DriverFiveB 43
#define DriverFiveC 45
#define DriverFiveD 47

#define DriverSixA 42
#define DriverSixB 44
#define DriverSixC 46
#define DriverSixD 48

// Running values of what should be displayed on the specific nixie tube
uint8_t TubeOneValue = 0;
uint8_t TubeTwoValue = 0;
uint8_t TubeThreeValue = 0;
uint8_t TubeFourValue = 0;
uint8_t TubeFiveValue = 0;
uint8_t TubeSixValue = 0;

// The operation mode
// 0 = Cycle through all digits in sync
// 1 = Cycle through all digits, offset by one
// 2 = RTC mode
// 3 = Random mode
int Mode = 0;

// The delay used when using cycles or random modes
int ModeDelay = 1000;

void NixieControl(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t value){
  digitalWrite(d, (value & 0x08) >> 3);
  digitalWrite(c, (value & 0x04) >> 2);
  digitalWrite(b, (value & 0x02) >> 1);
  digitalWrite(a, value & 0x01);
}

void WriteAllZero() {
    NixieControl(DriverOneA, DriverOneB, DriverOneC, DriverOneD, 0);
    NixieControl(DriverTwoA, DriverTwoB, DriverTwoC, DriverTwoD, 0);
    NixieControl(DriverThreeA, DriverThreeB, DriverThreeC, DriverThreeD, 0);
    NixieControl(DriverFourA, DriverFourB, DriverFourC, DriverFourD, 0);
    NixieControl(DriverFiveA, DriverFiveB, DriverFiveC, DriverFiveD, 0);
    NixieControl(DriverSixA, DriverSixB, DriverSixC, DriverSixD, 0);
}

void setup() {
    pinMode(DriverOneA, OUTPUT);
    pinMode(DriverOneB, OUTPUT);
    pinMode(DriverOneC, OUTPUT);
    pinMode(DriverOneD, OUTPUT);

    pinMode(DriverTwoA, OUTPUT);
    pinMode(DriverTwoB, OUTPUT);
    pinMode(DriverTwoC, OUTPUT);
    pinMode(DriverTwoD, OUTPUT);

    pinMode(DriverThreeA, OUTPUT);
    pinMode(DriverThreeB, OUTPUT);
    pinMode(DriverThreeC, OUTPUT);
    pinMode(DriverThreeD, OUTPUT);

    pinMode(DriverFourA, OUTPUT);
    pinMode(DriverFourB, OUTPUT);
    pinMode(DriverFourC, OUTPUT);
    pinMode(DriverFourD, OUTPUT);

    pinMode(DriverFiveA, OUTPUT);
    pinMode(DriverFiveB, OUTPUT);
    pinMode(DriverFiveC, OUTPUT);
    pinMode(DriverFiveD, OUTPUT);

    pinMode(DriverSixA, OUTPUT);
    pinMode(DriverSixB, OUTPUT);
    pinMode(DriverSixC, OUTPUT);
    pinMode(DriverSixD, OUTPUT);

    // Set them to all output 0
    WriteAllZero();

}

void CycleFromZero()
{
    while (true) {
        for (int i = 0; i < 9; i ++) {
            NixieControl(DriverOneA, DriverOneB, DriverOneC, DriverOneD, i);
            NixieControl(DriverTwoA, DriverTwoB, DriverTwoC, DriverTwoD, i);
            NixieControl(DriverThreeA, DriverThreeB, DriverThreeC, DriverThreeD, i);
            NixieControl(DriverFourA, DriverFourB, DriverFourC, DriverFourD, i);
            NixieControl(DriverFiveA, DriverFiveB, DriverFiveC, DriverFiveD, i);
            NixieControl(DriverSixA, DriverSixB, DriverSixC, DriverSixD, i);
            delay(ModeDelay);
        }
    }
}

void CycleFromOffset() {
    TubeOneValue = 0;
    TubeTwoValue = 1;
    TubeThreeValue = 2;
    TubeFourValue = 3;
    TubeFiveValue = 4;
    TubeSixValue = 5;
    while (true) {
        if (TubeOneValue > 9) {
            TubeOneValue = 0;
        }
        if (TubeTwoValue > 9) {
            TubeTwoValue = 0;
        }
        if (TubeThreeValue > 9) {
            TubeThreeValue = 0;
        }
        if (TubeFourValue > 9) {
            TubeFourValue = 0;
        }
        if (TubeFiveValue > 9) {
            TubeFiveValue = 0;
        }
        if (TubeSixValue > 9) {
            TubeSixValue = 0;
        }
        NixieControl(DriverOneA, DriverOneB, DriverOneC, DriverOneD, TubeOneValue);
        NixieControl(DriverTwoA, DriverTwoB, DriverTwoC, DriverTwoD, TubeTwoValue);
        NixieControl(DriverThreeA, DriverThreeB, DriverThreeC, DriverThreeD, TubeThreeValue);
        NixieControl(DriverFourA, DriverFourB, DriverFourC, DriverFourD, TubeFourValue);
        NixieControl(DriverFiveA, DriverFiveB, DriverFiveC, DriverFiveD, TubeFiveValue);
        NixieControl(DriverSixA, DriverSixB, DriverSixC, DriverSixD, TubeSixValue);
        TubeOneValue++;
        TubeTwoValue++;
        TubeThreeValue++;
        TubeFourValue++;
        TubeFiveValue++;
        TubeSixValue++;
        delay(ModeDelay);
    }
}

void CycleRandom() {
    while (true) {
        int TubeOneRandom = rand() % 10;
        int TubeTwoRandom = rand() % 10;
        int TubeThreeRandom = rand() % 10;
        int TubeFourRandom = rand() % 10;
        int TubeFiveRandom = rand() % 10;
        int TubeSixRandom = rand() % 10;
        NixieControl(DriverOneA, DriverOneB, DriverOneC, DriverOneD, TubeOneValue);
        NixieControl(DriverTwoA, DriverTwoB, DriverTwoC, DriverTwoD, TubeTwoValue);
        NixieControl(DriverThreeA, DriverThreeB, DriverThreeC, DriverThreeD, TubeThreeValue);
        NixieControl(DriverFourA, DriverFourB, DriverFourC, DriverFourD, TubeFourValue);
        NixieControl(DriverFiveA, DriverFiveB, DriverFiveC, DriverFiveD, TubeFiveValue);
        NixieControl(DriverSixA, DriverSixB, DriverSixC, DriverSixD, TubeSixValue);
        delay(ModeDelay);
    }
}

void RTCMode()
{
    // Under construction. I don't have an RTC module yet.
}

void loop() {
    switch (Mode) {
        case 0:
            CycleFromZero();
        break;

        case 1:
            CycleFromOffset();
        break;

        case 2:
            RTCMode();
        break;

        case 3:
            CycleRandom();
        break;

        default:
            CycleFromZero();
        break;
    }

}
