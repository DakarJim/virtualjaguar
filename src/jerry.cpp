//
// JERRY Core
//
// Originally by David Raingeard
// GCC/SDL port by Niels Wagenaar (Linux/WIN32) and Carwin Jones (BeOS)
// Cleanups/rewrites/fixes by James L. Hammons
//
//	------------------------------------------------------------
//	JERRY REGISTERS (Mapped by Aaron Giles)
//	------------------------------------------------------------
//	F10000-F13FFF   R/W   xxxxxxxx xxxxxxxx   Jerry
//	F10000            W   xxxxxxxx xxxxxxxx   JPIT1 - timer 1 pre-scaler
//	F10002            W   xxxxxxxx xxxxxxxx   JPIT2 - timer 1 divider
//	F10004            W   xxxxxxxx xxxxxxxx   JPIT3 - timer 2 pre-scaler
//	F10008            W   xxxxxxxx xxxxxxxx   JPIT4 - timer 2 divider
//	F10010            W   ------xx xxxxxxxx   CLK1 - processor clock divider
//	F10012            W   ------xx xxxxxxxx   CLK2 - video clock divider
//	F10014            W   -------- --xxxxxx   CLK3 - chroma clock divider
//	F10020          R/W   ---xxxxx ---xxxxx   JINTCTRL - interrupt control register
//	                  W   ---x---- --------      (J_SYNCLR - clear synchronous serial intf ints)
//	                  W   ----x--- --------      (J_ASYNCLR - clear asynchronous serial intf ints)
//	                  W   -----x-- --------      (J_TIM2CLR - clear timer 2 [tempo] interrupts)
//	                  W   ------x- --------      (J_TIM1CLR - clear timer 1 [sample] interrupts)
//	                  W   -------x --------      (J_EXTCLR - clear external interrupts)
//	                R/W   -------- ---x----      (J_SYNENA - enable synchronous serial intf ints)
//	                R/W   -------- ----x---      (J_ASYNENA - enable asynchronous serial intf ints)
//	                R/W   -------- -----x--      (J_TIM2ENA - enable timer 2 [tempo] interrupts)
//	                R/W   -------- ------x-      (J_TIM1ENA - enable timer 1 [sample] interrupts)
//	                R/W   -------- -------x      (J_EXTENA - enable external interrupts)
//	F10030          R/W   -------- xxxxxxxx   ASIDATA - asynchronous serial data
//	F10032            W   -x------ -xxxxxxx   ASICTRL - asynchronous serial control
//	                  W   -x------ --------      (TXBRK - transmit break)
//	                  W   -------- -x------      (CLRERR - clear error)
//	                  W   -------- --x-----      (RINTEN - enable receiver interrupts)
//	                  W   -------- ---x----      (TINTEN - enable transmitter interrupts)
//	                  W   -------- ----x---      (RXIPOL - receiver input polarity)
//	                  W   -------- -----x--      (TXOPOL - transmitter output polarity)
//	                  W   -------- ------x-      (PAREN - parity enable)
//	                  W   -------- -------x      (ODD - odd parity select)
//	F10032          R     xxx-xxxx x-xxxxxx   ASISTAT - asynchronous serial status
//	                R     x------- --------      (ERROR - OR of PE,FE,OE)
//	                R     -x------ --------      (TXBRK - transmit break)
//	                R     --x----- --------      (SERIN - serial input)
//	                R     ----x--- --------      (OE - overrun error)
//	                R     -----x-- --------      (FE - framing error)
//	                R     ------x- --------      (PE - parity error)
//	                R     -------x --------      (TBE - transmit buffer empty)
//	                R     -------- x-------      (RBF - receive buffer full)
//	                R     -------- ---x----      (TINTEN - enable transmitter interrupts)
//	                R     -------- ----x---      (RXIPOL - receiver input polarity)
//	                R     -------- -----x--      (TXOPOL - transmitter output polarity)
//	                R     -------- ------x-      (PAREN - parity enable)
//	                R     -------- -------x      (ODD - odd parity)
//	F10034          R/W   xxxxxxxx xxxxxxxx   ASICLK - asynchronous serial interface clock
//	F10036          R     xxxxxxxx xxxxxxxx   JPIT1 - timer 1 pre-scaler
//	F10038          R     xxxxxxxx xxxxxxxx   JPIT2 - timer 1 divider
//	F1003A          R     xxxxxxxx xxxxxxxx   JPIT3 - timer 2 pre-scaler
//	F1003C          R     xxxxxxxx xxxxxxxx   JPIT4 - timer 2 divider
//	------------------------------------------------------------
//	F14000-F17FFF   R/W   xxxxxxxx xxxxxxxx   Joysticks and GPIO0-5
//	F14000          R     xxxxxxxx xxxxxxxx   JOYSTICK - read joystick state
//	F14000            W   x------- xxxxxxxx   JOYSTICK - latch joystick output
//	                  W   x------- --------      (enable joystick outputs)
//	                  W   -------- xxxxxxxx      (joystick output data)
//	F14002          R     xxxxxxxx xxxxxxxx   JOYBUTS - button register
//	F14800-F14FFF   R/W   xxxxxxxx xxxxxxxx   GPI00 - reserved (CD-ROM?)
//	F15000-F15FFF   R/W   xxxxxxxx xxxxxxxx   GPI01 - reserved
//	F16000-F16FFF   R/W   xxxxxxxx xxxxxxxx   GPI02 - reserved
//	F17000-F177FF   R/W   xxxxxxxx xxxxxxxx   GPI03 - reserved
//	F17800-F17BFF   R/W   xxxxxxxx xxxxxxxx   GPI04 - reserved
//	F17C00-F17FFF   R/W   xxxxxxxx xxxxxxxx   GPI05 - reserved
//	------------------------------------------------------------
//	F18000-F1FFFF   R/W   xxxxxxxx xxxxxxxx   Jerry DSP
//	F1A100          R/W   xxxxxxxx xxxxxxxx   D_FLAGS - DSP flags register
//	                R/W   x------- --------      (DMAEN - DMA enable)
//	                R/W   -x------ --------      (REGPAGE - register page)
//	                  W   --x----- --------      (D_EXT0CLR - clear external interrupt 0)
//	                  W   ---x---- --------      (D_TIM2CLR - clear timer 2 interrupt)
//	                  W   ----x--- --------      (D_TIM1CLR - clear timer 1 interrupt)
//	                  W   -----x-- --------      (D_I2SCLR - clear I2S interrupt)
//	                  W   ------x- --------      (D_CPUCLR - clear CPU interrupt)
//	                R/W   -------x --------      (D_EXT0ENA - enable external interrupt 0)
//	                R/W   -------- x-------      (D_TIM2ENA - enable timer 2 interrupt)
//	                R/W   -------- -x------      (D_TIM1ENA - enable timer 1 interrupt)
//	                R/W   -------- --x-----      (D_I2SENA - enable I2S interrupt)
//	                R/W   -------- ---x----      (D_CPUENA - enable CPU interrupt)
//	                R/W   -------- ----x---      (IMASK - interrupt mask)
//	                R/W   -------- -----x--      (NEGA_FLAG - ALU negative)
//	                R/W   -------- ------x-      (CARRY_FLAG - ALU carry)
//	                R/W   -------- -------x      (ZERO_FLAG - ALU zero)
//	F1A102          R/W   -------- ------xx   D_FLAGS - upper DSP flags
//	                R/W   -------- ------x-      (D_EXT1ENA - enable external interrupt 1)
//	                R/W   -------- -------x      (D_EXT1CLR - clear external interrupt 1)
//	F1A104            W   -------- ----xxxx   D_MTXC - matrix control register
//	                  W   -------- ----x---      (MATCOL - column/row major)
//	                  W   -------- -----xxx      (MATRIX3-15 - matrix width)
//	F1A108            W   ----xxxx xxxxxx--   D_MTXA - matrix address register
//	F1A10C            W   -------- -----x-x   D_END - data organization register
//	                  W   -------- -----x--      (BIG_INST - big endian instruction fetch)
//	                  W   -------- -------x      (BIG_IO - big endian I/O)
//	F1A110          R/W   xxxxxxxx xxxxxxxx   D_PC - DSP program counter
//	F1A114          R/W   xxxxxxxx xx-xxxxx   D_CTRL - DSP control/status register
//	                R     xxxx---- --------      (VERSION - DSP version code)
//	                R/W   ----x--- --------      (BUS_HOG - hog the bus!)
//	                R/W   -----x-- --------      (D_EXT0LAT - external interrupt 0 latch)
//	                R/W   ------x- --------      (D_TIM2LAT - timer 2 interrupt latch)
//	                R/W   -------x --------      (D_TIM1LAT - timer 1 interrupt latch)
//	                R/W   -------- x-------      (D_I2SLAT - I2S interrupt latch)
//	                R/W   -------- -x------      (D_CPULAT - CPU interrupt latch)
//	                R/W   -------- ---x----      (SINGLE_GO - single step one instruction)
//	                R/W   -------- ----x---      (SINGLE_STEP - single step mode)
//	                R/W   -------- -----x--      (FORCEINT0 - cause interrupt 0 on GPU)
//	                R/W   -------- ------x-      (CPUINT - send GPU interrupt to CPU)
//	                R/W   -------- -------x      (DSPGO - enable DSP execution)
//	F1A116          R/W   -------- -------x   D_CTRL - upper DSP control/status register
//	                R/W   -------- -------x      (D_EXT1LAT - external interrupt 1 latch)
//	F1A118-F1A11B     W   xxxxxxxx xxxxxxxx   D_MOD - modulo instruction mask
//	F1A11C-F1A11F   R     xxxxxxxx xxxxxxxx   D_REMAIN - divide unit remainder
//	F1A11C            W   -------- -------x   D_DIVCTRL - divide unit control
//	                  W   -------- -------x      (DIV_OFFSET - 1=16.16 divide, 0=32-bit divide)
//	F1A120-F1A123   R     xxxxxxxx xxxxxxxx   D_MACHI - multiply & accumulate high bits
//	F1A148            W   xxxxxxxx xxxxxxxx   R_DAC - right transmit data
//	F1A14C            W   xxxxxxxx xxxxxxxx   L_DAC - left transmit data
//	F1A150            W   -------- xxxxxxxx   SCLK - serial clock frequency
//	F1A150          R     -------- ------xx   SSTAT
//	                R     -------- ------x-      (left - no description)
//	                R     -------- -------x      (WS - word strobe status)
//	F1A154            W   -------- --xxxx-x   SMODE - serial mode
//	                  W   -------- --x-----      (EVERYWORD - interrupt on MSB of every word)
//	                  W   -------- ---x----      (FALLING - interrupt on falling edge)
//	                  W   -------- ----x---      (RISING - interrupt of rising edge)
//	                  W   -------- -----x--      (WSEN - enable word strobes)
//	                  W   -------- -------x      (INTERNAL - enables serial clock)
//	------------------------------------------------------------
//	F1B000-F1CFFF   R/W   xxxxxxxx xxxxxxxx   Local DSP RAM
//	------------------------------------------------------------
//	F1D000          R     xxxxxxxx xxxxxxxx   ROM_TRI - triangle wave
//	F1D200          R     xxxxxxxx xxxxxxxx   ROM_SINE - full sine wave
//	F1D400          R     xxxxxxxx xxxxxxxx   ROM_AMSINE - amplitude modulated sine wave
//	F1D600          R     xxxxxxxx xxxxxxxx   ROM_12W - sine wave and second order harmonic
//	F1D800          R     xxxxxxxx xxxxxxxx   ROM_CHIRP16 - chirp
//	F1DA00          R     xxxxxxxx xxxxxxxx   ROM_NTRI - traingle wave with noise
//	F1DC00          R     xxxxxxxx xxxxxxxx   ROM_DELTA - spike
//	F1DE00          R     xxxxxxxx xxxxxxxx   ROM_NOISE - white noise
//	------------------------------------------------------------

