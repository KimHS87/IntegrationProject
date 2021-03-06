/** @file system.c 
*   @brief System Driver Source File
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains:
*   - API Functions
*   .
*   which are relevant for the System driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */


/* Include Files */

#include "HL_system.h"
#include "HL_reg_pcr.h"
#include "HL_pinmux.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void systemInit(void)
*   @brief Initializes System Driver
*
*   This function initializes the System driver.
*
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void setupPLL(void)
{

/* USER CODE BEGIN (3) */
/* USER CODE END */

    /* Disable PLL1 and PLL2 */
	systemREG1->CSDISSET = 0x00000002U | 0x00000040U; 
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while((systemREG1->CSDIS & 0x42U) != 0x42U)
    {
    /* Wait */
    }
    
    /* Clear Global Status Register */
    systemREG1->GBLSTAT = 0x301U;
    
    /** - Configure PLL control registers */
    /** @b Initialize @b Pll1: */

    /**   - Setup pll control register 1:
    *     - Setup reset on oscillator slip 
    *     - Setup bypass on pll slip
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reset on oscillator fail
    *     - Setup reference clock divider 
    *     - Setup Pll multiplier          
    */
    systemREG1->PLLCTL1 =  (uint32)0x00000000U 
                        |  (uint32)0x20000000U 
                        |  (uint32)((uint32)0x1FU << 24U) 
                        |  (uint32)0x00000000U 
                        |  (uint32)((uint32)(8U - 1U)<< 16U) 
                        |  (uint32)((uint32)(150U - 1U)<< 8U);

    /**   - Setup pll control register 2
    *     - Setup spreading rate
    *     - Setup bandwidth adjustment
    *     - Setup internal Pll output divider
    *     - Setup spreading amount
    */
    systemREG1->PLLCTL2 =  (uint32)((uint32)255U << 22U)
                        |  (uint32)((uint32)7U << 12U)
                        |  (uint32)((uint32)(1U - 1U) << 9U)
                        |  (uint32)61U;

    /** @b Initialize @b Pll2: */

    /**   - Setup pll2 control register :
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reference clock divider 
    *     - Setup internal Pll output divider
    *     - Setup Pll multiplier          
    */
    systemREG2->PLLCTL3 = (uint32)((uint32)(1U - 1U) << 29U)
                        | (uint32)((uint32)0x1FU << 24U) 
                        | (uint32)((uint32)(8U - 1U)<< 16U) 
                        | (uint32)((uint32)(150U - 1U) << 8U);

    /** - Enable PLL(s) to start up or Lock */
    systemREG1->CSDIS = 0x00000000U    
                      | 0x00000000U 
                      | 0x00000008U 
                      | 0x00000080U 
                      | 0x00000000U 
                      | 0x00000040U 
                      | 0x00000000U
                      | 0x00000004U;
}

void trimLPO(void)
{

/* USER CODE BEGIN (4) */
/* USER CODE END */

    /** @b Initialize Lpo: */
    /** Load TRIM values from OTP if present else load user defined values */
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    if(LPO_TRIM_VALUE != 0xFFFFU)
    {
    
        systemREG1->LPOMONCTL  = (uint32)((uint32)1U << 24U)
                                | LPO_TRIM_VALUE;
    }
    else
    {

    	systemREG1->LPOMONCTL 	=  (uint32)((uint32)1U << 24U)
                                 | (uint32)((uint32)16U << 8U)
                                 | 16U;
    }

/* USER CODE BEGIN (5) */
/* USER CODE END */

}

