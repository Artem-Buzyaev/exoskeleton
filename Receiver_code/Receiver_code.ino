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
