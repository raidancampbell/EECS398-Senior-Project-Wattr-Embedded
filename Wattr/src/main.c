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

#define ARM_MATH_CM4

#include <asf.h>
#include <math.h>
#include <ASF/thirdparty/CMSIS/Include/arm_math.h>
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

volatile int count = -1;


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


void float_to_string(char* output, float32_t v, uint8_t decimalDigits) {
	uint8_t i = 1;
	uint8_t intPart, fractPart;
	for (;decimalDigits!=0; i*=10, decimalDigits--);
	intPart = (uint8_t)v;
	fractPart = (uint8_t)((v-(float32_t)(uint8_t)v)*i);
	sprintf(output, "%d.%d", intPart, fractPart);
}



char* create_measurement_string(ReadingPacket *packet) {
	// Line Voltage y=mx+b
	float32_t line_voltage = (float32_t)(packet->voltage);
	line_voltage *= 0.000237748f;
	line_voltage -= 0.14427f;
	
	// Line Voltage float-to-string
	char* line_voltage_string = malloc(6);
	float_to_string(line_voltage_string, line_voltage, 3);
	
	// Line Current y=mx+b
	float32_t line_current = (float32_t)(packet->current);
	line_current -= 953.97194f;
	line_current /= 113240.82786f;
	
	// Line Current float-to-string
	char* line_current_string = malloc(6);
	float_to_string(line_current_string, line_current, 2);
	
	// Line Active Power y=mx+b
	uint32_t unsigned_active_power = (uint32_t)packet->active_power;
	float32_t line_active_power = (float32_t)(unsigned_active_power);
	line_active_power *= 0.016121f;
	line_active_power -= 7.0595f;
	
	// Line Active Power float-to-string
	char *line_active_power_string = malloc(6);
	float_to_string(line_active_power_string, line_active_power, 1);
	
	// Line Apparent Power y=mx+b
	float32_t line_apparent_power = (float32_t)(packet->apparent_power);
	line_apparent_power *= 0.019658f;
	line_apparent_power -= 11.7168f;
	
	// Line Apparent Power float-to-string
	char* line_apparent_power_string = malloc(6);
	float_to_string(line_apparent_power_string, line_apparent_power, 1);
	
	// Power Factor Calculation
	float32_t line_power_factor;
	line_power_factor = ((float32_t)((uint32_t)(packet->active_power)))/((float32_t)(packet->apparent_power));
	line_power_factor *= 0.827f;
	
	// Power Factor float-to-string
	char* line_power_factor_string = malloc(6);
	float_to_string(line_power_factor_string, line_power_factor, 2);

	// Phase Angle Calculation
	float32_t line_phase_angle = acosf(line_power_factor);
	char* line_phase_angle_string = malloc(6);
	float_to_string(line_phase_angle_string, line_phase_angle, 4);
	
	// Reactive Power Calculation
	float32_t line_reactive_power = tanf(line_phase_angle) * line_active_power;
	char* line_reactive_power_string = malloc(6);
	float_to_string(line_reactive_power_string, line_reactive_power, 4);
	
	// Line Frequency Calculation
	float32_t line_frequency = (float32_t)(packet->period);
	line_frequency *= 0.0000022f;
	line_frequency = 1 / line_frequency;
	
	char* line_frequency_string = malloc(6);
	float_to_string(line_frequency_string, line_frequency, 4);
	
	char* measurement = malloc(60);
	sprintf(measurement, "%sV %sA %sHz %sW %sVA %sVAR PF:%s Angle:%s", line_voltage_string, line_current_string, line_frequency_string, line_active_power_string, line_apparent_power_string, line_reactive_power_string, line_power_factor_string, line_phase_angle_string);
	
	free(line_voltage_string);
	free(line_current_string);
	free(line_apparent_power_string);
	free(line_active_power_string);
	free(line_power_factor_string);
	free(line_reactive_power_string);
	free(line_phase_angle_string);
	
	return measurement;
}

int32_t fix_signed_24(int32_t number) {
	int32_t fixed = (int32_t)number;
	number = number << 8;
	number /= 256;
	
	return number;
}

ReadingPacket* create_packet() {
	ReadingPacket *packet = (ReadingPacket*)calloc(1, sizeof(ReadingPacket));
	packet->header = 0x59;
	packet->reserved = 0x0000;
	packet->flags = ADE7753_FLAGS_NONE;
	packet->footer = 0x5254464d;
	
	return packet;
}

