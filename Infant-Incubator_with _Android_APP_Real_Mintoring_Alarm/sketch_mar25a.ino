// Include Libraries

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <PulseSensorPlayground.h>

#include "DHT.h"

// I2C LCD Configuration (Change 0x27 to your LCD's address if needed)

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions

const int PULSE_PIN = A0;  // Pulse Sensor to A0

const int LED_PIN = 13;  // LED blinks with pulse (Optional)

const int THRESHOLD = 550;  // Pulse sensitivity threshold

const int DHT_PIN = 2;  // DHT sensor to pin 2

#define DHTTYPE DHT11  // DHT11 or DHT22

const int RELAY_1_PIN = 7;  // Lamp 1 control via relay

const int RELAY_2_PIN = 8;  // Lamp 2 control via relay

const int BUTTON_PIN = 11;  // Push Button for Lamp 2

const int ENA = 6;  // Fan speed (PWM pin)

const int IN1 = 9;  // Fan direction 1

const int IN2 = 10;  // Fan direction 2

// Object Initialization

PulseSensorPlayground pulseSensor;

DHT dht(DHT_PIN, DHTTYPE);

// Variables for Toggle Button

bool lamp2State = false;  // Track Lamp 2 status (OFF initially)

bool lastButtonState = HIGH;  // Last state of the button

unsigned long lastDebounceTime = 0;  // Debounce timer

const unsigned long debounceDelay = 50;  // Debounce delay

// Setup Function

void setup() {

  Serial.begin(9600);  // Start Serial Monitor

  // Initialize LCD

  lcd.init();

  lcd.backlight();

  lcd.setCursor(0, 0);

  lcd.print("System Starting...");

  // Initialize Pulse Sensor

  pulseSensor.analogInput(PULSE_PIN);

  pulseSensor.blinkOnPulse(LED_PIN);

  pulseSensor.setThreshold(THRESHOLD);

  if (pulseSensor.begin()) {

    Serial.println("Pulse Sensor Initialized");
  }

  // Initialize DHT Sensor

  dht.begin();

  // Initialize Relay and Fan Pins

  pinMode(RELAY_1_PIN, OUTPUT);

  pinMode(RELAY_2_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up

  digitalWrite(RELAY_1_PIN, HIGH);  // Lamp 1 OFF initially

  digitalWrite(RELAY_2_PIN, HIGH);  // Lamp 2 OFF initially

  pinMode(IN1, OUTPUT);

  pinMode(IN2, OUTPUT);

  pinMode(ENA, OUTPUT);

  delay(2000);  // Wait for LCD to initialize

  lcd.clear();
}

// Function to Control Fan

void startFan() {

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  analogWrite(ENA, 255);  // Full Speed
}

// Function to Stop Fan

void stopFan() {

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  analogWrite(ENA, 0);
}

// Function to Update LCD Display

void updateLCD(int bpm, float temp, float humidity) {

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("BPM:");

  lcd.print(bpm);

  lcd.setCursor(9, 0);

  lcd.print("T:");

  lcd.print(temp);

  lcd.print("C");

  lcd.setCursor(0, 1);

  lcd.print("H:");

  lcd.print(humidity);

  lcd.print("%");
}

// Function to Print Data to Serial Monitor

void printToSerial(int bpm, float temp, float humidity) {

  Serial.print("♥ BPM: ");

  Serial.print(bpm);

  Serial.print(" | Temp: ");

  Serial.print(temp);

  Serial.print("C | Hum: ");

  Serial.print(humidity);

  Serial.print("% | Lamp 2: ");

  Serial.println(lamp2State ? "ON" : "OFF");
}

// Main Loop

void loop() {

  int bpm = pulseSensor.getBeatsPerMinute();

  float temp = dht.readTemperature();

  float humidity = dht.readHumidity();

  // Update LCD Display

  updateLCD(bpm, temp, humidity);

  // Print to Serial Monitor

  printToSerial(bpm, temp, humidity);

  // Control Lamp 1 and Fan based on Temperature

  if (temp >= 32) {

    digitalWrite(RELAY_1_PIN, LOW);  // Lamp 1 ON

    startFan();  // Fan ON

  } else {

    digitalWrite(RELAY_1_PIN, HIGH);  // Lamp 1 OFF

    stopFan();  // Fan OFF
  }

  // Button Toggle Logic for Lamp 2

  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {  // Button Pressed

    if (millis() - lastDebounceTime > debounceDelay) {  // Debounce check

      lamp2State = !lamp2State;  // Toggle Lamp 2 state

      digitalWrite(RELAY_2_PIN, lamp2State ? LOW : HIGH);  // Change Lamp 2

      Serial.print("Lamp 2 is now ");

      Serial.println(lamp2State ? "ON" : "OFF");

      lastDebounceTime = millis();  // Reset debounce timer
    }
  }

  lastButtonState = buttonState;  // Update last button state

  delay(1000);  // Update every second
}
make bpm dont excced 90 make it from 60 to 90