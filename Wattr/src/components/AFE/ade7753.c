/*
 * ade7753.c
 *
 * Created: 12 Nov 2013 10:12:42 AM
 *  Author: Andrew
 */ 

#include <asf.h>
#include <components/AFE/ade7753.h>

void spi_master_initialize(void) {
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
	spi_set_bits_per_transfer(ADE7753_SPI_MODULE, SPI_CHIP_SEL, SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(ADE7753_SPI_MODULE, SPI_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(ADE7753_SPI_MODULE, SPI_CHIP_SEL, SPI_DLYBS,	SPI_DLYBCT);
	spi_enable(ADE7753_SPI_MODULE);
}

void spi_master_transfer(void *p_buf, uint32_t size) {
	uint32_t i;
	uint8_t uc_pcs;
	static uint16_t data;

	uint8_t *p_buffer;

	p_buffer = p_buf;

	for (i = 0; i < size; i++) {
		spi_write(ADE7753_SPI_MODULE, p_buffer[i], 0, 1);
		/* Wait transfer done. */
		while ((spi_read_status(ADE7753_SPI_MODULE) & SPI_SR_RDRF) == 0);
		spi_read(ADE7753_SPI_MODULE, &data, &uc_pcs);
		p_buffer[i] = data;
	}
}


/**
 * \brief Read the value of a register as well as the checksum
 *
 * \param ic_register The register we want to read
 * \param data A pointer to where we want to store the data we read
 * \param length The length (in bytes) of the register we are reading
 * \param checksum A pointer to where we want to store the checksum of the data we read
 */
void ade7753_read(uint8_t ic_register, uint32_t *data, uint8_t length, uint8_t *checksum) {
	// Tell the ADE7753 that we want to read the register
	spi_master_transfer(&ic_register, sizeof(ic_register));

	// Pointer magic to store the variable-length result from the register
	uint8_t result = 0x00;
	uint32_t running_result = 0x00000000;

	int i;
	
	// For how ever many bytes, read that
	for (i = 0; i < length; i++) {
		result = 0x00;
		spi_master_transfer(&result, 1);
		
		// and add it to our result
		running_result |= (((uint32_t)result) << (8 * (length - 1 - i)));
		
	}
	
	*data = running_result;

	// Verify the result
	*checksum = 0x3E;
	spi_master_transfer(checksum, sizeof(uint8_t));
	*checksum = 0x00;
	spi_master_transfer(checksum, sizeof(uint8_t));
}

void ade7753_write(uint8_t ic_register, uint32_t *data, uint8_t length) {
	ic_register |= ADE7753_WRITE_COMMAND;
	
	spi_master_transfer(&ic_register, sizeof(ic_register));
	
	if (length == BITS8) {
		spi_master_transfer(data, BITS8);	
	} else if (length == BITS16) {
		uint8_t upper_nibble = (*data) >> 8;
		uint8_t lower_nibble = (*data) & 0xFF;
		
		spi_master_transfer(&upper_nibble, BITS8);	
		spi_master_transfer(&lower_nibble, BITS8);		
	}
}

/**
 * \brief The ADE7753 has a register CHKSUM (0x3E) which returns the number of zeros
 *	that the last read/write operation returned
 *
 * \param result The result of the read register
 * \param *checksum The value of the checksum register
 * \return True if the result has the correct number of zeros according to the checksum
 *
 */
uint8_t verify_result(uint32_t *result, uint8_t *checksum) {
	uint8_t i;
	uint8_t ones = 0;
	
	for (i = 0; i < sizeof(*result)*8; i++) {
		if (*result & (1 << i)) {
			ones++;
		}
	}
	
	return (ones == (*checksum));
}


void ade7753_calibrate_watt(void) {
	printf("ADE7753 Watt Calibration.\r\n");
	
	char input; 
	
	uint32_t cfdn_int = 336;
	ade7753_write(ADE7753_REGISTER_CFDEN, &cfdn_int, ADE7753_REGISTER_CFDEN_BYTES);
	
	printf("Apply %dV to the input and %dA to the output. PF:1 Press any key to continue.\r\n", 120, 7);
	usart_serial_getchar(UART0, &input);
	
	uint32_t linecyc_int = 2000;
	ade7753_write(ADE7753_REGISTER_LINECYC, &linecyc_int, ADE7753_REGISTER_LINECYC_BYTES);
	
	uint32_t mode_register = 0x0080;
	ade7753_write(ADE7753_REGISTER_MODE, &mode_register, ADE7753_REGISTER_MODE_BYTES);
	
	uint32_t irqen_register = 0x04;
	ade7753_write(ADE7753_REGISTER_IRQEN, &irqen_register, ADE7753_REGISTER_IRQEN_BYTES);
	
	uint32_t interrupt_status = 0x00;
	uint8_t interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);
	
	int count = 0;
	
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
	
	interrupt_status = 0x00;
	interrupt_checksum = 0x00;	
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);
	
	count = 0;
	
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
	
	uint32_t active_energy = 0;
	uint32_t apparant_energy = 0;
	uint32_t reactive_energy = 0;
	uint32_t period = 0;
	
	uint8_t active_energy_checksum = 0;
	uint8_t apparant_energy_checksum = 0;
	uint8_t period_checksum = 0;
	uint8_t reactive_energy_checksum = 0;
	
	ade7753_read(ADE7753_REGISTER_LAENERGY,  &active_energy,   ADE7753_REGISTER_LAENERGY_BYTES,  &active_energy_checksum);
	ade7753_read(ADE7753_REGISTER_LVAENERGY, &apparant_energy, ADE7753_REGISTER_LVAENERGY_BYTES, &apparant_energy_checksum);
	ade7753_read(ADE7753_REGISTER_LVARENERGY, &reactive_energy, ADE7753_REGISTER_LVARENERGY_BYTES, &reactive_energy_checksum);
	ade7753_read(ADE7753_REGISTER_PERIOD,    &period,          ADE7753_REGISTER_PERIOD_BYTES,    &period_checksum);
	
	printf("Active Energy: %d (%d); Apparant Energy: %d (%d); Reactive Energy: %d (%d); Period: %d (%d)\r\n", active_energy, active_energy_checksum, apparant_energy, apparant_energy_checksum, reactive_energy, reactive_energy_checksum, period, period_checksum);
	
}

