// "Left"
const int PIN_DIT = D0;
// "Right"
const int PIN_DAH = D1;
const int PIN_SPEED_CTRL = D2;
const int PIN_PITCH_CTRL = D3;
const int PIN_BUZZER_CTRL = D9;

// Placeholder: actual value tbd later 
const uint32_t TOUCH_THRESHOLD = 40000;  

const int WPM_MIN  = 5;
const int WPM_MAX  = 35;
// 400Hz-1000Hz
const int FREQ_MIN = 400;   
const int FREQ_MAX = 1000;  

bool isTouched(int pin) {
  uint32_t val = touchRead(pin);
  return val > TOUCH_THRESHOLD;
}

// 0-4095
int currentWPM() {
  int raw = analogRead(PIN_SPEED_CTRL); 
  return map(raw, 0, 4095, WPM_MIN, WPM_MAX);
}

int currentFreqHz() {
  int raw = analogRead(PIN_PITCH_CTRL);
  return map(raw, 0, 4095, FREQ_MIN, FREQ_MAX);
}

void buzzerOn(int freqHz) {
  tone(PIN_BUZZER_CTRL, freqHz);
}

void buzzerOff() {
  noTone(PIN_BUZZER_CTRL);
}

// Comes with standard one-dit-length inter-element gap
void sendElement(int durationMs) {
  buzzerOn(currentFreqHz());
  delay(durationMs);
  buzzerOff();
  delay(durationMs);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_SPEED_CTRL, INPUT);
  pinMode(PIN_PITCH_CTRL, INPUT);
  pinMode(PIN_BUZZER_CTRL, OUTPUT);

  Serial.println("Paddle ready");
}

void loop() {
  // Serial.print("DIT: "); Serial.print(touchRead(PIN_DIT));
  // Serial.print("DAH: "); Serial.println(touchRead(PIN_DAH));

  // PARIS timing? 
  // https://morsecode.world/international/timing/ 
  int wpm = currentWPM();
  int ditMs = 1200 / wpm;   
  int dahMs = ditMs * 3;

  if (isTouched(PIN_DIT)) {
    sendElement(ditMs);
  } else if (isTouched(PIN_DAH)) {
    sendElement(dahMs);
  } else {
    buzzerOff();
  }
}