inline void read_voltage(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_VRMS,   &(packet->voltage), ADE7753_REGISTER_VRMS_BYTES,    &(packet->voltage_checksum));
}

inline void read_current(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_IRMS,   &(packet->current), ADE7753_REGISTER_IRMS_BYTES,    &(packet->current_checksum));
}

inline void read_period(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_PERIOD, &(packet->period),  ADE7753_REGISTER_PERIOD_BYTES,  &(packet->period_checksum));
}

inline void read_active_power(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_LAENERGY,   &(packet->active_power),    ADE7753_REGISTER_LAENERGY_BYTES,  &(packet->active_power_checksum));
	packet->active_power = fix_signed_24(packet->active_power);
}

inline void read_apparent_power(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_LVAENERGY,  &(packet->apparent_power), ADE7753_REGISTER_LVAENERGY_BYTES, &(packet->apparent_power_checksum));
}

inline void read_reactive_power(ReadingPacket *packet) {
	ade7753_read(ADE7753_REGISTER_LVARENERGY, &(packet->reactive_power), ADE7753_REGISTER_LVARENERGY_BYTES, &(packet->reactive_power_checksum));
	packet->reactive_power = fix_signed_24(packet->reactive_power);
}



void ZX_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(LED1_GPIO);
	ioport_toggle_pin_level(FP_LED2_GPIO);
	
	if (count >= 0 || count == -2) {
		ReadingPacket *packet = create_packet();
		
		read_voltage(packet);
		read_current(packet);
		read_period(packet);
			
		char* measurement = create_measurement_string(packet);
		
		printf("%s\r\n", measurement);
		
		if (count != -2) {
			count--;
		}

		free(packet);
		free(measurement);
	}
}

void IRQ_Handler(uint32_t id, uint32_t mask) {
	uint32_t interrupt_status = 0x00;
	uint8_t interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);

	ReadingPacket *packet = create_packet();
	
	read_voltage(packet);
	read_current(packet);
	read_period(packet);
	
	read_active_power(packet);
	read_apparent_power(packet);
	
	char* measurement = create_measurement_string(packet);
	
	printf("%s\r\n", measurement);
	
	free(packet);
	free(measurement);
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


int main (void) {

	
	
	irq_initialize_vectors();
	cpu_irq_enable();
	
	sysclk_init();
	board_init();
		
	pmc_enable_periph_clk(PIN_ADE7753_ZX_ID);
	pio_handler_set(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_ID, PIN_ADE7753_ZX_MASK, PIN_ADE7753_ZX_ATTR, ZX_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_ADE7753_ZX_ID);
	pio_handler_set_priority(PIN_ADE7753_ZX_PIO, (IRQn_Type)PIN_ADE7753_ZX_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_ADE7753_ZX_PIO, PIN_ADE7753_ZX_MASK);
	
		pmc_enable_periph_clk(PIN_ADE7753_IRQ_ID);
		pio_handler_set(PIN_ADE7753_IRQ_PIO, PIN_ADE7753_IRQ_ID, PIN_ADE7753_IRQ_MASK, PIN_ADE7753_IRQ_ATTR, IRQ_Handler);
		NVIC_EnableIRQ((IRQn_Type)PIN_ADE7753_IRQ_ID);
		pio_handler_set_priority(PIN_ADE7753_IRQ_PIO, (IRQn_Type)PIN_ADE7753_IRQ_ID, IRQ_PRIOR_PIO);
		pio_enable_interrupt(PIN_ADE7753_IRQ_PIO, PIN_ADE7753_IRQ_MASK);
	
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
uint32_t linecyc_int = 2000;
ade7753_write(ADE7753_REGISTER_LINECYC, &linecyc_int, ADE7753_REGISTER_LINECYC_BYTES);

uint32_t mode_register = 0x0080;
ade7753_write(ADE7753_REGISTER_MODE, &mode_register, ADE7753_REGISTER_MODE_BYTES);

uint32_t irqen_register = 0x04;
ade7753_write(ADE7753_REGISTER_IRQEN, &irqen_register, ADE7753_REGISTER_IRQEN_BYTES);


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


	sd_mmc_init();
		
	
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
