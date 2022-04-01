      // точное напряжение на пине 5V (в данном случае зависит от стабилизатора на плате Arduino)
float R1 = 140000.0;
float R2 = 39000.0;
void setup() {

  // Объявляем работу с последоватлеьным портом в самом начале
  Serial.begin(9600);
  pinMode(0, INPUT);
  // Теперь мы можем писать сообщения
  Serial.println ("Hello, Arduino Master");
}

void loop() {
  float value = analogRead(0);
  float  vout = (value * 5.03) / 1024.0;
  float vin = vout / (R2/(R1+R2));
  Serial.println(vin);
  if (vin<0.09) {
    vin=0.0;// обнуляем нежелательное значение
  }
}
