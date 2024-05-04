#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include <Arduino.h>
#include <SocketIoClient.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>
#include <Preferences.h>

void init_socket();
void connection_socket();
void send_socket(String info);
void deep_sleep();


#endif
