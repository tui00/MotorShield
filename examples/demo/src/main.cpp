#include <Servo.h>
#include <MotorShield.h>

// Objects for servos
Servo servo1;
Servo servo2;

// An object for controlling the motor shield
MotorShield motorShield(true);

void setup() {
// Initializing servos using definitions from the library
  servo1.attach(MS_SERVO_1_PIN); // Pin 10
  servo2.attach(MS_SERVO_2_PIN); // Pin 9

  // Set the initial positions of the servos
  servo1.write(0);
  servo2.write(90);

  delay(1000); // Wait for the servos to reach their positions

  // Configure the motors
  // Motor 1: forward, 50% speed
 motorShield.setDirection(1, Forward);
 motorShield.setSpeed(1, 128); // 128 out of 255

 // Motor 2: backward, 75% speed
 motorShield.setDirection(2, Backward);
 motorShield.setSpeed(2, 191); // 191 out of 255
}

void loop() {
  static unsigned long lastServoUpdate = 0;
  static int servoPos = 0;
  static bool servoDirection = true; // true — forward, false — backward

  // We update the servos every 20 ms
  if (millis() - lastServoUpdate >= 20) {
    lastServoUpdate = millis();

    if (servoDirection) {
      servoPos += 5;
      if (servoPos >= 180) {
        servoDirection = false;
      }
    } else {
      servoPos -= 5;
      if (servoPos <= 0) {
        servoDirection = true;
      }
    }

    // Move both servos: one by position, the other by mirroring
    servo1.write(servoPos);
    servo2.write(180 - servoPos);
  }

  static unsigned long lastMotorChange = 0;

  // Change the direction of the motors every 4 seconds
  if (millis() - lastMotorChange >= 4000) {
    lastMotorChange = millis();

    static bool motor1Forward = true;
    static bool motor2Forward = true;

    // Switch the direction of motor 1
    if (motor1Forward) {
      motorShield.setDirection(1, Backward);
      motor1Forward = false;
    } else {
      motorShield.setDirection(1, Forward);
      motor1Forward = true;
    }

    // Switch the direction of motor 2
    if (motor2Forward) {
      motorShield.setDirection(2, Forward);  // Note: it was originally Backward
      motor2Forward = false;
    } else {
      motorShield.setDirection(2, Backward);
      motor2Forward = true;
    }
  }

  // Change the speed of the motors every 6 seconds
  static unsigned long lastSpeedChange = 0;
  if (millis() - lastSpeedChange >= 6000) {
    lastSpeedChange = millis();

    static bool highSpeed = true;

    if (highSpeed) {
      // High speed: 80%
      motorShield.setSpeed(1, 204);  // 204 out of 255 ≈ 80%
      motorShield.setSpeed(2, 204);
      highSpeed = false;
    } else {
      // Low speed: 30%
      motorShield.setSpeed(1, 76);   // 76 out of 255 ≈ 30%
      motorShield.setSpeed(2, 76);
      highSpeed = true;
    }
  }
}
