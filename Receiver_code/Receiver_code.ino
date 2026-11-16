#include <nRF24L01.h>
/*
  Подключаем библиотеку  для работы с nRF24L01+
*/
#include <RF24.h>

/*
  Создаём объект radio для работы с библиотекой RF24,
  указывая номера выводов модуля (CE, SS).
*/
RF24 radio(9, 10);

/*
  Объявляем массив для хранения и передачи данных
  (до 32 байт включительно).
*/
int receivedData[5];

uint8_t pipe;

uint8_t i;
void setup() {
  /*
    Инициируем передачу данных по шине UART в монитор
    последовательного порта на скорости 115200 бит/сек.
  */
  Serial.begin(115200);
  /*
    Инициируем работу nRF24L01+
  */
  radio.begin();
  /*
    Указываем канал передачи данных (от 0 до 125)
    (один приёмник может принимать данные от нескольких передатчиков — до 6 труб).
    Выбираем канал в котором нет шумов!
  */
  radio.setChannel(112);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, 0x0123456789LL);
  radio.startListening();
}
void loop() {
  /*
    Если в буфере имеются принятые данные, то получаем номер трубы
    по которой эти данные пришли в переменную pipe.
  */
  if (radio.available(&pipe)) {
    /*
      Читаем данные из буфера в массив receivedData указывая
      сколько всего байт может поместиться в массив.
    */
    radio.read(&receivedData, sizeof(receivedData));
    /*
      Если данные пришли от 1 передатчика (по 1 трубе),
      то можно выполнить соответствующее действие ...
    */
    Serial.print("Данные [ ");
    for (i = 0; i < 5; i++) {
      Serial.print((char) receivedData[i]);
      Serial.print(' ');
    }
    Serial.print("] пришли по трубе ");
    Serial.println(pipe);
  }
}




///////////////ВРЕМЕННО
#include <TroykaIMU.h> // Библиотека для работы с модулями IMU
#include <SoftwareSerial.h>
#include <nRF24L01.h>
/*
  Подключаем библиотеку  для работы с nRF24L01+
*/
#include <RF24.h>





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
RF24 radio(9, 10);
void setup() {
  // pinmode(rxPIN, INPUT);
  // pinmode(txPIN, OUTPUT);
 // Открываем последовательный порт
 Serial.begin(9600);

 radio.begin();
 radio.setChannel(112);
 radio.setDataRate(RF24_2MBPS);
 radio.setPALevel(RF24_PA_MIN);
 radio.openWritingPipe(0x0123456789LL);
 // Выводим сообщение о начале инициализации
 // Инициализируем гироскоп
 gyroscope.begin();
 // Инициализируем акселерометр
 accelerometer.begin();
 // Инициализируем фильтр
 filter.begin();
 // Выводим сообщение об удачной инициализации
int dataToBeTransmitted[5] = {'6', '1', '2', '3', '4'};
}
void loop() {

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
 Serial.print("Z: ");
 Serial.print(yaw);
 Serial.print("\t\t");
 Serial.print("Y: ");
 Serial.print(pitch);
 Serial.print("\t\t");
 Serial.print("X: ");
 Serial.println(roll);



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
 int dataToBeTransmitted[3] = {4, 5, 6};
 radio.write(&dataToBeTransmitted, sizeof(dataToBeTransmitted));
 

  // the success callback is called whenever data is received

 
}
