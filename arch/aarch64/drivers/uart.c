#include <drivers/uart.h>
#include <stdint.h>

#define UART0_BASE 0x09000000

// Указатели на регистры через volatile, чтобы компилятор не оптимизировал обращение к памяти
#define UART_DR   (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART_FR   (*(volatile uint32_t *)(UART0_BASE + 0x18))

// Флаг заполненности буфера передачи (Transmit FIFO Full - бит 5)
#define UART_FR_TXFF (1 << 5)

void uart_init(void) {

}

bool uart_available(void) {
    return true;
}

void uart_putc(const char c) {
    while (UART_FR & UART_FR_TXFF) {
        // Пустой цикл ожидания
    }
    // Записываем байт в регистр данных
    UART_DR = (uint32_t) c;
}

void uart_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uart_putc(*str++);
    }
}