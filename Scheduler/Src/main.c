/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

uint8_t resource_in_use = 1;
uint32_t current_task = 0;

uint32_t task_stacks[MAX_TASK] =
								{
								T1_STACK_START,
								T2_STACK_START,
								T3_STACK_START,
								T4_STACK_START
								};
uint32_t task_handlers[MAX_TASK] =
								{
								(uint32_t)task1_handler,
								(uint32_t)task2_handler,
								(uint32_t)task3_handler,
								(uint32_t)task4_handler
								};

uint32_t psp_top_of_stack[MAX_TASK] =
								{
								T1_STACK_START,
								T2_STACK_START,
								T3_STACK_START,
								T4_STACK_START
								};


int main(void)
{
	enable_all_exceptions();

	initialise_msp_stack_addr(SCHED_STACK_START);

	init_tasks_stack();

	led_init_all();

	change_sp_to_psp();

	uint32_t sys_tick_hz = SYST_CLK / 1000;
	init_SysTick_Timer(sys_tick_hz);

	task1_handler();
    /* Loop forever */
	for(;;);
}

void init_SysTick_Timer(uint32_t ticks)
{
	//1. Program reload value.
	*SYST_RVR = ((0x00FFFFFF & ticks) -1);
	//2. Clear current value.
	*SYST_CVR &= ~(0x00FFFFFF);
	//3. Program Control and Status register.
	*SYST_CSR |= (0x00000007);

}

uint32_t get_psp_value(void)
{
	return psp_top_of_stack[current_task];
}


void save_psp_value(uint32_t psp)
{
	psp_top_of_stack[current_task] = psp;
}

void update_next_task(void)
{
	current_task = (current_task + 1)% MAX_TASK;
}

__attribute ((naked)) void initialise_msp_stack_addr(uint32_t stack_addr)
{
	__asm volatile ("MSR MSP, %0": : "r"(stack_addr):);

	__asm volatile ("BX LR");

}

__attribute ((naked)) void change_sp_to_psp(void)
{
	// 1. Update the PSP register to contain next Active Task PSP stored value
	__asm volatile ("PUSH {LR}"); // this is to avoid LR being lost
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");

	// 2. Change the CONTROL reg to make SP to be PSP;
	__asm volatile ("MOV R0, #0x02 ");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("BX LR"); // The earlier Pushed LR is used to return back to the callee
}

void enable_all_exceptions(void)
{
	uint32_t *pSHCRS = (uint32_t *)SHCRS;
	*pSHCRS |= (1<< 16);	//USGFAULTENA
	*pSHCRS |= (1<< 17);	//BUSFAULTENA
	*pSHCRS |= (1<< 18);	//MEMFAULTENA
}


__attribute ((naked)) void SysTick_Handler(void)
{

		// Save the SF2 of the running Task
		//1. Get current running task's PSP value
		__asm volatile ("MRS R0, PSP");
		//2. Using that PSP value store SF2( R4 to R11)
		__asm volatile ("STMDB R0!,{R4-R11}");

		__asm volatile ("PUSH {LR}");
		//3. Save the current value of PSP; the decremented R0
		__asm volatile ("BL save_psp_value");

		// 4. Identify the next Active Task
		__asm volatile ("BL update_next_task");


		//5. get its past PSP value
		__asm volatile ("BL get_psp_value");

		//6. Using that PSP value retrieve SF2(R4 to R11)
		__asm volatile ("LDMIA R0!,{R4-R11}");

		//7. update PSP and exit
		__asm volatile("MSR PSP,R0");

		// 8. Get the correct LR from the stack
		__asm volatile("POP {LR}");

		// 9. return from the handler as it is naked using LR
		__asm volatile("BX LR");
}


void init_tasks_stack(void)
{
	// Each of the stacks need to have the SF1 and SF2 initialised.
	uint32_t *pPSP = NULL;
	for (int i = 0; i < MAX_TASK; i++)
	{
		pPSP = (uint32_t*)task_stacks[i];

		pPSP--;
		*pPSP = DUMMY_XPSR;

		pPSP--;
		*pPSP = task_handlers[i];

		pPSP--;
		*pPSP = 0xFFFFFFFD;  // Return to Thread mode, exception return uses non-floating-point state from
							 // the PSP and execution uses PSP after return
		// Save R0 - R12 , total 13 registers.
		for (int j = 0; j < 13; j++)
		{
			pPSP--;
			*pPSP = 0x00;
		}

		psp_top_of_stack[i] = (uint32_t) pPSP;
	}
}

void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		delay(DELAY_COUNT_1S);
		led_off(LED_GREEN);
		delay(DELAY_COUNT_1S);
	}

}

void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
		led_off(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
	}

}

void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		delay(DELAY_COUNT_250MS);
		led_off(LED_BLUE);
		delay(DELAY_COUNT_250MS);
	}

}

void task4_handler(void)

{
	while(1)
	{
		led_on(LED_RED);
		delay(DELAY_COUNT_125MS);
		led_off(LED_RED);
		delay(DELAY_COUNT_125MS);
	}


}


//2. implement the fault handlers
void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}


void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}
