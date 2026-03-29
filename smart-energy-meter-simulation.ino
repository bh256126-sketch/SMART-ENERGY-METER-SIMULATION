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
  if (power < 1) {
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  Serial.println("Status: LOW usage");

  } else if (power < 2) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    Serial.println("Status: MEDIUM usage");

  } else {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
    Serial.println("Status: HIGH usage");
  }
  Serial.println(power);
  Serial.println(energy);
  float cost = energy * tariff;
  Serial.print("Cost (UGX): ");
  Serial.println(cost);
  delay(1000); // read every 1 second  

}

