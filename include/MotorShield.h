#define MS_SERVO_1_PIN 10
#define MS_SERVO_2_PIN 9

#ifndef MS_DONT_SWAP_M3_AND_M4
#define MS_MOTOR_3_PWM_PIN 5
#define MS_MOTOR_4_PWM_PIN 6

#define MS_MOTOR_3_A_SHIFT 5
#define MS_MOTOR_3_B_SHIFT 7
#define MS_MOTOR_4_A_SHIFT 0
#define MS_MOTOR_4_B_SHIFT 6
#else
#define MS_MOTOR_3_PWM_PIN 6
#define MS_MOTOR_4_PWM_PIN 5

#define MS_MOTOR_3_A_SHIFT 0
#define MS_MOTOR_3_B_SHIFT 6
#define MS_MOTOR_4_A_SHIFT 5
#define MS_MOTOR_4_B_SHIFT 7
#endif

#define MS_MOTOR_1_PWM_PIN 11
#define MS_MOTOR_2_PWM_PIN 3

#define MS_DIR_LATCH_PIN 12
#define MS_DIR_SER_PIN 8
#define MS_DIR_CLK_PIN 4
#define MS_DIR_EN_PIN 7

#define MS_MOTOR_1_A_SHIFT 2
#define MS_MOTOR_1_B_SHIFT 3
#define MS_MOTOR_2_A_SHIFT 1
#define MS_MOTOR_2_B_SHIFT 4

#include <Arduino.h>
#include <stdint.h>

/// @brief Enum for driving directions
enum class MotorDirection : uint8_t
{
  Release = 0,
  Forward = 1,
  Backward = 2,
  Brake = 3,
};

/// @brief A class for controlling motors on
class MotorShield
{
protected:
  /// @brief Status of the IC3 shift register
  uint8_t dir = 0;

  /// @brief Function for setting the IC3 register value
  void sendDir()
  {
    digitalWrite(MS_DIR_LATCH_PIN, LOW);
    shiftOut(MS_DIR_SER_PIN, MS_DIR_CLK_PIN, MSBFIRST, dir);
    digitalWrite(MS_DIR_LATCH_PIN, HIGH);
  }

  /// @brief Function for setting the correct A and B bits of the desired motor in the IC3 register
  /// @param motor Motor number (can be viewed on the shield)
  /// @param a Bit A
  /// @param b Bit B
  void setAB(uint8_t motor, bool a, bool b)
  {
    bitWrite(dir, getShift(motor, true), a);
    bitWrite(dir, getShift(motor, false), b);
    sendDir();
  }

  /// @brief Function to get the desired shift for bit A or B of the desired motor
  /// @param motor Motor number (Can be viewed on the shield)
  /// @param a If bit a is true, otherwise bit b
  /// @return The shift for the desired bit in the IC3 register
  uint8_t getShift(uint8_t motor, bool a)
  {
    switch ((motor - 1) % 4)
    {
    case 0:
      return a ? MS_MOTOR_1_A_SHIFT : MS_MOTOR_1_B_SHIFT;
    case 1:
      return a ? MS_MOTOR_2_A_SHIFT : MS_MOTOR_2_B_SHIFT;
    case 2:
      return a ? MS_MOTOR_3_A_SHIFT : MS_MOTOR_3_B_SHIFT;
    case 3:
      return a ? MS_MOTOR_4_A_SHIFT : MS_MOTOR_4_B_SHIFT;
    }
    return 0;
  }

  /// @brief Function to get the correct EN pin of the correct motor
  /// @param motor Motor number (can be viewed on the shield)
  /// @return The correct pin
  uint8_t getPwm(uint8_t motor)
  {
    switch ((motor - 1) % 4)
    {
    case 0:
      return MS_MOTOR_1_PWM_PIN;
    case 1:
      return MS_MOTOR_2_PWM_PIN;
    case 2:
      return MS_MOTOR_3_PWM_PIN;
    case 3:
      return MS_MOTOR_4_PWM_PIN;
    }
    return 0;
  }

public:
  /// @brief Function for setting the direction of the motor
  /// @param motor The motor number (can be viewed on the shield)
  /// @param direction The direction of movement (MotorDirection)
  void setDirection(uint8_t motor, MotorDirection direction)
  {
    setAB(motor, (uint8_t)direction & 1, (uint8_t)direction >> 1);
  }

  /// @brief Function for setting the motor speed
  /// @param motor Motor number (can be viewed on the shield)
  /// @param speed Motor speed from 0 to 255
  void setSpeed(uint8_t motor, uint8_t speed)
  {
    analogWrite(getPwm(motor), speed);
  }

  /// @brief Constructor for motors ONLY. It does not adjust SER1 and SERVO_2
  /// @param setMaxSpeed If true, all motors will be at the set speed (But will not move, to start moving, call setDirection)
  MotorShield(bool setMaxSpeed)
  {
    pinMode(MS_DIR_CLK_PIN, OUTPUT);
    pinMode(MS_DIR_LATCH_PIN, OUTPUT);
    pinMode(MS_DIR_SER_PIN, OUTPUT);
    pinMode(MS_DIR_EN_PIN, OUTPUT);

    pinMode(MS_MOTOR_1_PWM_PIN, OUTPUT);
    pinMode(MS_MOTOR_2_PWM_PIN, OUTPUT);
    pinMode(MS_MOTOR_3_PWM_PIN, OUTPUT);
    pinMode(MS_MOTOR_4_PWM_PIN, OUTPUT);

    sendDir();

    digitalWrite(MS_DIR_EN_PIN, LOW); // Enable the chip outputs

    if (setMaxSpeed)
    {
      digitalWrite(MS_MOTOR_1_PWM_PIN, HIGH);
      digitalWrite(MS_MOTOR_2_PWM_PIN, HIGH);
      digitalWrite(MS_MOTOR_3_PWM_PIN, HIGH);
      digitalWrite(MS_MOTOR_4_PWM_PIN, HIGH);
    }
  }
};