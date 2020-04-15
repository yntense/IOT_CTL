#ifndef __HAL_ESP8266_H
#define __HAL_ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
 




    
typedef struct
{
    unsigned char id;
    tcp_client_state state;
    int data_length;
    char *data;

}tcp_client;

    
    
// HAL instance    
typedef struct
{
    UART_HandleTypeDef *usart;
    
    int tcp_client_online_amount;
    tcp_client tcp_client_array[5];
    
    int receiveframelength;
    int receiveframeflag;
    char buffer[1000];
    
} ESP8266_handleTypeDef;  



// function

uint8_t ESP8266_send ( ESP8266_handleTypeDef *hesp8266, uint8_t *data, uint16_t size, uint8_t Time_out);    
uint8_t ESP8266_receive_IT( ESP8266_handleTypeDef *hesp8266, uint8_t **data, uint16_t size, uint8_t Time_out);    

uint8_t ESP8266_MODE(ESP8266_handleTypeDef *hesp8266);
uint8_t ESP8266_INIT_AP(ESP8266_handleTypeDef *hesp8266);
uint8_t ESP8266_CREATE_TCP(ESP8266_handleTypeDef *hesp8266);
uint8_t esp8266_receive(ESP8266_handleTypeDef *hesp8266, uint32_t Time_out);
#ifdef __cplusplus
}
#endif


#endif