//#include <math.h>
#include "jaguar.h"
#include "wavetable.h"
#include "jerry.h"
#include "clock.h"

//Note that 44100 Hz requires samples every 22.675737 usec.
#define NEW_TIMER_SYSTEM
//#define JERRY_DEBUG

/*static*/ uint8 * jerry_ram_8;

//#define JERRY_CONFIG	0x4002						// ??? What's this ???

uint8 analog_x, analog_y;

static uint32 JERRYPIT1Prescaler;
static uint32 JERRYPIT1Divider;
static uint32 JERRYPIT2Prescaler;
static uint32 JERRYPIT2Divider;
static int32 jerry_timer_1_counter;
static int32 jerry_timer_2_counter;

static uint32 jerry_i2s_interrupt_divide = 8;
static int32 jerry_i2s_interrupt_timer = -1;
uint32 jerryI2SCycles;
uint32 jerryIntPending;

// Private function prototypes

void JERRYResetPIT1(void);
void JERRYResetPIT2(void);
void JERRYResetI2S(void);

void JERRYPIT1Callback(void);
void JERRYPIT2Callback(void);
void JERRYI2SCallback(void);

//This approach is probably wrong, since the timer is continuously counting down, though
//it might only be a problem if the # of interrupts generated is greater than 1--the M68K's
//timeslice should be running during that phase... (The DSP needs to be aware of this!)
void jerry_i2s_exec(uint32 cycles)
{
#ifndef NEW_TIMER_SYSTEM
	extern uint16 serialMode;						// From DAC.CPP
	if (serialMode & 0x01)							// INTERNAL flag (JERRY is master)
	{

	// Why is it called this? Instead of SCLK? Shouldn't this be read from DAC.CPP???
//Yes, it should. !!! FIX !!!
		jerry_i2s_interrupt_divide &= 0xFF;

		if (jerry_i2s_interrupt_timer == -1)
		{
		// We don't have to divide the RISC clock rate by this--the reason is a bit
		// convoluted. Will put explanation here later...
// What's needed here is to find the ratio of the frequency to the number of clock cycles
// in one second. For example, if the sample rate is 44100, we divide the clock rate by
// this: 26590906 / 44100 = 602 cycles.
// Which means, every 602 cycles that go by we have to generate an interrupt.
			jerryI2SCycles = 32 * (2 * (jerry_i2s_interrupt_divide + 1));
		}

		jerry_i2s_interrupt_timer -= cycles;
		if (jerry_i2s_interrupt_timer <= 0)
		{
//This is probably wrong as well (i.e., need to check enable lines)... !!! FIX !!!
			DSPSetIRQLine(DSPIRQ_SSI, ASSERT_LINE);
			jerry_i2s_interrupt_timer += jerryI2SCycles;
#ifdef JERRY_DEBUG
			if (jerry_i2s_interrupt_timer < 0)
				WriteLog("JERRY: Missed generating an interrupt (missed %u)!\n", (-jerry_i2s_interrupt_timer / jerryI2SCycles) + 1);
#endif
		}
	}
	else											// JERRY is slave to external word clock
	{
		// This is just a temporary kludge to see if the CD bus mastering works
		// I.e., this is totally faked...!
// The whole interrupt system is pretty much borked and is need of an overhaul.
// What we need is a way of handling these interrupts when they happen instead of
// scanline boundaries the way it is now.
		jerry_i2s_interrupt_timer -= cycles;
		if (jerry_i2s_interrupt_timer <= 0)
		{
//This is probably wrong as well (i.e., need to check enable lines)... !!! FIX !!! [DONE]
			if (ButchIsReadyToSend())//Not sure this is right spot to check...
			{
//	return GetWordFromButchSSI(offset, who);
				SetSSIWordsXmittedFromButch();
				DSPSetIRQLine(DSPIRQ_SSI, ASSERT_LINE);
			}
			jerry_i2s_interrupt_timer += 602;
		}
	}
#else
	RemoveCallback(JERRYI2SCallback);
	JERRYI2SCallback();
#endif
}

