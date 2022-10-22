#ifndef __COROUTINE_H__
#define __COROUTINE_H__
#include<stdio.h>
#include<ucontext.h>
#include<setjmp.h>
#define _END_CODE 1
#define _YEILD_CODE 2

void coroutine_init();
void coroutine_add(void (*func)());
void coroutine_start();

jmp_buf* _cur_jmp_buf;
jmp_buf base_jmp_buf;
#define coroutine_yeild {\
    if(setjmp(*_cur_jmp_buf) == 0) {\
        longjmp(base_jmp_buf, _YEILD_CODE);\
    }\
}

#define coroutine_end {\
    longjmp(base_jmp_buf, _END_CODE);\
}

#endif