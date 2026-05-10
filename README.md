# Soccer Goalie Arduino Project

This project is a mini soccer goalie game using an Arduino Uno, SG92R servo, potentiometer, photoresistor, LEDs, 3D printed parts, and a Processing display.

## Demo Video

YouTube demo: https://www.youtube.com/watch?v=WekJ-e6B-3E&t=7s

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

```text
5V --- photoresistor --- A1 --- 10k resistor --- GND
