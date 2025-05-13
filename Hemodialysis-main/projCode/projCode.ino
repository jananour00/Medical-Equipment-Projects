#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>               // For I2C communication
#include <LiquidCrystal_I2C.h>  // For LCD I2C

// Define pins
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 10;
const int ENA = 5;
const int ENB = 6;
const int LDR_PIN = A0;
const int LED_PIN = 13;
const int BUZZER_PIN = 12;

#define ONE_WIRE_BUS 2     // Temperature sensor pin
#define FLOW_SENSOR_PIN 3  // Flow sensor signal pin

// Initialize LCD (address, columns, rows)
// Common addresses are 0x27 or 0x3F for 16x2 LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int speed = 210;
int ldrValue = 0;

volatile int pulseCount = 0;
unsigned long lastTime = 0;
float flowRate = 0.0;

// Interrupt Service Routine
void pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  sensors.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(1000);
  lcd.clear();

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);

  lastTime = millis();
}

void loop() {
  ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  // Update LCD display
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC);
  lcd.print("C L:");
  lcd.print(ldrValue);
  
  lcd.setCursor(0, 1);
  lcd.print("Flow:");
  lcd.print(flowRate);
  lcd.print("L/min");

  // Check light and temperature conditions
  if (ldrValue < 150 || tempC > 40 || tempC < 25.0 || flowRate == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Condition triggered! Pumps stopped.");
    
    // Update LCD with warning
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WARNING!");
    lcd.setCursor(0, 1);
    lcd.print("Pumps stopped");
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Flow rate calculation
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 1000) {
    noInterrupts();
    int count = pulseCount;
    pulseCount = 0;
    interrupts();

    flowRate = count / 11.0;  // YF-S401 factor
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    lastTime = currentTime;
  }

  delay(500);
}