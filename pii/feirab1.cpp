#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int sensorPin = A0;
int led = 8;
int buzzer = 3;
float max = 100;
float voltage = 0.0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar OLED"));
    for(;;);
  }

  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  randomSeed(analogRead(A1)); // semente aleatória
  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {
  int valorSensor = analogRead(sensorPin);
  voltage = map(valorSensor, 0, 1023, 0, 300);
  Serial.println(voltage);

  display.clearDisplay();

  if (voltage > max) {
    // --- TENSÃO ALTA ---
    tone(buzzer, 1000);
    digitalWrite(led, HIGH);
    // Tela fica vazia
  } else {
    // --- NORMAL ---
    noTone(buzzer);
    digitalWrite(led, LOW);

    int desenho = random(1, 6); // 1 a 5 desenhos diferentes

    switch (desenho) {
      case 1: // Quadrado
        display.drawRect(40, 20, 40, 40, SSD1306_WHITE);
        break;
      case 2: // Círculo
        display.drawCircle(64, 32, 20, SSD1306_WHITE);
        break;
      case 3: // Triângulo
        display.drawTriangle(20, 55, 108, 55, 64, 10, SSD1306_WHITE);
        break;
      case 4: // Carinha feliz
        display.drawCircle(64, 32, 20, SSD1306_WHITE);
        display.fillCircle(56, 28, 2, SSD1306_WHITE);
        display.fillCircle(72, 28, 2, SSD1306_WHITE);
        break;
      case 5: // Ondas senoidais
        for (int x = 0; x < 128; x += 2) {
          int y = 32 + (sin(x * 0.2) * 15);
          display.drawPixel(x, y, SSD1306_WHITE);
        }
        break;
    }
  }

  display.display();
  delay(300);
}
