#include <LiquidCrystal.h>

#define SIGNAL A0
#define GREEN 8      // updated
#define YELLOW 7     // updated
#define RED 13       // unchanged

float tariff = 756.2; 
float current = 0.5;
unsigned long previousTime = 0;
float energy;

// Wokwi LCD1602 pin mapping
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(SIGNAL, INPUT);
  previousTime = millis();

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energy Monitor");
  delay(1000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(SIGNAL);
  float voltage = sensorValue * (5.0 / 1023.0);
  float power = voltage * current;

  unsigned long currentTime = millis();
  float deltaTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  energy += power * (deltaTime / 3600.0);
  float cost = energy * tariff;

  String statusText = "";

  // LED + STATUS INTERPRETATION
  if (power < 1) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    statusText = "LOW usage     ";
  } 
  else if (power < 2) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    statusText = "MEDIUM usage  ";
  } 
  else {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
    statusText = "HIGH usage    ";
  }

  // LCD OUTPUT (interprets LED state)
  lcd.setCursor(0, 0);
  lcd.print("Status:");
  lcd.print(statusText);

  lcd.setCursor(0, 1);
  lcd.print("Cost:");
  lcd.print(cost, 1);
  lcd.print(" UGX   ");

  delay(1000);
}