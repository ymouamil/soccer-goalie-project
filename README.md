# Soccer Goalie Arduino Project

This project is a mini soccer goalie game using an Arduino Uno, SG92R servo, potentiometer, photoresistor, LEDs, 3D printed parts, and a Processing display.

## Features

- Potentiometer controls the goalie servo
- SG92R servo rotates the goalie
- Photoresistor detects when the ball enters the goal
- LEDs display the score
- Processing shows the scoreboard, goalie, ball animation, and sensor readings

## Pin Connections

| Part | Arduino Pin |
|---|---|
| Potentiometer middle pin | A0 |
| Photoresistor divider | A1 |
| LED 1 | D3 |
| LED 2 | D4 |
| LED 3 | D5 |
| Servo signal | D9 |

## Photoresistor Wiring

5V --- photoresistor --- A1 --- 10k resistor --- GND

## Folder Structure

- Arduino code: Arduino/soccer_goalie_arduino/soccer_goalie_arduino.ino
- Processing code: Processing/soccer_goalie_processing/soccer_goalie_processing.pde
- 3D files: 3D_Files
- Images and videos: Images

## Notes

The photoresistor works best when placed inside a small tube or tunnel so it only sees light from one direction. The ball should pass close to the sensor.
