/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
#include <unistd.h>
#include <ustdio.h>
#include <./../../userland/main.c>

//#include "../include/float.h"

#define HW32_REG(ADDRESS) (*((volatile unsigned long *)(ADDRESS)))
#define MAX_TASK 8

#define READY 0
#define WAITING 1
#define RUNNING 2
#define KILLED 3



uint32_t current_task = 0;
uint32_t magic_number = 0;
uint32_t total_task = 0;
uint32_t cur_magic_number = 100001;

struct tcb {
	uint32_t magic_number;
	long long stack[1024];
	uint32_t psp;
	uint8_t status;
};

struct tcb tasks[MAX_TASK];

void __sys_task_kill(uint32_t magic_number)
{
	for(int i = 0; i < MAX_TASK; i++)
	{
		if(tasks[i].magic_number == magic_number) {tasks[i].status = KILLED; break;}
	}
}

void switchToUnpriv (void) {
	// prepare a stack for unpriviledged mode
	const int INITIAL_STACK_SIZE = 256;
	unsigned int unpriv_stack[INITIAL_STACK_SIZE];
    for(int i = 0; i < INITIAL_STACK_SIZE; i++) unpriv_stack[i] = 0; // reset the unpriv stack
    
	// call an assembly function for turning on unpriviledged mode
    turn_on_unpriv_mode(unpriv_stack+INITIAL_STACK_SIZE);

    __asm volatile (
        ".global turn_on_unpriv_mode\n"
        "turn_on_unpriv_mode:\n"
	        "msr psp, r0\n" // set the unpriv_stack's last address on psp
	        "mov r0, #3\n" 
	        "msr control, r0\n" // set unpriv mode and psp
	        "isb\n" // throw away any prefetched instructions
    );
}

void setPSP(unsigned int x) {

    set_psp_asm(x);

    __asm volatile (
        ".global set_psp_asm\n"
        "set_psp_asm:\n"
	        "msr psp, r0\n"
	        "isb\n"
    );
}




void task_1 () {
    printf("Task 1 Started\n");
	int i = 0;
    while(1) {
        i++;
        if(i >= 10000000) {
			int t = getTime();
			int p = getPid();
			printf(">> Task 1 time : %d, p-id : %d\n", t, p);
            i = 0;
        }
    }
}

void task_2 () {
    printf("Task 2\n");
	int i = 0;
    while(1) {
        i++;
        if(i >= 10000000) {
			int t = getTime();
			int p = getPid();
			printf(">> Task 2 time : %d, p-id : %d\n", t, p);
            i = 0;
        }
    }
}

void task_3 () {
    printf("Task 3\n");
	int i = 0;
	int cnt = 0;
    while(1) {
        i++;
        if(i >= 10000000) {
			int p = getPid();
			printf(">> Task 3, p-id : %d\n",  p);
            i = 0;
			cnt++;
			if(cnt == 3) exit();
        }
    }
}



uint32_t psp_array[MAX_TASK]; 
int next_task,i, previous_task;

void PendSV_Handler(void) {
	if(!total_task) return;
	for(i = 0; i < total_task; i++) {
		psp_array[i] = tasks[i].psp;
	}
	// store the registers {R4-R11}
	__asm volatile(	"MRS R0, PSP\n" 
					"STMDB R0!, {R4-R11}\n"); 

	previous_task = current_task;
	while(1)
	{
		next_task = (previous_task)%(total_task-1) + 1;
		if(tasks[next_task].status == READY) break;
		else {
			if(next_task == current_task) {
				next_task = 0; break;
			}
			else previous_task = next_task;
		}
	}
	

	if(tasks[current_task].status != KILLED) tasks[current_task].status = READY;
	// int next_task = 0;
	__asm volatile("mov R5, %[v]": : [v] "r" (&current_task));
	__asm volatile("mov R6, %[v]": : [v] "r" (&next_task));
	__asm volatile("mov R7, %[v]": : [v] "r" (&psp_array));

	__asm volatile(
					"MOV R1, R5\n" // store current task address in R1
					"LDR R2, [R1]\n" // load current task value in R2
					"MOV R3, R7\n" // load psp array address in R3
					"STR R0, [R3, R2, LSL #2]\n" // store psp into PSP array current task position
					"MOV R4, R6\n" // load next task address in R4
					"LDR R4, [R4]\n" // load next task value in R4
					"STR R4, [R1]\n" // store next task value into current task
					"LDR R0, [R3, R4, LSL #2]\n" // load next task PSP into R0
					"LDMIA R0!, {R4-R11}\n" // load {R4-R11} from next task psp
					"MSR PSP, R0\n"  // update psp with next task psp
	);
	tasks[current_task].status = RUNNING;
	for(i = 0; i < total_task; i++) {
		tasks[i].psp = psp_array[i];
	}

	magic_number = tasks[current_task].magic_number;
	__asm volatile("BX LR\n");
}


void createThread(unsigned long address)
{
	tasks[total_task].magic_number = cur_magic_number++;
	tasks[total_task].psp = ((unsigned int) tasks[total_task].stack) + (sizeof tasks[total_task].stack) - 16*4;
	HW32_REG((tasks[total_task].psp + (14 << 2))) = (unsigned long) address; //PC
	HW32_REG((tasks[total_task].psp + (15 << 2))) = 0x01000000; //xPSR
	tasks[total_task].status = READY;
	total_task++;
}


void kmain(void)
{
	__sys_init();


	kprintf("Starting KMain......\n");
	magic_number = tasks[0].magic_number;
	total_task = 1;
	
	current_task = 0;
	createThread((unsigned long) task_1);
	createThread((unsigned long) task_2);
	createThread((unsigned long) task_3);


	setPSP((tasks[current_task].psp) + 16*4);

	__NVIC_SetPriority(SysTick_IRQn, 0x2);
	__NVIC_SetPriority(SVCall_IRQn, 1);
	__sysTick_enable();
	__NVIC_SetPriority(PendSV_IRQn, 0xFF);

	switchToUnpriv();

	__asm volatile("isb");

	// task_1();

	while(1) {
		int sum = 0;
		for(int i = 0; i < 10000000; i++) {
			sum += i;
		}
		printf("Waiting for PendSV...\n");
	}
}

