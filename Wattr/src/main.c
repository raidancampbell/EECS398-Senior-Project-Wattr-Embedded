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

#include <ASF/thirdparty/CMSIS/Include/arm_math.h>
#define ARM_MATH_CM4

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

typedef struct ReadingPacket {
	uint8_t		header;
	uint8_t		reserved;
	uint16_t	flags;
	
	uint32_t	epoch;
	
	uint32_t	voltage;				// ADE7753_REGISTER_VRMS
	uint32_t	current;				// ADE7753_REGISTER_IRMS
	uint32_t	period;					// ADE7753_REGISTER_PERIOD
	 int32_t	active_power;			// ADE7753_REGISTER_LAENERGY
	 int32_t	reactive_power;			// ADE7753_REGISTER_LVARENERGY
	uint32_t	apparent_power;			// ADE7753_REGISTER_LVAENERGY
	uint32_t	phase_angle;			// TODO: Calculation
	uint32_t	power_factor;			// TODO: Calculation
	
	uint8_t		voltage_checksum;
	uint8_t		current_checksum;
	uint8_t		period_checksum;
	uint8_t		active_power_checksum;
	
	uint8_t		reactive_power_checksum;
	uint8_t		apparent_power_checksum;
	uint8_t		phase_angle_checksum;
	uint8_t		power_factor_checksum;
	
	uint32_t	checksum;
	uint32_t	footer;
} ReadingPacket;

volatile int count = -1;


void printDouble(float32_t v, uint8_t decimalDigits)
{
	uint8_t i = 1;
	uint8_t intPart, fractPart;
	for (;decimalDigits!=0; i*=10, decimalDigits--);
	intPart = (uint8_t)v;
	fractPart = (uint8_t)((v-(float32_t)(uint8_t)v)*i);
	printf("%d.%d\r\n", intPart, fractPart);
}
/*
char* float_to_string(float32_t number, uint8_t digits) {
	char* value = '      ';
	uint8_t index = 0;
	
	if (number < 0.0) {
		value[index] = '-';
		index++;
		number = -number;
	}
	
	if (number >= 100) {
		value[index] = '1';
		index++;
	}
	
	
	uint8_t integer_part = (uint8_t)number;
	
	
}*/


void spam_measurment(ReadingPacket *packet) {
	float32_t line_voltage = (float32_t)(packet->voltage);
	line_voltage *= 0.000237748f;
	
	line_voltage -= 0.14427f;
	
	printDouble(line_voltage, 3);
	
	//printf("%d\r\n", (uint32_t)line_voltage);
	
	//printf("%f\r\n", line_voltage);
	
//	printf("%d,%e,%d,%d,%d,%d,%d\r\n", count, line_voltage/*packet->voltage*/, packet->current, packet->period, packet->active_power, packet->apparent_power, packet->reactive_power);
}

ReadingPacket *packet_ni;


void ZX_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(LED1_GPIO);
	ioport_toggle_pin_level(FP_LED2_GPIO);
	
	if (count >= 0 || count == -2) {
		ReadingPacket *packet = (ReadingPacket*)calloc(1, sizeof(ReadingPacket));
		packet->header = 0x59;
		packet->reserved = 0x0000;
		packet->flags = ADE7753_FLAGS_NONE;
		packet->footer = 0x5254464d;
		
		
		ade7753_read(ADE7753_REGISTER_VRMS,   &(packet->voltage), ADE7753_REGISTER_VRMS_BYTES,    &(packet->voltage_checksum));
		ade7753_read(ADE7753_REGISTER_IRMS,   &(packet->current), ADE7753_REGISTER_IRMS_BYTES,    &(packet->current_checksum));
		ade7753_read(ADE7753_REGISTER_PERIOD, &(packet->period),  ADE7753_REGISTER_PERIOD_BYTES,  &(packet->period_checksum));
	
		spam_measurment(packet);
		
		if (count != -2) {
			count--;
		}
		
		//packet_ni = packet;
		
		free(packet);
	}
}

void FP_ENCODER_Handler(uint32_t id, uint32_t mask) {
	if (ioport_get_pin_level(FP_ENCODER_Q1_GPIO) && !ioport_get_pin_level(FP_ENCODER_Q2_GPIO)) {
		printf("CCW\r\n");
	} else if (!ioport_get_pin_level(FP_ENCODER_Q1_GPIO) && ioport_get_pin_level(FP_ENCODER_Q2_GPIO)) {
		printf("CW\r\n");
	}
}

void FP_LOAD_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(FP_LED0_GPIO);
	ioport_toggle_pin_level(RELAY_1_GPIO);
	ioport_toggle_pin_level(RELAY_2_GPIO);
}




void vfd_write(uint8_t data) {
	uint8_t i = 1;
	for (i = 0; i < 8; i++) {
		ioport_set_pin_level(VFD_SCK, false);
		delay_us(1);
		if (data & (1 << i)) {
			ioport_set_pin_level(VFD_MOSI, true);
		} else {
			ioport_set_pin_level(VFD_MOSI, false);
		}
		ioport_set_pin_level(VFD_SCK, true);
		delay_us(1);
	}
	delay_us(17);
}


