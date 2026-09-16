#include <TroykaIMU.h>

// Создаем фильтр для расчета углов
Madgwick filter;

// Создаем объекты акселерометра и гироскопа
Accelerometer accel;
Gyroscope gyro;

unsigned long timer = 0;

void setup() {
  Serial.begin(9600);

  // Запускаем датчики. Библиотека сама внутри включит I2C (Wire)
  accel.begin();
  gyro.begin();
  
  // Запускаем фильтр на частоту 100 Герц
  filter.begin(100); 
}

void loop() {
  // Очень быстро считываем сырые данные
  accel.readGXYZ();
  gyro.readRadXYZ();

  // Обновляем математический фильтр
  filter.update(gyro.getGyroX_rads(), gyro.getGyroY_rads(), gyro.getGyroZ_rads(), 
                accel.getAccelX_g(), accel.getAccelY_g(), accel.getAccelZ_g());

  // Выводим градусы в монитор порта каждые 100 миллисекунд
  if (millis() - timer >= 100) {
    timer = millis();

    // Переводим данные в понятные углы (градусы)
    float roll  = filter.getRollDeg();   // Наклон влево / вправо
    float pitch = filter.getPitchDeg();  // Наклон вперед / назад

    Serial.print("Наклон влево/вправо: ");
    Serial.print(roll);
    Serial.print(" °\tНаклон вперед/назад: ");
    Serial.print(pitch);
    Serial.println(" °");
  }
}