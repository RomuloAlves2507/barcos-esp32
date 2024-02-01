/* Connections:
    mppt pin     mppt        ESP         ESP pin
    1            GND         GND         GND
    2            RX          TX          -              usar APENAS se for desejável enviar informaçoes (programar) o mppt. Para mais informações, ler documento XXXXX.
    3            TX          RX          16
    4            Power+      none        -              não usar!
*/



#include "dados_mppt.h"

typedef struct
{
  String ERR1 = "     Geen";
  String CS0 =  "      Uit";  //estado de operação - strings
  String CS2 =  "     Fout";
  String CS3 =  "     Bulk";
  String CS4 =  "Absorptie";
  String CS5 =  "  Druppel";
  float tensao_bateria;         // TENSAO NA SAIDA NO MPPT, Batt
  float corrente_carregamento;  
  float tensao_painel;    // TENSAO DOS PAINEIS SOLARES ("ENTRADA" DO MPPT, PV)
 // float rendimento_ontem;       
 // float potencia_ontem;         
  //float rendimento_hoje;        
  //float maxima_potencia;
  uint16_t erro_counter = 0;
} dadosmppt;

dadosmppt dm;

unsigned long previousmpptUpdate = 0;
int UPDATE_PERIOD_MS = 1000;

#define RXD2 16
#define TXD2 17

void init_mppt(void)
{

  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  
}

void read_mppt()
{
  //SE A PORTA SERIAL NÃO ESTIVER CONECTADA VAI SEMPRE MANDAR MENSAGEM DE ERRO COM VALORES NEGATIVOS
  if (!Serial2.available()){

    dm.erro_counter++;   //Num ciclo do mppt, ele gera muitos mais erros do que dados (fica um tempo ocioso). Com isso enviamos o último valor válido e, se tivermos 4000 erros no
    // Serial2.available enviamos o -1 (erro de leitura). O 4000 foi um valor obtido empiricamente, não tem uma lógica por trás
    
    if(dm.erro_counter > 4000){
    dm.tensao_bateria = -1.0;
    dm.corrente_carregamento =-1.0;
    dm.tensao_painel = -1.0;
    //dm.rendimento_ontem = -1.0;
    //dm.potencia_ontem = -1.0;
    //dm.rendimento_hoje = -1.0;
    //dm.maxima_potencia = -1.0;
    }

  }
  else {                                                  //  The device transmits blocks of data at 1 second intervals. Each field is sent using the following format:
    dm.erro_counter = 0;
    String label, val;                                    // <Newline><Field-Label><Tab><Field-Value>
    label = Serial2.readStringUntil('\t');                // this is the actual line that reads the label from the mppt controller
    val = Serial2.readStringUntil('\r\r\n');              // this is the line that reads the value of the label
    char buf[45];
    
    if (label == "I")   //Medido em mA                    // I chose to select certain paramaters that were good for me. check the Victron whitepaper for all labels.
    {                                                     // In this case I chose to read charging current   
      val.toCharArray(buf, sizeof(buf));                  // conversion of val to a character array. Don't ask me why, I saw it in one of the examples of Adafruit and it works.
      dm.corrente_carregamento = atof(buf);               // conversion to float
      dm.corrente_carregamento /= 1000;                   // calculating the correct value, see the Victron whitepaper for details. The value of label I is communicated in milli amps.
    }
    if (label == "V")   //Medido em mV
    {
      val.toCharArray(buf, sizeof(buf));                  // By studying these small routines, you can modify to reading the parameters you want,
      dm.tensao_bateria = atof(buf);                      // converting them to the desired value (milli-amps to amps or whatever)
      dm.tensao_bateria /= 1000;   
   }
    if (label == "VPV")   //Medido em mV
    {
      val.toCharArray(buf, sizeof(buf));                  // By studying these small routines, you can modify to reading the parameters you want,
      dm.tensao_painel = atof(buf);
      dm.tensao_painel /= 1000;                          // converting them to the desired value (milli-amps to amps or whatever)
    }
    /*if (label == "H20")   //Medido em 0.01 kWh
    {  
      val.toCharArray(buf, sizeof(buf));
      dm.potencia_ontem = atof(buf);
      dm.potencia_ontem /= 1000;    
    }
    if (label == "H21")   //Medida em W
    {  
      val.toCharArray(buf, sizeof(buf));
      dm.maxima_potencia = atof(buf);   
    }
    
    if (label == "H22")   //Medida em mW
    {
      val.toCharArray(buf, sizeof(buf));
      dm.rendimento_ontem = atof(buf);
      dm.rendimento_ontem /= 1000;
    }
    if (label == "H23")   //Medida em mW
    {
      val.toCharArray(buf, sizeof(buf));
      dm.potencia_ontem = atof(buf);
      dm.potencia_ontem /= 1000;
    }*/
    /*
    else if (label == "ERR")                               // This routine reads the error code. If there is no error, "Geen" will be printed. See line 24 of this sketch
    {                                                      // else the actual error code will be printed. Refer to the Victron whitepaper for reference.
      val.toCharArray(buf, sizeof(buf));
      int intvalor_medido = atoi(buf);
      if (intvalor_medido == 0)
      {
        dm.ERR1.toCharArray(dm.mensagem_erro, len);
      }
      else if (intvalor_medido != 0)
      {
        //itoa (dm.mensagem_erro, intvalor_medido, 12);
      }

    }
    else if (label == "CS")                                  // this routine reads Charge Status, see lines 25 - 29
    {
      val.toCharArray(buf, sizeof(buf));
      int intvalor_medido = atoi(buf);
      if (intvalor_medido == 0)
      {
        dm.CS0.toCharArray(dm.estado_carregamento, len);
      }
      else if (intvalor_medido == 2)
      {
        dm.CS2.toCharArray(dm.estado_carregamento, len);
      }
      else if (intvalor_medido == 3)
      {
        dm.CS3.toCharArray(dm.estado_carregamento, len);
      }
      else if (intvalor_medido == 4)
      {
        dm.CS4.toCharArray(dm.estado_carregamento, len);
      }
      else if (intvalor_medido == 5)
      {
        dm.CS5.toCharArray(dm.estado_carregamento, len);
      }
      dm.estado_carregamento[len] = ' '; 
      dm.estado_carregamento[len + 1] = 0;

      //Serial.print("CS: ");
      //Serial.println(estado_carregamento);
    }*/
  }
}

String get_dados_mppt()
{
  read_mppt();
  String result = "";
  result = String(dm.tensao_bateria) + "," + String(dm.tensao_painel) + "," + String(dm.corrente_carregamento);
  return result;
}