void setupFlash(void)
{

/* USER CODE BEGIN (6) */
/* USER CODE END */

    /** - Setup flash read mode, address wait states and data wait states */
    flashWREG->FRDCNTL =  0x00000000U 
                       | (uint32)((uint32)3U << 8U) 
                       |  3U;

    /** - Setup flash access wait states for bank 7 */
    FSM_WR_ENA_HL    = 0x5U;
    EEPROM_CONFIG_HL = 0x00000002U
                     | (uint32)((uint32)9U << 16U) ;

/* USER CODE BEGIN (7) */
/* USER CODE END */

    /** - Disable write access to flash state machine registers */
    FSM_WR_ENA_HL    = 0x2U;

    /** - Setup flash bank power modes */
    flashWREG->FBPWRMODE = 0x00000000U
                          | (uint32)((uint32)SYS_ACTIVE << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_ACTIVE << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_ACTIVE << 0U); /* BANK 0 */

/* USER CODE BEGIN (8) */
/* USER CODE END */
}

void periphInit(void)
{

/* USER CODE BEGIN (9) */
/* USER CODE END */

    /** - Disable Peripherals before peripheral powerup*/
    systemREG1->CLKCNTL &= 0xFFFFFEFFU;

    /** - Release peripherals from reset and enable clocks to all peripherals */
    /** - Power-up all peripherals */
    pcrREG1->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG1->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG1->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG1->PSPWRDWNCLR3 = 0xFFFFFFFFU;

    pcrREG2->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG2->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG2->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG2->PSPWRDWNCLR3 = 0xFFFFFFFFU;

    pcrREG3->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG3->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG3->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG3->PSPWRDWNCLR3 = 0xFFFFFFFFU;
    
    /** - Enable Peripherals */
    systemREG1->CLKCNTL |= 0x00000100U;
    
/* USER CODE BEGIN (10) */
/* USER CODE END */

}

void mapClocks(void)
{
    uint32 SYS_CSVSTAT, SYS_CSDIS;
    
/* USER CODE BEGIN (11) */
/* USER CODE END */

    /** @b Initialize @b Clock @b Tree: */
    /** - Setup system clock divider for HCLK */
    systemREG2->HCLKCNTL = 1U;
    
    /** - Disable / Enable clock domain */
    systemREG1->CDDIS = (uint32)((uint32)0U << 4U ) /* AVCLK 1 OFF */
                      | (uint32)((uint32)1U << 5U ) /* AVCLK 2 OFF */
	                  | (uint32)((uint32)0U << 8U ) /* VCLK3 OFF */
					  | (uint32)((uint32)0U << 9U ) /* VCLK4 OFF */
	                  | (uint32)((uint32)0U << 10U) /* AVCLK 3 OFF */
                      | (uint32)((uint32)0U << 11U); /* AVCLK 4 OFF */


    /* Always check the CSDIS register to make sure the clock source is turned on and check 
     * the CSVSTAT register to make sure the clock source is valid. Then write to GHVSRC to switch the clock.
     */
    /** - Wait for until clocks are locked */
    SYS_CSVSTAT = systemREG1->CSVSTAT;
    SYS_CSDIS = systemREG1->CSDIS;
    while ((SYS_CSVSTAT & ((SYS_CSDIS ^ 0xFFU) & 0xFFU)) != ((SYS_CSDIS ^ 0xFFU) & 0xFFU))
    { 
        SYS_CSVSTAT = systemREG1->CSVSTAT;
        SYS_CSDIS = systemREG1->CSDIS;
    } /* Wait */

/* USER CODE BEGIN (12) */
/* USER CODE END */

    /* Now the PLLs are locked and the PLL outputs can be sped up */
    /* The R-divider was programmed to be 0xF. Now this divider is changed to programmed value */
    systemREG1->PLLCTL1 = (systemREG1->PLLCTL1 & 0xE0FFFFFFU) | (uint32)((uint32)(1U - 1U) << 24U);
    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> " Clear and write to the volatile register " */
    systemREG2->PLLCTL3 = (systemREG2->PLLCTL3 & 0xE0FFFFFFU) | (uint32)((uint32)(1U - 1U) << 24U);

    /* Enable/Disable Frequency modulation */
	systemREG1->PLLCTL2 |= 0x00000000U;
    /** - Map device clock domains to desired sources and configure top-level dividers */
    /** - All clock domains are working off the default clock sources until now */
    /** - The below assignments can be easily modified using the HALCoGen GUI */
    
    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (uint32)((uint32)SYS_PLL1 << 24U) 
                       | (uint32)((uint32)SYS_PLL1 << 16U) 
                       | (uint32)((uint32)SYS_PLL1 << 0U);
                       
    /** - Setup synchronous peripheral clock dividers for VCLK1, VCLK2, VCLK3 */
    systemREG1->CLKCNTL  = (systemREG1->CLKCNTL & 0xF0FFFFFFU) 
						 | (uint32)((uint32)1U << 24U);
    systemREG1->CLKCNTL  = (systemREG1->CLKCNTL & 0xFFF0FFFFU) 
						 | (uint32)((uint32)1U << 16U); 

    systemREG2->CLK2CNTRL = (systemREG2->CLK2CNTRL & 0xFFFFFFF0U)
						 | (uint32)((uint32)1U << 0U);

/* USER CODE BEGIN (13) */
/* USER CODE END */
    
    /** - Setup RTICLK1 and RTICLK2 clocks */
    systemREG1->RCLKSRC = (uint32)((uint32)1U << 24U)        /* RTI2 divider (Not applicable for lock-step device)  */
                        | (uint32)((uint32)SYS_VCLK << 16U) /* RTI2 clock source (Not applicable for lock-step device) */
                        | (uint32)((uint32)1U << 8U)         /* RTI1 divider */
                        | (uint32)((uint32)SYS_VCLK << 0U); /* RTI1 clock source */

    /** - Setup asynchronous peripheral clock sources for AVCLK1 and AVCLK2 */
    systemREG1->VCLKASRC = (uint32)((uint32)SYS_VCLK << 8U)
                         | (uint32)((uint32)SYS_VCLK << 0U);

    systemREG2->VCLKACON1 =  (uint32)((uint32)(2U - 1U) << 24U) 
                           | (uint32)((uint32)0U << 20U) 
                           | (uint32)((uint32)SYS_VCLK << 16U)
                           | (uint32)((uint32)(1U - 1U) << 8U)
                           | (uint32)((uint32)0U << 4U) 
                           | (uint32)((uint32)SYS_VCLK << 0U);

/* USER CODE BEGIN (14) */
/* USER CODE END */

}

