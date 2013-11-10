/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
#define BOARD_XOSC_HZ          12000000 // 12MHz

// External oscillator type.
//!< External clock signal
#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
#define BOARD_XOSC_STARTUP_US  500000

#define BOARD_FREQ_SLCK_XTAL            (32768U)
#define BOARD_FREQ_SLCK_BYPASS          (32768U)
#define BOARD_FREQ_MAINCK_XTAL          (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS        (12000000U)

#define BOARD_MCK                       CHIP_FREQ_CPU_MAX
#define BOARD_MCK                       CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US            15625


#define BOARD_NAME "WATTR1"
#define wattr
#define sam4e
#define cortexm4

#define PINS_UART0        (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_FLAGS  (IOPORT_MODE_MUX_A)

#define PINS_UART0_PORT   IOPORT_PIOA
#define PINS_UART0_MASK   (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_PIO    PIOA
#define PINS_UART0_ID     ID_PIOA
#define PINS_UART0_TYPE   PIO_PERIPH_A
#define PINS_UART0_ATTR   PIO_DEFAULT




// OSIF LED1 is on PE0
#define LED_1_NAME		  "OSIFU LED1"
#define PIN_LED_1       {PIO_PE0, PIOE, ID_PIOE, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK  PIO_PE0
#define PIN_LED_1_PIO   PIOE
#define PIN_LED_1_ID    ID_PIOE
#define PIN_LED_1_TYPE  PIO_OUTPUT_1
#define PIN_LED_1_ATTR  PIO_DEFAULT

#define LED1_GPIO            (PIO_PE0_IDX)
#define LED1_FLAGS           (0)
#define LED1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// OSIF LED2
#define LED_2_NAME		  "OSIFU LED2"
#define PIN_LED_2       {PIO_PE1, PIOE, ID_PIOE, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_2_MASK  PIO_PE1
#define PIN_LED_2_PIO   PIOE
#define PIN_LED_2_ID    ID_PIOE
#define PIN_LED_2_TYPE  PIO_OUTPUT_1
#define PIN_LED_2_ATTR  PIO_DEFAULT

#define LED2_GPIO            (PIO_PE1_IDX)
#define LED2_FLAGS           (0)
#define LED2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// OSIF LED3
#define LED_3_NAME		  "OSIFU LED3"
#define PIN_LED_3       {PIO_PE2, PIOE, ID_PIOE, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_3_MASK  PIO_PE2
#define PIN_LED_3_PIO   PIOE
#define PIN_LED_3_ID    ID_PIOE
#define PIN_LED_3_TYPE  PIO_OUTPUT_1
#define PIN_LED_3_ATTR  PIO_DEFAULT

#define LED3_GPIO            (PIO_PE2_IDX)
#define LED3_FLAGS           (0)
#define LED3_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define LED3_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// Front Panel LED0
#define FP_LED_0_NAME		  "Front Panel LED0"
#define PIN_FP_LED_0       {PIO_PD23, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_FP_LED_0_MASK  PIO_PD23
#define PIN_FP_LED_0_PIO   PIOD
#define PIN_FP_LED_0_ID    ID_PIOD
#define PIN_FP_LED_0_TYPE  PIO_OUTPUT_1
#define PIN_FP_LED_0_ATTR  PIO_DEFAULT

#define FP_LED0_GPIO            (PIO_PD23_IDX)
#define FP_LED0_FLAGS           (0)
#define FP_LED0_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define FP_LED0_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// Front Panel LED1
#define FP_LED_1_NAME		  "Front Panel LED1"
#define PIN_FP_LED_1       {PIO_PD22, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_FP_LED_1_MASK  PIO_PD22
#define PIN_FP_LED_1_PIO   PIOD
#define PIN_FP_LED_1_ID    ID_PIOD
#define PIN_FP_LED_1_TYPE  PIO_OUTPUT_1
#define PIN_FP_LED_1_ATTR  PIO_DEFAULT

