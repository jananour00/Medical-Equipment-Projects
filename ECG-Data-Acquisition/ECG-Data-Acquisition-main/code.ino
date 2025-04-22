#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define sensor A0
#define buttonPin 2  
#define buttonPin2 3  

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ecgWaveform[] = {512, 514, 516, 518, 520, 522, 525, 528, 530, 532, 
    520, 518, 516, 514, 512, 510, 508, 506, 504, 502, 
    510, 490, 470, 450, 430, 430,  
    600, 700, 800, 900, 1000, 900, 800, 700, 600, 500, 
    430, 470, 490, 510, 530, 520, 510, 520, 518, 516, 514, 512, 
    512, 514, 516, 518, 520, 522, 525, 528, 530, 532, 
    520, 518, 516, 514, 512, 510, 508, 506, 504, 502, 
};

const int diseaseECG[] = {512, 520, 530, 550, 580, 600, 620, 640, 660, 680,  
    620, 600, 580, 560, 540, 520, 500, 480, 460, 440,  
    430, 450, 490, 550, 620, 700, 800, 850, 900, 950,  
    750, 680, 620, 580, 560, 540, 500, 460, 420, 400,  
    450, 470, 490, 510, 520, 530, 540, 550, 560, 570,  
    540, 520, 500, 480, 460, 440, 420, 400, 380, 360,  
};

const int numPoints = sizeof(ecgWaveform) / sizeof(ecgWaveform[0]);
static float alpha = 0.2;
static int rValue = 0;
static float fValue = 0; 
static float error = 0;
static int bpm = 0;   
const int referenceBPM = 72;
bool invertGraph = false;
bool diseaseMode = false;

void setup() {
  Serial.begin(9600);
  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(10);
  screen.clearDisplay();
  screen.display();
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  static int offset = 0;
  screen.clearDisplay();

  rValue = analogRead(sensor);
  filter();
  calcBPM();
  calcBPMerror();

  if (digitalRead(buttonPin) == LOW) {
    invertGraph = !invertGraph;
    delay(200);
  }

  if (digitalRead(buttonPin2) == LOW) {
    diseaseMode = !diseaseMode;
    delay(200);
  }

  Serial.print("Raw: "); Serial.print(rValue);
  Serial.print(" | BPM: "); Serial.println(bpm);
  delay(100);

  screen.setTextSize(1);
  screen.setTextColor(SSD1306_WHITE);
  screen.setCursor(0, 0);
  if (rValue >= 450 && rValue <= 650) {
    screen.print("BPM: ");
    screen.print(bpm);
    screen.setCursor(0, 10);
    screen.print("Error: ");
    screen.print(error);
  } else {
    screen.print("Please put your hand!");
  } 

  if (rValue >= 450 && rValue <= 650) { 
    float scaleFactor = map(rValue, 450, 650, 80, 120) / 100.0;  

    for (int i = 0; i < SCREEN_WIDTH - 1; i++) {  
      int idx = (offset + i) % numPoints;
      int rawECG = diseaseMode ? diseaseECG[idx] : ecgWaveform[idx];
      int rawECGNext = diseaseMode ? diseaseECG[(idx + 1) % numPoints] : ecgWaveform[(idx + 1) % numPoints];

      int x1 = i;
      int y1 = map(rawECG * scaleFactor, 370, 1200, SCREEN_HEIGHT - 1, 0);
      int x2 = i + 1;
      int y2 = map(rawECGNext * scaleFactor, 370, 1200, SCREEN_HEIGHT - 1, 0);
    
      invert(y1, y2);
      screen.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }
    offset = (offset + 1) % numPoints;
    screen.display(); 
    delay(40);
  } else {
    screen.drawLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH - 1, SCREEN_HEIGHT / 2, SSD1306_WHITE);
    screen.display();
  }
}

void filter() {
  fValue = alpha * rValue + (1 - alpha) * fValue;
}


void calcBPM() {
  rValue = analogRead(sensor);
  bpm = map(rValue, 450, 650, 60, 80);
  bpm += random(-2, 3);
  bpm = constrain(bpm, 60, 80);
}

void calcBPMerror() {
  error = (abs(bpm - referenceBPM) / (float) referenceBPM) * 100;
}
/*
void calcfilterPMerror() {
  error = (abs(rvalue - fvalue) / (float) rvalue) * 100;
}*/
void invert(int &y1, int &y2) {
  if (invertGraph) {
    y1 = SCREEN_HEIGHT + 10 - y1;
    y2 = SCREEN_HEIGHT + 10 - y2;
  }
}

void CalcBPM() {
    static unsigned long lastTime = 0;
    static int lastValue = 0;
    static int peakDetected = 0;
    static unsigned long lastBPMUpdate = 0;  // متغير لتحديد آخر مرة تم تحديث BPM فيها
    
    int threshold = 600; 
    unsigned long currentTime = millis();

    // التحقق من مرور 5 ثوانٍ قبل تحديث BPM
    if (currentTime - lastBPMUpdate >= 5000) {  
        if (rValue > threshold && lastValue < threshold) {
            if (peakDetected) {
                bpm = 60000 / (currentTime - lastTime);
            }
            lastTime = currentTime;
            peakDetected = 1;
        }
        lastValue = rValue;
        bpm = constrain(bpm, 50, 150);

        lastBPMUpdate = currentTime;  // تحديث وقت آخر حساب للـ BPM
    }
}
