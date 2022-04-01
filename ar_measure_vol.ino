#include "DHT.h"
#include <TroykaMQ.h>
#define DHTPIN 2
#define PIN_MQ135         A1
#define PIN_MQ135_HEATER  11
MQ135 mq135(PIN_MQ135);

float R1 = 140000.0;
float R2 = 39000.0;
DHT dht(DHTPIN, DHT22);
void setup() {
  
  // Объявляем работу с последоватлеьным портом в самом начале
  Serial.begin(9600);
  dht.begin();
  pinMode(1, INPUT);
  pinMode(0, INPUT);
  mq135.heaterPwrHigh();
  Serial.println("Heated sensor");
  // Теперь мы можем писать сообщения
  Serial.println ("Hello, Arduino Master");
}

void loop() {
  float h = dht.readHumidity(); //Измеряем влажность
  float t = dht.readTemperature();
  float value = analogRead(0);
  float  vout = (value * 5.03) / 1024.0;
  float vin = vout / (R2/(R1+R2));
  if (!mq135.isCalibrated() && mq135.heatingCompleted()) {
    // выполняем калибровку датчика на чистом воздухе
    mq135.calibrate();
    // если известно сопротивление датчика на чистом воздухе 
    // можно его указать вручную, допустим 160
    // mq135.calibrate(160);
    // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
    Serial.print("Ro = ");
    Serial.println(mq135.getRo());
  }
  if (mq135.isCalibrated() && mq135.heatingCompleted()) {
    // выводим отношения текущего сопротивление датчика
    // к сопротивлению датчика в чистом воздухе (Rs/Ro)
    Serial.print("Ratio: ");
    Serial.print(mq135.readRatio());
    // выводим значения газов в ppm
    Serial.print("\tCO2: ");
    Serial.print(mq135.readCO2());
    Serial.println(" ppm");
    delay(1000);
  }

  Serial.print("Напряжение: ");
  Serial.println(vin);
  delay(1000);
  if (vin<0.09) {
    vin=0.0;// обнуляем нежелательное значение
  }
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }
  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" *C "); //Вывод показателей на экран
  delay(1000);
}

