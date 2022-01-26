// Dotstar Includes:
#include <Arduino.h>
#include <AsyncDelay.h>

#include <FastLED.h>

#define PlayerNum 1

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define LED_TYPE    SK9822
#define COLOR_ORDER BGR
#define NUM_LEDS    47
CRGB leds[NUM_LEDS];        // Place for final LED buffer at end of each cycle
CRGB LED_BUFFER[NUM_LEDS];      // volatile LED buffer for pushing/poping
CRGB spitOverlay[NUM_LEDS];  // color spit buffer
CRGB ppitOverlay[NUM_LEDS];  // color spit buffer
CRGB tickOverlay[NUM_LEDS]; // Create tick overlay
CRGB drumKitOverlay[NUM_LEDS];  // Drumkit sprite buffer
CRGB ARGBOverlay[NUM_LEDS];  // ARGB sprite buffer
CRGB funfettiOverlay[NUM_LEDS];  // FunFetti buffer
CRGB bellOverlay[NUM_LEDS];  // Drumkit sprite buffer

// colored dots that pulse for drums
CRGBPalette16 drumPalette = RainbowColors_p; // Choose a color palette
// colored dots that glow for bells
CRGBPalette16 bellPalette = ForestColors_p; // Choose a color palette
// Support vars for Fire effect
CRGBPalette16 gPal; // https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes

#define BRIGHTNESS          200
#define FRAMES_PER_SECOND  120
//#define FRAMES_PER_SECOND  240
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// List of patterns to cycle through.  Each is defined as a separate function below.
// The order in which you declare things is the order of layering from bottom to top
// first in is written first to the strip
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { BGi, HSv, rainbow, ppit, spit, ticks, drumKit, ARGB, fairyFire, funfetti, bell, mouth, addGlitter };

bool patternEnArray[ARRAY_SIZE( gPatterns)] = {0}; // Create an enable array of same size as pattern array to enable/dissable pattern layers

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// LED Function Parameter Arrays (see LEDPatterns tab for functions and param descriptions)
uint8_t BGiParams[] = {0, 0, 0, 0, NUM_LEDS}; // R, G, B, LED_start, len
uint8_t HSVParams[] = {0, 0, 0, 0, NUM_LEDS}; // H, S, V, LED_start, len
uint8_t BOWParams[] = {0, NUM_LEDS, 5, 0};  // Start, len, hue Delta
uint8_t PPTParams[] = {0, 0, 0, 0, NUM_LEDS, 10}; // H, S, V, LED_start, len, fade_rate
uint8_t SPTParams[] = {0, 0, 0, 0, NUM_LEDS, 10}; // H, S, V, LED_start, len, fade_rate
uint8_t FIRParams[] = {0, 0, 0, 0, NUM_LEDS, 254}; // Hue, Sparking, Cooling, LED_start, len, fade
uint8_t DKTParams[] = {0}; // on/off
uint8_t ARGBParams[] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 30}; // indexR, wideR, brightR, indexG, wideG, brightG,indexB, wideB, brightB, fade_rate
uint8_t TCKParams[] = {4, 0, 0, 0, 127, 20}; // density, direction, hue, saturation, volume, tickSteps
uint8_t CFIParams[] = {0, 0, 0, 0, 169, 255}; // (on/off) h, s, v, start, len, density
uint8_t MOUParams[] = {0, 0, 0, 2, 1, 169}; // on/off h, s, v, start, len
uint8_t GLRParams[] = {40, 0, 127, 0, 2}; // h, s, v, randColorSpread, density

uint8_t RGBHSV[] = {0, 96, 160}; //hue translation for RGB
uint8_t glitterForce = 0; // intensity of Glitter
uint8_t glitterPos = -1; // Place glitter randomly lights up to report back to MaxMSP

// left-of-center Positions of ZX sensors relative to DotStar strip LED positions for visualization
uint8_t ZXSensorPos[] = {10, 31, 52, 73, 94, 115, 136, 157};

//==========================================================
// Sensor Includes:
//==========================================================
    #include "Wire.h"
    #include <ZX_Sensor.h>
    #include <SPI.h>


// uncoment one of these - which format do we want stuff output on serial?
#define OUTPUT_BINARY
//#define OUTPUT_READABLE

unsigned long previousMillis = 0; // will store last time samples were sent
const long interval = 10;  // interval at which to send samples (milliseconds)

const int MPU_ADDR = 0x68; // also define i2c address of MPU

#include "I2Cdev.h"
#include "MPU6050.h"
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

  MPU6050 accelgyro; // another called accelgyro
#define INTERRUPT_PIN 11  // use pin 11 on Micro, use pin 2 on Arduino Uno & most boards
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer


