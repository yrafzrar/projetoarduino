#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int sensorPin = A0;
int led = 8;
int buzzer = 7;
float maxTensao = 120.0;
float voltage = 0.0;

// Variáveis para debounce do LCD
unsigned long lastLCDUpdate = 0;
const long LCD_UPDATE_INTERVAL = 500;

void setup() {
  Serial.begin(9600);

  // --- Inicializa OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar OLED"));
    for (;;);
  }

  // --- Inicializa LCD ---
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Iniciando...");

  // --- Pinos ---
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(A1)); // seed aleatória

  delay(1000);
  display.clearDisplay(); //check display
  display.display();
}

void loop() {
  int valorSensor = analogRead(sensorPin);
  // Cálculo da tensão
  voltage = (valorSensor * 300.0) / 1023.0;
  
  Serial.print("Tensao: ");
  Serial.println(voltage);

  // --- Atualiza LCD com debounce ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tensao: ");
    lcd.print(voltage, 1);
    lcd.print("V");
    
    lcd.setCursor(0, 1);
    if (voltage > maxTensao) {
      lcd.print("Equip: DESLIGADO");
    } else {
      lcd.print("Equip: LIGADO   ");
    }
    
    lastLCDUpdate = currentMillis;
  }

  display.clearDisplay();

  if (voltage > maxTensao) {
    // --- TENSÃO ALTA ---
    tone(buzzer, 1000);
    digitalWrite(led, LOW);

    // Mostra alerta no OLED em vez de deixar vazio
    display.clearDisplay();
    
  } else {
    // --- NORMAL ---
    noTone(buzzer);
    digitalWrite(led, HIGH);

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
        display.drawCircle(64, 38, 8, SSD1306_WHITE);
        break;
      case 5: // Ondas 
        for (int x = 0; x < 128; x += 2) {
          int y = 32 + (sin(x * 0.2) * 15);
          display.drawPixel(x, y, SSD1306_WHITE);
        }
        break;
    }
  }

  display.display(); //Config display oled
  delay(400);
}
