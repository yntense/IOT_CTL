
#include "hal_esp8266.h"


uint8_t ESP8266_send ( ESP8266_handleTypeDef *hesp8266, uint8_t *data, uint16_t size, uint8_t Time_out)
{
   return HAL_UART_Transmit(hesp8266->usart, data, size, Time_out);
    
}

