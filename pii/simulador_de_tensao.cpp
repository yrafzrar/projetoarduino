#include <SPI.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> //bibliotecas
#include <Wire.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int sensorPin = A0;    
int buzzer = 3;
int ledVerde = 7;
int ledVermelho = 11;

float tensaoAC = 0.0;
int tensaomax = 130;
int tensaomin = 100;
void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Sistema Protecao AC");
  display.display();
  delay(1000);

  Serial.begin(9600);
}

void loop() {
  int valorSensor = analogRead(sensorPin);

  tensaoAC = map(valorSensor, 0, 1023, 0, 300);
  

  // Atualiza OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Sistema Protecao AC");
  display.print("Tensao: ");
  display.println(tensaoAC);
  display.print("Max config: ");
  display.print(tensaomax);
  display.println("V");

  Serial.print("Tensao simulada: ");
  Serial.print(tensaoAC);
  Serial.println(" V");

  if (tensaoAC > tensaomax) {
    
    tone(buzzer, 1000);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);

    display.setCursor(0,40);
    display.println("TENSAO ELEVADA");
    display.print("EQUIPAMENTO DESLIGADO");
  }
  if (tensaoAC < tensaomin){
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    tone(buzzer, 500);
    display.setCursor(0,40);
    display.println("TENSAO BAIXA");
    display.print("EQUIPAMENTO DESLIGADO");
  }
  else 
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  }
  display.display();
  delay(200);
}
