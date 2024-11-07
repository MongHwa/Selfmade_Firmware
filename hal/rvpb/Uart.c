#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"

extern volatile PL011_t* Uart;

void Hal_uart_init(void) {
    //Enable UART
    Uart->uartcr.bits.UARTEN = 0;   //하드웨어 off
    Uart->uartcr.bits.TXE = 1;      //출력 on
    Uart->uartcr.bits.RXE = 1;      //입력 on
    Uart->uartcr.bits.UARTEN = 1;   //하드웨어 on

    //Enable input interrupt
    Uart->uartimsc.bits.RXIM = 1;

    //Register UART interrupt handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch) {
    while(Uart->uartfr.bits.TXFF);  //출력 버퍼 0 될 때까지 대기
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void) {
    uint32_t data;
    
    while(Uart->uartfr.bits.RXFE);

    data = Uart->uartdr.all;

    if(data & 0xFFFFFF00) {
        //에러 발생
        Uart->uartrsr.all = 0xFF;
        return 0;
    }

    return (uint8_t)(data & 0xFF);
}

//Interrupt, Event
static void interrupt_handler(void) {
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);

    Kernel_send_events(KernelEventFlag_UartIn);
}