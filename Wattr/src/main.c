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

/* Chip select. */
#define SPI_CHIP_SEL 0

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 1

static uint32_t gs_ul_spi_clock = 500000;

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

/* Delay before SPCK. */
#define SPI_DLYBS 0x40

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10

static void spi_master_initialize(void)
{
	puts("-I- Initialize SPI as master\r");

	/* Configure an SPI peripheral. */
	spi_enable_clock(ADE7753_SPI_MODULE);
	spi_disable(ADE7753_SPI_MODULE);
	spi_reset(ADE7753_SPI_MODULE);
	spi_set_lastxfer(ADE7753_SPI_MODULE);
	spi_set_master_mode(ADE7753_SPI_MODULE);
	spi_disable_mode_fault_detect(ADE7753_SPI_MODULE);
	spi_set_peripheral_chip_select_value(ADE7753_SPI_MODULE, SPI_CHIP_SEL);
	spi_set_clock_polarity(ADE7753_SPI_MODULE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(ADE7753_SPI_MODULE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(ADE7753_SPI_MODULE, SPI_CHIP_SEL,
	SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(ADE7753_SPI_MODULE, SPI_CHIP_SEL,
	(sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(ADE7753_SPI_MODULE, SPI_CHIP_SEL, SPI_DLYBS,
	SPI_DLYBCT);
	spi_enable(ADE7753_SPI_MODULE);
}

/**
 * \brief Perform SPI master transfer.
 *
 * \param pbuf Pointer to buffer to transfer.
 * \param size Size of the buffer.
 */
static void spi_master_transfer(void *p_buf, uint32_t size)
{
	uint32_t i;
	uint8_t uc_pcs;
	static uint16_t data;

	uint8_t *p_buffer;

	p_buffer = p_buf;

	for (i = 0; i < size; i++) {
		spi_write(ADE7753_SPI_MODULE, p_buffer[i], 0, 0);
		/* Wait transfer done. */
		while ((spi_read_status(ADE7753_SPI_MODULE) & SPI_SR_RDRF) == 0);
		spi_read(ADE7753_SPI_MODULE, &data, &uc_pcs);
		p_buffer[i] = data;
	}
}

int main (void)
{
	sysclk_init();
	board_init();

	spi_master_initialize();

	ioport_toggle_pin_level(LED1_GPIO);


	ioport_set_pin_level(ADE7753_GPIO, true);

	/* Initialize the console uart */
	configure_console();

	ioport_toggle_pin_level(LED2_GPIO);
	puts(STRING_HEADER);
	ioport_toggle_pin_level(LED2_GPIO);
	
	

	/*for(;;) {
		gets(uc_key);
		
		puts(uc_key);		
	}*/
	
	uint8_t uc_key;
	
	ioport_toggle_pin_level(LED3_GPIO);
	usart_serial_getchar(UART0, uc_key);
	ioport_toggle_pin_level(LED3_GPIO);
	usart_serial_putchar(UART0, uc_key);
	ioport_toggle_pin_level(LED3_GPIO);
	
	for (;;) {
		scanf("%c", (char *)&uc_key);
		
		if (uc_key == '1') {
			ioport_toggle_pin_level(LED1_GPIO);
		} else if (uc_key == '2') {
			ioport_toggle_pin_level(LED2_GPIO);
		} else if (uc_key == '3') {
			ioport_toggle_pin_level(LED3_GPIO);
		} else if (uc_key == '4') {
			ioport_toggle_pin_level(RELAY_1_GPIO);
		} else if (uc_key == '5') {
			ioport_toggle_pin_level(RELAY_2_GPIO);
		} else if (uc_key == '6') {
			
			
			uint8_t cmd = 0x26; // Temp
			// uint8_t cmd = 0x3F; // Die Revision
			
			ioport_set_pin_level(ADE7753_GPIO, false);
			spi_master_transfer(&cmd, sizeof(cmd));
			ioport_set_pin_level(ADE7753_GPIO, true);
			
			
			char result[32];
			sprintf(result, "%d", cmd);
			
			puts(result);
			puts("\n");
			
			
			
			// we are reading register 0x26 which is TEMP
			
		}
		
		puts("\n");
		puts("\r");
	}

	
	


	// Insert application code here, after the board has been initialized.
}
