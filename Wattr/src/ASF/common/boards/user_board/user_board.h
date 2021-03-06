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


// ZH Input from ADE7753
#define ADE7753_ZX_NAME			"ADE7753 ZX"
#define ADE7753_ZX_GPIO			(PIO_PC10_IDX)
#define ADE7753_ZX_FLAGS		(IOPORT_MODE_OPEN_DRAIN)
#define ADE7753_ZX_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_ADE7753_ZX			{PIO_PC10, PIOC, ID_PIOC, PIO_INPUT, PIO_OPENDRAIN | PIO_IT_FALL_EDGE}
#define PIN_ADE7753_ZX_MASK		PIO_PC10
#define PIN_ADE7753_ZX_PIO		PIOC
#define PIN_ADE7753_ZX_ID		ID_PIOC
#define PIN_ADE7753_ZX_TYPE		PIO_INPUT
#define PIN_ADE7753_ZX_ATTR		(PIO_OPENDRAIN | PIO_IT_FALL_EDGE)
#define PIN_ADE7753_ZX_IRQn		PIOC_IRQn

// IRQ Input from ADE7753
#define ADE7753_IRQ_NAME			"ADE7753 IRQ"
#define ADE7753_IRQ_GPIO			(PIO_PA15_IDX)
#define ADE7753_IRQ_FLAGS		(IOPORT_MODE_OPEN_DRAIN)
#define ADE7753_IRQ_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_ADE7753_IRQ			{PIO_PA15, PIOA, ID_PIOA, PIO_INPUT, PIO_OPENDRAIN | PIO_IT_FALL_EDGE}
#define PIN_ADE7753_IRQ_MASK		PIO_PA15
#define PIN_ADE7753_IRQ_PIO		PIOA
#define PIN_ADE7753_IRQ_ID		ID_PIOA
#define PIN_ADE7753_IRQ_TYPE		PIO_INPUT
#define PIN_ADE7753_IRQ_ATTR		(PIO_OPENDRAIN | PIO_IT_FALL_EDGE)
#define PIN_ADE7753_IRQ_IRQn		PIOA_IRQn

// ZH Input from ADE7753
#define UART0_RTS_NAME			"UART0 RTS"
#define UART0_RTS_GPIO			(PIO_PC0_IDX)
#define UART0_RTS_FLAGS			(IOPORT_MODE_OPEN_DRAIN)
#define UART0_RTS_SENSE			(IOPORT_SENSE_RISING)

#define PIN_UART0_RTS			{PIO_PC0, PIOC, ID_PIOC, PIO_INPUT, PIO_OPENDRAIN | PIO_IT_RISE_EDGE}
#define PIN_UART0_RTS_MASK		PIO_PC0
#define PIN_UART0_RTS_PIO		PIOC
#define PIN_UART0_RTS_ID		ID_PIOC
#define PIN_UART0_RTS_TYPE		PIO_INPUT
#define PIN_UART0_RTS_ATTR		(PIO_OPENDRAIN | PIO_IT_RISE_EDGE)
#define PIN_UART0_RTS_IRQn		PIOC_IRQn

// Load Disconnect Button
#define FP_BUTTON_LOAD_NAME			"Front Panel Load"
#define FP_BUTTON_LOAD_GPIO			(PIO_PD28_IDX)
#define FP_BUTTON_LOAD_FLAGS		(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_LOAD_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_LOAD			{PIO_PD28, PIOD, ID_PIOD, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_LOAD_MASK		PIO_PD28
#define PIN_FP_BUTTON_LOAD_PIO		PIOD
#define PIN_FP_BUTTON_LOAD_ID		ID_PIOD
#define PIN_FP_BUTTON_LOAD_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_LOAD_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_LOAD_IRQn		PIOD_IRQn


// FP Back button
#define FP_BUTTON_BACK_NAME			"Front Panel Back"
#define FP_BUTTON_BACK_GPIO			(PIO_PC13_IDX)
#define FP_BUTTON_BACK_FLAGS		(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_BACK_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_BACK			{PIO_PC13, PIOC, ID_PIOC, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_BACK_MASK		PIO_PC13
#define PIN_FP_BUTTON_BACK_PIO		PIOC
#define PIN_FP_BUTTON_BACK_ID		ID_PIOC
#define PIN_FP_BUTTON_BACK_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_BACK_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_BACK_IRQn		PIOC_IRQn





// Front Panel Up
#define FP_BUTTON_UP_NAME			"Front Panel Up"
#define FP_BUTTON_UP_GPIO			(PIO_PD27_IDX)
#define FP_BUTTON_UP_FLAGS			(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_UP_SENSE			(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_UP			{PIO_PD27, PIOD, ID_PIOD, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_UP_MASK		PIO_PD27
#define PIN_FP_BUTTON_UP_PIO		PIOD
#define PIN_FP_BUTTON_UP_ID			ID_PIOD
#define PIN_FP_BUTTON_UP_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_UP_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_UP_IRQn		PIOD_IRQn

// Front Panel Down
#define FP_BUTTON_DOWN_NAME			"Front Panel Down"
#define FP_BUTTON_DOWN_GPIO			(PIO_PD25_IDX)
#define FP_BUTTON_DOWN_FLAGS		(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_DOWN_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_DOWN			{PIO_PD25, PIOD, ID_PIOD, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_DOWN_MASK		PIO_PD25
#define PIN_FP_BUTTON_DOWN_PIO		PIOD
#define PIN_FP_BUTTON_DOWN_ID		ID_PIOD
#define PIN_FP_BUTTON_DOWN_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_DOWN_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_DOWN_IRQn		PIOD_IRQn

