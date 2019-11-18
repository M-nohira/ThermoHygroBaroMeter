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

//MODE SETTINGS             ---
#define MODE_TEMP 0
#define MODE_HUMI 1
#define MODE_PRES 2
#define SWITCH_TIME 5000
//DISPLAY TIMING SETTINGS   ---
#define DISPLAY_HIGH_DELAY 3
#define DISPLAY_LOW_DELAY 1
//DISPLAY SETTINGS          ---
#define USE_SMALL_DEGREEC 1
#define USE_FAHRENHEIT 0

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS);

int mode = MODE_PRES;
long last_time = 0;

byte num[20] =
    {
        0b00111111, //0
        0b00000110, //1
        0b01011011, //2
        0b01001111, //3
        0b01100110, //4
        0b01101101, //5
        0b01111101, //6
        0b00100111, //7
        0b01111111, //8
        0b01101111, //9
        0b01110111, //A 10
        0b01111100, //b 11
        0b00111001, //C 12
        0b01011110, //d 13
        0b01111001, //E 14
        0b01110001, //F 15
        0b01110110, //H 16
        0b00011111, //J 17
        0b01000000, //- 18
        0b01011000, //small C 19

};

byte buff[4];

void SetNum(int nums);
void Set_SR(byte pattern);
void UpdateDisplay(byte display[4]);
void SetDigOut(int dig);

void setup()
{
    pinMode(SER, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(SRCLK, OUTPUT);
    pinMode(SRCLR, OUTPUT);
    pinMode(DI1, OUTPUT);
    pinMode(DI2, OUTPUT);
    pinMode(DI3, OUTPUT);
    pinMode(DI4, OUTPUT);
    digitalWrite(SRCLR, HIGH);
    bme.begin();
}

void loop()
{

    int dpCNT = 0;

    if (last_time + SWITCH_TIME <= millis())
    {
        mode = (mode == MODE_TEMP) ? MODE_HUMI : (mode == MODE_HUMI) ? MODE_PRES : MODE_TEMP;
        last_time = millis();
    }

    if (mode == MODE_TEMP)
    {

        float f_temp = bme.readTemperature();
        bool isMinus = false;
        dpCNT = 1;

        //華氏表示用処理===========
        if (USE_FAHRENHEIT == 1)
            f_temp = (f_temp * 1.8) + 32;

        if (f_temp >= 100)
        {
            f_temp / 10;
            dpCNT = 0;
        }
        // 氷点下表示用処理==============
        if (f_temp < 0)
        {
            f_temp = abs(f_temp);
            isMinus = true;
        }
        if (f_temp <= -10.00F)
        {
            f_temp = abs(f_temp / 10);
            dpCNT = 0;
        }
        //====================================

        int i_temp = (f_temp * 100); //25.55 => 2555

        SetNum(i_temp);

        if (isMinus == true)
            buff[0] = 18; //第一桁目をマイナスにする

        //単位表示用設定
        if (USE_FAHRENHEIT == 1)
            buff[3] = num[15];
        else if (USE_SMALL_DEGREEC)
            buff[3] = num[19];
        else
            buff[3] = num[12];
    }
    if (mode == MODE_HUMI)
    {
        int humi = (bme.readHumidity() * 10); //45.41 => 454.1
        SetNum(humi);
        dpCNT = 2;
        buff[0] = num[16]; //H
    }
    if (mode == MODE_PRES)
    {
        int pres = bme.readPressure() / 100.0F;
        dpCNT = 5;
        SetNum(pres);
    }

    UpdateDisplay(buff, dpCNT);
}

void SetNum(int number)
{
    int nums[4] = {(number / 1000), ((number % 1000) / 100), ((number % 100) / 10), ((number % 10))};
    buff[0] = num[nums[0] <= 9 ? nums[0] : 14];
    buff[1] = num[nums[1] <= 9 ? nums[1] : 14];
    buff[2] = num[nums[2] <= 9 ? nums[2] : 14];
    buff[3] = num[nums[3] <= 9 ? nums[3] : 14];
}

void Set_SR(byte pattern)
{
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, pattern);
    digitalWrite(RCLK, HIGH);
}

void UpdateDisplay(byte display[4], int dp)
{
    for (int cnt = 0; cnt < 4; cnt++)
    {
        byte segment = display[cnt] | ((cnt == dp) ? 0b10000000 : 0x00);

        SetDigOut(cnt + 1);
        Set_SR(segment);
        delay(DISPLAY_HIGH_DELAY);

        digitalWrite(RCLK, LOW);
        digitalWrite(SRCLR, HIGH);
        digitalWrite(SRCLR, LOW);
        digitalWrite(SRCLR, HIGH);
        digitalWrite(RCLK, HIGH);

        delay(DISPLAY_LOW_DELAY);
    }
}

void SetDigOut(int dig)
{
    digitalWrite(DI1, dig == 1 ? LOW : HIGH);
    digitalWrite(DI2, dig == 2 ? LOW : HIGH);
    digitalWrite(DI3, dig == 3 ? LOW : HIGH);
    digitalWrite(DI4, dig == 4 ? LOW : HIGH);
}