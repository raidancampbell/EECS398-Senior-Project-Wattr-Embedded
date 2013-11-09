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
//#define BOARD_XOSC_STARTUP_US  500000

#define BOARD_FREQ_SLCK_XTAL            (32768U)
#define BOARD_FREQ_SLCK_BYPASS          (32768U)
#define BOARD_FREQ_MAINCK_XTAL          (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS        (12000000U)

#define BOARD_MCK                       CHIP_FREQ_CPU_MAX



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

// OSIF LED2 is on PE0
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

// OSIF LED3 is on PE0
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




#endif // USER_BOARD_H
