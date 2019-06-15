/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "HL_rti.h"
#include "HL_sci.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void sci_display(sciBASE_t *sci, char *buf, int len);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    rtiInit();
    sciInit();

    rtiEnableNotification(rtiREG1,rtiNOTIFICATION_COMPARE0);
    rtiEnableNotification(rtiREG1,rtiNOTIFICATION_COMPARE1);

    rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK0);

    _enable_IRQ();

    for(;;)
        ;


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(rtiBASE_t *rtiREG,uint32 notification){
    char buf[64];
    int len;
    if(notification==rtiNOTIFICATION_COMPARE0){
        sprintf(buf,"cmp0 start\n\r\0");
        len=strlen(buf);
        sci_display(sciREG1,buf,len);
        /*enable_IRQ();
        for(;;)
            ;*/
        sprintf(buf,"cmp0 end\n\r\0");
        len=strlen(buf);
        sci_display(sciREG1,buf,len);
    }else{
        sprintf(buf,"cmp1 start\n\r\0");
        len=strlen(buf);
        sci_display(sciREG1,buf,len);

        sprintf(buf,"cmp1 end\n\r\0");
        len=strlen(buf);
        sci_display(sciREG1,buf,len);
    }
}


void sci_display(sciBASE_t *sci, char *buf, int len)
{
    while(len--){
        while(sci->FLR&0x4==4)
            ;
        sciSendByte(sci,*buf++);
    }
}
/* USER CODE END */
