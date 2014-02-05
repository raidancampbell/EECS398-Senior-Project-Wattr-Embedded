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
#include <components/vfd.h>
#include "stdio_serial.h"
#include "conf_clock.h"
#include "conf_board.h"


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


#define MENU_STATE_SPLASH  0
#define MENU_STATE_VAFAAR  1
#define MENU_STATE_TRIG    2
#define MENU_STATE_COST    3
#define MENU_STATE_CONFIG  4

volatile uint8_t menu_state = MENU_STATE_SPLASH; 

volatile int32_t  count = -1;
volatile uint32_t zx_count = 0;
volatile int32_t  epoch = 1385917633;

volatile uint32_t global_voltage = 0;
volatile uint32_t global_current = 0;
volatile uint32_t global_period = 0;
volatile int32_t  global_active_power = 0;
volatile uint32_t global_apparent_power = 0;
volatile uint8_t  global_bullshit_checksum = 0;

volatile int32_t global_watt_hours_epoch = 0;
volatile float32_t global_watt_hours = 0.0f;

volatile char* global_voltage_string;
volatile char* global_current_string;
volatile char* global_frequency_string;
volatile char* global_active_power_string;
volatile char* global_apparent_power_string;
volatile char* global_reactive_power_string;
volatile char* global_power_factor_string;
volatile char* global_phase_angle_string;

char* create_measurement_string() {
	// Line Voltage y=mx+b
	float32_t line_voltage = (float32_t)(global_voltage);
	line_voltage *= 0.000237748f;
	line_voltage -= 0.14427f;
	global_voltage_string = gcvtf(line_voltage, 5, global_voltage_string);
	
	if (line_voltage >= 126.0f) {
		ioport_set_pin_level(FP_LED1_GPIO, true);
	} else if (line_voltage <= 114.0f) {
		ioport_set_pin_level(FP_LED1_GPIO, true);
	} else {
		ioport_set_pin_level(FP_LED1_GPIO, false);
	}
	
	// Line Current y=mx+b
	float32_t line_current = (float32_t)(global_current);
	line_current -= 953.97194f;
	line_current /= 113240.82786f;
	global_current_string = gcvtf(line_current, 3, global_current_string);
	
	// Line Active Power y=mx+b
	uint32_t unsigned_active_power = (uint32_t)global_active_power;
	float32_t line_active_power = (float32_t)(unsigned_active_power);
	line_active_power *= 0.016121f;
	line_active_power -= 0.0195f;
	global_active_power_string = gcvtf(line_active_power, 3, global_active_power_string);
	
	float32_t line_active_power_watt_hour = (float32_t)(unsigned_active_power);
	line_active_power_watt_hour *= .000075018f;
	line_active_power_watt_hour -= .03266f;
	
	global_watt_hours += line_active_power_watt_hour;
	
	// Line Apparent Power y=mx+b
	float32_t line_apparent_power = (float32_t)(global_apparent_power);
	line_apparent_power *= 0.019658f;
	line_apparent_power -= 1.3968f;
	global_apparent_power_string = gcvtf(line_apparent_power, 3, global_apparent_power_string);

	// Power Factor Calculation
	float32_t line_power_factor;
	line_power_factor = ((float32_t)((uint32_t)(global_active_power)))/((float32_t)(global_apparent_power));
	line_power_factor *= 0.827f;
	global_power_factor_string = gcvtf(line_power_factor, 3, global_power_factor_string);
	
	// Phase Angle Calculation
	float32_t line_phase_angle = acosf(line_power_factor);
	float32_t line_phase_angle_radian = line_phase_angle * 57.29f; // radians to degrees
	global_phase_angle_string = gcvtf(line_phase_angle_radian, 3, global_phase_angle_string);
	
	// Reactive Power Calculation
	float32_t line_reactive_power = sqrt
		((line_apparent_power * line_apparent_power) - (line_active_power * line_active_power));
	global_reactive_power_string = gcvtf(line_reactive_power, 2, global_reactive_power_string);
	
	// Line Frequency Calculation
	float32_t line_frequency = (float32_t)(global_period);
	line_frequency += 110.0f;
	line_frequency *= 0.0000022f;
	line_frequency = 1 / line_frequency;
	
	//char* line_frequency_string = malloc(32);
	//line_frequency_string = gcvtf(line_frequency, 5, line_frequency_string);
	global_frequency_string = gcvtf(line_frequency, 3, global_frequency_string);
	
	char* measurement = malloc(128);
	sprintf(measurement, "%d,%u,0,%s,%s,%s,%s,%s,%s,%s,%s", epoch, zx_count, global_voltage_string, global_current_string, global_frequency_string, global_active_power_string, global_apparent_power_string, global_reactive_power_string, global_power_factor_string, global_phase_angle_string);

	return measurement;
}

