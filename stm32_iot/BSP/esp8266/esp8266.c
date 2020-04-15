#include "esp8266.h"
#include "stdio.h"
ESP8266_handleTypeDef esp8266;



uint8_t ESP8266_init(void)
{
    esp8266.usart = &huart2;
    esp8266.receiveframelength = 0;
    esp8266.receiveframeflag = 0;
    return 0;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //printf("%c",*(esp8266.buffer));
   
    //printf("%d",esp8266.usart->ErrorCode);
     esp8266.receiveframelength++;
   // esp8266.receiveframeflag = 1;
//     if(  n  (esp8266.usart, UART_FLAG_IDLE))
//     {
//         __HAL_UART_FLUSH_DRREGISTER(esp8266.usart);
//         esp8266.receiveframeflag = 0;
//     }
     
    HAL_UART_Receive_IT(esp8266.usart, &esp8266.buffer[esp8266.receiveframelength], 1);
    __HAL_UART_ENABLE_IT(esp8266.usart, UART_FLAG_IDLE);
}

