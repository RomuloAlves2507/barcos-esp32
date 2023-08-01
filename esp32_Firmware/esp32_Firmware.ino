#include "wifi.h"
#include "socketio.h"
#include "sensors.h"
#include "dados_mppt.h"

void setup()
{ 
  Serial.begin(19200);
  delay(250);
  init_wifi();
  init_socket();
  init_mppt();
  setupDHT();
  setupADS();

  pinMode(27, INPUT);
}
//FORMATAÇÃO
//CORRENTE DO MOTOR / CORRENTE DAS BATERIAS / TEMPERATURA / UMIDADE / TENSAO DE ALIMENTAÇÃO DA PCB / ESTADO DA ARRAY SOLAR 1 / ESTADO ARRAY SOLAR 2
//TENSAO NA SAIDA DO MPPT (BATT) / TENSAO NA ENTRADA DO MPPT (PLACAS) / CORRENTE DO MPPT

unsigned long previousmillis = 0;
void loop()
{
  connection_socket();
  unsigned long currentmillis = millis();

  if (currentmillis - previousmillis >= 1000)
  {
    String result = get_dados_mppt(); //Ordem dos dados: tensao das baterias(saída MPPT) / tensao do painel solar(entrada MPPT) / corrente MPPT
    float temperature = get_temperature();
    float humidity = get_humidity();
    float voltage_alimentation = get_font_voltage();
    float motor_current = get_motor_current();
    float battery_current = get_battery_current();
    int solarArray1_state = get_solarArray1_state();
    int solarArray2_state = get_solarArray2_state();
  
    String all_info =  String(motor_current) + "," + String(battery_current) + "," + String(temperature) + "," + String(humidity)
                        + "," + String(voltage_alimentation) + "," + String(solarArray1_state) + "," + String(solarArray2_state) + "," + result;
                        
    send_socket(all_info);
    previousmillis = currentmillis;

    if(digitalRead(27) == 0){
      delay(500);
      ESP.restart();
    }
  }
}
