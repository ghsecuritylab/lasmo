#include "F7/shell_rtt.h"

#include <stdio.h>
#include <string.h>
#include "RTT/SEGGER_RTT.h"

#include "F7/shell_rtt_conf.h"

static char getkey_buf[GETKEY_BUF_SIZE+1] = {0};

static void shell_rtt_help(void){
  SEGGER_RTT_printf(0,"help ");
  for(size_t i=0 ; shell_command_names[i] ; i++){
    SEGGER_RTT_printf(0,"%s ",shell_command_names[i]);
  }
  SEGGER_RTT_printf(0,"\n");
}

void lsm_shell_rtt_init(void){
  while(1){
    SEGGER_RTT_printf(0, ">>> ");
    for(int i=0; i<GETKEY_BUF_SIZE; i++){
      int u = SEGGER_RTT_WaitKey();
      if( u == 10){
        getkey_buf[i] = 0;
        break;
      }
      getkey_buf[i] = (char) u;
    }
    if(strcmp(getkey_buf,"help") == 0){
        shell_rtt_help();
        continue;
    }
    for(size_t i=0; shell_command_names[i] ; i++){
      if(strcmp(getkey_buf,shell_command_names[i]) == 0){
        (*shell_command_fcts[i])();
        break;
      }
    }
  }
}