void JERRYExecPIT(uint32 cycles)
{
//This is wrong too: Counters are *always* spinning! !!! FIX !!! [DONE]
//	if (jerry_timer_1_counter)
		jerry_timer_1_counter -= cycles;

	if (jerry_timer_1_counter <= 0)
	{
//Also, it can generate a CPU interrupt as well... !!! FIX !!! or does it? Maybe it goes Timer->GPU->CPU?
		DSPSetIRQLine(DSPIRQ_TIMER0, ASSERT_LINE);	// This does the 'IRQ enabled' checking...
//		JERRYResetPIT1();
		jerry_timer_1_counter += (JERRYPIT1Prescaler + 1) * (JERRYPIT1Divider + 1);
	}

//This is wrong too: Counters are *always* spinning! !!! FIX !!! [DONE]
//	if (jerry_timer_2_counter)
		jerry_timer_2_counter -= cycles;

	if (jerry_timer_2_counter <= 0)
	{
//Also, it can generate a CPU interrupt as well... !!! FIX !!! or does it? Maybe it goes Timer->GPU->CPU?
		DSPSetIRQLine(DSPIRQ_TIMER1, ASSERT_LINE);	// This does the 'IRQ enabled' checking...
//		JERRYResetPIT2();
		jerry_timer_2_counter += (JERRYPIT2Prescaler + 1) * (JERRYPIT2Divider + 1);
	}
}

