#ifndef ESP8266_H
#define ESP8266_H

#include "stm32f4xx_hal.h"

// Paramètres WiFi et API
#define WIFI_SSID     "CSS"
#define WIFI_PASSWORD "karimazer"
#define API_KEY       "Votre_Write_API_Key"
#define THINGSPEAK_IP "184.106.153.149"
#define THINGSPEAK_PORT 80

extern UART_HandleTypeDef huart1;

// Prototypes des fonctions
void ESP_Init(const char *ssid, const char *password);
void ESP_SendData(float value);
void ESP_SendCommand(const char *command, const char *expected_response, uint32_t timeout);

#endif
