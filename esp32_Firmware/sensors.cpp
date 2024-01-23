#include "sensors.h"

Adafruit_ADS1115 ads_1115;

DHT dht(PIN_DHT, DHTTYPE);


//####################### FUNÇÕES DE LEITURA DOS DADOS DO ADS1115
float get_motor_current(){
    ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625
    
    int16_t result = ads_1115.readADC_SingleEnded(ADS1115_MOTOR_CURRENT);
    float voltageRead = result*(2*LIMIT_GAIN_ONE/(RESOLUTION_16BIT-1.0))/1000.0; 

    float hallAmps = 0;

    hallAmps = (voltageRead - VREF_100A) * (HSTS016L_NOMINAL_CURRENT_100A / HSTS016L_VARIATION);

    return hallAmps;
}

float get_battery_current(){
    ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625

  int16_t result = ads_1115.readADC_SingleEnded(ADS1115_BATTERY_CURRENT);
  float voltageRead = result*(2*LIMIT_GAIN_ONE/(RESOLUTION_16BIT-1.0))/1000.0; 
  float hallAmps = 0;
  if (voltageRead >= VREF_150A)
      hallAmps = (voltageRead - VREF_150A) * (HSTS016L_NOMINAL_CURRENT_150A / HSTS016L_VARIATION);
  else if (voltageRead < VREF_150A)
      hallAmps = (VREF_150A - voltageRead) * (HSTS016L_NOMINAL_CURRENT_150A / HSTS016L_VARIATION);
  return hallAmps;
}

float get_font_voltage(){
  ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625

  int16_t result = ads_1115.readADC_SingleEnded(ADS1115_FONT_VOLTAGE);
  float voltageRead = result*(2*LIMIT_GAIN_ONE/(RESOLUTION_16BIT-1.0))/1000.0; 
  //Find voltage before voltage divider
  float R1 = 47000.0, R2 = 15000.0;  
  
  return voltageRead * (R1 + R2)/R2;
}

//####################### LEITURA DOS DADOS DAS ENTRADAS ANALÓGICAS DAS PORTAS GPIO34 E GPIO35 - ACS712

float get_solarArray1_state(){
  int bitsRead = analogRead(PIN_ACS_1);
  float voltageRead = bitsRead * ESP_MAXIMUM_VOLTAGE_IN / 4095.0;  //Convert from bits to the float number representing the voltage read

  /*int array_state = 0;

  if (abs((voltageRead - ACS_1_AVARAGE)) > 0.2){
    array_state = 1;
  }
  */

  float ampRead = abs(voltageRead - ACS_2_AVARAGE)/ACS712_OUTPUT_SENSITIVITY;
  
  return ampRead;

}

float get_solarArray2_state(){
  int bitsRead = analogRead(PIN_ACS_2);
  float voltageRead = bitsRead * ESP_MAXIMUM_VOLTAGE_IN / 4095.0;  //Convert from bits to the float number representing the voltage read
  
  /*int array_state = 0;

  if (abs((voltageRead - ACS_2_AVARAGE)) > 0.2){
    array_state = 1;
  }*/

  float ampRead = abs(voltageRead - ACS_2_AVARAGE)/ACS712_OUTPUT_SENSITIVITY;

  return ampRead;
}

//####################### LEITURA DOS DADOS DAS RELACIONADAS AO DHT
float get_temperature(){
  float t = dht.readTemperature();
  if(isnan(t)){
    return -5.0;
  }
  else{
    return t;
  }
}

float get_humidity(){
  float t = dht.readHumidity();
  if(isnan(t)){
    return -5.0;
  }
  else{
    return t;
  }
}

//#######################
void setupDHT(){
  dht.begin();
}

void setupADS(){
    ads_1115.begin();
}
