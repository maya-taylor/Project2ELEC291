/*
* ----------------------------------------------------------------------------
* THE COFFEEWARE LICENSE� (Revision 1
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform specific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <EFM8LB1.h>
#include <stdint.h>

// Pins used by the SPI interface:
// 	P0.0: SCK
// 	P0.1: MISO
// 	P0.2: MOSI
// 	P0.3: SS*

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
    // P1.0-P1.5 => MISO/IRQ/SCK/MOSI/CE/CSN
    // P1M0=0x3C;
    P0MDOUT|=0b_0000_0101;
    P1MDOUT|=0b_0011_1100;
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state) P1_4=1; else P1_4=0;
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state) P1_5=1; else P1_5=0;
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state) P1_2=1; else P1_2=0;
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state) P1_3=1; else P1_3=0;
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return P1_0;
}
/* ------------------------------------------------------------------------- */
