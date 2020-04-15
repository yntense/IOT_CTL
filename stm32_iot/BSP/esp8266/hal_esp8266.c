
#include "hal_esp8266.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
uint8_t ESP8266_send ( ESP8266_handleTypeDef *hesp8266, uint8_t *data, uint16_t size, uint8_t Time_out)
{
    return HAL_UART_Transmit(hesp8266->usart, data, size, Time_out);

}

uint8_t ESP8266_receive_IT( ESP8266_handleTypeDef *hesp8266, uint8_t **data, uint16_t size, uint8_t Time_out)
{

    HAL_UART_Receive_IT(hesp8266->usart, &hesp8266->buffer[hesp8266->receiveframelength], 1);
    // __HAL_UART_ENABLE_IT(hesp8266->usart, UART_FLAG_IDLE);
    return 0;
}

static uint8_t esp8266_transmit_receive(ESP8266_handleTypeDef *hesp8266, uint8_t *data, uint16_t size, uint32_t Time_out)
{
    unsigned char *send_data = NULL;

    if(!data)
    {
        return 1;
    }
    send_data = data;
    HAL_UART_Transmit(hesp8266->usart, send_data, size, Time_out);
    hesp8266->receiveframelength = 0;
    while((HAL_UART_Receive(hesp8266->usart, &hesp8266->buffer[hesp8266->receiveframelength], 1, Time_out) )== HAL_OK)
    {
        if((hesp8266->buffer[hesp8266->receiveframelength] == '\n') && (hesp8266->buffer[hesp8266->receiveframelength-1] == '\r'))
        {
            if((hesp8266->buffer[hesp8266->receiveframelength-2] == 'K') && (hesp8266->buffer[hesp8266->receiveframelength-3] == 'O'))
            {
                goto out;
            }
        }

        hesp8266->receiveframelength++;
    }
out:

    HAL_UART_Transmit(&huart1, hesp8266->buffer, hesp8266->receiveframelength+1, 100);

    hesp8266->receiveframelength = 0 ;
    return 0;
}



uint8_t ESP8266_MODE(ESP8266_handleTypeDef *hesp8266)
{
    unsigned char data[] = "AT+CWMODE=3\r\n";
    esp8266_transmit_receive(hesp8266, data, sizeof(data)-1,5000);
    return 0;
}

uint8_t ESP8266_INIT_AP(ESP8266_handleTypeDef *hesp8266)
{
    unsigned char data[] = "AT+CWSAP=\"MY_ESP\",\"12345678\",1,3,4,0\r\n";
    esp8266_transmit_receive(hesp8266, data, sizeof(data)-1,5000);
    return 0;
}

int ESP8266_INIT_STA(ESP8266_handleTypeDef *hesp8266, char *sta_ssid, int ssid_length, char *sta_key, int key_length)
{
    int i,j,k;
    char ssid[ssid_length+1],key[key_length+1];
    printf("%s\r\n", sta_key);
    if(!sta_ssid || !sta_key)
    {
        printf("fail\r\n");
        return -1;
    }
    memset(ssid, 0,ssid_length+1);
    memset(key, 0,ssid_length+1);
    memcpy(ssid, sta_ssid, ssid_length);
    memcpy(key, sta_key, key_length);

    key[key_length] = '\0';
    ssid[ssid_length] = '\0';
    printf("sta-ssid %s \r\n",ssid);
    i = 19 + ssid_length + key_length;
    char data[i];
    sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, key);
    i = strlen(data);
    printf("data %s",data);
    esp8266_transmit_receive(hesp8266, (unsigned char *)data, i,7000);
    return 0;
}

uint8_t ESP8266_CREATE_TCP(ESP8266_handleTypeDef *hesp8266)
{
    unsigned char data_mux[] = "AT+CIPMUX=1\r\n";
    esp8266_transmit_receive(hesp8266, data_mux, sizeof(data_mux)-1,5000);
    unsigned char data_server[] = "AT+CIPSERVER=1\r\n";
    esp8266_transmit_receive(hesp8266, data_server, sizeof(data_server)-1,5000);
    unsigned char data_ap[] = "AT+CIPAP?\r\n";
    esp8266_transmit_receive(hesp8266, data_ap, sizeof(data_ap)-1,5000);

    return 0;
}

