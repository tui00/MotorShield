# MotorShield Library

Library for working with the motor shield (Motor Shield) on Arduino. The main value is ready-made `define`s with pin numbering, so you don't have to spend time searching for pinouts on the Internet.

## Description of define-s

### Servo pins
* `MS_SERVO_1_PIN` — servo pin 1 (default 10).
* `MS_SERVO_2_PIN` — servo pin 2 (default 9).

### Motor pins (PWM)
* `MS_MOTOR_1_PWM_PIN` — PWM pin of motor No. 1 (11).
* `MS_MOTOR_2_PWM_PIN` - PWM pin of motor No. 2 (3).
* `MS_MOTOR_3_PWM_PIN` and `MS_MOTOR_4_PWM_PIN` — depend on the macro `MS_DONT_SWAP_M3_AND_M4`:
* Without `MS_DONT_SWAP_M3_AND_M4 `: motor 3 — pin 5, motor 4 — pin 6.
  * With `MS_DONT_SWAP_M3_AND_M4': motor 3 — pin 6, motor 4 — pin 5.

### Bit shifts for direction control (A and B)
The positions of the bits in the shift register (IC3) for each motor are determined:
* Motor 1: `MS_MOTOR_1_A_SHIFT' (2), `MS_MOTOR_1_B_SHIFT` (3).
* Motor 2: `MS_MOTOR_2_A_SHIFT` (1), `MS_MOTOR_2_B_SHIFT` (4).
* Motor 3: `MS_MOTOR_3_A_SHIFT` and `MS_MOTOR_3_B_SHIFT` depend on `MS_DONT_SWAP_M3_AND_M4` (default 5 and 7, otherwise 0 and 6).
* Motor 4: `MS_MOTOR_4_A_SHIFT` and `MS_MOTOR_4_B_SHIFT' — the same (default is 0 and 6, otherwise 5 and 7).

### Shift register pins (IC3)
* `MS_DIR_LATCH_PIN` (12) — register latch pin.
* 'MS_DIR_SER_PIN' (8) is the serial input (data) of the register.
* `MS_DIR_CLK_PIN` (4) — clock input (clock) of the register.
* `MS_DIR_EN_PIN` (7) — enable output (enable) of the chip.

---

## Description of the enum and class

### `MotorDirection` (enum)
Sets the possible directions of the motor:
* `Release` (0) — release (the motor is not controlled).
* `Forward` (1) — forward.
* `Backward` (2) — backward.
* `Brake` (3) — braking (active).

### Class `MotorShield`

#### Protected (protected) methods and fields
* `uint8_t dir` — current state of the shift register (IC3).
* `void sendDir()` — sends the value of `dir` to the shift register.
* `void setAB(uint8_t motor, bool a, bool b)` — sets the A and B bits for the specified motor in the register and updates it.
* `uint8_t getShift(uint8_t motor, bool a)` — returns the shift (bit position) for the A bit (`a = true`) or B bit (`a = false`) of the specified motor.
* `uint8_t getPwm(uint8_t motor)` — returns the PWM pin for the specified motor (1–4).

#### Public (public) methods
* `void setDirection(uint8_t motor, uint8_t direction)` — sets the direction of motion for motor # `motor` according to the value from `MotorDirection`.
* `void setSpeed(uint8_t motor, uint8_t speed)` — sets the speed of motor # `motor` in the range of 0–255 (PWM).
* `MotorShield(bool setMaxSpeed)` — constructor. Configures the pins as outputs, initializes the register, and sets a high level on all PWM pins when `setMaxSpeed = true` (but the motors do not start rotating until `setDirection` is called).

---

## Important limitations and notes

This library:
* **Does not implement a smooth start/stop** — the speed changes instantly.
* **Does not provide anti-chattering protection** when switching directions.
* **Does not have built-in speed stabilization** (no PID controller).
* **Does not monitor overheating or overload** of the motors.
* **Does not support encoders** or speed/position feedback.
* **Assumes that the user is responsible for monitoring the logic of operation** (for example, it does not allow simultaneous forward and backward movements).
*For the most part, it is only needed to `define` pins** - its main value is that it eliminates the need to search for the pinout of the motor shield on the Internet or on the Internet.

The library provides a minimal set of functions for controlling the direction and speed of the motors through the shift register and PWM. Complex scenarios (smooth overclocking, stabilization, feedback) will require modification or the use of other libraries.

---

## Feedback

If you find a bug, create an [Issue](https://github.com/tui00/MotorShield/issues). If you want to get an answer to the problem faster, write to me in [Telegram](https://t.me/tui00abc)

If you write to Telegram, it's better to just send a link to [Telegraph](https://telegra.ph)

### Be sure to specify:
* If you write by email, Telegram: Library name (MotorShield)
* Library version
* The MC where the bug occurs
* If available: SDK version for development
* How do I get the bug?
* Motor shield model (if known)
* Is there a bug in the examples?
* How should I address you?