void systemInit(void)
{
/* USER CODE BEGIN (15) */
/* USER CODE END */

    /* Configure PLL control registers and enable PLLs.
     * The PLL takes (127 + 1024 * NR) oscillator cycles to acquire lock.
     * This initialization sequence performs all the tasks that are not
     * required to be done at full application speed while the PLL locks.
     */
    setupPLL();
    

/* USER CODE BEGIN (16) */
/* USER CODE END */
    
    /* Enable clocks to peripherals and release peripheral reset */
    periphInit();

/* USER CODE BEGIN (17) */
/* USER CODE END */

    /* Configure device-level multiplexing and I/O multiplexing */
    muxInit();
    
/* USER CODE BEGIN (18) */
/* USER CODE END */

    /** - Set up flash address and data wait states based on the target CPU clock frequency
     * The number of address and data wait states for the target CPU clock frequency are specified
     * in the specific part's datasheet.
     */
    setupFlash();

/* USER CODE BEGIN (19) */
/* USER CODE END */

    /** - Configure the LPO such that HF LPO is as close to 10MHz as possible */
    trimLPO();

/* USER CODE BEGIN (20) */
/* USER CODE END */

    /** - Wait for PLLs to start up and map clock domains to desired clock sources */
    mapClocks();

/* USER CODE BEGIN (21) */
/* USER CODE END */

    /** - set ECLK pins functional mode */
    systemREG1->SYSPC1 = 0U;

    /** - set ECLK pins default output value */
    systemREG1->SYSPC4 = 0U;

    /** - set ECLK pins output direction */
    systemREG1->SYSPC2 = 1U;

    /** - set ECLK pins open drain enable */
    systemREG1->SYSPC7 = 0U;

    /** - set ECLK pins pullup/pulldown enable */
    systemREG1->SYSPC8 = 0U;   

    /** - set ECLK pins pullup/pulldown select */
    systemREG1->SYSPC9 = 1U;

    /** - Setup ECLK */
    systemREG1->ECPCNTL = (uint32)((uint32)0U << 24U)
                        | (uint32)((uint32)0U << 23U)
                        | (uint32)((uint32)(8U - 1U) & 0xFFFFU);

/* USER CODE BEGIN (22) */
/* USER CODE END */
}

