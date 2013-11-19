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
void ade7753_read(uint8_t ic_register, void *data, uint8_t length, uint8_t *checksum) {
	// Tell the ADE7753 that we want to read the register
	spi_master_transfer(&ic_register, sizeof(ic_register));

	// Pointer magic to store the variable-length result from the register
	uint8_t result = 0x00;
	uint32_t running_result = 0x00000000;
	
	uint32_t *p_data;
	p_data = data;
	int i;
	
	// For how ever many bytes, read that
	for (i = 0; i < length; i++) {
		result = 0x00;
		spi_master_transfer(&result, 1);
		
		// and add it to our result
		running_result = (running_result << (8 * (length - i))) | result;
		
	}
	
	*p_data = running_result;

	// Verify the result
	*checksum = 0x3E;
	spi_master_transfer(checksum, sizeof(uint8_t));
	*checksum = 0x00;
	spi_master_transfer(checksum, sizeof(uint8_t));
}

/*

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
*/

void ade7753_write(uint8_t ic_register, void *data, uint8_t length) {
	// I think this works
	ic_register |= ADE7753_WRITE_MASK;
	
	spi_master_transfer(&ic_register, sizeof(ic_register));
	//ade7753_read(*data, data, length, &ic_register);
	
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