void JERRYResetI2S(void)
{
	//WriteLog("i2s: reseting\n");
//This is really SCLK... !!! FIX !!!
	jerry_i2s_interrupt_divide = 8;
	jerry_i2s_interrupt_timer = -1;
}

void JERRYResetPIT1(void)
{
#ifndef NEW_TIMER_SYSTEM
/*	if (!JERRYPIT1Prescaler || !JERRYPIT1Divider)
		jerry_timer_1_counter = 0;
	else//*/
//Small problem with this approach: Overflow if both are = $FFFF. !!! FIX !!!
		jerry_timer_1_counter = (JERRYPIT1Prescaler + 1) * (JERRYPIT1Divider + 1);

//	if (jerry_timer_1_counter)
//		WriteLog("jerry: reseting timer 1 to 0x%.8x (%i)\n",jerry_timer_1_counter,jerry_timer_1_counter);

#else
	RemoveCallback(JERRYPIT1Callback);
	double usecs = (float)(JERRYPIT1Prescaler + 1) * (float)(JERRYPIT1Divider + 1) * RISC_CYCLE_IN_USEC;
	SetCallbackTime(JERRYPIT1Callback, usecs);
#endif
}

void JERRYResetPIT2(void)
{
#ifndef NEW_TIMER_SYSTEM
/*	if (!JERRYPIT2Prescaler || !JERRYPIT2Divider)
	{
		jerry_timer_2_counter = 0;
		return;
	}
	else//*/
		jerry_timer_2_counter = (JERRYPIT2Prescaler + 1) * (JERRYPIT2Divider + 1);

//	if (jerry_timer_2_counter)
//		WriteLog("jerry: reseting timer 2 to 0x%.8x (%i)\n",jerry_timer_2_counter,jerry_timer_2_counter);

#else
	RemoveCallback(JERRYPIT2Callback);
	double usecs = (float)(JERRYPIT2Prescaler + 1) * (float)(JERRYPIT2Divider + 1) * RISC_CYCLE_IN_USEC;
	SetCallbackTime(JERRYPIT2Callback, usecs);
#endif
}

