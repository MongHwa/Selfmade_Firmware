#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

static void Hw_init(void);
static void Printf_test(void);

void main()
{
    Hw_init();

    uint32_t i = 100;
    while(i--) {
        Hal_uart_put_char('N');
    }
    Hal_uart_put_char('\n');

    putstr("Hello World!\n");

    while(true);
}

static void Hw_init(void) {
    Hal_interrupt_init();
    Hal_uart_init();
}

static void Printf_test(void) {
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i = 5;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u\n", 0);
}

//Task Creation Test
void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Kernel_init(void) {
    uint32_t taskId;

    Kernel_task_init();
    Kernel_event_flag_init();

    taskId = Kernel_task_create(User_task0);
    if(NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if(NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if(NOT_ENOUGH_TASK_NUM == taskId) {
        putstr("Task2 creation fail\n");
    }

    Kernel_start();
}

void User_task0(void) {
    debug_printf("User Task #0\n");
    while(true);
}

void User_task1(void) {
    debug_printf("User Task #1\n");
    while(true);
}

void User_task2(void) {
    debug_printf("User Task #2\n");
    while(true);
}
//Task Creation Test End