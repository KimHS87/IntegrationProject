/** @file HL_sys_dma.c 
*   @brief DMA Driver Source File
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains:
*   - API Functions
*   - Interrupt Handlers
*   .
*   which are relevant for the DMA driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */


#include "HL_sys_dma.h"

/** @fn void dmaEnable(void)
*   @brief enables DMA module
*
*   This function brings DMA out of reset
*/
void dmaEnable(void)
{
/* USER CODE BEGIN (0) */
/* USER CODE END */

    dmaREG->GCTRL  = 0x00010000U; /* enable dma      */
    dmaREG->GCTRL |= 0x00000300U; /* stop at suspend */

/* USER CODE BEGIN (1) */
/* USER CODE END */
}

/** @fn void dmaDisable(void)
*   @brief disables DMA module
*
*   This function disables DMA module
*/
void dmaDisable(void)
{
/* USER CODE BEGIN (2) */
/* USER CODE END */

	while((dmaREG->GCTRL & 0x00004000U) != 0U)
	{
	} /* Wait */

	/* Disable DMA module */
	dmaREG->GCTRL = 0U;

/* USER CODE BEGIN (3) */
/* USER CODE END */
}

/** @fn void dmaSetCtrlPacket(uint32 channel)
*   @brief Set control packet
*
*   This function sets control packet
*/
void dmaSetCtrlPacket(dmaChannel_t channel, g_dmaCTRL g_dmaCTRLPKT)
{
    uint8 i,j;

/* USER CODE BEGIN (4) */
/* USER CODE END */

    dmaRAMREG->PCP[channel].ISADDR  =  g_dmaCTRLPKT.SADD;

    dmaRAMREG->PCP[channel].IDADDR  =  g_dmaCTRLPKT.DADD;

    dmaRAMREG->PCP[channel].ITCOUNT = (g_dmaCTRLPKT.FRCNT << 16U) | g_dmaCTRLPKT.ELCNT;

    dmaRAMREG->PCP[channel].CHCTRL  = (g_dmaCTRLPKT.RDSIZE    << 14U) | (g_dmaCTRLPKT.WRSIZE    << 12U) | (g_dmaCTRLPKT.TTYPE << 8U)| \
                                      (g_dmaCTRLPKT.ADDMODERD << 3U ) | (g_dmaCTRLPKT.ADDMODEWR << 1U ) | (g_dmaCTRLPKT.AUTOINIT);

    dmaRAMREG->PCP[channel].CHCTRL |= (g_dmaCTRLPKT.CHCTRL << 16U);

    dmaRAMREG->PCP[channel].EIOFF   = (g_dmaCTRLPKT.ELDOFFSET << 16U) | (g_dmaCTRLPKT.ELSOFFSET);

    dmaRAMREG->PCP[channel].FIOFF   = (g_dmaCTRLPKT.FRDOFFSET << 16U) | (g_dmaCTRLPKT.FRSOFFSET);

    i = channel / 8U;          /* Find the register to write */
    j = channel % 8U;          /* Find the offset            */
    j = (uint8)7U - j;         /* Reverse the order          */
    j = j * 4U;                /* Find the bit position      */

    dmaREG->PAR[i] &= ~((uint32)0xFU << j);
    dmaREG->PAR[i] |= (g_dmaCTRLPKT.PORTASGN << j);

/* USER CODE BEGIN (5) */
/* USER CODE END */
}

/** @fn void dmaSetChEnable(uint32 channel,uint32 type)
*   @brief Enable channel
*   @param[in] channel DMA channel
*   @param[in] type Type of triggering
*                    - DMA_HW: Enables the selected DMA channel for hardware triggering
*                    - DMA_SW: Enables the selected DMA channel for software triggering
*
*   This function enables the DMA channel for hardware or software triggering
*/
void dmaSetChEnable(dmaChannel_t channel, dmaTriggerType_t type)
{
/* USER CODE BEGIN (6) */
/* USER CODE END */

	if(type == DMA_HW)
	{
		dmaREG->HWCHENAS = (uint32)1U << channel;
	}
    else
    {
    	dmaREG->SWCHENAS = (uint32)1U << channel;
    }

/* USER CODE BEGIN (7) */
/* USER CODE END */
}

