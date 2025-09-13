#include <Arduino.h>

const int ntcPin = A0;    // NTC input
const int fanPin = 13;    // Fan simulated by LED
const float R0 = 10000;   // 10k reference resistor
const float BETA = 3950;  // Beta value
const float T0 = 298.15;  // 25°C in Kelvin

unsigned long lastToggle = 0;
int pwmState = LOW;
int dutyCycle = 0; // 0 = off, 50 = half, 100 = full

float readTemperature() {
  int adc = analogRead(ntcPin);
  float Rntc = R0 * (1023.0 / adc - 1.0);
  float tempK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(Rntc / R0));
  return tempK - 273.15; // Convert to Celsius
}

void softwarePWM(int duty) {
  // duty = 0..100
  static unsigned long lastMillis = 0;
  unsigned long now = micros();

  int period = 10000; // 10ms period => 100Hz PWM
  int onTime = period * duty / 100;

  if (now % period < onTime) {
    digitalWrite(fanPin, HIGH);
  } else {
    digitalWrite(fanPin, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
}

void loop() {
  float tempC = readTemperature();

  if (tempC < 25) {
    dutyCycle = 0;
  } else if (tempC < 30) {
    dutyCycle = 50;
  } else {
    dutyCycle = 100;
  }

  softwarePWM(dutyCycle);

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" °C | Fan: ");
  if (dutyCycle == 0) Serial.println("OFF");
  else if (dutyCycle == 50) Serial.println("50%");
  else Serial.println("100%");

  delay(200);
}
