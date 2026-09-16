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
