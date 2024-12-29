#include "esp8266.h"
#include <string.h>
#include <stdio.h>

void ESP_Init(const char *ssid, const char *password) {
    char cmd[100];

    ESP_SendCommand("AT\r\n", "OK", 2000); // Vérification du module
    ESP_SendCommand("AT+CWMODE=1\r\n", "OK", 2000); // Mode station

    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    ESP_SendCommand(cmd, "WIFI CONNECTED", 10000);
}

void ESP_SendData(float value) {
    char tcp_cmd[100];
    char http_req[200];
    char send_cmd[50];

    sprintf(tcp_cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", THINGSPEAK_IP, THINGSPEAK_PORT);
    ESP_SendCommand(tcp_cmd, "CONNECT", 5000);

    sprintf(http_req, "GET /update?api_key=%s&field1=%.2f\r\nHost: %s\r\nConnection: close\r\n\r\n",
            API_KEY, value, THINGSPEAK_IP);
    sprintf(send_cmd, "AT+CIPSEND=%d\r\n", strlen(http_req));

    ESP_SendCommand(send_cmd, ">", 2000);
    ESP_SendCommand(http_req, "OK", 5000);
    ESP_SendCommand("AT+CIPCLOSE\r\n", NULL, 2000);
}

void ESP_SendCommand(const char *command, const char *expected_response, uint32_t timeout) {
    char buffer[100];
    HAL_UART_Transmit(&huart1, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
    HAL_UART_Receive(&huart1, (uint8_t *)buffer, sizeof(buffer), timeout);

    if (expected_response != NULL && strstr(buffer, expected_response) == NULL) {
        HAL_UART_Transmit(&huart1, (uint8_t *)"Erreur ESP\n", 11, HAL_MAX_DELAY);
    }
}