void JERRYPIT1Callback(void)
{
	DSPSetIRQLine(DSPIRQ_TIMER0, ASSERT_LINE);	// This does the 'IRQ enabled' checking...
	JERRYResetPIT1();
}

void JERRYPIT2Callback(void)
{
	DSPSetIRQLine(DSPIRQ_TIMER1, ASSERT_LINE);	// This does the 'IRQ enabled' checking...
	JERRYResetPIT2();
}

void JERRYI2SCallback(void)
{
	// Why is it called this? Instead of SCLK? Shouldn't this be read from DAC.CPP???
//Yes, it should. !!! FIX !!!
	jerry_i2s_interrupt_divide &= 0xFF;
	// We don't have to divide the RISC clock rate by this--the reason is a bit
	// convoluted. Will put explanation here later...
// What's needed here is to find the ratio of the frequency to the number of clock cycles
// in one second. For example, if the sample rate is 44100, we divide the clock rate by
// this: 26590906 / 44100 = 602 cycles.
// Which means, every 602 cycles that go by we have to generate an interrupt.
	jerryI2SCycles = 32 * (2 * (jerry_i2s_interrupt_divide + 1));

//This should be in this file with an extern reference in the header file so that
//DAC.CPP can see it... !!! FIX !!!
	extern uint16 serialMode;						// From DAC.CPP

	if (serialMode & 0x01)							// INTERNAL flag (JERRY is master)
	{
		DSPSetIRQLine(DSPIRQ_SSI, ASSERT_LINE);		// This does the 'IRQ enabled' checking...
		double usecs = (float)jerryI2SCycles * RISC_CYCLE_IN_USEC;
		SetCallbackTime(JERRYI2SCallback, usecs);
	}
	else											// JERRY is slave to external word clock
	{
//Note that 44100 Hz requires samples every 22.675737 usec.
//When JERRY is slave to the word clock, we need to do interrupts either at 44.1K
//sample rate or at a 88.2K sample rate (11.332... usec).
/*		// This is just a temporary kludge to see if the CD bus mastering works
		// I.e., this is totally faked...!
// The whole interrupt system is pretty much borked and is need of an overhaul.
// What we need is a way of handling these interrupts when they happen instead of
// scanline boundaries the way it is now.
		jerry_i2s_interrupt_timer -= cycles;
		if (jerry_i2s_interrupt_timer <= 0)
		{
//This is probably wrong as well (i.e., need to check enable lines)... !!! FIX !!! [DONE]
			if (ButchIsReadyToSend())//Not sure this is right spot to check...
			{
//	return GetWordFromButchSSI(offset, who);
				SetSSIWordsXmittedFromButch();
				DSPSetIRQLine(DSPIRQ_SSI, ASSERT_LINE);
			}
			jerry_i2s_interrupt_timer += 602;
		}*/
	}
}


void jerry_init(void)
{
//	clock_init();
//	anajoy_init();
	joystick_init();
	DACInit();
//This should be handled with the cart initialization...
//	eeprom_init();
	memory_malloc_secure((void **)&jerry_ram_8, 0x10000, "JERRY RAM/ROM");
	memcpy(&jerry_ram_8[0xD000], wave_table, 0x1000);

	JERRYPIT1Prescaler = 0xFFFF;
	JERRYPIT2Prescaler = 0xFFFF;
	JERRYPIT1Divider = 0xFFFF;
	JERRYPIT2Divider = 0xFFFF;
}

void jerry_reset(void)
{
//	clock_reset();
//	anajoy_reset();
	joystick_reset();
	eeprom_reset();
	JERRYResetI2S();
	DACReset();

	memset(jerry_ram_8, 0x00, 0xD000);		// Don't clear out the Wavetable ROM...!
	JERRYPIT1Prescaler = 0xFFFF;
	JERRYPIT2Prescaler = 0xFFFF;
	JERRYPIT1Divider = 0xFFFF;
	JERRYPIT2Divider = 0xFFFF;
	jerry_timer_1_counter = 0;
	jerry_timer_2_counter = 0;
}

void jerry_done(void)
{
	WriteLog("JERRY: M68K Interrupt control ($F10020) = %04X\n", GET16(jerry_ram_8, 0x20));
	memory_free(jerry_ram_8);
//	clock_done();
//	anajoy_done();
	joystick_done();
	DACDone();
	eeprom_done();
}

bool JERRYIRQEnabled(int irq)
{
	// Read the word @ $F10020 
	return jerry_ram_8[0x21] & (1 << irq);
}

void JERRYSetPendingIRQ(int irq)
{
	// This is the shadow of INT (it's a split RO/WO register)
	jerryIntPending |= (1 << irq);
}

