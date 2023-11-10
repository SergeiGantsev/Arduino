#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN5 5 //Датчик температуры
DHT dht(DHTPIN5, DHT11);
int time1 = 30; //Периодичность считывания температуры
long curTimer1 = - time1 * 1000; // Чтобы сразу запустилось
float curH = 0;
float curT = 0;

LiquidCrystal_I2C lcd(0x3F,16,2);  // Устанавливаем дисплей
int time2 = 1; //Периодичность вывода на LCD
long curTimer2 = - time2 * 1000; // Чтобы сразу запустилось

void setup() {
  pinMode(DHTPIN5, INPUT);
  dht.begin();

  lcd.init(); 
  lcd.backlight();// Включаем подсветку дисплея
    
  Serial.begin(9600);
}

void loop() {
  if (millis() - curTimer1 > time1 * 1000) {
    float h = dht.readHumidity(); //Измеряем влажность
    float t = dht.readTemperature(); //Измеряем температуру
  
    if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
      Serial.println("Ошибка считывания");
      curH = 0;
      curT = 0;
    }
    else {
      curH = h;
      curT = t;      
    }

    Serial.print("Влажность: ");
    Serial.print(curH);
    Serial.print(" %\t");
    Serial.print("Температура: ");
    Serial.print(curT);
    Serial.println(" *C "); //Вывод показателей на экран

    curTimer1 = millis(); 
  }

  if (millis() - curTimer2 > time2 * 1000) {
    lcd.setCursor(0, 0);
    String s = "Humidity = ";
    s+=int(curH);
    s+="%";    
    lcd.print(s);
    
    lcd.setCursor(0, 1);
    s = "Temperature = ";
    s+=int(curT);   
    lcd.print(s);       
    
    curTimer2 = millis();     
  }
  
}
