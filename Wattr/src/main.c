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

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0


typedef struct Reading {
	uint32_t voltage;				// ADE7753_REGISTER_VRMS
	uint32_t current;				// ADE7753_REGISTER_IRMS
	uint32_t frequency;				// 1 / ADE7753_REGISTER_PERIOD
	uint32_t active_power;			// ADE7753_REGISTER_RAENERGY
	uint32_t reactive_power;		// ADE7753_REGISTER_LVARENERGY
	uint32_t apparent_power;		// ADE7753_REGISTER_LVAENERGY
	uint32_t phase_angle;			// TODO: Calculation
	uint32_t power_factor;			// TODO: Calculation
	
	uint8_t voltage_checksum;
	uint8_t current_checksum;
	uint8_t frequency_checksum;
	uint8_t active_power_checksum;
	
	uint8_t reactive_power_checksum;
	uint8_t apparent_power_checksum;
	uint8_t phase_angle_checksum;
	uint8_t power_factor_checksum;
} Reading;


typedef struct Wire {
	uint8_t		header;
	uint8_t		reserved;
	uint16_t	flags;
	
	//struct reading		payload;
	
	uint32_t	checksum;
	uint32_t	footer;
} Wire;

volatile int count = 0;
uint32_t voltage = 0x00;
uint32_t current = 0x00;
uint32_t period = 0x00;
uint8_t checksum = 0x00;


void ZX_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(LED1_GPIO);

	
	if (count != 0) {
		voltage = 0x00;
		current = 0x00;
		period = 0x00;
		checksum = 0x00;
	
	
		ade7753_read(0x17, &voltage, 3, &checksum);
		ade7753_read(0x16, &current, 3, &checksum);
		ade7753_read(0x27, &period, 2, &checksum);
		printf("%d,%d,%d,%d\r\n", count, voltage, current, period);
		
		count--;
	}
}

void FP_LOAD_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(FP_LED0_GPIO);
	ioport_toggle_pin_level(RELAY_1_GPIO);
	ioport_toggle_pin_level(RELAY_2_GPIO);
}

void FP_BACK_Handler(uint32_t id, uint32_t mask) {
	ioport_toggle_pin_level(FP_LED3_GPIO);
	printf("***********************************************BACK\r\n");
}

void FP_LRUD_Handler(uint32_t id, uint32_t mask) {
	ioport_toggle_pin_level(LED3_GPIO);
	printf("Up: %d Left %d Right %d Down %d\r\n", ioport_get_pin_level(FP_BUTTON_UP_GPIO), ioport_get_pin_level(FP_BUTTON_LEFT_GPIO), ioport_get_pin_level(FP_BUTTON_RIGHT_GPIO), ioport_get_pin_level(FP_BUTTON_DOWN_GPIO));
}


void FP_SELECT_Handler(uint32_t id, uint32_t mask) {
	ioport_toggle_pin_level(FP_LED3_GPIO);
}

void FP_ENCODER_Handler(uint32_t id, uint32_t mask) {
	printf("%d %d\r\n", ioport_get_pin_level(FP_ENCODER_Q1_GPIO), ioport_get_pin_level(FP_ENCODER_Q2_GPIO));

}

