// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   IRQ_DISPATCHER.C    	   |                                                                    |
|-------------------------------                                                                    |
| These functions are part of the IRQs attending routines, they dispatch the exceptions to	    	|
| userspace callbacks, where further actions may be taken.											|
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <keyboard_driver.h>
#include <video_lib.h>
#include <process_manager.h>

static void int_20();

static void int_21();


void irqDispatcher(uint64_t irq) {

	switch (irq) {

		case 0:			// Timer tick
			int_20();
			break;

		case 1:			// Keyboard
			int_21();
			break;

	}

	return;

}


// TIMER TICK
void int_20() {

	 cursorTickListener();

}


// Keyboard
void int_21() {

	handleKeyStroke();

}