
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

    main.c: Main application
*/

#include <stddef.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "debug.h"
#include "uart.h"

void init(void) {
  board_init();
  debug_init();
  uart_init();

  sei();

}

uint8_t read_byte(uint8_t rom_ram, uint8_t p252, uint8_t p253, uint16_t addr) {
  uint8_t data;

  set_addr(addr);
  set_p252(p252);
  set_p253(p253);
  if(rom_ram)
    set_cs_ram(FALSE);
  else
    set_cs_rom(FALSE);
  set_oe(FALSE);
  set_latch(FALSE);
  set_clock(TRUE);
  _delay_us(6);
  data = read_data();
  set_p252(FALSE);
  set_p253(FALSE);
  if(rom_ram)
    set_cs_ram(TRUE);
  else
    set_cs_rom(TRUE);
  set_oe(TRUE);
  set_latch(TRUE);
  set_clock(FALSE);

  return data;
}

uint8_t cmp_blk(uint8_t rom_ram, uint32_t sz) {
  uint32_t i;

  for(i = 0; i < sz; i++) {
    if(read_byte(rom_ram, FALSE, FALSE, i) != read_byte(rom_ram, FALSE, FALSE, i + sz))
      break;
  }
  if(i == sz)
    return TRUE;
  return FALSE;
}

uint16_t getsize(uint8_t rom_ram) {
  uint32_t size = 65536;
  uint16_t skip = 32768;

  while(TRUE) {
    if(cmp_blk(rom_ram, skip)) {
      size = skip;
      if(skip > 1)
        skip = skip/2;
    } else {
      break;
    }
  }
  return size;
}

void read_mem(uint8_t rom_ram, uint8_t p252, uint8_t p253, uint32_t sz, uint8_t fmt) {
  uint8_t data;
  uint8_t bank_lo = 0;
  uint8_t bank_hi = 1;

  if(p252 && p253) {
    bank_hi = 4;
  } else if(!p252 && p253) {
    bank_lo = 2;
  } else if(p252 && !p253) {
    bank_hi = 2;
  }

  for(uint8_t b = bank_lo; b < bank_hi; b++) {
    for(uint32_t i = 0; i < sz ; i++) {
      data = read_byte(rom_ram, b & 1, b & 2, i);
      if(fmt) {
        uart_puthex(data);
        uart_putc(' ');
      } else {
        uart_putc(data);
      }
    }
  }

}


int main(void) __attribute__((OS_main));
//int  __attribute__ ((noreturn)) main(void);
int main(void) {
  uint8_t cmd;

  uint32_t romsize = 0;
  uint32_t ramsize = 0;
  uint8_t p252_rom = FALSE;
  uint8_t p253_rom = FALSE;
  uint8_t p252_ram = FALSE;
  uint8_t p253_ram = FALSE;
  uint8_t bHEX = FALSE;

  init();

  uart_puts_P(PSTR("\r\n" TOSTRING(CONFIG_HARDWARE_NAME) " Version: " VERSION));
  uart_putcrlf();

  // perform tests

  //get ROM size
  romsize = getsize(FALSE);
  uart_puts_P(PSTR("ROM Size is $"));
  if(romsize == 65536)
    uart_putc('1');
  uart_puthex(romsize >> 8);
  uart_puthex(romsize);
  uart_putcrlf();
  ramsize = getsize(TRUE);
  uart_puts_P(PSTR("RAM Size is $"));
  if(ramsize == 65536)
    uart_putc('1');
  uart_puthex(ramsize >> 8);
  uart_puthex(ramsize);
  uart_putcrlf();

  // check to see if P252 and P253 change anything on ROM

  if(read_byte(FALSE, FALSE, FALSE, 0) != read_byte(FALSE, TRUE, FALSE, 0)) {
    p252_rom = TRUE;
    uart_puts_P(PSTR("P25.2 affects ROM contents"));
  } else {
    uart_puts_P(PSTR("P25.2 does not affect ROM contents"));
  }
  uart_putcrlf();

  if(read_byte(FALSE, FALSE, FALSE, 0) != read_byte(FALSE, FALSE, TRUE, 0)) {
    p253_rom = TRUE;
    uart_puts_P(PSTR("P25.3 affects ROM contents"));
  } else {
    uart_puts_P(PSTR("P25.3 does not affect ROM contents"));
  }
  uart_putcrlf();

  if(read_byte(TRUE, FALSE, FALSE, 0) != read_byte(TRUE, TRUE, FALSE, 0)) {
    p252_ram = TRUE;
    uart_puts_P(PSTR("P25.2 affects RAM contents"));
  } else {
    uart_puts_P(PSTR("P25.2 does not affect RAM contents"));
  }
  uart_putcrlf();

  if(read_byte(TRUE, FALSE, FALSE, 0) != read_byte(TRUE, FALSE, TRUE, 0)) {
    p253_ram = TRUE;
    uart_puts_P(PSTR("P25.3 affects RAM contents"));
  } else {
    uart_puts_P(PSTR("P25.3 does not affect RAM contents"));
  }
  uart_putcrlf();


  uart_puts_P(PSTR("Read Cartridge:"));
  uart_putcrlf();
  uart_puts_P(PSTR("h: Output Data in HEX format"));
  uart_putcrlf();
  uart_puts_P(PSTR("b: Output Data in binary format (default)"));
  uart_putcrlf();
  uart_puts_P(PSTR("o: Read ROM"));
  uart_putcrlf();
  uart_puts_P(PSTR("a: Read RAM"));
  uart_putcrlf();

  while(TRUE) {
    while(!uart_data_available());
    cmd = uart_getc();
    switch(cmd) {
      case 'h':
        uart_puts_P(PSTR("Switching to hex output"));
        uart_putcrlf();
        bHEX = TRUE;
        break;
      case 'b':
        uart_puts_P(PSTR("Switching to binary output"));
        uart_putcrlf();
        bHEX = FALSE;
        break;
      case 'o':
        read_mem(FALSE, p252_rom, p253_rom, romsize, bHEX);
        break;
      case 'a':
        read_mem(TRUE, p252_ram, p253_ram, ramsize, bHEX);
        break;
    }
  }
}
