#include "LogTrace.h"

void logtrace_print(uint8_t level, const char* format,...){
  
  if( !SerialInit ){
    Serial.begin(SERIAL_BAUD);
    Serial.println("[debug]: Serial_online");
    SerialInit = true;
  }
  
  char buf[MAX_BUFFER];
  memset(buf,0,MAX_BUFFER);

  if( level <= TRACE_LVL )
  {
    va_list args;
    va_start(args, format);
    vsnprintf(buf,MAX_BUFFER,format,args);
    va_end(args);
    Serial.print(logT[level]);
    Serial.print(buf);
    Serial.print("\n");
  }
}
