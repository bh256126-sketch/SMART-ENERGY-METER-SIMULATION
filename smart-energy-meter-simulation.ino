
#define BLYNK_TEMPLATE_ID "TMPL2vXLMLiEc"
#define BLYNK_TEMPLATE_NAME "Smart Energy Meter"
#define BLYNK_AUTH_TOKEN "qmrenGOffMXq6xxncFyPOXif1MHE12gF"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define SIGNAL 34
#define GREEN 13      // updated
#define YELLOW 14     // updated
#define RED 27

#define I2C_SDA 21
#define I2C_SCL 22      // unchanged

float tariff = 756.2; 
float current = 0.5;
unsigned long previousTime = 0;
float energy = 0;

//Set LCD address
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Hello, ESP32!");
  // Serial.begin(9600);
  pinMode(SIGNAL, INPUT);
  previousTime = millis();

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Energy Monitor");

  delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
  Blynk.run();

  int sensorValue = analogRead(SIGNAL);
  float voltage = sensorValue * (3.3 / 4095.0);
  float power = voltage * current;

  unsigned long currentTime = millis();
  float deltaTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  energy += power * (deltaTime / 3600.0);
  float cost = energy * tariff;

  String statusText = "";

  // LED + STATUS INTERPRETATION
  if (power < 0.5) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    statusText = "LOW ";
  } 
  else if (power < 1.2) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    statusText = "MEDIUM ";
  } 
  else {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
    statusText = "HIGH ";
  }

  // LCD OUTPUT (interprets LED state)
  lcd.setCursor(0, 0);
  lcd.print(statusText);
  lcd.print(energy);
  lcd.print("KWh");

  lcd.setCursor(0, 1);
  lcd.print("Cost:");
  lcd.print(cost, 1);
  lcd.print(" UGX   ");

  // send to dashboard
  Blynk.virtualWrite(V0, power);
  Blynk.virtualWrite(V1, energy);
  Blynk.virtualWrite(V2, cost);
  Blynk.virtualWrite(V3, statusText);

  delay(1000);
}