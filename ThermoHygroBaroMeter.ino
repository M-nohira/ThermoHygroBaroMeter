#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SER 0
#define RCLK 1
#define SRCLK 2
#define SRCLR 3

#define DI1 5
#define DI2 6
#define DI3 7
#define DI4 8

#define MODE_TEMP   0
#define MODE_HUMI   1
#define MODE_PRES   2

#define SWITCH_TIME 5000
#define DELAY 1

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS);

int mode = 0;
long last_time;

byte num[10] =
{
    0b00111111,//0
    0b00000110,//1
    0b01011011,//2
    0b01001111,//3
    0b01100110,//4
    0b01101101,//5
    0b01111001,//6
    0b00100111,//7
    0b01111111,//8
    0b01101111,//9
};

void Set_SR(byte pattern);
void Toggle_Pin(int pin,int delay);

void setup()
{
    bme.begin();
    pinMode(SER,OUTPUT);
    pinMode(RCLK,OUTPUT);
    pinMode(SRCLK,OUTPUT);
    pinMode(SRCLR,OUTPUT);
    pinMode(DI1,OUTPUT);
    pinMode(DI2,OUTPUT);
    pinMode(DI3,OUTPUT);
    pinMode(DI4,OUTPUT);
    digitalWrite(SRCLR,HIGH);

}

void loop()
{
    byte buff[4];
    if(last_time + SWITCH_TIME <= millis())
    {
        mode = (mode == MODE_TEMP)? MODE_HUMI:(mode == MODE_HUMI)? MODE_PRES:MODE_TEMP;
    }

    if(mode == MODE_TEMP)
    {
        int temp = (bme.readTemperature() * 100);
        buff[0] = num[temp / 1000];
        buff[1] = num[(temp % 1000) / 100];
        buff[2] = num[(temp % 100) / 10];
        buff[3] = num[(temp % 10)];
    }
    if(mode == MODE_HUMI)
    {
        int humi = (bme.readHumidity() * 100);
        buff[0] = num[ humi / 1000];
        buff[1] = num[(humi % 1000) / 100];
        buff[2] = num[(humi % 100) / 10];
        buff[3] = num[(humi % 10)];
    }
    if(mode == MODE_PRES)
    {
        int pres = bme.readPressure();
        buff[0] = num[ pres / 1000];
        buff[1] = num[(pres % 1000) / 100];
        buff[2] = num[(pres % 100) / 10];
        buff[3] = num[(pres % 10)];

    }

}

void Set_SR(byte pattern)
{
    digitalWrite(RCLK,LOW);
    for(int cnt = 0; cnt < 8; cnt++)
    {
        int dSER = pattern & (0b1 >> cnt);
        digitalWrite(SER,dSER);
        Toggle_Pin(SRCLK);
    }
    Toggle_Pin(RCLK);
}

void Toggle_Pin(int pin)
{
    digitalWrite(pin,LOW);
    digitalWrite(pin,HIGH);
    delay(DELAY);
    digitalWrite(pin,LOW);
}