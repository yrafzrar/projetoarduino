#include <SPI.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> //bibliotecas
#include <Wire.h>
#include <ZMPT101B.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define SENSITIVITY 500.0f

ZMPT101B voltageSensor(A0, 50.0); //detecta o sensor
int led= 8;
int buzzer = 3;
float max = 100;
int rele = 7;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar OLED"));
    for(;;);
  }
  
  pinMode(led, OUTPUT);
  pinMode(rele, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  voltageSensor.setSensitivity(SENSITIVITY);
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Sistema Protecao AC");
  display.display();
  delay(1000);



}

void loop() {
  float voltage = voltageSensor.getRmsVoltage();
  // Atualiza OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Sistema Protecao AC");
  display.print("Tensao: ");
  display.println(voltage);
  display.print("Max config: ");
  display.print(max);
  display.println("V");
  Serial.println(voltage);
  if (voltage > max) {
    digitalWrite(rele, LOW); 
    tone(buzzer, 1000);
    digitalWrite(led,HIGH);
    display.setCursor(0,40);
    display.println("TENSAO ELEVADA");
    display.print("EQUIPAMENTO DESLIGADO");
  }
  else{
    
    digitalWrite(rele, HIGH);
    noTone(buzzer);
    digitalWrite(led, LOW);
  }
  
  display.display();
  delay(200);
  

}
