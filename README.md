# Automated Metal Detector and Radio-Controlled Car

This project showcases an automated metal detector integrated with a radio-controlled car. The system detects changes in oscillation frequency caused by variations in inductance, signaling the presence of metallic objects. Additionally, joystick voltage detection translates into motion commands for the robot, which utilizes an H-bridge for motor control.

## Features

### Hardware:
- **Controller:** EFM8LB12 microcontroller
- **Robot:** STM32 microcontroller
- **Wireless Communication:** JDY40 radio transmitters
- **Additional Radio:** JDY40 for Python features

### Bonus Features:

#### Improved Reliability:
- Utilized 1-byte strings for precise movement and metal detection instructions.
- Implemented encoding and decoding of single ASCII characters for enhanced data reliability.
- Normalized metal detection data using a moving average to reduce noise and false positives.
- Dynamically calibrated joystick voltage and metal detector frequency upon microcontroller restart.
- Mapped joystick values to consistent ranges for accurate control.

#### Accessibility/User Experience:
- Metal detection level displayed on laptop and LCD for better readability.
- Joystick position and car velocity displayed on laptop for easy tracking.
- Alternative control methods including path drawing, keyboard control, and voice commands.
- Emergency stop button triggers LED indicator for quick identification.
- Joystick button serves as an alternative emergency stop for easier access.

#### General Features:
- Real-time display of joystick data including velocity, position, and distance traveled.
- LEDs indicate powered lines for easy debugging.
- Ability to debug using JDY40 connected to laptop for command replication.

## Solution Evaluation

Extensive testing revealed the unreliability of sending 7-character strings over JDY40, leading to the implementation of encoding ASCII characters for improved reliability. Calibration upon microcontroller restart and dynamic joystick voltage initialization minimized issues with off-center values. Switching to resistor circuitry simplified hardware design and improved system compatibility. Disabling interrupts during critical operations enhanced frequency readings' accuracy.

## Detailed Design

![image](https://github.com/maya-taylor/Project2ELEC291/assets/146891846/936e8f9f-cdee-49f4-a9f4-d244722c6f16)

### STM32 Blocks:
- **Oscillator/Metal Detector:** Utilized Colpitts oscillator for metal detection with optimized frequency output.
- **H-Bridge:** Employed two H-bridges with optocouplers for motor control.
- **JDY40:** Wired separately from H-bridge to prevent signal interference.

![image](https://github.com/maya-taylor/Project2ELEC291/assets/146891846/2fd12ffb-63af-4ede-9a91-bd9b94f42370)

### EFM8 Blocks:
- **Joystick:** Connected as per specifications with interpreted wiring.
- **LCD:** Interfaced according to provided guidelines.
- **Buzzer:** Configured with resistor and diode for optimal performance.

![image](https://github.com/maya-taylor/Project2ELEC291/assets/146891846/d846ba42-223d-4185-82bb-87d78cf04a4a)

### Laptop Blocks:
- **JDY40:** Configured for serial transmission following specific steps.

## Software Design

### C Software Design:
- Implemented reliable radio signaling and communications with optimized wait functions.
- Processed joystick data and metal detection using dynamic calibration and normalization.
- Controlled emergency stop, motor operations, and manual/automatic modes based on received commands.

### Python Software Design:
- Implemented path drawing and execution, voice control, keyboard control, and joystick mapping functionalities.
- Utilized Tkinter for GUI development and speech_recognition for voice control.
- Enabled keyboard detection and control using curses library.

## Solution Assessment

Extensive testing validated robot movement, including precise turns and circles. Motor movement was verified through a dedicated test bench. Additional features were repeatedly tested for consistency, including emergency stop functionality.

