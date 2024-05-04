#include "socketio.h"
SocketIoClient socket;

Preferences preferences; 

// char host[] = "192.168.1.7"; // Socket.IO Server Address
char host[] = "barcos-backend.onrender.com"; // Socket.IO Server Address
int port = 443; // Socket.IO Port Address
// int port = 4000; // Socket.IO Port Address
char path[] = "/socket.io/?EIO=3&transport=websocket"; // Socket.IO Base Path
bool useSSL = true; // Use SSL Authentication
const char * sslFingerprint = "";  // SSL Certificate Fingerprint

void socket_Connected(const char * payload, size_t length) {
  Serial.println("Socket.IO Connected!");
}

void socket_event(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void onFileUploaded(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);

  preferences.begin("otaNamespace", false);
  preferences.putBool("resetar", true);
  preferences.end();

  ESPhttpUpdate.rebootOnUpdate(true);
  //t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.7:4000/downloadOTA");
  t_httpUpdate_return ret = ESPhttpUpdate.update("https://barcos-backend.onrender.com/downloadOTA");
  Serial.println(ret);
  switch (ret) {
    case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str()); break;

    case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

    case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); Serial.println("TESTE 2"); //deep_sleep(); break;
  }
}

// inicializa o socket.io-client
void init_socket()
{
  Serial.println("Configurando o socket.io");
  socket.on("connect", socket_Connected);
  socket.on("my-event", socket_event);
  socket.on("file-uploaded", onFileUploaded);

  if (useSSL) {
    socket.beginSSL(host, port, path, sslFingerprint);
  } else {
    socket.begin(host, port, path);
  }
  delay(500);
}

void connection_socket()
{
  socket.loop();
}

// envia uma mensagem para o servidor via socket
// void send_socket(String info)
// {
//   DynamicJsonDocument doc(1024);
//   JsonArray array = doc.to<JsonArray>();

//   array.add("newinfo");

//   // add payload (parameters) for the event
//   array.add(info);

//   // JSON to String (serializion)
//   String output;
//   serializeJson(doc, output);

//   // Send event
//   socket.sendEVENT(output);
// }
void send_socket(String info)
{
  String message = "\"" + info + "\"";
  socket.emit("newinfo", message.c_str());
}

void deep_sleep() {
  preferences.begin("otaNamespace", false);
  preferences.putBool("resetar", false);
  preferences.end();
  
  esp_sleep_enable_timer_wakeup(2*1000000);
  esp_deep_sleep_start();
}