//
// JERRY byte access (read)
//
uint8 JERRYReadByte(uint32 offset, uint32 who/*=UNKNOWN*/)
{
#ifdef JERRY_DEBUG
	WriteLog("JERRY: Reading byte at %06X\n", offset);
#endif
	if ((offset >= DSP_CONTROL_RAM_BASE) && (offset < DSP_CONTROL_RAM_BASE+0x20))
		return DSPReadByte(offset, who);
	else if ((offset >= DSP_WORK_RAM_BASE) && (offset < DSP_WORK_RAM_BASE+0x2000))
		return DSPReadByte(offset, who);
	// LRXD/RRXD/SSTAT $F1A148/4C/50 (really 16-bit registers...)
	else if (offset >= 0xF1A148 && offset <= 0xF1A153)
		return DACReadByte(offset, who);
//	F10036          R     xxxxxxxx xxxxxxxx   JPIT1 - timer 1 pre-scaler
//	F10038          R     xxxxxxxx xxxxxxxx   JPIT2 - timer 1 divider
//	F1003A          R     xxxxxxxx xxxxxxxx   JPIT3 - timer 2 pre-scaler
//	F1003C          R     xxxxxxxx xxxxxxxx   JPIT4 - timer 2 divider
//This is WRONG!
//	else if (offset >= 0xF10000 && offset <= 0xF10007)
//This is still wrong. What needs to be returned here are the values being counted down
//in the jerry_timer_n_counter variables... !!! FIX !!! [DONE]

//This is probably the problem with the new timer code... This is invalid
//under the new system... !!! FIX !!!
	else if ((offset >= 0xF10036) && (offset <= 0xF1003D))
	{
#ifndef NEW_TIMER_SYSTEM
//		jerry_timer_1_counter = (JERRYPIT1Prescaler + 1) * (JERRYPIT1Divider + 1);
		uint32 counter1Hi = (jerry_timer_1_counter / (JERRYPIT1Divider + 1)) - 1;
		uint32 counter1Lo = (jerry_timer_1_counter % (JERRYPIT1Divider + 1)) - 1;
		uint32 counter2Hi = (jerry_timer_2_counter / (JERRYPIT2Divider + 1)) - 1;
		uint32 counter2Lo = (jerry_timer_2_counter % (JERRYPIT2Divider + 1)) - 1;

		switch(offset & 0x0F)
		{
		case 6:
//			return JERRYPIT1Prescaler >> 8;
			return counter1Hi >> 8;
		case 7:
//			return JERRYPIT1Prescaler & 0xFF;
			return counter1Hi & 0xFF;
		case 8:
//			return JERRYPIT1Divider >> 8;
			return counter1Lo >> 8;
		case 9:
//			return JERRYPIT1Divider & 0xFF;
			return counter1Lo & 0xFF;
		case 10:
//			return JERRYPIT2Prescaler >> 8;
			return counter2Hi >> 8;
		case 11:
//			return JERRYPIT2Prescaler & 0xFF;
			return counter2Hi & 0xFF;
		case 12:
//			return JERRYPIT2Divider >> 8;
			return counter2Lo >> 8;
		case 13:
//			return JERRYPIT2Divider & 0xFF;
			return counter2Lo & 0xFF;
		}
#else
#endif
	}
//	else if (offset >= 0xF10010 && offset <= 0xF10015)
//		return clock_byte_read(offset);
//	else if (offset >= 0xF17C00 && offset <= 0xF17C01)
//		return anajoy_byte_read(offset);
	else if (offset >= 0xF14000 && offset <= 0xF14003)
		return joystick_byte_read(offset) | eeprom_byte_read(offset);
	else if (offset >= 0xF14000 && offset <= 0xF1A0FF)
		return eeprom_byte_read(offset);
	
	return jerry_ram_8[offset & 0xFFFF];
}

