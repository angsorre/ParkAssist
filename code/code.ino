// every distance measure is in cm;
#include <math.h>
float MOUNT_DISTANCE = 40.0; // distance to the wall at which the laser is mounted
#define LIMIT_ANGLE 60 // angle at which the servo should be limited
#define BUTTON_PIN 8  // button that triggers the distance update
#define POTENTIOMETER_PIN 14 // pin to which the distance potentiometer is connected

/// ------- TASKS -------- //
#include <TaskScheduler.h>
Scheduler ts;

// --- read distance
#include <HCSR04.h>
HCSR04 hc(2, 4); // (trig, echo)
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
Task tUpdateMountDistance(TASK_SECOND,
                  TASK_FOREVER,
                  &updateMountDistance,
                  &ts,
                  true);

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  pointer.attach(9);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  ts.execute();
}


// -- task functions
void readMeasure() {
  distance = hc.dist();
}

void moveLaser() {
  int deg = atan(distance/MOUNT_DISTANCE) * 180/M_PI;
  if(deg < LIMIT_ANGLE) {
    pointer.write(deg);
  }
}

void updateMountDistance() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    MOUNT_DISTANCE = map(analogRead(POTENTIOMETER_PIN), 0,1023, 30, 101);
  }
}