/** @fn void dmaReqAssign(uint32 channel,uint32 reqline)
*   @brief Assign DMA request lines to channels
*   @param[in] channel DMA channel
*   @param[in] reqline DMA request line
*
*   This function assigns DMA request lines to channels
*/
void dmaReqAssign(dmaChannel_t channel, dmaRequest_t reqline)
{
    uint8 i,j;

/* USER CODE BEGIN (8) */
/* USER CODE END */
	
    i = channel / 4U;        /* Find the register to configure */
    j = channel % 4U;        /* Find the offset                */
    j = (uint8)3U - j;       /* reverse the byte order         */
    j = j * 8U;              /* find the bit location          */

    /* Mapping channel 'i' to request line 'j' */
    dmaREG->DREQASI[i] &= ~((uint32)0xFFU   << j);
    dmaREG->DREQASI[i] |=  ((uint32)reqline << j);
	
/* USER CODE BEGIN (9) */
/* USER CODE END */
}

/** @fn void dmaSetPriority(uint32 channel, dmaPRIORITY_t priority)
*   @brief Assign Priority to the channel
*   @param[in] channel DMA channel
*   @param[in] priority Priority queue to which channel needs to be assigned
*                        - LOWPRIORITY : The selected channel will be assigned to low priority queue
*                        - HIGHPRIORITY: The selected channel will be assigned to high priority queue
*
*   This function assigns the selected priority to the selected channel
*/
void dmaSetPriority(dmaChannel_t channel, dmaPriorityQueue_t priority)
{
/* USER CODE BEGIN (10) */
/* USER CODE END */

	if (priority == LOWPRIORITY)
	{
		dmaREG->CHPRIOR = (uint32)1U << channel;
	}
	else
	{
		dmaREG->CHPRIOS = (uint32)1U << channel;
	}

/* USER CODE BEGIN (11) */
/* USER CODE END */
}