//
// JERRY word access (read)
//
uint16 JERRYReadWord(uint32 offset, uint32 who/*=UNKNOWN*/)
{
#ifdef JERRY_DEBUG
	WriteLog("JERRY: Reading word at %06X\n", offset);
#endif

	if ((offset >= DSP_CONTROL_RAM_BASE) && (offset < DSP_CONTROL_RAM_BASE+0x20))
		return DSPReadWord(offset, who);
	else if (offset >= DSP_WORK_RAM_BASE && offset <= DSP_WORK_RAM_BASE + 0x1FFF)
		return DSPReadWord(offset, who);
	// LRXD/RRXD/SSTAT $F1A148/4C/50 (really 16-bit registers...)
	else if (offset >= 0xF1A148 && offset <= 0xF1A153)
		return DACReadWord(offset, who);
//	F10036          R     xxxxxxxx xxxxxxxx   JPIT1 - timer 1 pre-scaler
//	F10038          R     xxxxxxxx xxxxxxxx   JPIT2 - timer 1 divider
//	F1003A          R     xxxxxxxx xxxxxxxx   JPIT3 - timer 2 pre-scaler
//	F1003C          R     xxxxxxxx xxxxxxxx   JPIT4 - timer 2 divider
//This is WRONG!
//	else if ((offset >= 0xF10000) && (offset <= 0xF10007))
//This is still wrong. What needs to be returned here are the values being counted down
//in the jerry_timer_n_counter variables... !!! FIX !!! [DONE]
	else if ((offset >= 0xF10036) && (offset <= 0xF1003D))
	{
//		jerry_timer_1_counter = (JERRYPIT1Prescaler + 1) * (JERRYPIT1Divider + 1);
		uint32 counter1Hi = (jerry_timer_1_counter / (JERRYPIT1Divider + 1)) - 1;
		uint32 counter1Lo = (jerry_timer_1_counter % (JERRYPIT1Divider + 1)) - 1;
		uint32 counter2Hi = (jerry_timer_2_counter / (JERRYPIT2Divider + 1)) - 1;
		uint32 counter2Lo = (jerry_timer_2_counter % (JERRYPIT2Divider + 1)) - 1;

		switch(offset & 0x0F)
		{
		case 6:
//			return JERRYPIT1Prescaler;
			return counter1Hi;
		case 8:
//			return JERRYPIT1Divider;
			return counter1Lo;
		case 10:
//			return JERRYPIT2Prescaler;
			return counter2Hi;
		case 12:
//			return JERRYPIT2Divider;
			return counter2Lo;
		}
		// Unaligned word reads???
	}
//	else if ((offset >= 0xF10010) && (offset <= 0xF10015))
//		return clock_word_read(offset);
	else if (offset == 0xF10020)
		return jerryIntPending;
//	else if ((offset >= 0xF17C00) && (offset <= 0xF17C01))
//		return anajoy_word_read(offset);
	else if (offset == 0xF14000)
		return (joystick_word_read(offset) & 0xFFFE) | eeprom_word_read(offset);
	else if ((offset >= 0xF14002) && (offset < 0xF14003))
		return joystick_word_read(offset);
	else if ((offset >= 0xF14000) && (offset <= 0xF1A0FF))
		return eeprom_word_read(offset);

/*if (offset >= 0xF1D000)
	WriteLog("JERRY: Reading word at %08X [%04X]...\n", offset, ((uint16)jerry_ram_8[(offset+0)&0xFFFF] << 8) | jerry_ram_8[(offset+1)&0xFFFF]);//*/

	offset &= 0xFFFF;				// Prevent crashing...!
	return ((uint16)jerry_ram_8[offset+0] << 8) | jerry_ram_8[offset+1];
}

//
// JERRY byte access (write)
//
void JERRYWriteByte(uint32 offset, uint8 data, uint32 who/*=UNKNOWN*/)
{
#ifdef JERRY_DEBUG
	WriteLog("jerry: writing byte %.2x at 0x%.6x\n",data,offset);
#endif
	if ((offset >= DSP_CONTROL_RAM_BASE) && (offset < DSP_CONTROL_RAM_BASE+0x20))
	{
		DSPWriteByte(offset, data, who);
		return;
	}
	else if ((offset >= DSP_WORK_RAM_BASE) && (offset < DSP_WORK_RAM_BASE+0x2000))
	{
		DSPWriteByte(offset, data, who);
		return;
	}
	// SCLK ($F1A150--8 bits wide)
//NOTE: This should be taken care of in DAC...
	else if ((offset >= 0xF1A152) && (offset <= 0xF1A153))
	{
//		WriteLog("JERRY: Writing %02X to SCLK...\n", data);
		if ((offset & 0x03) == 2)
			jerry_i2s_interrupt_divide = (jerry_i2s_interrupt_divide & 0x00FF) | ((uint32)data << 8);
		else
			jerry_i2s_interrupt_divide = (jerry_i2s_interrupt_divide & 0xFF00) | (uint32)data;

		jerry_i2s_interrupt_timer = -1;
		jerry_i2s_exec(0);
//		return;
	}
	// LTXD/RTXD/SCLK/SMODE $F1A148/4C/50/54 (really 16-bit registers...)
	else if (offset >= 0xF1A148 && offset <= 0xF1A157)
	{ 
		DACWriteByte(offset, data, who);
		return; 
	}
	else if (offset >= 0xF10000 && offset <= 0xF10007)
	{
		switch (offset & 0x07)
		{
		case 0:
			JERRYPIT1Prescaler = (JERRYPIT1Prescaler & 0x00FF) | (data << 8);
			JERRYResetPIT1();
			break;
		case 1:
			JERRYPIT1Prescaler = (JERRYPIT1Prescaler & 0xFF00) | data;
			JERRYResetPIT1();
			break;
		case 2:
			JERRYPIT1Divider = (JERRYPIT1Divider & 0x00FF) | (data << 8);
			JERRYResetPIT1();
			break;
		case 3:
			JERRYPIT1Divider = (JERRYPIT1Divider & 0xFF00) | data;
			JERRYResetPIT1();
			break;
		case 4:
			JERRYPIT2Prescaler = (JERRYPIT2Prescaler & 0x00FF) | (data << 8);
			JERRYResetPIT2();
			break;
		case 5:
			JERRYPIT2Prescaler = (JERRYPIT2Prescaler & 0xFF00) | data;
			JERRYResetPIT2();
			break;
		case 6:
			JERRYPIT2Divider = (JERRYPIT2Divider & 0x00FF) | (data << 8);
			JERRYResetPIT2();
			break;
		case 7:
			JERRYPIT2Divider = (JERRYPIT2Divider & 0xFF00) | data;
			JERRYResetPIT2();
		}
		return;
	}
/*	else if ((offset >= 0xF10010) && (offset <= 0xF10015))
	{
		clock_byte_write(offset, data);
		return;
	}//*/
	// JERRY -> 68K interrupt enables/latches (need to be handled!)
	else if (offset >= 0xF10020 && offset <= 0xF10023)
	{
WriteLog("JERRY: (68K int en/lat - Unhandled!) Tried to write $%02X to $%08X!\n", data, offset);
	}
/*	else if ((offset >= 0xF17C00) && (offset <= 0xF17C01))
	{
		anajoy_byte_write(offset, data);
		return;
	}*/
	else if ((offset >= 0xF14000) && (offset <= 0xF14003))
	{
		joystick_byte_write(offset, data);
		eeprom_byte_write(offset, data);
		return;
	}
	else if ((offset >= 0xF14000) && (offset <= 0xF1A0FF))
	{
		eeprom_byte_write(offset, data);
		return;
	}

//Need to protect write attempts to Wavetable ROM (F1D000-FFF)
	if (offset >= 0xF1D000 && offset <= 0xF1DFFF)
		return;

	jerry_ram_8[offset & 0xFFFF] = data;
}

