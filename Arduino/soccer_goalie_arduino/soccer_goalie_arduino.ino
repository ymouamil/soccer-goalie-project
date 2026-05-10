// Soccer Goalie Project
// A0 = potentiometer
// A1 = photoresistor
// D3, D4, D5 = LEDs
// D9 = SG92R servo signal

const int potPin = A0;
const int lightPin = A1;

const int led1 = 3;
const int led2 = 4;
const int led3 = 5;

const int servoPin = 9;

int score = 0;

// Servo settings
int servoAngle = 90;
int servoLeft = 45;
int servoRight = 135;

int minPulse = 900;
int maxPulse = 2100;

// Photoresistor values
int lightBase = 0;
int lightValue = 0;
int lightChange = 0;

// Tune this
int goalThreshold = 12;

// This is lower than goalThreshold.
// Sensor must fall below this before it can score again.
int resetThreshold = 5;

// Goal state
bool goalBlocked = false;
bool readyForNextGoal = true;

unsigned long lastGoalTime = 0;
unsigned long goalCooldown = 1800;   // longer cooldown after a goal

// LED/game states
bool goalFlash = false;
unsigned long goalFlashStart = 0;

bool winMode = false;
unsigned long winStart = 0;

unsigned long lastSerial = 0;

void setup() {
  Serial.begin(9600);

  pinMode(servoPin, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  // Keep photoresistor uncovered during this.
  int total = 0;

  for (int i = 0; i < 100; i++) {
    total += analogRead(lightPin);
    delay(5);
  }

  lightBase = total / 100;
  lightValue = lightBase;

  blinkStart();
}

void loop() {
  readPot();

  // Send servo pulse before sensor/LED logic
  moveServo(servoAngle);

  readLight();
  checkGoal();
  updateLEDs();
  sendData();

  // Send servo pulse again so it does not freeze
  moveServo(servoAngle);
}

void readPot() {
  int potValue = analogRead(potPin);

  // Reversed direction.
  // If servo turns wrong way, swap servoRight and servoLeft.
  servoAngle = map(potValue, 0, 1023, servoRight, servoLeft);
}

void moveServo(int angle) {
  int pulse = map(angle, 0, 180, minPulse, maxPulse);

  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(servoPin, LOW);

  delay(8);
}

void readLight() {
  lightValue = analogRead(lightPin);

  // Fixed baseline. Works whether value rises or falls.
  lightChange = abs(lightValue - lightBase);
}

void checkGoal() {
  bool blockedNow = lightChange > goalThreshold;
  bool returnedToNormal = lightChange < resetThreshold;

  // Re-arm only when the sensor is back to normal
  if (returnedToNormal) {
    goalBlocked = false;
    readyForNextGoal = true;
  }

  // Count only if:
  // 1. sensor is blocked
  // 2. it is armed
  // 3. enough time passed after last goal
  // 4. not in win mode
  if (blockedNow &&
      readyForNextGoal &&
      !goalBlocked &&
      !winMode &&
      millis() - lastGoalTime > goalCooldown) {

    goalBlocked = true;
    readyForNextGoal = false;
    lastGoalTime = millis();

    score++;

    goalFlash = true;
    goalFlashStart = millis();

    if (score >= 3) {
      winMode = true;
      winStart = millis();
    }
  }
}

void updateLEDs() {
  if (winMode) {
    winLights();

    if (millis() - winStart > 2400) {
      winMode = false;
      score = 0;
      showScore();

      // After reset, require sensor to be normal again
      readyForNextGoal = false;
      goalBlocked = true;
    }

    return;
  }

  if (goalFlash) {
    flashGoal();

    if (millis() - goalFlashStart > 500) {
      goalFlash = false;
      showScore();
    }

    return;
  }

  showScore();
}

void showScore() {
  digitalWrite(led1, score >= 1 ? HIGH : LOW);
  digitalWrite(led2, score >= 2 ? HIGH : LOW);
  digitalWrite(led3, LOW);
}

void flashGoal() {
  bool on = (millis() / 80) % 2 == 0;

  digitalWrite(led1, on ? HIGH : LOW);
  digitalWrite(led2, on ? HIGH : LOW);
  digitalWrite(led3, on ? HIGH : LOW);
}

void winLights() {
  int stepNum = (millis() / 90) % 8;

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  if (stepNum == 0) digitalWrite(led1, HIGH);
  if (stepNum == 1) digitalWrite(led2, HIGH);
  if (stepNum == 2) digitalWrite(led3, HIGH);
  if (stepNum == 3) digitalWrite(led2, HIGH);

  if (stepNum == 4 || stepNum == 6) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
}

void blinkStart() {
  digitalWrite(led1, HIGH);
  delay(80);

  digitalWrite(led2, HIGH);
  delay(80);

  digitalWrite(led3, HIGH);
  delay(80);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void sendData() {
  if (millis() - lastSerial < 120) {
    return;
  }

  lastSerial = millis();

  if (winMode) {
    Serial.print("WIN");
  } else if (goalFlash) {
    Serial.print("GOAL");
  } else {
    Serial.print("READY");
  }

  Serial.print(",");
  Serial.print(score);

  Serial.print(",");
  Serial.print(servoAngle);

  Serial.print(",");
  Serial.print(lightValue);

  Serial.print(",");
  Serial.print(lightBase);

  Serial.print(",");
  Serial.println(lightChange);
}