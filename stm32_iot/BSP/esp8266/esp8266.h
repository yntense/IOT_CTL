#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "hal_esp8266.h"

 //instance 1
extern ESP8266_handleTypeDef esp8266;

    
 //instance 1 init
uint8_t ESP8266_init(void);


#ifdef __cplusplus
}
#endif

#endif