#ifndef __HAL_ESP8266_H
#define __HAL_ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"


// HAL instance    
typedef struct
{
    UART_HandleTypeDef *usart;
    
} ESP8266_handleTypeDef;  

// function

uint8_t ESP8266_send ( ESP8266_handleTypeDef *hesp8266, uint8_t *data, uint16_t size, uint8_t Time_out);    




#ifdef __cplusplus
}
#endif


#endif

