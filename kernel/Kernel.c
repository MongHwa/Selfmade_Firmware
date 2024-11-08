#include "stdint.h"
#include "stdbool.h"

#include "Kernel.h"

void Kernel_start(void) {
    Kernel_task_start();
}

void Kernel_yield(void) {
    Kernel_task_scheduler();
}

bool Kernel_send_msg(KernelMsgQ_t Qname, void* data, uint32_t count) {
    uint8_t* d = (uint8_t*)data;

    for(uint32_t i = 0; i < count; i++) {
        //에러 나면 넣었던 만큼 다시 빼내기
        if(Kernel_msgQ_enqueue(Qname, *d) == false) {
            for(uint32_t j = 0; j < i; j++) {
                uint8_t rollback;
                Kernel_msgQ_dequeue(Qname, &rollback);
            }

            return false;
        }
        d++;
    }

    return true;
}

uint32_t Kernel_recv_msg(KernelMsgQ_t Qname, void* out_data, uint32_t count) {
    uint8_t* d = (uint8_t*)out_data;

    for(uint32_t i = 0; i < count; i++) {
        if(Kernel_msgQ_enqueue(Qname, d) == false) {
            //지금까지 읽었던 데이터 개수만 return
            return i;
        }
        d++;
    }

    return count;
}