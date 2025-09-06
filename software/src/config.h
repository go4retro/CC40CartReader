/*
    CC40CartReader - Texas Instruments cc40 Cartridge Reader
    Copyright Jim Brain and RETRO Innovations, 2025

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License only.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    config.h: User-configurable options to simplify hardware changes and/or
              reduce the code/ram requirements of the code.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "version.h"

#include <util/delay.h>

/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef VER_PATCH
#ifdef VER_FIX
  #define VER_TEXT           TOSTRING(VER_MAJOR) "." TOSTRING(VER_MINOR) "." TOSTRING(VER_PATCH) "." TOSTRING(VER_FIX)
#else
  #define VER_TEXT           TOSTRING(VER_MAJOR) "." TOSTRING(VER_MINOR) "." TOSTRING(VER_PATCH)
#endif
#else
  #define VER_TEXT           TOSTRING(VER_MAJOR) "." TOSTRING(VER_MINOR)
#endif
#ifdef VER_PRERELEASE
  #define VERSION "" VER_TEXT TOSTRING(VER_PRERELEASE) ""
#else
  #define VERSION "" VER_TEXT ""
#endif

#define FLASH_MEM_DATA  1

#include <avr/io.h>

#include "autoconf.h"

/* ----- Common definitions for all AVR hardware variants ------ */

#define UART_DOUBLE_SPEED


#if CONFIG_HARDWARE_VARIANT == 1
/* ---------- Hardware configuration: CC40CartReader v1 ---------- */

#define DATA_DDR      DDRA
#define DATA_OUT      PORTA
#define DATA_IN       PINA

#define ADDR_HI_DDR   DDRC
#define ADDR_HI_OUT   PORTC

#define ADDR_LO_DDR   DDRL
#define ADDR_LO_OUT   PORTL

#define CS_ROM_DDR    DDRB
#define CS_ROM_OUT    PORTB
#define CS_ROM_PIN    (1<<PIN0)

#define CS_RAM_DDR    DDRD
#define CS_RAM_OUT    PORTD
#define CS_RAM_PIN    (1<<PIN0)

#define WE_DDR        DDRD
#define WE_OUT        PORTD
#define WE_PIN        (1<<PIN1)

#define OE_DDR        DDRG
#define OE_OUT        PORTG
#define OE_PIN        (1<<PIN2)

#define P252_DDR      DDRD
#define P252_OUT      PORTD
#define P252_PIN      (1<<PIN7)

#define P253_DDR      DDRK
#define P253_OUT      PORTK
#define P253_PIN      (1<<PIN7)

#define LATCH_DDR     DDRK
#define LATCH_OUT     PORTK
#define LATCH_PIN     (1<<PIN6)

#define RST_DDR       DDRG
#define RST_OUT       PORTG
#define RST_PIN       (1<<PIN1)

#define RDWR_DDR      DDRB
#define RDWR_OUT      PORTB
#define RDWR_PIN      (1<<PIN2)

#define CLK_DDR      DDRB
#define CLK_OUT      PORTB
#define CLK_PIN      (1<<PIN3)

#define EN_DDR       DDRB
#define EN_OUT       PORTB
#define EN_PIN       (1<<PIN1)

static inline void set_addr(uint16_t addr) {
  ADDR_HI_OUT = addr >> 8;
  ADDR_LO_OUT = addr & 0xff;
}

static inline void set_oe(uint8_t i) {
  if(i)
    OE_OUT |= OE_PIN;
  else
    OE_OUT &= ~OE_PIN;
}

static inline void set_we(uint8_t i) {
  if(i)
    WE_OUT |= WE_PIN;
  else
    WE_OUT &= ~WE_PIN;
}

static inline void set_rw(uint8_t i) {
  if(i)
    RDWR_OUT |= RDWR_PIN;
  else
    RDWR_OUT &= ~RDWR_PIN;
}

static inline void set_enable(uint8_t i) {
  if(i)
    EN_OUT |= EN_PIN;
  else
    EN_OUT &= ~EN_PIN;
}

static inline void set_latch(uint8_t i) {
  if(i)
    LATCH_OUT |= LATCH_PIN;
  else
    LATCH_OUT &= ~LATCH_PIN;
}

static inline void set_clock(uint8_t i) {
  if(i)
    CLK_OUT |= CLK_PIN;
  else
    CLK_OUT &= ~CLK_PIN;
}

static inline void set_cs_rom(uint8_t i) {
  if(i)
    CS_ROM_OUT |= CS_ROM_PIN;
  else
    CS_ROM_OUT &= ~CS_ROM_PIN;
}

static inline void set_cs_ram(uint8_t i) {
  if(i)
    CS_RAM_OUT |= CS_RAM_PIN;
  else
    CS_RAM_OUT &= ~CS_RAM_PIN;
}

static inline void set_reset(uint8_t i) {
  if(i)
    RST_OUT |= RST_PIN;
  else
    RST_OUT &= ~RST_PIN;
}

static inline void set_p252(uint8_t i) {
  if(i)
    P252_OUT |= P252_PIN;
  else
    P252_OUT &= ~P252_PIN;
}

static inline void set_p253(uint8_t i) {
  if(i)
    P253_OUT |= P253_PIN;
  else
    P253_OUT &= ~P253_PIN;
}

static inline uint8_t read_data(void) {
  return DATA_IN;
}

static inline void board_init(void) {
  RST_DDR |= RST_PIN;
  set_reset(FALSE);

  ADDR_HI_DDR = 0xff;
  ADDR_LO_DDR = 0xff;

  RDWR_OUT |= RDWR_PIN;
  RDWR_DDR |= RDWR_PIN;

  CS_ROM_OUT |= CS_ROM_PIN;
  CS_ROM_DDR |= CS_ROM_PIN;

  CS_RAM_OUT |= CS_RAM_PIN;
  CS_RAM_DDR |= CS_RAM_PIN;

  OE_OUT |= OE_PIN;
  OE_DDR |= OE_PIN;

  WE_OUT |= WE_PIN;
  WE_DDR |= WE_PIN;

  CLK_DDR |= CLK_PIN;

  EN_OUT |= EN_PIN;
  EN_DDR |= EN_PIN;

  LATCH_DDR |= LATCH_PIN;

  P252_DDR |= P252_PIN;
  P253_DDR |= P253_PIN;

  _delay_us(100);
  set_reset(TRUE);
}

#else
#  error "CONFIG_HARDWARE_VARIANT is unset or set to an unknown value."
#endif

/* ---------------- End of user-configurable options ---------------- */

#ifdef CONFIG_UART_DEBUG
#  define UART0_ENABLE
#  define DYNAMIC_UART
#  define UART0_BAUDRATE CONFIG_UART_DEBUG_RATE
#elif defined CONFIG_UART_BAUDRATE
#  define UART0_ENABLE
#  define UART0_BAUDRATE CONFIG_UART_BAUDRATE
#else
#  define UART0_ENABLE
#  define UART0_BAUDRATE 57600
#endif

#ifdef CONFIG_UART_BUF_SHIFT
 #define UART0_TX_BUFFER_SHIFT CONFIG_UART_BUF_SHIFT
 #define UART0_RX_BUFFER_SHIFT CONFIG_UART_BUF_SHIFT
#endif

#ifdef FLASH_MEM_DATA
#define MEM_CLASS PROGMEM
#define mem_read_byte(x) pgm_read_byte(&(x))
#define mem_read_word(x) pgm_read_word(&(x))
#else
#define MEM_CLASS
#define mem_read_byte(x) (x)
#define mem_read_word(x) (x)
#endif

#endif /*CONFIG_H*/
