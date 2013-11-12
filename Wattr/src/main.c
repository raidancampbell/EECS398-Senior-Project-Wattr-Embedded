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

#define SPI_MASTER_BASE ADE7753_SPI_MODULE
#define SPI_CHIP_SEL 1
#define SPI_CLK_POLARITY 0
#define SPI_CLK_PHASE 0
#define SPI_DLYBS 0x40
static uint32_t gs_ul_spi_clock = 500000;
#define SPI_DLYBCT 0x10

static void spi_master_initialize(void)
{
	puts("-I- Initialize SPI as master\r");

	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI_MASTER_BASE);
	spi_disable(SPI_MASTER_BASE);
	spi_reset(SPI_MASTER_BASE);
	spi_set_lastxfer(SPI_MASTER_BASE);
	spi_set_master_mode(SPI_MASTER_BASE);
	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_SEL);
	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
	SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,
	(sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
	SPI_DLYBCT);
	spi_enable(SPI_MASTER_BASE);
}

static void spi_master_transfer(void *p_buf, uint32_t size)
{
	uint32_t i;
	uint8_t uc_pcs;
	static uint16_t data;

	uint8_t *p_buffer;

	p_buffer = p_buf;

	for (i = 0; i < size; i++) {
		spi_write(SPI_MASTER_BASE, p_buffer[i], 0, 1);
		/* Wait transfer done. */
		while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
		spi_read(SPI_MASTER_BASE, &data, &uc_pcs);
		p_buffer[i] = data;
	}
}

int main (void)
{
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
				cmd = 0x3F;										// die revision
				//puts("-> Master sending CMD_TEST... \r");
				spi_master_transfer(&cmd, sizeof(cmd));
				cmd = 0x00;
				spi_master_transfer(&cmd, sizeof(cmd));
				
				
				uint8_t mask = 0x3E;
				spi_master_transfer(&mask, sizeof(mask));
				mask = 0x00;
				spi_master_transfer(&mask, sizeof(mask));
				
				printf("-E- Response 0x%x with a checksum of 0x%x \n\r", (unsigned)cmd, (unsigned)mask);
				break;
				
		}
		 
	}
	
}