int32_t fix_signed_24(int32_t number) {
	int32_t fixed = (int32_t)number;
	number = number << 8;
	number /= 256;
	
	return number;
}

inline void read_voltage() {
	ade7753_read(ADE7753_REGISTER_VRMS,   &global_voltage, ADE7753_REGISTER_VRMS_BYTES,    &global_bullshit_checksum);
}

inline void read_current() {
	ade7753_read(ADE7753_REGISTER_IRMS,   &global_current, ADE7753_REGISTER_IRMS_BYTES,    &global_bullshit_checksum);
}

inline void read_period() {
	ade7753_read(ADE7753_REGISTER_PERIOD, &global_period,  ADE7753_REGISTER_PERIOD_BYTES,  &global_bullshit_checksum);
}

inline void read_active_power() {
	uint8_t checksum = 0;
	ade7753_read(ADE7753_REGISTER_LAENERGY, &global_active_power,    ADE7753_REGISTER_LAENERGY_BYTES,  &checksum);

	global_active_power = fix_signed_24(global_active_power);
	global_active_power	*= 10;
}

inline void read_apparent_power() {
	uint8_t checksum = 0;
	ade7753_read(ADE7753_REGISTER_LVAENERGY,   &global_apparent_power,    ADE7753_REGISTER_LVAENERGY_BYTES,  &checksum);
	global_apparent_power *= 10;
}

void ZX_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(LED1_GPIO);
	ioport_toggle_pin_level(FP_LED2_GPIO);
	if (zx_count == 60) {
		zx_count = 0;
		epoch++;
	}
		
	zx_count++;		
		
	read_voltage();
	read_current();
	read_period();
		
	char* measurement = create_measurement_string();

	printf("%s\r\n", measurement);

	free(measurement);
}

void IRQ_Handler(uint32_t id, uint32_t mask) {
	uint32_t interrupt_status = 0x00;
	uint8_t interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);

	read_active_power();
	read_apparent_power();
	
	uint32_t unsigned_active_power = (uint32_t)global_active_power; 
	float32_t line_active_power_watt_hour = (float32_t)(unsigned_active_power);
	line_active_power_watt_hour *= .000075018f;
	line_active_power_watt_hour -= .03266f;
	
	if (line_active_power_watt_hour >= 0) {
		global_watt_hours += line_active_power_watt_hour;
	}
}


uint32_t changed = false;

void FP_DOWN_Handler(uint32_t id, uint32_t mask) {
	if (menu_state != MENU_STATE_CONFIG) {
		menu_state++;
		changed = true;
	}
}

void FP_UP_Handler(uint32_t id, uint32_t mask) {
	if (menu_state != MENU_STATE_SPLASH) {
		menu_state--;
		changed = true;
	}
}


void FP_ENCODER_Handler(uint32_t id, uint32_t mask) {
	if (ioport_get_pin_level(FP_ENCODER_Q1_GPIO) && !ioport_get_pin_level(FP_ENCODER_Q2_GPIO)) {
		//printf("CCW\r\n");
		FP_UP_Handler(0,0);
	} else if (!ioport_get_pin_level(FP_ENCODER_Q1_GPIO) && ioport_get_pin_level(FP_ENCODER_Q2_GPIO)) {
		//printf("CW\r\n");
		FP_DOWN_Handler(0,0);
	}
}

void FP_BACK_Handler(uint32_t id, uint32_t mask) {	
	global_watt_hours_epoch = epoch;
	global_watt_hours = 0.0f;
}

void FP_LOAD_Handler(uint32_t id, uint32_t mask) {	
	ioport_toggle_pin_level(FP_LED0_GPIO);
	ioport_toggle_pin_level(RELAY_1_GPIO);
	ioport_toggle_pin_level(RELAY_2_GPIO);
}

