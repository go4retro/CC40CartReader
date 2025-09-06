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

    debug.h: Quick defines to allow optional debug output to UART
*/

#ifndef DEBUG_H
#define DEBUG_H

#define debug_puts_P(x) _debug_puts_P(PSTR(x))

#if defined CONFIG_UART_DEBUG || defined CONFIG_UART_DEBUG_SW || defined ARDUINO_UART_DEBUG
void debug_putc(uint8_t data);
void debug_puts(const char *text);
void _debug_puts_P(const char *text);
void debug_puthex(uint8_t hex);
void debug_putdec(uint8_t dec);
void debug_putcrlf(void);
void debug_trace(void *ptr, uint16_t start, uint16_t len);
void debug_init(void);
#else
#define debug_init()            do {} while(0)
#define debug_putc(x)           do {} while(0)
#define debug_puthex(x)         do {} while(0)
#define debug_putdec(x)         do {} while(0)
#define debug_trace(x,y,z)      do {} while(0)
#define debug_puts(x)           do {} while(0)
#define _debug_puts_P(x)        do {} while(0)
#define debug_putcrlf()         do {} while(0)
#endif
#endif
