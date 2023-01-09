#include <unistd.h>
// #include <syscall_def.h>

void write(char* ch) {


    __asm volatile("mov r5, %[v]": : [v] "r" (ch));
    
    __asm volatile("stmdb r13!, {r5}");

    // int x = SYS_write;
	// __asm volatile("mov R5, %[v]": : [v] "r" (&SYS_write));

    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 55\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}

void read(char **ch) 
{
    
    char *get = "abc";

    __asm volatile("mov r5, %[v]": : [v] "r" (&get));
    __asm volatile("stmdb r13!, {r5}");

    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 50\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );

    (*ch) = get;
}

int utilGetTime()
{
    unsigned int t;

    __asm volatile("mov r5, %[v]": : [v] "r" (&t));
    __asm volatile("stmdb r13!, {r5}");

    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 113\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );

    return t;
}

void uReboot()
{
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 119\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}

int uGetPid() 
{
    unsigned int t;

    __asm volatile("mov r5, %[v]": : [v] "r" (&t));
    __asm volatile("stmdb r13!, {r5}");

    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 5\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );

    return t;
}

void uYield()
{
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 120\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}

void exit() 
{
    __asm volatile (
        "stmdb r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
        "svc 3\n"
        "nop\n"
        "ldmia r13!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}\n"
    );
}