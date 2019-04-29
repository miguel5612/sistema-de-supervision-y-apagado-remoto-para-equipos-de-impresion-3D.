#ifndef ONHUBCONFIG_H
#define ONHUBCONFIG_H

#define serDebug true

#define DebugBaudRate 115200
#define sep ','

#define outTopic "/PVDLab/bogohackI3B/OUT"
#define inTopic "/PVDLab/bogohackI3B/IN"
#define wiFiname "OnHub_SSAR3D_bogohackI3B"

#define URL "http://airq.dronefenix.a2hosted.com/bot"
#define banderaPeticionGet 6 //Maximo de publicaciones al broker antes de llamar a la pagina bot para despertar al hosting en caso que el iis haya apagado el servicio

#define initialCharacter "Inicio"
#define endCharacter "Final"

#define relayPin D1
#define wifiPin D8

#define arduinoTXPin D3
#define arduinoRXPin D2

#define minDelay 10
#define relayEEPROMAdressState 125 // Max 512, value between 0-512
//#define MQTT_MAX_PACKET_SIZE 200 //modify this into pub-sub client library
#define maxLen 300 //the maximum message size, including header, is 128 bytes by default.
#define deltaLimite 6 //6 grados de diferencia

#endif // onHubConfig.h
