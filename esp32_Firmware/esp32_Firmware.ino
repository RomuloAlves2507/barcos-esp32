#include "wifi.h"
#include "socketio.h"
#include "sensors.h"
#include "dados_mppt.h"
Preferences preferences2;

void setup()
{ 
  Serial.begin(19200);
  delay(250);

  preferences2.begin("otaNamespace", false);
  bool resetar = preferences2.getBool("resetar", false);
  preferences2.end();
  if(resetar) {
    deep_sleep();
  }

  init_wifi();
  init_socket();
  init_mppt();
  //setupDHT();
  //setupADS();

}
//FORMATAÇÃO
//CORRENTE DO MOTOR / CORRENTE DAS BATERIAS / TEMPERATURA / UMIDADE / TENSAO DE ALIMENTAÇÃO DA PCB / ESTADO DA ARRAY SOLAR 1 / ESTADO ARRAY SOLAR 2
//TENSAO NA SAIDA DO MPPT (BATT) / TENSAO NA ENTRADA DO MPPT (PLACAS) / CORRENTE DO MPPT

//FORMATAÇÃO
//CORRENTE DO MOTOR / CORRENTE DAS BATERIAS / TEMPERATURA / UMIDADE / TENSAO DE ALIMENTAÇÃO DA PCB / ESTADO DA ARRAY SOLAR 1 / ESTADO ARRAY SOLAR 2
//TENSAO NA SAIDA DO MPPT (BATT) / TENSAO NA ENTRADA DO MPPT (PLACAS) / CORRENTE DO MPPT

unsigned long previousmillis = 0;
void loop() {
  connection_socket();
  unsigned long currentmillis = millis();

  // O mppt tem uma taxa lenta de envio dos seus dados, portanto se deixarmos ele no if abaixo podemos pegar o período em que ele está ocioso (não enviando dados), gerando 
  // assim erros nos dados. Da forma que está, garantimos que estamos enviando o último dado válido.

  String result = get_dados_mppt(); //Ordem dos dados: tensao das baterias(saída MPPT) / tensao do painel solar(entrada MPPT) / corrente MPPT

  if (currentmillis - previousmillis >= 1000) {
    float temperature = get_temperature();
    float humidity = get_humidity();
    //float voltage_alimentation = get_font_voltage();
    float voltage_alimentation = 0.0; //não temos mais esse dados pq a trilha queimou
    float motor_current = get_motor_current();
    float battery_current = get_battery_current();
    float solarArray1_state = get_solarArray1_state();
    float solarArray2_state = get_solarArray2_state();
  
    String version = "1";
    String all_info =  version + "," + String(motor_current) + "," + String(battery_current) + "," + String(temperature) + "," + String(humidity)
                        + "," + String(voltage_alimentation) + "," + String(solarArray1_state) + "," + String(solarArray2_state) + "," + result;


    // testando 1 a 1:

    //String all_info = version + "," + String(VREF_MOTOR) + "," + String(motor_current) + "," + String(VREF_BATERY) + "," + String(battery_current) + "," +String(voltage_alimentation) + ",6,7,8,9,10";

    //String all_info = version + ",1," + String(battery_current) + ",3,4,5,6,7,8,9,10";

    //String all_info = version + ",1,2," + String(temperature) + ",4,5,6,7,8,9,10";

    //String all_info = version + ",1,2,3," + String(humidity) + ",5,6,7,8,9,10";

    //String all_info = version + ",1,2,3,4," + String(voltage_alimentation) + ",6,7,8,9,10";

    //String all_info = version + ",1,2,3,4,5," + String(solarArray1_state) + ",7,8,9,10";

    //String all_info = version + ",1,2,3,4,5,6," + String(solarArray2_state) + ",8,9,10";

    //String all_info = version + ",1,2,3,4,5,6,7," + result;

    //String all_info = version + ",1,2,3,4,5,6,7,8,9,10";

    //Serial.println(all_info);

    send_socket(all_info);
    previousmillis = currentmillis;

    // verifica se o wifi continua conectado, necessário na troca de aparelho 
    if (WiFi.status() != WL_CONNECTED) {
      init_wifi();
      init_socket();
    }

  }
}
