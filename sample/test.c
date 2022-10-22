#include"../CR/coroutine.h"
#include<stdio.h>
#include<ucontext.h>
#include<setjmp.h>

//ucontext_t ct1,ct2,ct3;
//#define STACK_SIZE (60 * 1024)
//char stack1[STACK_SIZE],stack2[STACK_SIZE],stack3[STACK_SIZE];

//jmp_buf main_jmp_buf, test1_jmp_buf, test2_jmp_buf, test3_jmp_buf;



void test1(){
    for(int i=1;i<=7;i++){
        printf("Test1 : %d\n", i);
        coroutine_yeild;
    }
    printf("Test1 : end\n");
    coroutine_end;
}

void test2(){
    for(int i=1;i<=3;i++){
        printf("Test2 : %d\n", i);
        coroutine_yeild;
    }
    printf("Test2 : end\n");
    coroutine_end;
}

void test3(){
    for(int i=1;i<=6;i++){
        printf("Test3 : %d\n", i);
        coroutine_yeild;
    }
    printf("Test3 : end\n");
    coroutine_end;
}



int main(int argc, char* argv[]){
    coroutine_init();
    coroutine_add(test1);
    coroutine_add(test2);
    coroutine_add(test3);
    coroutine_start();
    return 0;
}