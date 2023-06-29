#include "socketio.h"

SocketIOclient socket;

// inicializa o socket.io-client
void init_socket()
{
  // socket.begin("192.168.15.43", 4000, "/socket.io/?EIO=4");
  // socket.begin("server-rastreador-barco.herokuapp.com", 80, "/socket.io/?EIO=4");
  socket.beginSSL("barcos-backend.onrender.com", 443, "/socket.io/?EIO=4");

  socket.onEvent(socketIOEvent);
  Serial.println("Configurado o socket.io");
  delay(500);
}

void connection_socket()
{
  socket.loop();
}


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            Serial.printf("[IOc] Desconectado!\n");
            break;
        case sIOtype_CONNECT:
            Serial.printf("[IOc] Conectado a url: %s\n", payload);
            socket.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
            // Serial.println((char *)&payload[0]);           
            break;
    }
}


// envia uma mensagem para o servidor via socket
void send_socket(String info)
{
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  array.add("newinfo");

  // add payload (parameters) for the event
  array.add(info);

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socket.sendEVENT(output);
}