#define FP_LED1_GPIO            (PIO_PD22_IDX)
#define FP_LED1_FLAGS           (0)
#define FP_LED1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define FP_LED1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// Front Panel LED2
#define FP_LED_2_NAME		  "Front Panel LED2"
#define PIN_FP_LED_2       {PIO_PD21, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_FP_LED_2_MASK  PIO_PD21
#define PIN_FP_LED_2_PIO   PIOD
#define PIN_FP_LED_2_ID    ID_PIOD
#define PIN_FP_LED_2_TYPE  PIO_OUTPUT_1
#define PIN_FP_LED_2_ATTR  PIO_DEFAULT

#define FP_LED2_GPIO            (PIO_PD21_IDX)
#define FP_LED2_FLAGS           (0)
#define FP_LED2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define FP_LED2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

#define BOARD_NUM_OF_LED 3

// Front Panel LED3
#define FP_LED_3_NAME		  "Front Panel LED3"
#define PIN_FP_LED_3       {PIO_PD20, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_FP_LED_3_MASK  PIO_PD20
#define PIN_FP_LED_3_PIO   PIOD
#define PIN_FP_LED_3_ID    ID_PIOD
#define PIN_FP_LED_3_TYPE  PIO_OUTPUT_1
#define PIN_FP_LED_3_ATTR  PIO_DEFAULT

#define FP_LED3_GPIO            (PIO_PD20_IDX)
#define FP_LED3_FLAGS           (0)
#define FP_LED3_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define FP_LED3_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// Relay 1
#define RELAY_1_NAME		"Relay K1"
#define PIN_RELAY_1       {PIO_PC14, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_RELAY_1_MASK  PIO_PC14
#define PIN_RELAY_1_PIO   PIOC
#define PIN_RELAY_1_ID    ID_PIOC
#define PIN_RELAY_1_TYPE  PIO_OUTPUT_1
#define PIN_RELAY_1_ATTR  PIO_DEFAULT

#define RELAY_1_GPIO            (PIO_PC14_IDX)
#define RELAY_1_FLAGS           (0)
#define RELAY_1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define RELAY_1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

// Relay 2
#define RELAY_2_NAME		"Relay K2"
#define PIN_RELAY_2       {PIO_PC16, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_RELAY_2_MASK  PIO_PC16
#define PIN_RELAY_2_PIO   PIOC
#define PIN_RELAY_2_ID    ID_PIOC
#define PIN_RELAY_2_TYPE  PIO_OUTPUT_1
#define PIN_RELAY_2_ATTR  PIO_DEFAULT

#define RELAY_2_GPIO            (PIO_PC16_IDX)
#define RELAY_2_FLAGS           (0)
#define RELAY_2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define RELAY_2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** HSMCI pins definition. */
/*! Number of slot connected on HSMCI interface */
#define SD_MMC_HSMCI_MEM_CNT      1
#define SD_MMC_HSMCI_SLOT_0_SIZE  4
#define PINS_HSMCI   {0x3fUL << 26, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_PULLUP}
/** HSMCI MCCDA pin definition. */
#define PIN_HSMCI_MCCDA_GPIO            (PIO_PA28_IDX)
#define PIN_HSMCI_MCCDA_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCCK pin definition. */
#define PIN_HSMCI_MCCK_GPIO             (PIO_PA29_IDX)
#define PIN_HSMCI_MCCK_FLAGS            (IOPORT_MODE_MUX_C)
/** HSMCI MCDA0 pin definition. */
#define PIN_HSMCI_MCDA0_GPIO            (PIO_PA30_IDX)
#define PIN_HSMCI_MCDA0_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA1 pin definition. */
#define PIN_HSMCI_MCDA1_GPIO            (PIO_PA31_IDX)
#define PIN_HSMCI_MCDA1_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA2 pin definition. */
#define PIN_HSMCI_MCDA2_GPIO            (PIO_PA26_IDX)
#define PIN_HSMCI_MCDA2_FLAGS           (IOPORT_MODE_MUX_C)
/** HSMCI MCDA3 pin definition. */
#define PIN_HSMCI_MCDA3_GPIO            (PIO_PA27_IDX)
#define PIN_HSMCI_MCDA3_FLAGS           (IOPORT_MODE_MUX_C)

/** SD/MMC card detect pin definition. */
#define PIN_HSMCI_CD             {PIO_PA20, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}
#define SD_MMC_0_CD_GPIO         (PIO_PA20_IDX)
#define SD_MMC_0_CD_PIO_ID       ID_PIOA
#define SD_MMC_0_CD_FLAGS        (IOPORT_MODE_PULLUP)
#define SD_MMC_0_CD_DETECT_VALUE 0

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS        (IOPORT_MODE_MUX_A)


