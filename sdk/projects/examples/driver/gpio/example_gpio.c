/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     example_gpio.c
 * @brief    the main function for the GPIO driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <stdio.h>
#include "soc.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include "drv_usart.h"
#include "pin_name.h"
#include "pin.h"

#define LED_MODE_BLINK 0
#define LED_MODE_RUN 1

/*
 * Global Variables
 * 
 */
static uint32_t led_mode = 0;

/*****************************************************************************

gpio test part

*****************************************************************************/
static uint32_t led_flag = 0;

volatile static bool int_flag = 1;

static void gpio_interrupt_handler(int32_t idx)
{
    int_flag = 0;
}

void example_pin_gpio_init(void)
{
    drv_pinmux_config(EXAMPLE_GPIO_PIN, EXAMPLE_GPIO_PIN_FUNC);
}

void gpio_set_pin(pin_name_e gpio_pin)
{
	gpio_pin_handle_t pin = NULL;
	
	example_pin_gpio_init();
	pin = csi_gpio_pin_initialize(gpio_pin, gpio_interrupt_handler);
	
	csi_gpio_pin_config_mode(pin, GPIO_MODE_PULLUP);
	csi_gpio_pin_config_direction(pin, GPIO_DIRECTION_OUTPUT);
	
	csi_gpio_pin_write(pin, 1);
}

void gpio_reset_pin(pin_name_e gpio_pin)
{
	gpio_pin_handle_t pin = NULL;
	
	example_pin_gpio_init();
	pin = csi_gpio_pin_initialize(gpio_pin, gpio_interrupt_handler);
	
	csi_gpio_pin_config_mode(pin, GPIO_MODE_PULLUP);
	csi_gpio_pin_config_direction(pin, GPIO_DIRECTION_OUTPUT);
	
	csi_gpio_pin_write(pin, 0);
}

int user_delay(uint32_t delay_time)
{
	uint32_t i = delay_time;
	while(i!=0) i--;
	return 0;
}

void LED_All_On(void){
	gpio_set_pin(PA1);//LED D1 single
	gpio_set_pin(PA2);//LED D2 PA2 and PA4 are in serial connection to control LD2
	gpio_set_pin(PA4);//LED D2
	gpio_set_pin(PA3);//LED D3 PA3 and PA5 are in serial connection to control LD3
	gpio_set_pin(PA5);//LED D3
	gpio_set_pin(PA9);//LED D4
	gpio_set_pin(PA7);//LED D5 PA7 and PA8 are in serial connection to control LD5
	gpio_set_pin(PA8);//LED D5
	gpio_set_pin(PA12);//LED D6
	gpio_set_pin(PA13);//LED D7
	gpio_set_pin(PA14);//LED D8
	gpio_set_pin(PA15);//LED D9
}

void LED_All_Off(void){
	gpio_reset_pin(PA1);
	gpio_reset_pin(PA2);
	gpio_reset_pin(PA4);
	gpio_reset_pin(PA3);
	gpio_reset_pin(PA5);
	gpio_reset_pin(PA9);//LED D4
	gpio_reset_pin(PA7);
	gpio_reset_pin(PA8);
	gpio_reset_pin(PA12);//LED D6
	gpio_reset_pin(PA13);//LED D7
	gpio_reset_pin(PA14);
	gpio_reset_pin(PA15);
}

static uint32_t led_runcount = 0;
void LED_Run(void){
	LED_All_Off();
	led_runcount++;
	if(led_runcount == 1){
		gpio_set_pin(PA1);//LED D1 single
	}
	else if(led_runcount == 2){
		gpio_set_pin(PA2);//LED D2 PA2 and PA4 are in serial connection to control LD2
		gpio_set_pin(PA4);//LED D2
	}
	else if(led_runcount == 3){
		gpio_set_pin(PA3);//LED D3 PA3 and PA5 are in serial connection to control LD3
		gpio_set_pin(PA5);//LED D3
	}
	else if(led_runcount == 4){
		gpio_set_pin(PA9);//LED D4
	}
	else if(led_runcount == 5){
		gpio_set_pin(PA7);//LED D5 PA7 and PA8 are in serial connection to control LD5
		gpio_set_pin(PA8);//LED D5
	}
	else if(led_runcount == 6){
		gpio_set_pin(PA12);//LED D6
	}
	else if(led_runcount == 7){
		gpio_set_pin(PA13);//LED D7
	}
	else if(led_runcount == 8){
		gpio_set_pin(PA14);//LED D8
	}
	else if(led_runcount == 9){
		gpio_set_pin(PA15);//LED D9
	}
	else
		led_runcount = 0;
}

void LED_Blink(void){
	if(!led_flag)
		LED_All_On();
	else if(led_flag)
		LED_All_Off();
	else
		;
	led_flag = !led_flag;
}
/*****************************************************************************

timer test part

*****************************************************************************/
static void timer_event_cb_fun(int32_t idx, timer_event_e event)
{
	if(led_mode == LED_MODE_BLINK){
		LED_Blink();
	}
	else if(led_mode == LED_MODE_RUN){
		LED_Run();
	}
	else {
		printf("led mode setting error\n");
		LED_All_On();
	}
}

static int32_t test_user_defined_fun(timer_handle_t timer_handle)
{
    int32_t ret;

    ret = csi_timer_config(timer_handle, TIMER_MODE_RELOAD);

    if (ret < 0) {
        return -1;
    }

    ret = csi_timer_set_timeout(timer_handle, 1000000);

    if (ret < 0) {
        return -1;
    }

    ret = csi_timer_start(timer_handle);

    if (ret < 0) {
        return -1;
    }

    return 0;
}

