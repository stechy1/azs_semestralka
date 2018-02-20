/*
 * gmouse_lld_STMPE811_board.h
 *
 *  Created on: 21. 12. 2017
 *      Author: Petr
 */

#ifndef GMOUSE_LLD_STMPE811_BOARD_H_
#define GMOUSE_LLD_STMPE811_BOARD_H_

#include "../Drivers/BSP/Components/stmpe811/stmpe811.h"
#include "../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.h"
#include "../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_ts.h"

// Resolution and Accuracy Settings
#define GMOUSE_STMPE811_PEN_CALIBRATE_ERROR		8
#define GMOUSE_STMPE811_PEN_CLICK_ERROR			6
#define GMOUSE_STMPE811_PEN_MOVE_ERROR			4
#define GMOUSE_STMPE811_FINGER_CALIBRATE_ERROR	14
#define GMOUSE_STMPE811_FINGER_CLICK_ERROR		18
#define GMOUSE_STMPE811_FINGER_MOVE_ERROR		14

// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_STMPE811_BOARD_DATA_SIZE			0

// Options - Leave these commented to make it user configurable in the gfxconf.h
//#define GMOUSE_STMPE811_READ_PRESSURE		FALSE
//#define GMOUSE_STMPE811_SELF_CALIBRATE	FALSE
//#define GMOUSE_STMPE811_TEST_MODE			FALSE

// If TRUE this board has the STMPE811 IRQ pin connected to a GPIO.
// Note: For tested hardware this is unreliable and should be set to FALSE until tested.
//			Symptoms are that mouse readings just appear to stop for a bit. Lifting the touch
//			and re-applying the touch cause readings to start again.
#define GMOUSE_STMPE811_GPIO_IRQPIN				FALSE

// If TRUE this is a really slow CPU and we should always clear the FIFO between reads.
#define GMOUSE_STMPE811_SLOW_CPU				FALSE

// Maximum timeout
#define STMPE811_TIMEOUT			0x3000

static bool_t init_board(GMouse* m, unsigned driverinstance) {
	BSP_TS_Init(240,320);
	return TRUE;
}

#if GMOUSE_STMPE811_GPIO_IRQPIN
	static bool_t getpin_irq(GMouse* m) {

	}
#endif

static GFXINLINE void aquire_bus(GMouse* m) {
}

static GFXINLINE void release_bus(GMouse* m) {
}

static void write_reg(GMouse* m, uint8_t reg, uint8_t val) {
	IOE_Write((uint8_t)TS_I2C_ADDRESS, reg, val);
}

static uint8_t read_byte(GMouse* m, uint8_t reg) {
	return IOE_Read((uint8_t)TS_I2C_ADDRESS, reg);
}

static uint16_t read_word(GMouse* m, uint8_t reg) {
	uint8_t  data[2];
	uint16_t result;

	IOE_ReadMultiple((uint8_t)TS_I2C_ADDRESS, reg, data, sizeof(data)) ;

	/* Calculate positions values */
	result = (data[0] << 8)|(data[1] << 0);

	return result;
}


#endif /* GMOUSE_LLD_STMPE811_BOARD_H_ */
