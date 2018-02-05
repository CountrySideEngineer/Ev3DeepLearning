/**
 * @file    Sample applicaton of deep-learning, q-learning.
 * 
 * @date    2018.02.04
 * @versoin 1.0
 * @author  Countryside-Engineer
 */
#include "ev3api.h"
#include "q_learn.h"

const motor_port_t left_motor_port = EV3_PORT_A;
const motor_port_t right_motor_port = EV3_PORT_D;
const sensor_port_t sonic_sensor_port = EV3_PORT_1;
const sensor_port_t button_port = EV3_PORT_2;
const lcdfont_t font_size = EV3_FONT_SMALL;

char msg_buf[32];

void debug_write_msg(const char *msg, int line_index);

void main_task(intptr_t unused)
{
    int left_motor_output = 0;
    int right_motor_output = 0;
    int right_motor_state = 0;
    int left_motor_state = 0;
    int next_left_motor_state = 0;
    int next_right_motor_state = 0;
    int is_press = 0;
    int action = 0;
    int loop_count = 0;
    ledcolor_t led_color = LED_OFF;
    int16_t distance = 0;

    ev3_lcd_set_font(font_size);

    ev3_motor_config(left_motor_port, MEDIUM_MOTOR);
    ev3_motor_config(right_motor_port, MEDIUM_MOTOR);
    ev3_sensor_config(sonic_sensor_port, ULTRASONIC_SENSOR);
    ev3_sensor_config(button_port, TOUCH_SENSOR);

    //init_qlearning();

    while (1) {
        ev3_led_set_color(led_color);
        loop_count++;
        led_color = loop_count % 4;
        if (LED_OFF == led_color) {
            loop_count = 0;
        }

        left_motor_output = ev3_motor_get_power(left_motor_port);
        right_motor_output = ev3_motor_get_power(right_motor_port);
        distance = ev3_ultrasonic_sensor_get_distance(sonic_sensor_port);
        is_press = ev3_touch_sensor_is_pressed(button_port);

        left_motor_state = power2state(left_motor_output);
        right_motor_state = power2state(right_motor_output);

        action = select_action(left_motor_state, right_motor_state, distance);
        next_state(left_motor_output, right_motor_output, distance,
            action,
            &next_left_motor_state, &next_right_motor_state);
        update_qvalues(left_motor_state, right_motor_state, distance,
            action,
            next_left_motor_state, next_right_motor_state, is_press);

        left_motor_output = state2power(next_left_motor_state);
        right_motor_output = state2power(next_right_motor_state);

        ev3_motor_set_power(left_motor_port, left_motor_output);
        ev3_motor_set_power(right_motor_port, right_motor_output);

        sprintf(msg_buf, "%d - %d - %d",
            left_motor_output, right_motor_output, distance);
        debug_write_msg(msg_buf, 0);

        tslp_tsk(50);
    }
}

/**
 *  @brief  Commonly used function.
 *          Display message for debug on LCD.
 *
 *  @param[in,out]  msg Pointer to message to display.
 *  @param          line_index  The row number of message.
 */
void debug_write_msg(const char *msg, int line_index) {
    int32_t line_pos_x = 0;

    //基本的に引数にNULLが指定されることはないが、念のために確認する。
    if (NULL != msg) {
        line_pos_x = EV3_FONT_SMALL * ((int32_t)line_index);
        
        ev3_lcd_draw_string((char *)msg, 0, line_pos_x);
    }
}
