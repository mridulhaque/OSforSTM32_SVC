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



uint32_t current_task = 0;
uint32_t next_task = 1;
uint32_t magic_number = 0;
uint32_t total_task = 0;
uint32_t cur_magic_number = 100001;

struct tcb {
	uint32_t magic_number;
	long long stack[1024];
	uint32_t psp;
};

struct tcb tasks[4];

void unpriv (void) {
	unsigned int empty[256];
    for(int i = 0; i < 256; i++) empty[i] = 0;

    // kprintf("Inside Read : %s\n", ch);
    
    task_init_env_2(empty+256);

    __asm volatile (
        ".global task_init_env_2\n"
        "task_init_env_2:\n"
	        "msr psp, r0\n"
	        "mov r0, #3\n"
	        "msr control, r0\n"
	        "isb\n"
    );
}

void setPSP(unsigned int x) {

    task_init_env_3(x);

    __asm volatile (
        ".global task_init_env_3\n"
        "task_init_env_3:\n"
	        "msr psp, r0\n"
	        "isb\n"
    );
}




void thread1 () {
    printf("Thread 1\n");
	int i = 0;

	int cnt = 0;

    while(1) {
        i++;
        if(i >= 10000000) {
			int t = getTime();
			int p = getPid();
			printf("..........Thread 1 time : %d, pid : %d\n", t, p);

            // printf("in Thread 1 some val reached.\n");
            i = 0;
			cnt++;
			if(cnt == 2) {
				// reboot();
				yield();
				cnt = 0;
			}
        }
    }
}

void thread2 () {
    printf("Thread 2\n");
	int i = 0;
    while(1) {
        i++;
        if(i >= 10000000) {
			int t = getTime();
			int p = getPid();

			printf("Thread 2 time : %d, pid : %d\n", t, p);

            // printf("in Thread 2 some val reached.\n");
            i = 0;
        }
    }
}



uint32_t psp_array[4];

void PendSV_Handler(void) {

	for(int i = 0; i < total_task; i++) {
		psp_array[i] = tasks[i].psp;
	}
	__asm volatile(	"MRS R0, PSP\n"
					"STMDB R0!, {R4-R11}\n");
	__asm volatile("mov R5, %[v]": : [v] "r" (&current_task));
	__asm volatile("mov R6, %[v]": : [v] "r" (&next_task));
	__asm volatile("mov R7, %[v]": : [v] "r" (&psp_array));
	next_task++;
	if(next_task == total_task) next_task = 0;
	__asm volatile(
					"MOV R1, R5\n"
					"LDR R2, [R1]\n"
					"MOV R3, R7\n"
					"STR R0, [R3, R2, LSL #2]\n"
					"MOV R4, R6\n"
					"LDR R4, [R4]\n"
					"STR R4, [R1]\n"
					"LDR R0, [R3, R4, LSL #2]\n"
					"LDMIA R0!, {R4-R11}\n"
					"MSR PSP, R0\n");

	for(int i = 0; i < total_task; i++) {
		tasks[i].psp = psp_array[i];
	}
	magic_number = tasks[current_task].magic_number;
	__asm volatile("BX LR\n");

}


void createThread(unsigned long address)
{
	tasks[total_task].magic_number = cur_magic_number++;
	tasks[total_task].psp = ((unsigned int) tasks[total_task].stack) + (sizeof tasks[total_task].stack) - 16*4;
	HW32_REG((tasks[total_task].psp + (14 << 2))) = (unsigned long) address;
	HW32_REG((tasks[total_task].psp + (15 << 2))) = 0x01000000;

	total_task++;

	if(total_task > 1) {
		next_task = 1;
	} else {
		next_task = 0;
	}
}


void kmain(void)
{
	__sys_init();


	kprintf("Kmain started.\n");
	

	createThread((unsigned long) thread1);
	createThread((unsigned long) thread2);

	current_task = 0;
	setPSP((tasks[current_task].psp) + 16*4);


	__NVIC_SetPriority(SysTick_IRQn, 0x2);
	__NVIC_SetPriority(SVCall_IRQn, 1);
	__sysTick_enable();
	__NVIC_SetPriority(PendSV_IRQn, 0xFF);

	unpriv();

	__asm volatile("isb");


	printf(".....................\n");

	int x;
	char* ch = "ABBBCBNBNBCNBNCBNC";

	scanf("%d %s", &x, &ch);

	printf("Int : %d, String : %s\n", x, ch);
	
	magic_number = tasks[0].magic_number;

	thread1();

	kprintf("Kmain ended.\n");

	while(1) {
			
	}
}

