#include "sensors.h"

Adafruit_ADS1115 ads_1115;

DHT dht(PIN_DHT, DHTTYPE);


//TODO: COLOCAR LINKS PRAS DOCUMENTACOES E DATASHEETS

float reference_battery_value(){
  // float ref_battery_value = analogRead(PIN_VREF_BATTERY);
  // return ref_battery_value* 5 / 4095.0;
  
  ads_1115.setGain(GAIN_ONE);

  int16_t result = ads_1115.readADC_SingleEnded(3);
  float voltageRead = ads_1115.computeVolts(result);

  return voltageRead;
}

float get_motor_current(){
    //como o sinal de tensão q vem do sensor de corrente varia no max 0.625V, 
    //acho que o ganho 4 é melhor pq é mais preciso (mas aí precisa ler só o offset de tensão):
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit =      0.125mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit =       0.03125mV
    ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625 //TESTAR OUTROS GANHOS
   
    int16_t result = ads_1115.readADC_SingleEnded(ADS1115_MOTOR_CURRENT);
    float voltageRead = ads_1115.computeVolts(result);

    float motorAmps = (voltageRead / RESISTANCE_MOTOR);

    return motorAmps;
}

float get_battery_current(){
  ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625
  
  float VREF_BATTERY = reference_battery_value();
  
  int16_t result = ads_1115.readADC_SingleEnded(ADS1115_BATTERY_CURRENT);
  float voltageRead = ads_1115.computeVolts(result);

  float hallAmps = 0;

  hallAmps = (voltageRead - VREF_BATTERY) * (200 / HSTS016L_VARIATION);

  return hallAmps;
}

float get_font_voltage(){
  // ads_1115.setGain(GAIN_ONE);        // 1x gain   + 4.096V  1 bit = 0.0625

  // int16_t result = ads_1115.readADC_SingleEnded(ADS1115_FONT_VOLTAGE);
  // float voltageRead = ads_1115.computeVolts(result); 
  //Find voltage before voltage divider
  float R1 = 47000.0, R2 = 15000.0;  

  float ref_motor_value = analogRead(35);
  float voltageRead = ref_motor_value* 3.3 / 4095.0;
  
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

//Implementação do botão Reset
/*
void initResetButton() {
  pinMode(PIN_RESET_BUTTON, INPUT);
}

void checkResetButton() {
  if (digitalRead (PIN_RESET_BUTTON) == HIGH) {
    Serial.println("RESETOU!!!");
    delay(2000);
    ESP.restart();
  }
}
*/
