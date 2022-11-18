#include"coroutine.h"
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<limits.h>

#define ALPHA 0.5

typedef unsigned char bool;
typedef unsigned char byte;
const bool true = 1;
const bool false = 0;
#define COROUTINE_STACK_SIZE (40 * 1024)

typedef struct{
    void* func;
    ucontext_t uc;
    jmp_buf jmp;
    byte stack[COROUTINE_STACK_SIZE];
    bool enable;
    time_t cpuBurst;
    time_t guess;
}_coroutine_element;

typedef struct {
    _coroutine_element* data;
    int size;
    int reserve_size;
}_coroutine_container;

_coroutine_container _coroutine;

void push_back(_coroutine_container* c, void (*func)()){
    c->size++;
    if(c->size==c->reserve_size){
        c->reserve_size *= 2;
        c->data = realloc(c->data, c->reserve_size * sizeof(_coroutine_element));
    }
    c->data[c->size-1].func = func;
    c->data[c->size-1].enable = true;
}

// Moving memory cause too much time. Discarded.
/*void erase(_coroutine_container* c, int index){
    c->size--;
    memmove(c->data + index, c->data+index + 1, c->size - index);
}*/

void coroutine_init(){
    _coroutine.data = malloc(sizeof(_coroutine_element));
    _coroutine.size = 0;
    _coroutine.reserve_size = 1;
}

void coroutine_add(void (*func)()){
    push_back(&_coroutine, func);
}


void coroutine_start(){
    /*< Init coroutine*/
    for(int i=0;i<_coroutine.size;i++){
        getcontext(&(_coroutine.data[i].uc));
        _coroutine.data[i].uc.uc_stack.ss_sp = _coroutine.data[i].stack;
        _coroutine.data[i].uc.uc_stack.ss_size = sizeof(byte) * COROUTINE_STACK_SIZE;
        _coroutine.data[i].uc.uc_link = NULL;
        makecontext(&_coroutine.data[i].uc, _coroutine.data[i].func, 1);
    }
    /*Init coroutine >*/
    
    /*< Execute first time*/
    for(int i=0;i<_coroutine.size;i++){
        switch(setjmp(base_jmp_buf)){
            case 0:{
                _cur_jmp_buf = &_coroutine.data[i].jmp;
                setcontext(&_coroutine.data[i].uc);
                break;
            }
            case _END_CODE:{
                _coroutine.data[i].enable = false;
                break;
            }
        }
    }
    /*< Execute first time >*/


    bool isContinue = true;
    time_t prevT=0,nextT=0;
    int i=0;
    while(isContinue){
        isContinue = false;
        // Round robin for now..
        // Might change to a better alogorithm in the future
        //for(int i=0;i<_coroutine.size;i++){
            if(_coroutine.data[i].enable){
                //start
                _coroutine.data[i].cpuBurst=time(NULL);
                switch(setjmp(base_jmp_buf)){
                    case 0:{
                        _cur_jmp_buf = &_coroutine.data[i].jmp;
                        longjmp(_coroutine.data[i].jmp, 1);
                        break;
                    }
                    case _YEILD_CODE:{
                        isContinue = true;
                        break;
                    }
                    case _END_CODE:{
                        _coroutine.data[i].enable = false;
                        break;
                    }
                }
                _coroutine.data[i].cpuBurst=time(NULL)-_coroutine.data[i].cpuBurst;
                _coroutine.data[i].guess=prevT;
                nextT=ALPHA*_coroutine.data[i].cpuBurst+(1-ALPHA)*prevT;
                prevT=_coroutine.data[i].cpuBurst;
                if(_coroutine.data[i].cpuBurst<=_coroutine.data[i].guess){
                    unsigned long long mn=LLONG_MAX;
                    for(int j=0;j<_coroutine.size;++j){
                        if(_coroutine.data[j].enable){
                            if(_coroutine.data[j].cpuBurst<=mn){
                                mn=_coroutine.data[j].cpuBurst;
                                i=j;
                            }
                        }
                    }
                }else{
                    unsigned long long mx=LLONG_MIN;
                    for(int j=0;j<_coroutine.size;++j){
                        if(_coroutine.data[j].enable){
                            if(_coroutine.data[j].cpuBurst>=mx){
                                mx=_coroutine.data[j].cpuBurst;
                                i=j;
                            }
                        }
                    }
                }
                //end
            }
        //}
    }
}