void systemPowerDown(uint32 mode)
{

/* USER CODE BEGIN (23) */
/* USER CODE END */

    /* Disable clock sources */
    systemREG1->CSDISSET = mode & 0x000000FFU;

    /* Disable clock domains */
    systemREG1->CDDIS = (mode >> 8U) & 0x00000FFFU;

    /* Idle CPU */
    /*SAFETYMCUSW 88 S MR:2.1 <APPROVED> "Assembly in C needed" */
    _gotoCPUIdle_();

/* USER CODE BEGIN (24) */
/* USER CODE END */

}

/* USER CODE BEGIN (25) */
/* USER CODE END */

resetSource_t getResetSource(void)
{
    register resetSource_t rst_source;

    if ((SYS_EXCEPTION & (uint32)POWERON_RESET) != 0U)
    {
        /* power-on reset condition */
        rst_source = POWERON_RESET;
        
        /* Clear all exception status Flag and proceed since it's power up */
        SYS_EXCEPTION = 0x0000FFFFU;
    }
    else if ((SYS_EXCEPTION & (uint32)EXT_RESET) != 0U)
    {
        /* Reset caused due to External reset. */
        rst_source = EXT_RESET;
        SYS_EXCEPTION = EXT_RESET;
    }   
    else if ((SYS_EXCEPTION & (uint32)DEBUG_RESET) !=0U)
    {
        /* Reset caused due Debug reset request */
        rst_source = DEBUG_RESET;
        SYS_EXCEPTION = DEBUG_RESET;
    }   
    else if ((SYS_EXCEPTION & (uint32)OSC_FAILURE_RESET) != 0U)
    {
     /* Reset caused due to oscillator failure.
        Add user code here to handle oscillator failure */
        rst_source = OSC_FAILURE_RESET;
        SYS_EXCEPTION = OSC_FAILURE_RESET;
    }
    else if ((SYS_EXCEPTION & (uint32)WATCHDOG_RESET) !=0U)
    {
        /* Reset caused due watchdog violation */
        rst_source = WATCHDOG_RESET;
        SYS_EXCEPTION = WATCHDOG_RESET;
    }
    else if ((SYS_EXCEPTION & (uint32)WATCHDOG2_RESET) !=0U)
    {
        /* Reset caused due watchdog violation */
        rst_source = WATCHDOG2_RESET;
        SYS_EXCEPTION = WATCHDOG2_RESET;
    }   
    else if ((SYS_EXCEPTION & (uint32)CPU1_RESET) !=0U)
    {
        /* Reset caused due to CPU1 reset.
        CPU reset can be caused by CPU self-test completion, or
        by toggling the "CPU RESET" bit of the CPU Reset Control Register. */
        rst_source = CPU1_RESET;
        SYS_EXCEPTION = CPU1_RESET;
    }
    else if ((SYS_EXCEPTION & (uint32)CPU0_RESET) !=0U)
    {
        /* Reset caused due to CPU0 reset.
        CPU reset can be caused by CPU self-test completion, or
        by toggling the "CPU RESET" bit of the CPU Reset Control Register. */
        rst_source = CPU0_RESET;
        SYS_EXCEPTION = CPU0_RESET;
    }
    else if ((SYS_EXCEPTION & (uint32)SW_RESET) != 0U)
    {
        /* Reset caused due to software reset. */
        rst_source = SW_RESET;
        SYS_EXCEPTION = SW_RESET;
    }   
    else
    {
        /* No_reset occured. */
        rst_source = NO_RESET;
        SYS_EXCEPTION = NO_RESET;
    }
    
    return rst_source;
}

/* USER CODE BEGIN (26) */
/* USER CODE END */

