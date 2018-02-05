/**
 * @file    Main source code of Q-Learning sample.
 * @author  Country-side engineer.
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include "q_learn.h"

extern int qvalues[STATE_MOTOR][STATE_MOTOR][STATE_DIST][ACTION_NUM];

/**
 * Main function.
 * 
 * @return  Result of test. The value must be "0".
 */
int main(void)
{
    int motor_state_1 = 0;
    int motor_state_2 = 0;
    int next_motor_state_1 = 0;
    int next_motor_state_2 = 0;
    int distance = 0;
    int action = 0;
    int loop_count = 0;
    int index = 0;
    int step = 0;
    int row = 0;
    int col = 0;

    init_qlearning();

    for (index = 0; index < 100; index++) {
        motor_state_1 = 41; //Value 41 shows that motor output power is 0.
        motor_state_2 = 41; //Value 41 shows that motor output power is 0.
        next_motor_state_1 = 0;
        next_motor_state_2 = 0;
        distance = 0;
        action = 0;
        for (step = 0; step < 512; step++) {
            distance = rand() % 251;
            action = select_action(motor_state_1, motor_state_2, distance);
            next_state(motor_state_1, motor_state_2, 
                distance, action,
                &next_motor_state_1, &next_motor_state_2);
            update_qvalues(motor_state_1, motor_state_2,
                distance, action,
                next_motor_state_1, next_motor_state_2,
                rand() % 2);

            motor_state_1 = next_motor_state_1;
            motor_state_2 = next_motor_state_1;

            //printf("step = %d : motor_state_1 = %3d motor_state_2 = %3d\n",
            //    step, motor_state_1, motor_state_2);
        }
    }
    for (step = 0; step < ACTION_NUM; step++) {
        for (row = 0; row < STATE_MOTOR; row++) {
            for (col = 0; col < STATE_MOTOR; col++) {
                printf("%5d,", qvalues[row][col][41][step]);
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}

