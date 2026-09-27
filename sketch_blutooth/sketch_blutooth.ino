#include <SoftwareSerial.h>

// Назначаем пины: RX = D1 (GPIO5), TX = D2 (GPIO4)
#define BT_RX 5  // На ESP8266 подключается к TXD на HC-06
#define BT_TX 4  // На ESP8266 подключается к RXD на HC-06

SoftwareSerial BluetoothSerial(BT_RX, BT_TX);

void setup() {
  // Запуск аппаратного Serial для общения с ПК
  Serial.begin(115200);
//  Serial.println("ESP8266 готов к работе. Инициализация Bluetooth...");

  // Запуск программного Serial для общения с HC-06 (обычно скорость 9600)
  BluetoothSerial.begin(9600); 
  Serial.println("В Мониторе порта выставьте скорость 115200.");
}

void loop() {
  // Если пришли данные от смартфона/ПК через Bluetooth — выводим в Монитор порта
  if (BluetoothSerial.available()) {
    Serial.write(BluetoothSerial.read());
  }

  // Если мы пишем в Монитор порта на ПК — отправляем это по Bluetooth
  if (Serial.available()) {
    BluetoothSerial.write(Serial.read());
  }
}
