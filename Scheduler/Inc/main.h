/*
 * main.h
 *
 *  Created on: Sep 3, 2024
 *      Author: siddi
 */

#ifndef MAIN_H_
#define MAIN_H_

#define DUMMY_XPSR 0x01000000U

#define SYST_CLK 	((uint32_t)16000000)

#define SYST_RVR 	((uint32_t*)0xE000E014)
#define SYST_CVR 	((uint32_t*)0xE000E018)
#define SYST_CSR 	((uint32_t*)0xE000E010)

#define MAX_TASK 4U
#define SHCRS 0xE000ED24


/* some stack memory calculations */
#define SIZE_TASK_STACK          1024U
#define SIZE_SCHED_STACK         1024U

#define SRAM_START               0x20000000U
#define SIZE_SRAM                ( (128) * (1024))
#define SRAM_END                 ((SRAM_START) + (SIZE_SRAM) )

#define T1_STACK_START           SRAM_END
#define T2_STACK_START           ( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START           ( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START           ( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define SCHED_STACK_START        ( (SRAM_END) - (4 * SIZE_TASK_STACK) )


void enable_all_exceptions(void);
__attribute ((naked)) void change_sp_to_psp(void);


uint32_t get_psp_value(void);
void save_psp_value(uint32_t psp);

void init_tasks_stack(void);
void init_SysTick_Timer(uint32_t ticks);
__attribute ((naked)) void initialise_msp_stack_addr(uint32_t stack_addr);

void task1Handler(void);
void task2Handler(void);
void task3Handler(void);
void task4Handler(void);

#endif /* MAIN_H_ */
