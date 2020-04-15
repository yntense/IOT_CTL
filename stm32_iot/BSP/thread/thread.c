#include "thread.h"
#include "cmsis_os.h"
#include "stdio.h"



void esp_receive_thread(void const * argument)
{

  //  printf("hello esp_receive_thread\r\n");
    osDelay(10);


}


void main_thread(void const * argument)
{

    printf("hello main_thread\r\n");
    osDelay(1000);


}
