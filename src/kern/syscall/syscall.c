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

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <stdint.h>
#include <kunistd.h>
#include <stdlib.h>
#include <kmain.h>

void syscall(unsigned int *args)
{
/* The SVC_Handler calls this function to evaluate and execute the actual function */
/* Take care of return value or code */

	unsigned int svc_number;
	svc_number = ((char *) args[6])[-2];
	
	
	switch(svc_number)
	{
		/* Write your code to call actual function (kunistd.h/c or times.h/c and handle the return value(s) */
		case SYS_read:
			{
				char* ch_read; // may need to use malloc to allocate the memory.
				__sys_read(0, &ch_read);
				
				unsigned int string_add = args[19];
				*((char **)string_add) = ch_read;
				break;
			}
		case SYS_write:
			{
				unsigned int string_add = args[19];
				char *ch = string_add;
				__sys_write(STDOUT_FILENO, ch);
				break;
			}
		case SYS_reboot:
			{
				__sys_reboot();
			}
			break;	
		case SYS__exit:
			{
				int pid = __sys_getpid();
				__sys_task_kill(pid);
				__sys_yield();
				break;
			}
		case SYS_getpid:
			{
				unsigned int int_add = args[19];
				unsigned int *t = int_add;
				*((unsigned int **) int_add) = __sys_getpid();
				break;
			}
			break;
		case SYS___time:
			{
				unsigned int time_add = args[19];
				unsigned int *t = time_add;
				*((unsigned int **) time_add) = __getTime();
				break;
			}
		case SYS_yield:
			{
				__sys_yield();
			}
			break;				
		/* return error code see error.h and errmsg.h ENOSYS sys_errlist[ENOSYS]*/	
		default: ;
	}
/* Handle SVC return here */
}

