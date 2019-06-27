#ifndef _UART_INC_
#define _UART_INC_

#include <stdint.h>
#include <stdio.h>
#include <arduino.h>

#define   MAX_MSG_SIZE  255

#define   M_ACK         "[254]"
#define   M_TIMEOUT     "[253]"
#define   M_UNEXPECTED  "[252]"
#define   M_NOK         "[251]"

#define   M_PIN   "[001]"

#define   M_SMTP_INIT     "[100]"
#define   M_USERNAME      "[101]"
#define   M_PASSWORD      "[102]"
#define   M_SMTP_SERVER   "[103]"
#define   M_SMTP_PORT     "[104]"
#define   M_MAIL_SENDER   "[105]"
#define   M_MAIL_DEST     "[106]"
#define   M_SEND_MAIL     "[199]"

#define   M_WIFI_INIT       "[200]"
#define   M_WIFI_SID        "[201]"
#define   M_WIFI_PASS       "[202]"
#define   M_WIFI_CONNECT    "[299]"
#define   M_WIFI_DCONNECT   "[298]"

void get_message( String* message );
void send_ack( char* ack_type );
bool wait_for_next_message( char* answer, uint32_t timeout, char *message_content );
bool wait_for_next_message( char* answer, uint32_t timeout );
void pars_message( String message, char* message_header, char* message_content ); 
void send_message( char* header, char* message_content );
void send_message( char* header );

#endif