void vfd_write_string(char data[]) {
	
	int index = 0;
	for (;;) {
		if (data[index] == '\0') {
			return;
		} else {
			vfd_write(data[index]);
			index++;
		}
	}
}

/*
void writePort(uint8_t data) {
	while (CHECK(BUSY));
	for (uint8_t i=1; i; i+=i) {
		LOWER(SCK);
		_delay_us(1);
		if (data & i) RAISE(OUT); else LOWER(OUT);
		RAISE(SCK);
		_delay_us(1);
	}
	_delay_us(17);
}*/


int main (void) {

	
	packet_ni = 0;
	
	sysclk_init();
	board_init();
		
	pmc_enable_periph_clk(PIN_ADE7753_ZX_ID);
	pio_handler_set(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_ID, PIN_ADE7753_ZX_MASK, PIN_ADE7753_ZX_ATTR, ZX_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_ADE7753_ZX_ID);
	pio_handler_set_priority(PIN_ADE7753_ZX_PIO, (IRQn_Type)PIN_ADE7753_ZX_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_LOAD_ID);
	pio_handler_set(PIN_FP_BUTTON_LOAD_PIO, PIN_FP_BUTTON_LOAD_ID, PIN_FP_BUTTON_LOAD_MASK, PIN_FP_BUTTON_LOAD_ATTR, FP_LOAD_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_LOAD_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_LOAD_PIO, (IRQn_Type)PIN_FP_BUTTON_LOAD_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_LOAD_PIO, PIN_FP_BUTTON_LOAD_MASK);
	
	pmc_enable_periph_clk(PIN_FP_ENCODER_Q1_ID);
	pio_handler_set(PIN_FP_ENCODER_Q1_PIO, PIN_FP_ENCODER_Q1_ID, PIN_FP_ENCODER_Q1_MASK, PIN_FP_ENCODER_Q1_ATTR, FP_ENCODER_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_ENCODER_Q1_ID);
	pio_handler_set_priority(PIN_FP_ENCODER_Q1_PIO, (IRQn_Type)PIN_FP_ENCODER_Q1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_ENCODER_Q1_PIO, PIN_FP_ENCODER_Q1_MASK);
	
	pmc_enable_periph_clk(PIN_FP_ENCODER_Q2_ID);
	pio_handler_set(PIN_FP_ENCODER_Q2_PIO, PIN_FP_ENCODER_Q2_ID, PIN_FP_ENCODER_Q2_MASK, PIN_FP_ENCODER_Q2_ATTR, FP_ENCODER_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_ENCODER_Q2_ID);
	pio_handler_set_priority(PIN_FP_ENCODER_Q2_PIO, (IRQn_Type)PIN_FP_ENCODER_Q2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_ENCODER_Q2_PIO, PIN_FP_ENCODER_Q2_MASK);
	
	
	
	
				
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
	
	ioport_set_pin_level(VFD_NRST, false);
	delay_ms(1);
	ioport_set_pin_level(VFD_NRST, true);
	
	delay_ms(500);
	
	vfd_write(0x1B);
	vfd_write('@');
	delay_ms(100);
	
	char stringdata[] = "Hello JTL!\0";
	
	vfd_write_string(stringdata);
	
	// We need to configure the ade7753...
	// ...to have a current gain of 2...
	uint8_t  gain		 = ADE7753_GAIN_PGA1_2;
	ade7753_write(ADE7753_REGISTER_GAIN, &gain, ADE7753_REGISTER_GAIN_BYTES);
	// ...and to measure energy for 120 half line cycles (e.g. 60 cycles or 1 second(ish))
	uint16_t line_cycles = 120;
	ade7753_write(ADE7753_REGISTER_LINECYC, &line_cycles, ADE7753_REGISTER_LINECYC_BYTES);
	
	// Magic fucking numbers brah.	
	uint8_t phase_offset = 14;
	ade7753_write(ADE7753_REGISTER_PHCAL, &phase_offset, ADE7753_REGISTER_PHCAL_BYTES);
	
	puts(STRING_HEADER);
		
	char input;
	uint32_t cmd = 0x00;
	uint8_t checksum = 0x00;
	
	uint32_t samples[10] = {0,0,0,0,0,0,0,0,0,0};
	uint8_t sample_checksum[10] = {0,0,0,0,0,0,0,0,0,0};
		
		int i = 0;
		int j = 0;
	
	//count = -2;
	
	
	//uint32_t foobar = 120;
	
	//float32_t barfoo = ((float32_t)foobar) * 0.51f;
	
//	float barfoo = 3.14f;
	
	//printf("%e\r\n", barfoo);
	printf("Test\r\n");
	
	printDouble(3.14, 2);
	
	/*for (;;) {
		if (packet_ni != 0) {
			spam_measurment(packet_ni);
			free(packet_ni);
			packet_ni = 0;
		}
	}*/
		
	
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
			case ',':
				count = -2;
				break;
			case '[':
				ade7753_calibrate_watt();
				break;
			case ']':
				ade7753_calibrate_watt_offset();
				break;
			case '\\':
				ade7753_calibrate_phase();
				break;
		}
	}
}
