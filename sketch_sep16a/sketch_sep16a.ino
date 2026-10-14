#include <Wire.h>
#include <TroykaIMU.h>

// Создаем фильтр для расчета углов
Madgwick filter;

// Создаем объекты акселерометра и гироскопа
Accelerometer accel;
Gyroscope gyro;

// Переменные для акселерометра (работает с float и указателями)
float ax, ay, az;

// Переменные для гироскопа (работает с int16_t и ссылками)
int16_t gx_raw, gy_raw, gz_raw;

unsigned long timer = 0;

void setup() {
  Serial.begin(9600);

  // Запускаем датчики
  accel.begin();
  gyro.begin();
  
  // Инициализируем фильтр
  filter.begin();
}

void loop() {
  // 1. Считываем данные акселерометра (в единицах g)
  accel.readGXYZ(&ax, &ay, &az);

  // 2. Считываем сырые данные гироскопа
  gyro.readXYZ(gx_raw, gy_raw, gz_raw);

  // 3. Переводим сырые данные гироскопа в радианы в секунду.
  // По умолчанию для Troyka-IMU на базе L3G4200D шаг шкалы составляет 0.00875 градусов на единицу (при 250 dps)
  // Умножаем на DEG_TO_RAD (0.01745329), чтобы получить радианы
  float gx = (float)gx_raw * 0.00875 * 0.0174532925;
  float gy = (float)gy_raw * 0.00875 * 0.0174532925;
  float gz = (float)gz_raw * 0.00875 * 0.0174532925;

  // Рассчитываем частоту обновления фильтра (fps)
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  float fps = 1000.0 / (now - lastUpdate);
  lastUpdate = now;

  // Задаем параметры фильтра (частота и коэффициент сглаживания)
  filter.setKoeff(fps, 0.22);

  // Обновляем фильтр
  filter.update(gx, gy, gz, ax, ay, az);

  // Выводим данные в монитор порта каждые 100 миллисекунд
  //if (millis() - timer >= 100) {
    //timer = millis();

    // Получаем углы в градусах
  float roll  = filter.getRollDeg();   // Наклон влево / вправо
  float pitch = filter.getPitchDeg();  // Наклон вперед / назад
  Serial.print("Наклон влево/вправо: ");
  Serial.print(roll);
  Serial.print(" °\tНаклон вперед/назад: ");
  Serial.print(pitch);
  Serial.println(" °");
  delay(100);
  //}
}
/// ВОДОРАЗДЕЛ



// подключении imu-сенсор на 10 степеней свободы v2: G в GND; V в 3.3V/5v; C в ScL; D в SDA.
#include <TroykaIMU.h> // Библиотека для работы с модулями IMU
#include <SoftwareSerial.h>

#define BT_RX 9 // порты для RX 0
#define BT_TX 10 // для TX 1 для ардуино у других плат см. свои !!!!!!!!!!!!!!!!!

SoftwareSerial BluetoothSerial(BT_RX, BT_TX); // создаём блютуз терминал с пинами RX и TX

// Создаём объект для фильтра Madgwick
Madgwick filter;
// Создаём объект для работы с гироскопом
Gyroscope gyroscope;
// Создаём объект для работы с акселерометром
Accelerometer accelerometer;
// Переменные для данных с гироскопа и акселерометра
float gx, gy, gz, ax, ay, az;
// Переменные для хранения самолётных углов ориентации
float yaw, pitch, roll;
// Переменная для хранения частоты выборок фильтра
float sampleRate = 100;
void setup() {
  // pinmode(rxPIN, INPUT);
  // pinmode(txPIN, OUTPUT);
 // Открываем последовательный порт
 Serial.begin(9600);
 BluetoothSerial.begin(9600); // открываем порт блютуза
 // Выводим сообщение о начале инициализации
 // Инициализируем гироскоп
 gyroscope.begin();
 // Инициализируем акселерометр
 accelerometer.begin();
 // Инициализируем фильтр
 filter.begin();
 // Выводим сообщение об удачной инициализации
 Serial.println(BluetoothSerial.available());
}
void loop() {
  BluetoothSerial.listen();
 // Запоминаем текущее время
 unsigned long startMillis = millis();
 // Считываем данные с акселерометра в единицах G
 accelerometer.readAccelerationGXYZ(ax, ay, az);
 // Считываем данные с гироскопа в радианах в секунду
 gyroscope.readRotationRadXYZ(gx, gy, gz);
 // Устанавливаем частоту фильтра
 filter.setFrequency(sampleRate);
 // Обновляем входные данные в фильтр
 filter.update(gx, gy, gz, ax, ay, az);
 // Получаем из фильтра углы: yaw, pitch и roll
 yaw = filter.getYawDeg(); // Коорлината Z
 pitch = filter.getPitchDeg(); //  Координата Y
 roll = filter.getRollDeg(); // Координата X


 // Выводим полученные углы Эйлера в Serial-порт
//  Serial.print("Z: ");
//  Serial.print(yaw);
//  Serial.print("\t\t");
//  Serial.print("Y: ");
//  Serial.print(pitch);
//  Serial.print("\t\t");
//  Serial.print("X: ");
//  Serial.println(roll);



//Вывод в блютуз
  // BluetoothSerial.print("Z: ");
  // BluetoothSerial.print(yaw);
  // BluetoothSerial.print("\t\t");
  // BluetoothSerial.print("Y: ");
  // BluetoothSerial.print(pitch);
  // BluetoothSerial.print("\t\t");
  // BluetoothSerial.print("X: ");
  // BluetoothSerial.println(roll);
 // Вычисляем затраченное время на обработку данных
 unsigned long deltaMillis = millis() - startMillis;
 // Вычисляем частоту обработки фильтра
 sampleRate = 1000 / deltaMillis;
 String ans = "a";
 
 if (BluetoothSerial.available() > 0) {
    BluetoothSerial.println(yaw);
    BluetoothSerial.read();

    // Serial.write(BluetoothSerial.read());
  }
//   // Serial.println("asdasd");
  if (Serial.available()) {
    BluetoothSerial.write(Serial.read());
  }
  
  // the success callback is called whenever data is received

 
}
