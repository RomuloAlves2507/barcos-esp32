#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
//#include <MCP3221.h>
#include <DHT.h>
 

//I2C
// datasheet https://www.filipeflop.com/img/files/download/Datasheet_ADC_ads1115.pdf
#define I2C_ADS1115  0x48  // Endereco do medidor ADC da bateria A0 - Batery Bank voltage // A1 - PV modules voltage

//ADS read
#define LIMIT_GAIN_ONE 4096.0
#define LIMIT_GAIN_TWO_THIRDS 6144.0
#define RESOLUTION_16BIT 65536.0
#define ADS1115_MOTOR_CURRENT 0 //HSTS016L 100A - CORRENTE DO MOTOR
#define ADS1115_BATTERY_CURRENT 1 //HSTS016L 150A - CORRENTE QUE SAI DO BANCO DE BATERIAS
#define ADS1115_FONT_VOLTAGE 2 //TENSAO DE ALIMENTACAO DO ESP32
#define ADS1115_MPPT 3 //PORTA VAZIA

//Constants
#define ESP_MAXIMUM_VOLTAGE_IN 3.3
#define ACS712_OUTPUT_SENSITIVITY 0.0833 // 83.3 mV/A - https://www.allegromicro.com/~/media/files/datasheets/acs712-datasheet.ashx
#define ACS712_VCC 5.0
#define DHTTYPE DHT11 // DHT 11
#define HSTS016L_NOMINAL_CURRENT_100A 100.0
#define HSTS016L_NOMINAL_CURRENT_150A 150.0
#define HSTS016L_VARIATION 0.625 
#define ACS_1_AVARAGE 2.5
#define ACS_2_AVARAGE 2.5
#define VREF_100A 2.5
#define VREF_150A 2.5

//Esp pins
#define PIN_ACS_1 34 //GPIO34
#define PIN_ACS_2 35 //GPIO35
#define PIN_DHT 15 //GPIO15 pino que estamos conectado

//Funções
float get_motor_current();
float get_battery_current();
float get_font_voltage();

float get_solarArray1_state();
float get_solarArray2_state();

float get_font_current();
float get_mppt_current();

float get_temperature();
float get_humidity();

void setupDHT();
void setupADS();

#endif
