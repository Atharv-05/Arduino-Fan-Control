int ntcPin = A0;    // NTC sensor
int fanPin = 13;    // Fan (LED on D13)

float R0 = 10000;   // 10k resistor
float BETA = 3950;  // Beta constant
float T0 = 298.15;  // 25C in Kelvin

int fanSpeed = 0;   // 0 = off, 50 = half, 100 = full

float readTemp() {
  int adc = analogRead(ntcPin);
  float Rntc = R0 * (1023.0 / adc - 1.0);
  float tempK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(Rntc / R0));
  return tempK - 273.15; // in C
}

void softPWM(int duty) {
  // simple software PWM
  unsigned long now = micros();
  int period = 10000; // 100Hz
  int onTime = period * duty / 100;
  if (now % period < onTime) digitalWrite(fanPin, HIGH);
  else digitalWrite(fanPin, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
}

void loop() {
  float tempC = readTemp();

  if (tempC < 25) fanSpeed = 0;
  else if (tempC < 30) fanSpeed = 50;
  else fanSpeed = 100;

  softPWM(fanSpeed);

  // print values
  Serial.print("Temp=");
  Serial.print(tempC);
  Serial.print(" C | Fan=");
  if (fanSpeed == 0) Serial.println("OFF");
  else if (fanSpeed == 50) Serial.println("50%");
  else Serial.println("100%");

  delay(200);
}
