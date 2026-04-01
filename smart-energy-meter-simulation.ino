#define SIGNAL A0
#define GREEN 8
#define YELLOW 12
#define RED 13
float tariff = 756.2; // UGX per kWh (fixed example value)
float current = 0.5; // fixed current
unsigned long previousTime = 0;
float energy;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SIGNAL, INPUT);
  previousTime = millis();
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(SIGNAL);
  float voltage = sensorValue * (5.0 / 1023.0);
  float power = voltage * current;
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - previousTime) / 1000.0; // seconds
  previousTime = currentTime;
  energy += power * (deltaTime / 3600.0); // divide by 3600.0 to put joules(per second) to kWh
}