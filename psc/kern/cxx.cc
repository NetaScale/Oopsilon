#include <exception>
#include <stdexcept>
#include <string>
#include "libkern/klib.h"

void hello2()
 {
                std::string str("hello");

        kprintf("STR: %s\n", str.c_str());
        throw std::runtime_error("Hello from an exception");
 }

void hello1()
{
                        try {
        hello2();
                }
                catch(std::runtime_error& f) {
                        kprintf("Exception caught: %s\n", f.what());
                }
}

extern "C" {
extern void (*__preinit_array_start)(void);
extern void (*__preinit_array_end)(void);
extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);
void __register_frame(void* begin);
extern void *__eh_frame_start;
extern void *__EH_FRAME_BEGIN__;


// void frame_dummy(void);
// void __do_global_ctors_aux(void);

void _init(void);
__attribute__((constructor))
 void fooey(void)
{
        kprintf("Hiya from constructor\n");       
}

}

extern "C" void helloc()
{

#if 0
        kprintf("eh frame start = %p\n", &__eh_frame_start);
        //kprintf("frame begin = %p\n", &__EH_FRAME_BEGIN__);

             kprintf("_init:\n"); 
               // frame_dummy();
               // __do_global_ctors_aux();
             _init();
        #endif


        #if 1
        __register_frame(&__eh_frame_start);
        kprintf("Preinit array\n");
        	for (void (**func)(void) = &__preinit_array_start; func != &__preinit_array_end;
	     func++) {
                kprintf("call preinit fun %p\n", func);
		(*func)();
             }

        kprintf("Init array\n");
        	for (void (**func)(void) = &__init_array_start; func != &__init_array_end;
	     func++) {
                kprintf("call init fun %p\n", func);
		(*func)();
             }
#endif
                kprintf("Hello from C++\n");
        hello1();
}
