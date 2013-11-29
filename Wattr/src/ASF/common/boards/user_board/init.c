/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>


/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

/**
 * \brief Set input mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 * \param sense Sense for interrupt detection (\ref ioport_sense)
 */
#define ioport_set_pin_input_mode(pin, mode, sense) \
	do {\
		ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);\
		ioport_set_pin_mode(pin, mode);\
		ioport_set_pin_sense_mode(pin, sense);\
	} while (0)


void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	ioport_init();
	
	fpu_enable();
	

	ioport_set_pin_dir(LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED1_GPIO, LED1_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED2_GPIO, LED2_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED3_GPIO, LED3_INACTIVE_LEVEL);

	ioport_set_pin_dir(FP_LED0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(FP_LED0_GPIO, FP_LED0_INACTIVE_LEVEL);
	ioport_set_pin_dir(FP_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(FP_LED1_GPIO, FP_LED1_INACTIVE_LEVEL);
	ioport_set_pin_dir(FP_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(FP_LED2_GPIO, FP_LED2_INACTIVE_LEVEL);
	ioport_set_pin_dir(FP_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(FP_LED3_GPIO, FP_LED3_INACTIVE_LEVEL);
	
	ioport_set_pin_dir(RELAY_1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RELAY_1_GPIO, RELAY_1_INACTIVE_LEVEL);
	ioport_set_pin_dir(RELAY_2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RELAY_2_GPIO, RELAY_2_INACTIVE_LEVEL);
		
	ioport_set_pin_dir(ADE7753_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(ADE7753_RST_GPIO, ADE7753_RST_ACTIVE_LEVEL);


	ioport_set_pin_dir(VFD_NRST, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(VFD_NRST, true);

	ioport_set_pin_dir(VFD_MOSI, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(VFD_MOSI, false);
	

	ioport_set_pin_dir(VFD_SCK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(VFD_SCK, false);


	ioport_set_pin_input_mode(ADE7753_ZX_GPIO, ADE7753_ZX_FLAGS, ADE7753_ZX_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_LOAD_GPIO, FP_BUTTON_LOAD_FLAGS, FP_BUTTON_LOAD_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_BACK_GPIO, FP_BUTTON_BACK_FLAGS, FP_BUTTON_BACK_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_SELECT_GPIO, FP_BUTTON_SELECT_FLAGS, FP_BUTTON_SELECT_SENSE);
	
	ioport_set_pin_input_mode(FP_BUTTON_UP_GPIO, FP_BUTTON_UP_FLAGS, FP_BUTTON_UP_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_DOWN_GPIO, FP_BUTTON_DOWN_FLAGS, FP_BUTTON_DOWN_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_LEFT_GPIO, FP_BUTTON_LEFT_FLAGS, FP_BUTTON_LEFT_SENSE);
	ioport_set_pin_input_mode(FP_BUTTON_RIGHT_GPIO, FP_BUTTON_RIGHT_FLAGS, FP_BUTTON_RIGHT_SENSE);
	
	ioport_set_pin_input_mode(FP_ENCODER_Q1_GPIO, FP_ENCODER_Q1_FLAGS, FP_ENCODER_Q1_SENSE);
	ioport_set_pin_input_mode(FP_ENCODER_Q2_GPIO, FP_ENCODER_Q2_FLAGS, FP_ENCODER_Q2_SENSE);
	
	
	
	ioport_set_pin_input_mode(PIN_ADE7753_IRQ_GPIO, PIN_ADE7753_IRQ_FLAGS, PIN_ADE7753_IRQ_SENSE);
	
	
	
	
	
	ioport_set_pin_input_mode(UART0_RTS_GPIO, UART0_RTS_FLAGS, UART0_RTS_SENSE);
	

	ioport_set_port_peripheral_mode(PINS_UART0_PORT, PINS_UART0, PINS_UART0_MASK);

	ioport_set_pin_peripheral_mode(SPI_MISO_GPIO, SPI_MISO_FLAGS);
	ioport_set_pin_peripheral_mode(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
	ioport_set_pin_peripheral_mode(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);
	
	ioport_set_pin_peripheral_mode(SPI_NPCS1_PC4_GPIO, SPI_NPCS1_PC4_FLAGS);
	
	
#define PHY_POWER_CTL PIO_PC25_IDX
	ioport_set_pin_dir(PHY_POWER_CTL, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PHY_POWER_CTL, false);

#define PHY_POWER_RST PIO_PC11_IDX
	ioport_set_pin_dir(PHY_POWER_RST, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PHY_POWER_RST, true);
	

}