//
// JERRY word access (write)
//
void JERRYWriteWord(uint32 offset, uint16 data, uint32 who/*=UNKNOWN*/)
{
#ifdef JERRY_DEBUG
	WriteLog( "JERRY: Writing word %04X at %06X\n", data, offset);
#endif

	if ((offset >= DSP_CONTROL_RAM_BASE) && (offset < DSP_CONTROL_RAM_BASE+0x20))
	{
		DSPWriteWord(offset, data, who);
		return;
	}
	else if ((offset >= DSP_WORK_RAM_BASE) && (offset < DSP_WORK_RAM_BASE+0x2000))
	{
		DSPWriteWord(offset, data, who);
		return;
	}
//NOTE: This should be taken care of in DAC...
	else if (offset == 0xF1A152)					// Bottom half of SCLK ($F1A150)
	{
		WriteLog("JERRY: Writing %04X to SCLK (by %s)...\n", data, whoName[who]);
//This should *only* be enabled when SMODE has its INTERNAL bit set! !!! FIX !!!
		jerry_i2s_interrupt_divide = (uint8)data;
		jerry_i2s_interrupt_timer = -1;
		jerry_i2s_exec(0);

		DACWriteWord(offset, data, who);
		return; 
	}
	// LTXD/RTXD/SCLK/SMODE $F1A148/4C/50/54 (really 16-bit registers...)
	else if (offset >= 0xF1A148 && offset <= 0xF1A156)
	{ 
		DACWriteWord(offset, data, who);
		return; 
	}
	else if (offset >= 0xF10000 && offset <= 0xF10007)
	{
		switch(offset & 0x07)
		{
		case 0:
			JERRYPIT1Prescaler = data;
			JERRYResetPIT1();
			break;
		case 2:
			JERRYPIT1Divider = data;
			JERRYResetPIT1();
			break;
		case 4:
			JERRYPIT2Prescaler = data;
			JERRYResetPIT2();
			break;
		case 6:
			JERRYPIT2Divider = data;
			JERRYResetPIT2();
		}
		// Need to handle (unaligned) cases???
		return;
	}
/*	else if (offset >= 0xF10010 && offset < 0xF10016)
	{
		clock_word_write(offset, data);
		return;
	}//*/
	// JERRY -> 68K interrupt enables/latches (need to be handled!)
	else if (offset >= 0xF10020 && offset <= 0xF10022)
	{
WriteLog("JERRY: (68K int en/lat - Unhandled!) Tried to write $%04X to $%08X!\n", data, offset);
	}
/*	else if (offset >= 0xF17C00 && offset < 0xF17C02)
	{
//I think this was removed from the Jaguar. If so, then we don't need this...!
		anajoy_word_write(offset, data);
		return;
	}*/
	else if (offset >= 0xF14000 && offset < 0xF14003)
	{
		joystick_word_write(offset, data);
		eeprom_word_write(offset, data);
		return;
	}
	else if (offset >= 0xF14000 && offset <= 0xF1A0FF)
	{
		eeprom_word_write(offset, data);
		return;
	}

//Need to protect write attempts to Wavetable ROM (F1D000-FFF)
	if (offset >= 0xF1D000 && offset <= 0xF1DFFF)
		return;

	jerry_ram_8[(offset+0) & 0xFFFF] = (data >> 8) & 0xFF;
	jerry_ram_8[(offset+1) & 0xFFFF] = data & 0xFF;
}