// Place to save accel gyro mpu6050 readings
int16_t ax, ay, az;
int16_t gx, gy, gz;

// MPU calibration vars:
int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
//int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;
//Change this 3 variables if you want to fine tune the skecth to your needs.
int buffersize=1000;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone=1;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


// Sparkfun XZ Sensor stuff:
//#define TCAADDR 0x71

// Constants
const int ZX_ADDR = 0x10;  // ZX Sensor I2C address

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
uint8_t x_pos = 255;
uint8_t z_pos = 255;

/*
// --- TCASESELECT --- 
//
// Set port to begin reading from
// 
// @param port_num  Port to read from
//
void tcaseselect(uint8_t port_num) 
{
  if (port_num < 8) {
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << port_num);
    Wire.endTransmission();  
  }
}
*/

// Analog Sensor Inputs
const int FSRPin = A0;  // Analog input pin that the FSR is attached to, using long FSR, and 10Kohm voltage divider
const int LinPin = A1;  // Analog input pin that the Line Pot is attached
const int IRPin = A2;  // Analog input pin IR Distance sensor is attached
//const int ePicDownPin = A3;  // Analog input pin that the ePic other direction is attached

int fsrVal = 0; // Place to store FSR val
int linVal = 0; // Place to store LinPot val
int irVal = 0; // Place to store FSR val
//int ePicDownVal = 0; // Place to store LinPot val


// ================================================================
// ===           Convert float to binary ROUTINE                ===
// ================================================================
typedef union {
 float floatingPoint;
 byte binary[4];
} binaryFloat;


void setup() {
  Serial.begin(230400); // Initialize serial port 
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  //==========================================================
// Sensors Setup:
//==========================================================
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

// ZX Init
    Wire.beginTransmission(ZX_ADDR);
      byte error = Wire.endTransmission();
      if (error == 0)
      {
         #ifdef OUTPUT_READABLE
         Serial.println("Found ZX Sensor!");
         #endif
         zx_sensor.init();
      }

  analogReadResolution(12); // 12 bit analog
  delay(5000);

  // *** Init MPU 6050 and serial stuff
  pinMode(INTERRUPT_PIN, INPUT);
#ifdef OUTPUT_READABLE
  Serial.println("Setting up MPU6050 raw");
#endif
  accelgyro.initialize();

    accelgyro.setZAccelOffset(1000); // 1688 factory default for my test chip
    accelgyro.setXGyroOffset(140); // was 220, 120=-.25, 180=.56, 150=.19, 135=-.06, 140=0
    accelgyro.setYGyroOffset(-10); // 76 = 1.192, 100=1.5, 50=0.8, 20=.35, -20=-.15, -10=0
    accelgyro.setZGyroOffset(60); //-75=-1.9, -20=-1., 20=-.5, 40=-.2, 60=.0
    

/*  
  #ifdef OUTPUT_READABLE
  Serial.println("Scanning for i2c MUXed devices...");
  #endif
  for (uint8_t t=0; t<8; t++) {
    delay(100);
      tcaseselect(t);
      #ifdef OUTPUT_READABLE
      Serial.print("TCA Port #"); Serial.println(t);
      #endif
      Wire.beginTransmission(ZX_ADDR);
      byte error = Wire.endTransmission();
      if (error == 0)
      {
         #ifdef OUTPUT_READABLE
         Serial.println("Found ZX Sensor on this port!");
         #endif
         zx_sensor.init();
      }
          
    }
        #ifdef OUTPUT_READABLE
        Serial.println("\ndone");
        #endif
*/     
  
  // Enable Rainbow Pattern On Startup: 
        patternEnArray[2] = 1;  // Rainbow On
        BOWParams[0] = 2;  // hue delta (length of hue transitions)
        BOWParams[1] = 0;   // Start LED
        BOWParams[2] = NUM_LEDS-1;   // Len
        BOWParams[3] = 200;   // Fade ammount, 0 strong to 255 weak

        clipLen(&BOWParams[1],&BOWParams[2]);
  delay(1000);
}


void loop()
{

  querySerial();

  callPatterns();

//==========================================================
// Sensor Stuff
//==========================================================

 // Check to see if it's time to send sensor vals
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    //Serial.print("Time: ");
    //Serial.println(currentMillis);
    // save the last time you sent vals
    previousMillis = currentMillis;
    
    // clear MPU fifo
    accelgyro.resetFIFO(); 
    
    // Read Sparkfun XZ sensor and report vals
    readXZ();

    // Read Analog Sensor Vals
    readAnalog();

    // Now measure MPU6050, update values in global registers
    measure_mpu6050();
    
    // Send new data out to Serial (either binary or human redable specified up top
    sendVals();

   }

}