// Front Panel Left
#define FP_BUTTON_LEFT_NAME			"Front Panel Left"
#define FP_BUTTON_LEFT_GPIO			(PIO_PD26_IDX)
#define FP_BUTTON_LEFT_FLAGS		(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_LEFT_SENSE		(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_LEFT			{PIO_PD26, PIOD, ID_PIOD, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_LEFT_MASK		PIO_PD26
#define PIN_FP_BUTTON_LEFT_PIO		PIOD
#define PIN_FP_BUTTON_LEFT_ID		ID_PIOD
#define PIN_FP_BUTTON_LEFT_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_LEFT_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_LEFT_IRQn		PIOD_IRQn

// Front Panel Right
#define FP_BUTTON_RIGHT_NAME			"Front Panel Right"
#define FP_BUTTON_RIGHT_GPIO			(PIO_PD24_IDX)
#define FP_BUTTON_RIGHT_FLAGS			(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_RIGHT_SENSE			(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_RIGHT				{PIO_PD24, PIOD, ID_PIOD, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_RIGHT_MASK		PIO_PD24
#define PIN_FP_BUTTON_RIGHT_PIO			PIOD
#define PIN_FP_BUTTON_RIGHT_ID			ID_PIOD
#define PIN_FP_BUTTON_RIGHT_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_RIGHT_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_RIGHT_IRQn		PIOD_IRQn

// FP Select button
#define FP_BUTTON_SELECT_NAME			"Front Panel Encoder Select"
#define FP_BUTTON_SELECT_GPIO			(PIO_PC29_IDX)
#define FP_BUTTON_SELECT_FLAGS			(IOPORT_MODE_DEBOUNCE, IOPORT_MODE_PULLUP)
#define FP_BUTTON_SELECT_SENSE			(IOPORT_SENSE_FALLING)

#define PIN_FP_BUTTON_SELECT			{PIO_PC29, PIOC, ID_PIOC, PIO_INPUT, PIO_DEBOUNCE | PIO_PULLUP | PIO_IT_FALL_EDGE}
#define PIN_FP_BUTTON_SELECT_MASK		PIO_PC29
#define PIN_FP_BUTTON_SELECT_PIO		PIOC
#define PIN_FP_BUTTON_SELECT_ID			ID_PIOC
#define PIN_FP_BUTTON_SELECT_TYPE		PIO_INPUT
#define PIN_FP_BUTTON_SELECT_ATTR		(PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)
#define PIN_FP_BUTTON_SELECT_IRQn		PIOC_IRQn

// FP Encoder Q1 button
#define FP_ENCODER_Q1_NAME			"Front Panel Encoder Q1"
#define FP_ENCODER_Q1_GPIO			(PIO_PC12_IDX)
#define FP_ENCODER_Q1_FLAGS			(IOPORT_MODE_PULLUP)
#define FP_ENCODER_Q1_SENSE			(IOPORT_SENSE_RISING)

#define PIN_FP_ENCODER_Q1			{PIO_PC12, PIOC, ID_PIOC, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE}
#define PIN_FP_ENCODER_Q1_MASK		PIO_PC12
#define PIN_FP_ENCODER_Q1_PIO		PIOC
#define PIN_FP_ENCODER_Q1_ID		ID_PIOC
#define PIN_FP_ENCODER_Q1_TYPE		PIO_INPUT
#define PIN_FP_ENCODER_Q1_ATTR		(PIO_PULLUP | PIO_IT_RISE_EDGE)
#define PIN_FP_ENCODER_Q1_IRQn		PIOC_IRQn

// FP Encoder Q2 button
#define FP_ENCODER_Q2_NAME			"Front Panel Encoder Q2"
#define FP_ENCODER_Q2_GPIO			(PIO_PC15_IDX)
#define FP_ENCODER_Q2_FLAGS			(IOPORT_MODE_PULLUP)
#define FP_ENCODER_Q2_SENSE			(IOPORT_SENSE_RISING)

#define PIN_FP_ENCODER_Q2			{PIO_PC15, PIOC, ID_PIOC, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE}
#define PIN_FP_ENCODER_Q2_MASK		PIO_PC15
#define PIN_FP_ENCODER_Q2_PIO		PIOC
#define PIN_FP_ENCODER_Q2_ID		ID_PIOC
#define PIN_FP_ENCODER_Q2_TYPE		PIO_INPUT
#define PIN_FP_ENCODER_Q2_ATTR		(PIO_PULLUP | PIO_IT_RISE_EDGE)
#define PIN_FP_ENCODER_Q2_IRQn		PIOC_IRQn











/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS        (IOPORT_MODE_MUX_A)

#define SPI_NPCS1_PC4_GPIO    (PIO_PC4_IDX)
#define SPI_NPCS1_PC4_FLAGS   (IOPORT_MODE_MUX_B)

// ADE7753 RST
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

#define VFD_NRST PIO_PC28_IDX
#define VFD_SCK  PIO_PB13_IDX
#define VFD_MOSI PIO_PB1_IDX

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
#define PIN_KSZ8051MNL_INTRP_IDX                PIO_PD18_IDX



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







#define CONSOLE_UART               UART0
#define CONSOLE_UART_ID            ID_UART0




#endif // USER_BOARD_H
