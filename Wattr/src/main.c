/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Wattr Hardware Revision 1--\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}



int main (void)
{
	sysclk_init();
	board_init();
	
	
	
	//spi_master_init(ADE7753_SPI_MODULE);

	ioport_set_pin_level(ADE7753_CS_GPIO, true);
	
	ioport_set_pin_level(LED1_GPIO, false);
	ioport_set_pin_level(LED2_GPIO, false);
	ioport_set_pin_level(LED3_GPIO, false);
	
	/* Initialize the console uart */
	configure_console();

	puts(STRING_HEADER);
	
	char input;
	uint8_t cmd;
	for (;;) {
		usart_serial_getchar(UART0, &input);
		usart_serial_putchar(UART0, input);
		usart_serial_putchar(UART0, '\r');
		usart_serial_putchar(UART0, '\n');

		switch (input) {
			case '1':
				ioport_toggle_pin_level(LED1_GPIO);
				break;
			case '2':
				ioport_toggle_pin_level(LED2_GPIO);
				break;
			case '3':
				ioport_toggle_pin_level(LED3_GPIO);
				break;
			case '4':
				ioport_toggle_pin_level(RELAY_1_GPIO);
				break;
			case '5':
				ioport_toggle_pin_level(RELAY_2_GPIO);
				break;
			case '6':
				cmd = 0x26; // Temp
				// uint8_t cmd = 0x3F; // Die Revision
				ioport_set_pin_level(ADE7753_CS_GPIO, false);
				
				spi_write(ADE7753_SPI_MODULE, 0x26, 0, 0);
				
				//spi_master_transfer(&cmd, sizeof(cmd));
				ioport_set_pin_level(ADE7753_CS_GPIO, true);
				
				char result[8];
				sprintf(result, "%d", cmd);
				
				puts(result);
				puts("\r");
				puts("\n");
				
		}
		 
	}
	
}