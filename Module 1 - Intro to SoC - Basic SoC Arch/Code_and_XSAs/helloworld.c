#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xil_types.h"
#include "sleep.h"

// Get device IDs from xparameters.h
#define BTN_ID XPAR_AXI_GPIO_BTN_DEVICE_ID
#define LED_ID XPAR_AXI_GPIO_LED_DEVICE_ID
#define SW_ID XPAR_AXI_GPIO_SW_DEVICE_ID

#define BTN_CHANNEL 1
#define LED_CHANNEL 1
#define SW_CHANNEL 1

#define BTN_MASK 0b1111
#define LED_MASK 0b1111
#define SW_MASK 0b0011


int main() {
	XGpio_Config *cfg_ptr;
	XGpio led_device, btn_device, sw_device;
	u32 data, data_led, data_btn, data_sw, btn_1, btn_2;

	xil_printf("Entered function main\r\n");

	// Initialize LED Device
	cfg_ptr = XGpio_LookupConfig(LED_ID);
	XGpio_CfgInitialize(&led_device, cfg_ptr, cfg_ptr->BaseAddress);

	// Initialize Button Device
	cfg_ptr = XGpio_LookupConfig(BTN_ID);
	XGpio_CfgInitialize(&btn_device, cfg_ptr, cfg_ptr->BaseAddress);

	// Initialize SW Device
	cfg_ptr = XGpio_LookupConfig(SW_ID);
	XGpio_CfgInitialize(&sw_device, cfg_ptr, cfg_ptr->BaseAddress);

	// Set Button Tristate
	XGpio_SetDataDirection(&btn_device, BTN_CHANNEL, BTN_MASK);

	// Set Led Tristate
	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);

	// Set SW Tristate
	XGpio_SetDataDirection(&sw_device, SW_CHANNEL, 0);

	while (1) {
		data_btn = XGpio_DiscreteRead(&btn_device, BTN_CHANNEL);
		data_btn &= BTN_MASK;
		data_sw = XGpio_DiscreteRead(&sw_device, SW_CHANNEL);
		data_sw &= SW_MASK;

		btn_1 = (data_btn & 0b1000) / 8;
		btn_2 = (data_btn & 0b0100) / 4;

		if (data_sw == 0) {
			data = 0;
			data_led = data_btn;
		} else if(data_sw == 1){                  // AND
			data = (btn_1 & btn_2);
			data_led = data_btn + data;
		} else if(data_sw == 2){                  // OR
			data = (btn_1 | btn_2);
			data_led = data_btn + data;
		} else if(data_sw == 3){                  // XOR
			data = (btn_1 ^ btn_2);
			data_led = data_btn + data;
		}
		XGpio_DiscreteWrite(&led_device, LED_CHANNEL, data_led);
		usleep(100);

		xil_printf("LED value: %u %u %u %u\r\n", btn_1, btn_2, 0, data);




	}
}
