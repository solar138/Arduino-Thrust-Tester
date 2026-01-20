# Arduino-Thrust-Tester
Controls a PWM-based ESC connected to a BLDC ducted fan along with taking measurements from an IR optical sensor. Allows simultaneous measurement of thrust, RPM, and PWM duty cycle to put together a graph. Runs fully automatically apart from initial load cell calibration. Designed for an Arduino UNO R4 Wi-Fi, though other boards should work.

## Hardware
Required:
- An Arduino board (recommended Arduino UNO R4)
- HX711 Load Cell Signal Amplifier (and a load cell)
- Brushless DC motor with ESC or other PWM-controlled engine.
- Infrared optical sensor or other rotation sensor that sends DC pulses. Make sure the fan has some kind of alternating pattern that the sensor can detect (such as by painting a black and white stripe with a marker).
- A weight of known value for calibration
- A computer, laptop, or other USB device for logging data.

Configuration:

<img width="2060" height="1382" alt="image" src="https://github.com/user-attachments/assets/4c0d7067-a256-4f19-8dd8-0f75b7199c91" />

## Usage
- Tests are designed to run automatically.
- Sending anything via serial will immediately stop all tests. Data is written to serial in pairs of RPM and load cell reading in Newtons.
- If using the Arduino IDE, be sure to clear the serial monitor after the initialization messages to allow you to copy all data to analysis programs.
- Avoid having anything in front of or behind the engine to prevent injury or damage. Do not touch the engine during tests.
- Check that the sensor is properly measuring RPM before starting tests. This can be done by unplugging the engine first and rotating the fan by hand to check if the light on the Infrared optical sensor is flashing.

## Example Data Readout
<img width="792" height="530" alt="image" src="https://github.com/user-attachments/assets/4d87e304-fa0f-45b8-8150-ef6c2969331f" />

Graph generated using Logger Pro.

