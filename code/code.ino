// every distance measure is in cm;
#include <math.h>
const float MOUNT_DISTANCE = 40.0; // distance to the wall at which the laser is mounted
#define LIMIT_ANGLE = 60; // angle at which the servo should be limited

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


void setup() {
  // put your setup code here, to run once:
  pointer.attach(9);
}

void loop() {
  ts.execute();
}



void readMeasure() {
  distance = hc.dist();
}

void moveLaser() {
  int deg = atan(distance/MOUNT_DISTANCE) * 180/M_PI;
  if(deg < LIMIT_ANGLE) {
    pointer.write(deg);
  }
}