// SPI CS
#define SPI_ADE7753_CS_NAME		"ADE7753 SPI /ChipSelect"
#define PIN_ADE7753_CS       {PIO_PC4, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_ADE7753_CS_MASK  PIO_PC4
#define PIN_ADE7753_CS_PIO   PIOC
#define PIN_ADE7753_CS_ID    ID_PIOC
#define PIN_ADE7753_CS_TYPE  PIO_OUTPUT_1
#define PIN_ADE7753_CS_ATTR  PIO_DEFAULT

#define ADE7753_CS_GPIO            (PIO_PC4_IDX)
#define ADE7753_CS_FLAGS           (0)
#define ADE7753_CS_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define ADE7753_CS_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW

// ADE7753_CS RST
#define SPI_ADE7753_RST_NAME		"ADE7753 SPI /RST"
#define PIN_ADE7753_RST       {PIO_PC3, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_ADE7753_RST_MASK  PIO_PC3
#define PIN_ADE7753_RST_PIO   PIOC
#define PIN_ADE7753_RST_ID    ID_PIOC
#define PIN_ADE7753_RST_TYPE  PIO_OUTPUT_1
#define PIN_ADE7753_RST_ATTR  PIO_DEFAULT

#define ADE7753_RST_GPIO            (PIO_PC3_IDX)
#define ADE7753_RST_FLAGS           (0)
#define ADE7753_RST_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_HIGH
#define ADE7753_RST_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_LOW


#define ADE7753_SPI_MODULE          SPI




/** USB D- pin (System function) */
#define PIN_USB_DM      {PIO_PB10}
/** USB D+ pin (System function) */
#define PIN_USB_DP      {PIO_PB11}
	
/* KSZ8051MNL relate PIN definition */
#define PIN_KSZ8051MNL_RXC_IDX                PIO_PD14_IDX
#define PIN_KSZ8051MNL_RXC_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXC_IDX                PIO_PD0_IDX
#define PIN_KSZ8051MNL_TXC_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXEN_IDX                PIO_PD1_IDX
#define PIN_KSZ8051MNL_TXEN_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD3_IDX                PIO_PD16_IDX
#define PIN_KSZ8051MNL_TXD3_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD2_IDX                PIO_PD15_IDX
#define PIN_KSZ8051MNL_TXD2_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD1_IDX                PIO_PD3_IDX
#define PIN_KSZ8051MNL_TXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_TXD0_IDX                PIO_PD2_IDX
#define PIN_KSZ8051MNL_TXD0_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD3_IDX                PIO_PD12_IDX
#define PIN_KSZ8051MNL_RXD3_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD2_IDX                PIO_PD11_IDX
#define PIN_KSZ8051MNL_RXD2_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD1_IDX                PIO_PD6_IDX
#define PIN_KSZ8051MNL_RXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXD0_IDX                PIO_PD5_IDX
#define PIN_KSZ8051MNL_RXD0_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXER_IDX                PIO_PD7_IDX
#define PIN_KSZ8051MNL_RXER_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_RXDV_IDX                PIO_PD4_IDX
#define PIN_KSZ8051MNL_RXDV_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_CRS_IDX                PIO_PD10_IDX
#define PIN_KSZ8051MNL_CRS_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_COL_IDX                PIO_PD13_IDX
#define PIN_KSZ8051MNL_COL_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_MDC_IDX          PIO_PD8_IDX
#define PIN_KSZ8051MNL_MDC_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_MDIO_IDX                PIO_PD9_IDX
#define PIN_KSZ8051MNL_MDIO_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_KSZ8051MNL_INTRP_IDX                PIO_PD28_IDX

/** HSMCI pins that shall be configured to access the SD card. */
#define BOARD_SD_PINS               PINS_HSMCI
/** HSMCI Card Detect pin. */
#define BOARD_SD_PIN_CD             PIN_HSMCI_CD

#define CONSOLE_UART               UART0
#define CONSOLE_UART_ID            ID_UART0

#endif // USER_BOARD_H