static int32_t test_timer(uint8_t timer_num)
{
    int32_t ret;
    timer_handle_t timer_handle;

    timer_handle = csi_timer_initialize(timer_num, timer_event_cb_fun);

    ret = test_user_defined_fun(timer_handle);

    if (ret < 0) {
        printf("test_reload_fun error\n");
        return -1;
    }

    return 0;
}
/*****************************************************************************

usart test part

*****************************************************************************/
static usart_handle_t g_usart_handle;
static volatile uint8_t rx_async_flag = 0;
static volatile uint8_t tx_async_flag = 0;
static volatile uint8_t rx_trigger_flag = 0;

uint8_t  data[18] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                     'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
                    };


static int32_t usart_receive_async(usart_handle_t usart, void *data, uint32_t num)
{
    int time_out = 0x7fffff;
    rx_async_flag = 0;

    csi_usart_receive(usart, data, num);

    while (time_out) {
        time_out--;

        if (rx_async_flag == 1) {
            break;
        }
    }

    if (0 == time_out) {
        return -1;
    }

    rx_async_flag = 0;
    return 0;
}

static int32_t usart_send_async(usart_handle_t usart, const void *data, uint32_t num)
{
    int time_out = 0x7ffff;
    tx_async_flag = 0;

    csi_usart_send(usart, data, num);

    while (time_out) {
        time_out--;

        if (tx_async_flag == 1) {
            break;
        }
    }

    if (0 == time_out) {
        return -1;
    }

    tx_async_flag = 0;
    return 0;
}

static void usart_event_cb(int32_t idx, uint32_t event)
{
    uint8_t g_data[15];

    switch (event) {
        case USART_EVENT_SEND_COMPLETE:
            tx_async_flag = 1;
            break;

        case USART_EVENT_RECEIVE_COMPLETE:
            rx_async_flag = 1;
            break;

        case USART_EVENT_RECEIVED:
            csi_usart_receive_query(g_usart_handle, g_data, 15);

        default:
            break;
    }
}

static void usart_event_cb_query(int32_t idx, uint32_t event)
{
    uint8_t g_data[15];
    uint8_t ret;

    switch (event) {
        case USART_EVENT_SEND_COMPLETE:
            tx_async_flag = 1;
            break;

        case USART_EVENT_RECEIVE_COMPLETE:
            rx_async_flag = 1;
            break;

        case USART_EVENT_RECEIVED:
            ret = csi_usart_receive_query(g_usart_handle, g_data, 15);
            csi_usart_send(g_usart_handle, g_data, ret);
            rx_trigger_flag = 1;
            break;

        default:
            break;
    }
}

static int32_t usart_wait_reply_async(usart_handle_t usart)
{
    volatile uint32_t i;
    uint8_t ch;
    char answer[20];

    for (i = 0; i < 20; i++) {
        answer[i] = '\0';
    }

    i = 0;

    while (i < 20) {
        if (0 == usart_receive_async(usart, &ch, 1)) {
            if (ch == '\n' || ch == '\r') {
                answer[i] = '\0';
                break;
            }

            if (ch == 127 || ch == '\b') {
                if (i > 0) {
                    i--;
                    usart_send_async(usart, &ch, 1);
                }
            } else {
                answer[i++] = ch;
                usart_send_async(usart, &ch, 1);
            }
        }
    }

    if ((i == 1) && (answer[0] == '1')) {
        return 1;
    } else if ((i == 1) && (answer[0] == '2')) {
        return 0;
    }

    return 2;
}

static int32_t usart_test_async_mode(usart_handle_t usart, int32_t uart_idx)
{
    uint32_t get;
    int32_t  ret;

    ///////////////////* async mode ///////////////////////
    /* Insure prompt information is displayed */
    /* Changes usart mode to interrupt mode */
    csi_usart_uninitialize(usart);
    usart = csi_usart_initialize(uart_idx, (usart_event_cb_t)usart_event_cb);

    if (usart == NULL) {
        return -1;
    }

    g_usart_handle = usart;
    ret = csi_usart_config(usart, 115200, USART_MODE_ASYNCHRONOUS, USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);

    if (ret < 0) {
        printf("csi_usart_config error %x\n", ret);
        return -1;
    }

    printf("\r\t LED Mode Configuration\n\t\t");
//    usart_send_async(usart, data, sizeof(data));
    printf("- - - 1. Blink Mode\n\t\t - - - 2. Run Mode\n\t\t ");

    while (1) {
        /* wait for the input*/
        get = usart_wait_reply_async(usart);

        if ((get == 1) || (get == 0)) {
            break;
        } else {
            printf("\n\tPlease enter '1' or '2'   ");
        }
    }

    if (get == 1) {
		led_mode = LED_MODE_BLINK;
        printf("\n- - -Blink Mode Choosed\n");
    } else {
		led_mode = LED_MODE_RUN;
        printf("\n- - -Run Mode Choosed\n");
    }

    return 0;
}

void example_pin_usart_init(void)
{
    drv_pinmux_config(EXAMPLE_PIN_USART_TX, EXAMPLE_PIN_USART_TX_FUNC);
    drv_pinmux_config(EXAMPLE_PIN_USART_RX, EXAMPLE_PIN_USART_RX_FUNC);
}

int test_usart(void){
    usart_handle_t p_csi_usart;
    int32_t ret;

    example_pin_usart_init();

    ret = usart_test_async_mode(p_csi_usart, EXAMPLE_USART_IDX);

    if (ret < 0) {
        printf("_usart_test error %x\n", ret);
        return -1;
    }

    return 0;
}
/*****************************************************************************
test_gpio: main function of the timer, gpio and usart test

INPUT: NULL

RETURN: NULL

*****************************************************************************/

int main(void)
{
	test_usart();
	test_timer(0);
	
	while(1){	
		test_usart();		
	}

	return 0;
}
