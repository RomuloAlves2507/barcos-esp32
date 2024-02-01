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

 // pinMode(27, INPUT);
}
//FORMATAÇÃO
//CORRENTE DO MOTOR / CORRENTE DAS BATERIAS / TEMPERATURA / UMIDADE / TENSAO DE ALIMENTAÇÃO DA PCB / ESTADO DA ARRAY SOLAR 1 / ESTADO ARRAY SOLAR 2
//TENSAO NA SAIDA DO MPPT (BATT) / TENSAO NA ENTRADA DO MPPT (PLACAS) / CORRENTE DO MPPT

unsigned long previousmillis = 0;
void loop()
{
  connection_socket();
  unsigned long currentmillis = millis();

  // O mppt tem uma taxa lenta de envio dos seus dados, portanto se deixarmos ele no if abaixo podemos pegar o período em que ele está ocioso (não enviando dados), gerando 
  // assim erros nos dados. Da forma que está, garantimos que estamos enviando o último dado válido.

  String result = get_dados_mppt(); //Ordem dos dados: tensao das baterias(saída MPPT) / tensao do painel solar(entrada MPPT) / corrente MPPT

  if (currentmillis - previousmillis >= 1000)
  {
    float temperature = get_temperature();
    float humidity = get_humidity();
    //float voltage_alimentation = get_font_voltage();
    float voltage_alimentation = 0.0; //não temos mais esse dados pq a trilha queimou
    float motor_current = get_motor_current();
    float battery_current = get_battery_current();
    float solarArray1_state = get_solarArray1_state();
    float solarArray2_state = get_solarArray2_state();
  
    String all_info =  String(motor_current) + "," + String(battery_current) + "," + String(temperature) + "," + String(humidity)
                        + "," + String(voltage_alimentation) + "," + String(solarArray1_state) + "," + String(solarArray2_state) + "," + result;

    //Serial.println(all_info);
    send_socket(all_info);
    previousmillis = currentmillis;

  }
}
