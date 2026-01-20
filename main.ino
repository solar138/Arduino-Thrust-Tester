#include "HX711.h"
#include <Servo.h>

// Pin configurations:
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 4;
const int ESC_PWM_PIN = 9;
const int SENSOR_PIN = 2;

// Other configurations:
float calibrationForce = 4.214; // Force applied to load cell for calibration
float calibrationValue = 89971; // Load cell reading when calibrated.

float targetRPM = 6000; // current targetRPM. Will be overwritten when rpmStart, rpmStep, and rpmEnd are used.

float rpmStart = 6000; // starting RPM to test at.
float rpmStep = 1000; // RPM increment between tests.
float rpmEnd = 15000; // final RPM to end a single test trial.

int trials = 5; // Number of times to cycle through the RPMs. Set to zero for infinite testing.

int stepDuration = 5000; // time to wait at each step in ms.

float p = 0.0003; // proportional control loop factor.

const int minSpeed = 1050; // Minimum PWM duty cycle in microseconds
const int maxSpeed = 1500; // Maximum PWM duty cycle in microseconds.

// Runtime variables below:
float speed = 0;

HX711 scale;
Servo esc;

unsigned volatile int ticks = 0;

void setup() {
 Serial.begin(57600);

 Serial.println("Initializing optical sensor...");
 attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), count, RISING);


 Serial.println("Initializing ESC...");
 esc.attach(ESC_PWM_PIN);
 
 if (esc.attached() == 1) {
  Serial.println("ESC ready");
 } else {
  Serial.println("ESC not ready");
 }

 esc.writeMicroseconds(minSpeed);

 scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
 Serial.println("Initializing Load Cell... Remove forces");
 delay(1000);
 scale.tare();
 scale.set_scale(calibrationValue / calibrationForce);
 Serial.println("Load Cell Ready");

 Serial.println("Beginning test");
 targetRPM = rpmStart;
}

const int revLength = 100;
int revIndex = 0;
long revs[revLength];

long previousClear = 0;
long previousRev = 0;
long currentRev = 1000000;
unsigned int rpm = 0;

long stepStartTime = 0;

void count() {
  revs[revIndex] = micros();
  revIndex++;
  if (revIndex >= revLength) revIndex = 0;
}

void getRPM() {
  long sum = 0;
  int pIndex = revIndex;
  int index = revIndex;
  for (int i = 1; i < revLength; i++) {
    index++;
    if (index >= revLength) index = 0;
    sum += revs[index] - revs[pIndex];

    pIndex = index;
  }

  rpm = 60000000 / ((double) sum / (double) (revLength - 1));

  if (rpm > 1000000) rpm = 0;
}

void loop() {
 if (scale.is_ready()) {
   float reading = scale.get_units(1);
   
   getRPM();

   float delta = targetRPM - rpm;

   speed += delta * p;
   
   int iSpeed = minSpeed + (int) speed;

   if (iSpeed < minSpeed) iSpeed = minSpeed;
   if (iSpeed > maxSpeed) iSpeed = maxSpeed;
   esc.writeMicroseconds(iSpeed);

   if (rpm > targetRPM + 250 || rpm < targetRPM - 250) stepStartTime = millis();
   
   if (millis() > stepStartTime + stepDuration) {
    Serial.print(String(rpm) + ", ");
    Serial.println(String((double) reading, 4));
    //Serial.println("Current speed: " + String(iSpeed));

    targetRPM += rpmStep;

    stepStartTime = millis();

    if (targetRPM > rpmEnd) {
      trials--;
      targetRPM = rpmStart;
      if (trials == 0) {
      Serial.println("Test complete");
      esc.writeMicroseconds(minSpeed);
      while (true) {}
      }
    }
   }
 }

 if (Serial.available()) {
  Serial.println("Stopping!");
  esc.writeMicroseconds(minSpeed);
  while (true) {}
 }
}