int main (void) {
	global_watt_hours_epoch = epoch;
	global_voltage_string = malloc(32);
	global_current_string = malloc(32);
	global_frequency_string = malloc(32);
	global_active_power_string = malloc(32);
	global_apparent_power_string = malloc(32);
	global_reactive_power_string = malloc(32);
	global_power_factor_string = malloc(32);
	global_phase_angle_string = malloc(32);
	
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
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_UP_ID);
	pio_handler_set(PIN_FP_BUTTON_UP_PIO, PIN_FP_BUTTON_UP_ID, PIN_FP_BUTTON_UP_MASK, PIN_FP_BUTTON_UP_ATTR, FP_UP_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_UP_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_UP_PIO, (IRQn_Type)PIN_FP_BUTTON_UP_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_UP_PIO, PIN_FP_BUTTON_UP_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_DOWN_ID);
	pio_handler_set(PIN_FP_BUTTON_DOWN_PIO, PIN_FP_BUTTON_DOWN_ID, PIN_FP_BUTTON_DOWN_MASK, PIN_FP_BUTTON_DOWN_ATTR, FP_DOWN_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_DOWN_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_DOWN_PIO, (IRQn_Type)PIN_FP_BUTTON_DOWN_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_DOWN_PIO, PIN_FP_BUTTON_DOWN_MASK);
	
	pmc_enable_periph_clk(PIN_FP_BUTTON_BACK_ID);
	pio_handler_set(PIN_FP_BUTTON_BACK_PIO, PIN_FP_BUTTON_BACK_ID, PIN_FP_BUTTON_BACK_MASK, PIN_FP_BUTTON_BACK_ATTR, FP_BACK_Handler);
	NVIC_EnableIRQ((IRQn_Type)PIN_FP_BUTTON_BACK_ID);
	pio_handler_set_priority(PIN_FP_BUTTON_BACK_PIO, (IRQn_Type)PIN_FP_BUTTON_BACK_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_FP_BUTTON_BACK_PIO, PIN_FP_BUTTON_BACK_MASK);
	/*
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
	*/	
	
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
			
	// We need to configure the ade7753...
	// ...to have a current gain of 2...
	uint8_t  gain		 = ADE7753_GAIN_PGA1_2;
	ade7753_write(ADE7753_REGISTER_GAIN, &gain, ADE7753_REGISTER_GAIN_BYTES);
	
	// and a measurment of 2000 half line cycles
	uint32_t linecyc_int = 200;
	ade7753_write(ADE7753_REGISTER_LINECYC, &linecyc_int, ADE7753_REGISTER_LINECYC_BYTES);

	uint32_t mode_register = 0x0080;
	ade7753_write(ADE7753_REGISTER_MODE, &mode_register, ADE7753_REGISTER_MODE_BYTES);

	uint32_t irqen_register = 0x04;
	ade7753_write(ADE7753_REGISTER_IRQEN, &irqen_register, ADE7753_REGISTER_IRQEN_BYTES);

	uint8_t phase_offset = 14;
	ade7753_write(ADE7753_REGISTER_PHCAL, &phase_offset, ADE7753_REGISTER_PHCAL_BYTES);
	
	char input;
	
	vfd_init();
	
	//vfd_cursor_on();

	vfd_gui_splash(__DATE__, __TIME__);
	
	delay_s(5);
	
	vfd_clear();
	
	menu_state = MENU_STATE_VAFAAR;

	for(;;) {
		if (menu_state == MENU_STATE_SPLASH) {
			if (changed == true) {
				vfd_clear();
				vfd_home();
				vfd_gui_splash(__DATE__, __TIME__);
				//vfd_gui_splash();
				changed = false;
			}
		} else if (menu_state == MENU_STATE_VAFAAR) {
			if (changed == true) {
				vfd_clear();
				changed = false;
			}
			vfd_home();
			vfd_gui_vaf_aar();
		}  else if (menu_state == MENU_STATE_TRIG) {
			if (changed == true) {
				vfd_clear();
				changed = false;
			}
			vfd_home();
			vfd_gui_trig();
		}  else if (menu_state == MENU_STATE_COST) {
			if (changed == true) {
				vfd_clear();
				changed = false;
			}
			
			vfd_home();
			
			vfd_gui_cost();
		} else if (menu_state == MENU_STATE_CONFIG) {
			if (changed == true) {
				vfd_clear();
				changed = false;
			}
			
			vfd_home();
			vfd_gui_config();
		} else {
			vfd_clear();
		}
	}
}