void ade7753_calibrate_watt_offset(void) {
	printf("ADE7753 Watt Offset Calibration.\r\n");
	
	char input;
	
	uint32_t cfdn_int = 336;
	ade7753_write(ADE7753_REGISTER_CFDEN, &cfdn_int, ADE7753_REGISTER_CFDEN_BYTES);
	
	printf("Apply %dV to the input and .015A to the output. PF:1 Press any key to continue.\r\n", 120);
	usart_serial_getchar(UART0, &input);	
	
	uint32_t linecyc_int = 35700;
	ade7753_write(ADE7753_REGISTER_LINECYC, &linecyc_int, ADE7753_REGISTER_LINECYC_BYTES);

	uint32_t mode_register = 0x0080;
	ade7753_write(ADE7753_REGISTER_MODE, &mode_register, ADE7753_REGISTER_MODE_BYTES);

	uint32_t irqen_register = 0x04;
	ade7753_write(ADE7753_REGISTER_IRQEN, &irqen_register, ADE7753_REGISTER_IRQEN_BYTES);

	uint32_t interrupt_status = 0x00;
	uint8_t interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);
	
	int count = 0;
	
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
	
	count = 0;
	
	interrupt_status = 0x00;
	interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);

		
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
		
	uint32_t active_energy;
	uint8_t active_energy_checksum;


	ade7753_read(ADE7753_REGISTER_LAENERGY,  &active_energy,   ADE7753_REGISTER_LAENERGY_BYTES,  &active_energy_checksum);

	printf("Active Energy: %d (%d).\r\n", active_energy, active_energy_checksum);

}

void ade7753_calibrate_phase(void) {
	printf("ADE7753 Phase Offset Calibration.\r\n");
	
	char input;
	
	printf("Apply %dV to the input and 7A to the output. PF: 0.5 Press any key to continue.\r\n", 120);
	usart_serial_getchar(UART0, &input);
	
	uint32_t linecyc_int = 2000;
	ade7753_write(ADE7753_REGISTER_LINECYC, &linecyc_int, ADE7753_REGISTER_LINECYC_BYTES);

	uint32_t mode_register = 0x0080;
	ade7753_write(ADE7753_REGISTER_MODE, &mode_register, ADE7753_REGISTER_MODE_BYTES);

	uint32_t irqen_register = 0x04;
	ade7753_write(ADE7753_REGISTER_IRQEN, &irqen_register, ADE7753_REGISTER_IRQEN_BYTES);

	uint32_t interrupt_status = 0x00;
	uint8_t interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);
	
	int count = 0;
	
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
	
	interrupt_status = 0x00;
	interrupt_checksum = 0x00;
	ade7753_read(ADE7753_REGISTER_RSTSTATUS, &interrupt_status, ADE7753_REGISTER_RSTSTATUS_BYTES, &interrupt_checksum);

	count = 0;
	
	for (;;) {
		printf("%d\r\n", count++);
		if (!ioport_get_pin_level(ADE7753_IRQ_GPIO)) {
			break;
		}
	}
	
	uint32_t active_energy;
	uint32_t period;

	uint8_t active_energy_checksum;
	uint8_t period_checksum;

	
	ade7753_read(ADE7753_REGISTER_LAENERGY,  &active_energy,   ADE7753_REGISTER_LAENERGY_BYTES,  &active_energy_checksum);
	ade7753_read(ADE7753_REGISTER_PERIOD,    &period,          ADE7753_REGISTER_PERIOD_BYTES,    &period_checksum);

	printf("Active Energy: %d (%d); Period: %d (%d).\r\n", active_energy, active_energy_checksum, period, period_checksum);
	
}
