#include "esp8266.h"

ESP8266_handleTypeDef esp8266;



uint8_t ESP8266_init(void)
{
    esp8266.usart = &huart1;
    
    return 0;
}