int main (void) {
	sysclk_init();
	board_init();
	
	
	pmc_enable_periph_clk(PIN_ADE7753_ZX_ID);
	pio_handler_set(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_ID, PIN_ADE7753_ZX_MASK, PIN_ADE7753_ZX_ATTR, ZX_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_ADE7753_ZX_ID);
	pio_handler_set_priority(PIN_ADE7753_ZX_PIO, (IRQn_Type)PIN_ADE7753_ZX_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_MASK);
	
	/*pmc_enable_periph_clk(PIN_UART0_RTS_ID);
	pio_handler_set(PIN_UART0_RTS_PIO, PIN_UART0_RTS_ID, PIN_UART0_RTS_MASK, PIN_UART0_RTS_ATTR, ZX_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_UART0_RTS_ID);
	pio_handler_set_priority(PIN_UART0_RTS_PIO, (IRQn_Type)PIN_UART0_RTS_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_UART0_RTS_PIO, PIN_UART0_RTS_MASK);*/
	
	/*pmc_enable_periph_clk(PIN_FP_BUTTON_LOAD_ID);
	pio_handler_set(PIN_FP_BUTTON_LOAD_PIO, PIN_FP_BUTTON_LOAD_ID, PIN_FP_BUTTON_LOAD_MASK, PIN_FP_BUTTON_LOAD_ATTR, FP_LOAD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_LOAD_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_LOAD_PIO, (IRQn_Type)PIN_FP_BUTTON_LOAD_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_LOAD_PIO, PIN_FP_BUTTON_LOAD_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_BACK_ID);
	pio_handler_set(PIN_FP_BUTTON_BACK_PIO, PIN_FP_BUTTON_BACK_ID, PIN_FP_BUTTON_BACK_MASK, PIN_FP_BUTTON_BACK_ATTR, FP_BACK_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_BACK_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_BACK_PIO, (IRQn_Type)PIN_FP_BUTTON_BACK_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_BACK_PIO, PIN_FP_BUTTON_BACK_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_SELECT_ID);
	pio_handler_set(PIN_FP_BUTTON_SELECT_PIO, PIN_FP_BUTTON_SELECT_ID, PIN_FP_BUTTON_SELECT_MASK, PIN_FP_BUTTON_SELECT_ATTR, FP_SELECT_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_SELECT_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_SELECT_PIO, (IRQn_Type)PIN_FP_BUTTON_SELECT_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_SELECT_PIO, PIN_FP_BUTTON_SELECT_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_UP_ID);
	pio_handler_set(PIN_FP_BUTTON_UP_PIO, PIN_FP_BUTTON_UP_ID, PIN_FP_BUTTON_UP_MASK, PIN_FP_BUTTON_UP_ATTR, FP_LRUD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_UP_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_UP_PIO, (IRQn_Type)PIN_FP_BUTTON_UP_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_UP_PIO, PIN_FP_BUTTON_UP_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_DOWN_ID);
	pio_handler_set(PIN_FP_BUTTON_DOWN_PIO, PIN_FP_BUTTON_DOWN_ID, PIN_FP_BUTTON_DOWN_MASK, PIN_FP_BUTTON_DOWN_ATTR, FP_LRUD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_DOWN_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_DOWN_PIO, (IRQn_Type)PIN_FP_BUTTON_DOWN_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_DOWN_PIO, PIN_FP_BUTTON_DOWN_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_LEFT_ID);
	pio_handler_set(PIN_FP_BUTTON_LEFT_PIO, PIN_FP_BUTTON_LEFT_ID, PIN_FP_BUTTON_LEFT_MASK, PIN_FP_BUTTON_LEFT_ATTR, FP_LRUD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_LEFT_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_LEFT_PIO, (IRQn_Type)PIN_FP_BUTTON_LEFT_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_LEFT_PIO, PIN_FP_BUTTON_LEFT_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_RIGHT_ID);
	pio_handler_set(PIN_FP_BUTTON_RIGHT_PIO, PIN_FP_BUTTON_RIGHT_ID, PIN_FP_BUTTON_RIGHT_MASK, PIN_FP_BUTTON_RIGHT_ATTR, FP_LRUD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_RIGHT_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_RIGHT_PIO, (IRQn_Type)PIN_FP_BUTTON_RIGHT_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_RIGHT_PIO, PIN_FP_BUTTON_RIGHT_MASK);*/
	
	/*pmc_enable_periph_clk(PIN_FP_ENCODER_Q1_ID);
	pio_handler_set(PIN_FP_ENCODER_Q1_PIO, PIN_FP_ENCODER_Q1_ID, PIN_FP_ENCODER_Q1_MASK, PIN_FP_ENCODER_Q1_ATTR, FP_ENCODER_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_ENCODER_Q1_ID);
	pio_handler_set_priority(PIN_FP_ENCODER_Q1_PIO, (IRQn_Type)PIN_FP_ENCODER_Q1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_ENCODER_Q1_PIO, PIN_FP_ENCODER_Q1_MASK);
	
	pmc_enable_periph_clk(PIN_FP_ENCODER_Q2_ID);
	pio_handler_set(PIN_FP_ENCODER_Q2_PIO, PIN_FP_ENCODER_Q2_ID, PIN_FP_ENCODER_Q2_MASK, PIN_FP_ENCODER_Q2_ATTR, FP_ENCODER_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_ENCODER_Q2_ID);
	pio_handler_set_priority(PIN_FP_ENCODER_Q2_PIO, (IRQn_Type)PIN_FP_ENCODER_Q2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_ENCODER_Q2_PIO, PIN_FP_ENCODER_Q2_MASK);*/
			
	ioport_set_pin_level(LED1_GPIO, false);
	ioport_set_pin_level(LED2_GPIO, false);
	ioport_set_pin_level(LED3_GPIO, false);
	ioport_set_pin_level(FP_LED0_GPIO, false);
	ioport_set_pin_level(FP_LED1_GPIO, false);
	ioport_set_pin_level(FP_LED2_GPIO, false);
	ioport_set_pin_level(FP_LED3_GPIO, true);
	ioport_set_pin_level(RELAY_1_GPIO, false);
	ioport_set_pin_level(RELAY_2_GPIO, false);
	
	
	
	/* Initialize the console uart */
	configure_console();

	spi_master_initialize();
	 int a = 0;
	for (;;){
		printf("Up: %d Left %d Right %d Down %d Load: %d Back %d Select %d Q1 %d Q2 %d\r\n", ioport_get_pin_level(FP_BUTTON_UP_GPIO), ioport_get_pin_level(FP_BUTTON_LEFT_GPIO), ioport_get_pin_level(FP_BUTTON_RIGHT_GPIO), ioport_get_pin_level(FP_BUTTON_DOWN_GPIO), ioport_get_pin_level(FP_BUTTON_LOAD_GPIO), ioport_get_pin_level(FP_BUTTON_BACK_GPIO), ioport_get_pin_level(FP_BUTTON_SELECT_GPIO), ioport_get_pin_level(FP_ENCODER_Q1_GPIO), ioport_get_pin_level(FP_ENCODER_Q2_GPIO));
	}
	

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
			case 'g':
			
				printf("Setting GAIN = 2 (I think)\r\n");
				uint8_t gain_register = ADE7753_REGISTER_GAIN | ADE7753_WRITE_MASK;
				uint8_t gain_value = 0b00000000;
				uint8_t gain_checksum = 0;
				
				ade7753_read(ADE7753_REGISTER_GAIN, &gain_value, 1, &gain_checksum);
				printf("Current Gain Before: %x\r\n", gain_value);
				
				gain_value = 0b00000001;
				
				spi_master_transfer(&gain_register, sizeof(gain_register));
				ade7753_read(gain_value, &gain_value, 1, &gain_checksum);
				
				gain_value = 0b00000000;
				
				
				ade7753_read(ADE7753_REGISTER_GAIN, &gain_value, 1, &gain_checksum);
				
				printf("Current Gain After: %x\r\n", gain_value);
				
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
			case 'b':
				count = 10;
				break;
			case 'n':
				count = 50;
				break;
			case 'm':
				count = 100;
				break;
		}
	}
}
