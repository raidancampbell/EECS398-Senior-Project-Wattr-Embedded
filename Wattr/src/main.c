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
#include <components/AFE/ade7753.h>

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Wattr Hardware Revision 1--\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/**
 *  Configure UART console.
 */
static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main (void) {
	sysclk_init();
	board_init();
		
		
	ioport_set_pin_level(LED1_GPIO, false);
	ioport_set_pin_level(LED2_GPIO, false);
	ioport_set_pin_level(LED3_GPIO, false);
	
	/* Initialize the console uart */
	configure_console();

	spi_master_initialize();

	puts(STRING_HEADER);
	
	char input;
	uint32_t cmd = 0x00;
	uint8_t checksum = 0x00;
	
	uint32_t samples[10] = {0,0,0,0,0,0,0,0,0,0};
	uint8_t sample_checksum[10] = {0,0,0,0,0,0,0,0,0,0};
		
		int i = 0;
		int j = 0;
	
	for (;;) {
		usart_serial_getchar(UART0, &input);
		usart_serial_putchar(UART0, input);
		usart_serial_putchar(UART0, '\r');
		usart_serial_putchar(UART0, '\n');

		switch (input) {
			case '1':
				printf("Switching LED1\r\n");
				ioport_toggle_pin_level(LED1_GPIO);
				break;
			case '2':
				printf("Switching LED2\r\n");
				ioport_toggle_pin_level(LED2_GPIO);
				break;
			case '3':
				printf("Switching LED3\r\n");
				ioport_toggle_pin_level(LED3_GPIO);
				break;
			case '4':
				printf("Switching RELAY 1\r\n");
				ioport_toggle_pin_level(RELAY_1_GPIO);
				break;
			case '5':
				printf("Switching RELAY 2\r\n");
				ioport_toggle_pin_level(RELAY_2_GPIO);
				break;
			case 'd':
				printf("Reading the DIEREV Register\r\n");
				ade7753_read(0x3F, &cmd, 1, &checksum);
				printf("Response 0x%x with a checksum of 0x%x %s \n\r", cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				break;
			case 'i':
				printf("Reading the IRMS Register\r\n");
				ade7753_read(0x16, &cmd, 3, &checksum);
				printf("Response 0x%x (%d) with a checksum of 0x%x %s \n\r", cmd, (signed)cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				break;
			case 'v':
				printf("Reading the VRMS Register\r\n");
				ade7753_read(0x17, &cmd, 3, &checksum);
				printf("Response 0x%x (%d) with a checksum of 0x%x %s \n\r", cmd, cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				break;
			case 'c':
			
				// [17:10:09] <+jtl> yetanothername: so, how obnoxious would it be for you to accumulate, say, 10 voltage (24-bit) readings
				// [17:10:14] <+jtl> on an inverval
			
				printf("Taking 10 VRMS samples with 26.042ms between them.\r\n");

				
				for (i = 0; i < 10; i++) {
					ade7753_read(0x17, &samples[i], 3, &sample_checksum[i]);
					for (j = 0; j < 1000000; j++) {}
				}
				
				for (i = 0; i < 10; i++) {
					printf("0x%x,%d,0x%x\r\n", samples[i], samples[i], sample_checksum[i]);
				}
				
				//
				uint32_t average = 0;
				
				for (i = 0; i < 10; i++) {
					average += samples[i];
				}
				
				average = average / 10;
		
				printf("average: %d\r\n", average);
				
				printf("Taking 10 IRMS samples with 26.042ms between them.\r\n");

				
				for (i = 0; i < 10; i++) {
					ade7753_read(0x16, &samples[i], 3, &sample_checksum[i]);
					for (j = 0; j < 1000000; j++) {}
				}
				
				for (i = 0; i < 10; i++) {
					printf("0x%x,%d,0x%x\r\n", samples[i], samples[i], sample_checksum[i]);
				}
				
				//
				average = 0;
				
				for (i = 0; i < 10; i++) {
					average += samples[i];
				}
				
				average = average / 10;
				
				printf("average: %d\r\n", average);
				break;
				
			case 'g':
			
				printf("sup bro");
				uint8_t gain_register = ADE7753_REGISTER_GAIN | ADE7753_WRITE_MASK;
				uint8_t gain_value = 0b00000000;
				uint8_t gain_checksum = 0;
				
				ade7753_read(ADE7753_REGISTER_GAIN, &gain_value, 1, &gain_checksum);
				printf("before: %x\r\n", gain_value);
				
				gain_value = 0b00000001;
				
				spi_master_transfer(&gain_register, sizeof(gain_register));
				ade7753_read(gain_value, &gain_value, 1, &gain_checksum);
				
				gain_value = 0b00000000;
				
				
				ade7753_read(ADE7753_REGISTER_GAIN, &gain_value, 1, &gain_checksum);
				
				printf("after: %x\r\n", gain_value);
				
				break;
			
			case 'a':
				printf("Reading the IRQEN Register\r\n");
				ade7753_read(0x0A, &cmd, 2, &checksum);
				printf("Response 0x%x (%b) with a checksum of 0x%x %s \n\r", cmd, cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				break;
			case 'p':
				printf("Reading the PERIOD Register\r\n");
				ade7753_read(0x27, &cmd, 2, &checksum);
				printf("Response 0x%x (%b) with a checksum of 0x%x %s \n\r", cmd, cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				break;
			case 's':
				printf("Reading the RSTSTATUS Register\r\n");
				for (;;) {
					ade7753_read(0x0C, &cmd, 2, &checksum);
					printf("Response 0x%x (%b) with a checksum of 0x%x %s \n\r", cmd, cmd, checksum, verify_result(&cmd, &checksum) ? "checksum passed" : "checksum failed");
				}
				break;
		}
	}
}