uint8_t esp8266_receive(ESP8266_handleTypeDef *hesp8266, uint32_t Time_out)
{
    char temp[10]= {0};
    int count = 0;
    char *pos = NULL;
    hesp8266->receiveframelength = 0;
    while((HAL_UART_Receive(hesp8266->usart, &hesp8266->buffer[hesp8266->receiveframelength], 1, Time_out) )!= HAL_TIMEOUT)
    {

        hesp8266->receiveframelength++;
    }
    hesp8266->buffer[hesp8266->receiveframelength] = '\0';
    if(hesp8266->receiveframelength > 0)
    {
        printf("start data %send\r\n", hesp8266->buffer);
        printf("length  %d\r\n\r\n", hesp8266->receiveframelength);

    }


    //匹配 connect 所在位置
    if((pos = strstr(hesp8266->buffer, "CONNECT")) != NULL )
    {
        //判断 id (CONNECT),所在的指针是否存在
        while( pos != NULL )
        {
            pos = pos -2;
            if(pos != NULL)
            {
//                printf("\r\npos %s connect\r\n", pos);
//                printf("pos %c connect\r\n", *pos);
//                printf("pos %p connect\r\n\r\n", pos);
                int i = 0;
                while( i < 5)
                {

                    if(hesp8266->tcp_client_array[i].state == WAIT_UPDATE)
                    {
                        hesp8266->tcp_client_array[i].id = *pos;
                        hesp8266->tcp_client_array[i].state = ONLINE;
                        //清除已经出现过的 CONNECT 字符串
                        printf("connect %c\r\n", hesp8266->tcp_client_array[i].id);
                        pos = strstr(pos+8, "CONNECT");
                        break;
                    }
                    i++;
                }
            }

        }
    }
    if( (pos = strstr(hesp8266->buffer, "CLOSED")) != NULL )
    {
        //判断 id (CONNECT),所在的指针是否存在
        while( pos != NULL )
        {
            pos = pos -2;
            if(pos != NULL)
            {
//                printf("\r\npos %s closed\r\n", pos);
//                printf("pos %c closed\r\n", *pos);
//                printf("pos %p closed\r\n\r\n", pos);
                int i = 0;
                while( i < 5)
                {

                    if(hesp8266->tcp_client_array[i].id == *pos)
                    {
                        printf("closed %c \r\n",hesp8266->tcp_client_array[i].id);
                        hesp8266->tcp_client_array[i].state = WAIT_UPDATE; 
                        pos = strstr(pos+8, "CLOSED");
                        break;
                    }
                    i++;
                }
            }

        }

    }

    if( (pos = strstr(hesp8266->buffer, "+")) != NULL )
    {
       char *temp_pos = NULL; 
       temp_pos = pos;
       int ssid_count = 0, key_count = 0, count = 0;
       char *ssid = NULL, *key = NULL;
       //判断 i(KEY),所在的指针是否存在 
       if(( pos = strstr(hesp8266->buffer, "KEY")) != NULL )
       {
            
            if(((int)(pos - temp_pos)) < 15)
            {
                pos = strtok(pos, ",");
                while(pos != NULL)
                {
                    
                    if(count == 0)
                    {
                        pos = strtok(NULL, ",");
                        ssid_count = strlen(pos);
                       
                    }
                    
                    if(count == 1)
                    {
                        char ssid[ssid_count+1];
                        memset(ssid, 0, ssid_count+1);
                        memcpy(ssid, pos, ssid_count);
                        ssid[ssid_count] = '\0';
                        
                        if(ssid != NULL)
                        {
                            printf("ssid %s, %d\r\n", ssid, ssid_count);
                        } 
                        pos = strtok(NULL, ",");
                        key_count = strlen(pos);
                        char key[key_count+1];
                        memset(key, 0, key_count+1);
                        memcpy(key, pos, key_count+1);                    
                        key[key_count] = '\0';
                        
                        if(key != NULL)
                        {
                            printf("key %s, %d\r\n", key, key_count);
                        }
                        printf("test %s\r\n", pos);
                        ESP8266_INIT_STA(hesp8266, ssid, ssid_count,key,key_count );
                        goto out;
                    }
                    
                    count++; 
                }

            }
            
       }
       //普通数据
        pos = strstr(hesp8266->buffer, "+");
        pos = strstr(pos, ",");
        pos++;
        pos = strstr(pos, ",");
        char id = *(pos-1); 
        temp_pos = pos+1;
        pos = strstr(pos, ":");
        int size = (int)(pos - temp_pos);
        char data_size[size+1];
        memset(data_size, 0 ,size+1);
        memcpy(data_size, temp_pos, size);
        data_size[size] = '\0';
        int data_length = atoi(data_size);
        printf("data_length %d\r\n", data_length);
        pos++;
        for(int i = 0; i < 5; i++)
        {
            if(hesp8266->tcp_client_array[i].id == id)
            {

                hesp8266->tcp_client_array[i].data = (char *)malloc(data_length+1); 
                memset(hesp8266->tcp_client_array[i].data, 0, data_length);
                memcpy(hesp8266->tcp_client_array[i].data, pos, data_length);
                hesp8266->tcp_client_array[i].data[data_length] = '\0';
                printf("client %c :%s\r\n", hesp8266->tcp_client_array[i].id, hesp8266->tcp_client_array[i].data);
                free(hesp8266->tcp_client_array[i].data);
                break;
            }
        }        
        
        


    }
 
out:
    memset(hesp8266->buffer, 0, hesp8266->receiveframelength);
    hesp8266->receiveframelength = 0 ;
    return 0;
}
