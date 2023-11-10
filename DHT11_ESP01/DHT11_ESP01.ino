/*
Тестировалось на Arduino IDE 1.8.5
Дата тестирования 08.11.2017г.
*/ 
 
#include "ESP8266WiFi.h"                                // Подключаем библиотеку ESP8266WiFi
#include "Adafruit_Sensor.h"                            // Adafruit_Sensor
#include "DHT.h"                                        // Подключаем библиотеку DHT
 
const char* ssid = "MGTS_GPON_B987";               // Название Вашей WiFi сети
const char* password = "D3PLNHF5";          // Пароль от Вашей WiFi сети
 
#define DHTPIN 2                                        // Пин к которому подключен датчик
#define DHTTYPE DHT11                                   // Используемый датчик DHT 11
  // #define DHTTYPE DHT21
  // #define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);                               // Инициализируем датчик
WiFiServer server(80);                                  // Указываем порт Web-сервера
 
void setup() 
{
  Serial.begin(115200);                                 // Скорость передачи 115200 
  delay(10);                                            // Пауза 10 мкс
  dht.begin();                                          // Инициализация DHT
                             
  Serial.println("");                                   // Печать пустой строки 
  Serial.print("Connecting to ");                       // Печать "Подключение к:"
  Serial.println(ssid);                                 // Печать "Название Вашей WiFi сети"
  
  WiFi.begin(ssid, password);                           // Подключение к WiFi Сети
  
  while (WiFi.status() != WL_CONNECTED)                 // Проверка подключения к WiFi сети
  {
   delay(500);                                          // Пауза 500 мкс
   Serial.print(".");                                   // Печать "."
  }
   Serial.println("");                                  // Печать пустой строки                                          
   Serial.println("WiFi connected");                    // Печать "Подключение к WiFi сети осуществлено"
   server.begin();                                      // Запуск Web сервера
   Serial.println("Web server running.");               // Печать "Веб-сервер запущен"
   delay(10000);                                        // Пауза 10 000 мкс
   Serial.println(WiFi.localIP());                      // Печатаем полученный IP-адрес ESP
}
void loop() 
{
 WiFiClient client = server.available();                // Получаем данные, посылаемые клиентом 
  
if (client){
  Serial.println("New client");                         // Отправка "Новый клиент"
  boolean blank_line = true;                            // Создаем переменную, чтобы определить конец HTTP-запроса 
  while (client.connected()){                           // Пока есть соединение с клиентом 
    if (client.available()){                            // Если клиент активен 
      
      char c = client.read();                            // Считываем посылаемую информацию в переменную "с"
      if (c == '\n' && blank_line){                      // Вывод HTML страницы 
        float t = dht.readTemperature();                  // Запрос на считывание температуры
        float h = dht.readHumidity();

        if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
          Serial.println("Ошибка считывания");
          //break;
        }
       
        client.println("HTTP/1.1 200 OK");               // Стандартный заголовок HTTP 
        client.println("Content-Type: text/html"); 
        client.println("Connection: close");             // Соединение будет закрыто после завершения ответа
        client.println("Refresh: 10");                   // Автоматическое обновление каждые 10 сек 
        client.println();
        client.println("<!DOCTYPE HTML>");               // Веб-страница создается с использованием HTML
        client.println("<html>");                        // Открытие тега HTML 
        client.println("<head>");
        client.print("<title>ESP8266 TEMP</title>");     // Название страницы
        client.println("</head>");
        client.println("<body>");
        client.println("<h1>ESP8266 - Temperature & Humidity</h1>"); 
        client.println("<h3>Temperature = ");
        client.println(t);                               // Отображение температуры
        client.println("*C</h3>");
        client.println("</head>");
        client.println("<body>");
        client.println("<h3>Humidity = ");
        client.println(h);                               // Отображение влажности
        client.println("</h3>");
        client.println("</body>");
        client.println("</html>");                       // Закрытие тега HTML 
        break;                                           // Выход
      }
     
      if (c == '\n'){                                 // Если "с" равен символу новой строки                                             
        blank_line = true;                             // Тогда начинаем новую строку
      }                                          
      else if (c != '\r'){                           // Если "с" не равен символу возврата курсора на начало строки                                        
        blank_line = false;                           // Тогда получаем символ на текущей строке 
      }                                        
    }
  }  
  client.stop();                                      // Закрытие соединения
  Serial.println("Client disconnected.");             // Печать "Клиент отключен"
  }
}
