/*
 * main.h
 *
 *  Created on: Sep 3, 2024
 *      Author: siddi
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_TASK 4U
#define SHCRS 0xE000ED24


#define RAM_END 0x20020000
#define T1_STACK_START RAM_END
#define T1_STACK_END (T1_STACK_START + 0x400)

#define T2_STACK_START T1_STACK_END
#define T2_STACK_END (T2_STACK_START + 0x400)

#define T3_STACK_START T2_STACK_END
#define T3_STACK_END (T3_STACK_START + 0x400)

#define T4_STACK_START T3_STACK_END
#define T4_STACK_END (T4_STACK_START + 0x400)

#define MSP_STACK_START T3_STACK_END
#define MSP_STACK_END (T4_STACK_START + 0x400)

void enable_all_exceptions(void);
__attribute ((naked)) void change_sp_to_msp(void);
uint32_t get_current_stack_msp(void);

void task1Handler(void);
void task2Handler(void);
void task3Handler(void);
void task4Handler(void);

#endif /* MAIN_H_ */
