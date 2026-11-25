#include <TroykaIMU.h> // Библиотека для работы с модулями IMU

#include <nRF24L01.h>  // Подключаем тип передатчика из библиотеки RF24
#include <RF24.h>  // Библиотека с работой радиопередачиков 

#include <async.h> // Библиотека для асинхронной работы для датчика наклона





Async asyncEngine = Async(); // создаем объект из Async

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

float x = 0.01; // переменная для коректировки оси Z можно откалибровать и тогда убрать

float dataToBeTransmitted[3]; // Передоваемый массив

int i = 0; // счетчик ля вывода передоваемого
RF24 radio(9, 10);
void setup() {

 Serial.begin(9600); // открываем последовательный порт
 
 radio.begin(); //  Инициируем работу nRF24L01+
 radio.setChannel(112); //Указываем канал передачи данных(от 0 до 125)без шумов
 radio.setDataRate(RF24_2MBPS); // Скорость передачи
 radio.setPALevel(RF24_PA_MIN); // Указываем мощность
 radio.openWritingPipe(0x0123456789LL); // Ключ по которому передаем можно только один

 gyroscope.begin(); // Инициализируем гироскоп
 accelerometer.begin(); // Инициализируем 
 filter.begin(); // Устанавливаем калибровочные данные
}
void loop() {
 asyncEngine.run(); // Запуск асинхронной функции
 short id = asyncEngine.setInterval(calculations, 10); // добавить функцию в асинхронный запуск

// (2)

 radio.write(&dataToBeTransmitted, sizeof(dataToBeTransmitted)); // Передаем массив и количество бит sizeof(dataToBeTransmitted) -  опредляет сколько нужно массиву
}


void calculations() { // Асинхронная функция для измерения угла

 unsigned long startMillis = millis(); // Запоминаем время
 // Считываем данные с акселерометра в единицах G
 accelerometer.readAccelerationGXYZ(ax, ay, az);

 // Считываем данные с гироскопа в радианах в секунду
 gyroscope.readRotationRadXYZ(gx, gy, gz);

 filter.setFrequency(sampleRate);  // Устанавливаем частоту фильтра
 // Обновляем входные данные в фильтр

 filter.update(gx, gy, gz, ax, ay, az);
 // Получаем из фильтра углы: yaw, pitch и roll
 yaw = filter.getYawDeg() - x; // Коорлината Z
 pitch = filter.getPitchDeg(); //  Координата Y
 roll = filter.getRollDeg(); // Координата X


// (1)


 
 unsigned long deltaMillis = millis() - startMillis; // время измерения

 dataToBeTransmitted[0] = roll; // Координата X
 dataToBeTransmitted[1] = pitch; //  Координата Y
 dataToBeTransmitted[2] = yaw; // Коорлината Z

 sampleRate = 1000 / deltaMillis; // Вычисляем частоту обработки фильтра через период
//  String ans = "a";
 x = x + 0.0005; // та самая перемнная для коректировки датчика наклона
}







 // Выводим полученные углы Эйлера в Serial-порт подставить для вывода в (1)
//  Serial.print("Z: ");
//  Serial.print(yaw);
//  Serial.print("\t\t");
//  Serial.print("Y: ");
//  Serial.print(pitch);
//  Serial.print("\t\t");
//  Serial.print("X: ");
//  Serial.println(roll);


// Вывод передачи (2)
//  Serial.print("[ ");
//  for (i = 0; i < 3; i++) {
//       Serial.print(dataToBeTransmitted[i]);
//       Serial.print(' ');
//     }
//  Serial.println("]");
