#ifndef _LOGTRACE_
#define _LOGTRACE_

#include <stdint.h>
#include <stdio.h>
#include <arduino.h>
#include "uart_inc.h"

#define T_ALL   4
#define T_DEBUG 3
#define T_INFO  2
#define T_ERROR 1
#define T_MUST  0 

#define TRACE_LVL T_ERROR

#define MAX_BUFFER 255
#define SERIAL_BAUD      115200 // Serial baud rate

static char* logT[] = {
  "[must]: ",
  "[error]: ",
  "[info]: ",
  "[debug]: ",
  "[all]"
};

static bool SerialInit = false;

void logtrace_print(uint8_t level, const char* format,...);

#endif
