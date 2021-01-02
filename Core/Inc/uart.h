#ifndef __UART_H
#define __UART_H

extern volatile uint8_t data_available_flag, rx_data, rx_str[100], str_end;

void delay_9600(void);
void half_delay_9600(void);


void uart_tx(uint8_t byte);
//unsigned char uart_rx(void);
void uart_print_string(char *p);


#endif