/** @fn void dmaEnableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype, dmaIntGroup_t group)
*   @brief Enable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be enabled
*                        - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                        - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                        - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                        - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                        - BER: Bus Error Interrupt will be disabled for the selected channel
*   @param[in] group   Group to which the interrupt is routed to.
*                        - DMA_INTA : Group A
*						 - DMA_INTB : Group B (Do not use this in case of Lock-step device)
*
*   This function enables the selected interrupt for the selected channel
*/
void dmaEnableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype, dmaIntGroup_t group)
{
/* USER CODE BEGIN (12) */
/* USER CODE END */

    dmaREG->GCHIENAS = (uint32)1U << channel;

    switch (inttype)
    {
    case FTC:   dmaREG->FTCINTENAS = (uint32)1U << channel;
				dmaREG->FTCMAP     = (dmaREG->FTCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case LFS:   dmaREG->LFSINTENAS = (uint32)1U << channel;
                dmaREG->LFSMAP     = (dmaREG->LFSMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case HBC:   dmaREG->HBCINTENAS = (uint32)1U << channel;
                dmaREG->HBCMAP     = (dmaREG->HBCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    case BTC:   dmaREG->BTCINTENAS = (uint32)1U << channel;
                dmaREG->BTCMAP     = (dmaREG->BTCMAP & ~((uint32)1U << channel)) | ((uint32)group << channel);
                break;
    default :
                break;
    }

/* USER CODE BEGIN (13) */
/* USER CODE END */
}
/** @fn void dmaDisableInterrupt(uint32 channel, dmaInterrupt_t inttype)
*   @brief Disable selected interrupt
*   @param[in] channel DMA channel
*   @param[in] inttype Interrupt to be disabled
*                       - FTC: Frame Transfer Complete Interrupt will be disabled for the selected channel
*                       - LFS: Last Frame Transfer Started Interrupt will be disabled for the selected channel
*                       - HBC: First Half Of Block Complete Interrupt will be disabled for the selected channel
*                       - BTC: Block transfer complete Interrupt will be disabled for the selected channel
*                       - BER: Bus Error Interrupt will be disabled for the selected channel
*
*   This function disables the selected interrupt for the selected channel
*/
void dmaDisableInterrupt(dmaChannel_t channel, dmaInterrupt_t inttype)
{
/* USER CODE BEGIN (14) */
/* USER CODE END */

    switch (inttype)
    {
    case FTC:   dmaREG->FTCINTENAR = (uint32)1U << channel;
                break;
    case LFS:   dmaREG->LFSINTENAR = (uint32)1U << channel;
                break;
    case HBC:   dmaREG->HBCINTENAR = (uint32)1U << channel;
                break;
    case BTC:   dmaREG->BTCINTENAR = (uint32)1U << channel;
                break;
    default :
                break;
    }

/* USER CODE BEGIN (15) */
/* USER CODE END */
}
/** @fn void dmaDefineRegion(dmaREGION_t region, uint32 start_add, uint32 end_add)
*   @brief Configure start and end address of the region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] start_add Start address of the the region
*   @param[in] end_add End address of the region
*
*   This function configure start and end address of the selected region
*/
void dmaDefineRegion(dmaMPURegion_t region, uint32 start_add, uint32 end_add)
{
/* USER CODE BEGIN (16) */
/* USER CODE END */

	if(region < 4U)
	{
		dmaREG->DMAMPR_L[region].STARTADD = start_add;
		dmaREG->DMAMPR_L[region].ENDADD   = end_add;
	}
	else
	{
		dmaREG->DMAMPR_H[region - 4U].STARTADD = start_add;
		dmaREG->DMAMPR_H[region - 4U].ENDADD   = end_add;
	}
	
/* USER CODE BEGIN (17) */
/* USER CODE END */
}

/** @fn void dmaEnableRegion(dmaREGION_t region, dmaRegionAccess_t access, dmaMPUInt_t intenable)
*   @brief Enable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*   @param[in] access Access permission of the selected region
*                      - FULLACCESS
*                      - READONLY
*                      - WRITEONLY
*                      - NOACCESS
*   @param[in] intenable Interrupt to be enabled or not
*                         - INTERRUPTA_ENABLE : Enable Group A interrupt for the selected region
*                         - INTERRUPTB_ENABLE : Enable Group B interrupt for the selected region (Do not use this in case of Lock-step device)
*                         - INTERRUPT_DISABLE : Disable interrupt for the selected region
*
*   This function enables the selected region with selected access permission with or without interrupt enable
*/
void dmaEnableRegion(dmaMPURegion_t region, dmaRegionAccess_t access, dmaMPUInt_t intenable)
{
	uint8 bitpos = 0U;

/* USER CODE BEGIN (18) */
/* USER CODE END */
	
	if(region < 4U)
	{
		bitpos = region * 8U;
		dmaREG->DMAMPCTRL1 &= ~(uint32)((uint32)0xFFU << bitpos);


		dmaREG->DMAMPCTRL1 |= ((uint32)1U << bitpos)                /* Enable the region */
    		        	   |  ((uint32)access << (bitpos + 1U))     /* Set access permission for the region */
    		               |  ((uint32)intenable << (bitpos + 3U)); /* Enable or Disable interrupt */
	}
	else
	{
		bitpos = (region - 4U) * 8U;
		dmaREG->DMAMPCTRL2 &= ~((uint32)0xFFU << bitpos);


		dmaREG->DMAMPCTRL2 |= ((uint32)1U << bitpos)                /* Enable the region */
    		        	   |  ((uint32)access << (bitpos + 1U))     /* Set access permission for the region */
    		               |  ((uint32)intenable << (bitpos + 3U)); /* Enable or Disable interrupt */
	}

/* USER CODE BEGIN (19) */
/* USER CODE END */
}

/** @fn void dmaDisableRegion(dmaREGION_t region)
*   @brief Disable the selected region
*   @param[in] region Memory Region
*                     - DMA_REGION0
*                     - DMA_REGION1
*                     - DMA_REGION2
*                     - DMA_REGION3
*
*   This function disables the selected region(no address checking done).
*/
void dmaDisableRegion(dmaMPURegion_t region)
{
	uint8 bitpos = 0U;

/* USER CODE BEGIN (20) */
/* USER CODE END */

	if(region < 4U)
	{
		bitpos = region * 8U;
		dmaREG->DMAMPCTRL1 &= ~((uint32)1U << bitpos);
	}
	else
	{
		bitpos = (region - 4U) * 8U;
		dmaREG->DMAMPCTRL2 &= ~((uint32)1U << bitpos);
	}
	
/* USER CODE BEGIN (21) */
/* USER CODE END */
}

/** @fn void dmaEnableECC(void)
*   @brief Enable ECC
*
*   This function enables ECC check
*/
void dmaEnableECC(void)
{
/* USER CODE BEGIN (22) */
/* USER CODE END */

	dmaREG->DMAPCR = 0xAU;

/* USER CODE BEGIN (23) */
/* USER CODE END */
}

/** @fn void dmaDisableECC(void)
*   @brief Disable ECC
*
*   This function disables ECC check
*/
void dmaDisableECC(void)
{
/* USER CODE BEGIN (24) */
/* USER CODE END */

	dmaREG->DMAPCR = 0x5U;

/* USER CODE BEGIN (25) */
/* USER CODE END */
}

/** @fn uint32 dmaGetReq(uint32 channel)
*   @brief Gets the request line number mapped to the selected channel
*   @param[in] channel DMA channel
*
*   This function returns the request line number mapped to the selected channel
*/
uint32 dmaGetReq(dmaChannel_t channel)
{
    uint8 i,j;

/* USER CODE BEGIN (26) */
/* USER CODE END */

    i = channel / 4U;        /* Find the register to configure */
    j = channel % 4U;        /* Find the offset                */
    j = (uint8)3U - j;       /* reverse the byte order         */
    j = j * 8U;              /* find the bit location          */

/* USER CODE BEGIN (27) */
/* USER CODE END */

    return ((dmaREG->DREQASI[i] >> j) & 0xFFU);
}

/** @fn boolean dmaIsChannelActive(dmaChannel_t channel)
*   @brief Gets the status of the DMA channel
*   @param[in] channel DMA channel
*
*   This function returns TRUE if the channel is currently being processed using one of the FIFOs.
*/
boolean dmaIsChannelActive(dmaChannel_t channel)
{
	boolean status;
	uint32 bitmask = (uint32)1U << channel;

/* USER CODE BEGIN (28) */
/* USER CODE END */

	if((dmaREG->DMASTAT & bitmask) == 0U)
	{
		status = FALSE;
	}
	else
	{
		status = TRUE;
	}

/* USER CODE BEGIN (29) */
/* USER CODE END */

	return status;
}

/** @fn boolean dmaIsBusy(void)
*   @brief Gets the status of the DMA bus
*
*   This function returns TRUE if DMA's external bus is busy in data transfers
*/
boolean dmaIsBusy(void)
{
	boolean status;

/* USER CODE BEGIN (30) */
/* USER CODE END */

	if((dmaREG->GCTRL & 0x4000U) == 0U)
	{
		status = FALSE;
	}
	else
	{
		status = TRUE;
	}

/* USER CODE BEGIN (31) */
/* USER CODE END */
	
	return status;
}

boolean dmaGetInterruptStatus(dmaChannel_t channel, dmaInterrupt_t inttype)
{
	boolean status;
	uint32 mask = (uint32)1U << channel;

/* USER CODE BEGIN (32) */
/* USER CODE END */

	switch(inttype)
	{
    case FTC:   status = ((dmaREG->FTCFLAG & mask) != 0U);
                break;
    case LFS:   status = ((dmaREG->LFSFLAG & mask) != 0U);
                break;
    case HBC:   status = ((dmaREG->HBCFLAG & mask) != 0U);
                break;
    case BTC:   status = ((dmaREG->BTCFLAG & mask) != 0U);
                break;
    default :	status = FALSE;
                break;
	}

/* USER CODE BEGIN (33) */
/* USER CODE END */

	return status;
}



