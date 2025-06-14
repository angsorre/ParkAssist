// every distance measure is in cm;
#include <math.h>
#include <EEPROM.h>


float MOUNT_DISTANCE = 40.0;  // distance to the wall (cm) at which the laser is mounted
float addr_MOUNT_DISTANCE = 4;
#define LIMIT_ANGLE 60                 // angle at which the servo should be limited
#define UPDATE_DISTANCE_BUTTON_PIN 8   // button that triggers the distance update
#define UPDATE_ZERO_DEG_BUTTON_PIN 12  // button that triggers the zero_deg update
#define POTENTIOMETER_PIN 14         // pin to which the distance potentiometer is connected
int ZERO_DEG = 0;
int addr_ZERO_DEG = 0;

/// ------- TASKS -------- //
#include <TaskScheduler.h>
Scheduler ts;

// --- read distance
#include <HCSR04.h>
HCSR04 hc(2, 4);  // (trig, echo)
float distance = MOUNT_DISTANCE;

void readMeasure();
Task tUpdateDistance(TASK_MILLISECOND * 200,
                     TASK_FOREVER,
                     &readMeasure,
                     &ts,
                     true);

// --- update laser angle
#include <Servo.h>
Servo pointer;

void moveLaser();
Task tUpdateLaser(TASK_MILLISECOND * 200,
                  TASK_FOREVER,
                  &moveLaser,
                  &ts,
                  true);

// -- check for distance updates
void updateMountDistance();
Task tUpdateMountDistance(TASK_SECOND * 5,
                          TASK_FOREVER,
                          &updateMountDistance,
                          &ts,
                          true);

// -- ZERO_DEG calibration
void updateZeroDeg();
Task tupdateZeroDeg(TASK_SECOND * 5,
                    TASK_FOREVER,
                    &updateZeroDeg,
                    &ts,
                    true);


// ------ SETUP & LOOP --------- //
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  pointer.attach(9);
  pinMode(UPDATE_DISTANCE_BUTTON_PIN, INPUT);
  ZERO_DEG = EEPROM.read(addr_ZERO_DEG);
  // EEPROM is read in bytes, however using an int is safe as value don't go over 255
  float temp;
  EEPROM.get(addr_MOUNT_DISTANCE, temp);  // allows non-bytes value writing
  if (temp < 30.0) {
    MOUNT_DISTANCE = temp;
  }
}

void loop() {
  ts.execute();
}


// -- task functions
void readMeasure() {
  distance = hc.dist();
}

void moveLaser() {
  int deg = atan(distance / MOUNT_DISTANCE) * 180 / M_PI;
  // received degree is mapped to match the zero of calibration
  deg = map(constrain(deg, ZERO_DEG, LIMIT_ANGLE), ZERO_DEG, LIMIT_ANGLE, 0, LIMIT_ANGLE);
  pointer.write(deg);
}

void updateMountDistance() {
  if (digitalRead(UPDATE_DISTANCE_BUTTON_PIN) == HIGH) {
    MOUNT_DISTANCE = map(analogRead(POTENTIOMETER_PIN), 0, 1023, 30, 101);
    EEPROM.put(addr_MOUNT_DISTANCE, MOUNT_DISTANCE);
  }
}

void updateZeroDeg() {
  if (digitalRead(UPDATE_ZERO_DEG_BUTTON_PIN) == HIGH) {
    int deg = atan(distance / MOUNT_DISTANCE) * 180 / M_PI;
    if (deg < LIMIT_ANGLE) {
      ZERO_DEG = deg;
      EEPROM.update(addr_ZERO_DEG, deg);
    }
    /*
    EEPROM writes are limited to 100k cycles, write() method should be as valuable as the value isn't supposed to change so often
    but update can save write cycles if it doesn't change.
  */
  }
}
