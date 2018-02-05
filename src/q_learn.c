/**
 * @file    Q-Learning source code.
 * 
 * @date    2018.02.04
 * @version 1.0
 * @author  Countryside-Engineer
 */
#include <stdio.h>
#include <stdlib.h>
//#include "ev3api.h"
#include "q_learn.h"

#define ALPHA       (10)    //Learning coefficient : LSB:0.01
#define GANNMA      (90)    //Discount rate : LSB:0.01
#define SEED        (100) //Seed of random values.

/***************************************************/
/*                                                 */ 
/*        Declaration of global variables.         */
/*                                                 */ 
/***************************************************/
/*
 * RANGE: 0 to 100
 * LSB:0.01
 */
int qvalues[STATE_MOTOR][STATE_MOTOR][STATE_DIST][ACTION_NUM];

/***************************************************/
/*                                                 */ 
/*        Declaration of local functions.          */
/*                                                 */ 
/***************************************************/
void init_qlearning(void);
void init_qvalues(void);
void next_state(int motor_state_1, int motor_state_2, int distance, int action,
	int *next_motor_state_1, int *next_motor_state_2);
int random100(void);
int action_random(void);
int select_action_by_q(int motor_state_1, int motor_state_2, int distance);

/**
 * Initialize values and configurations of q-learning.
 */
void init_qlearning(void)
{
    srand(SEED);

    init_qvalues();
}

/**
 * Initialize "Q" values.
 */
void init_qvalues(void)
{
    int i;
    int j;
    int k;
    int l;

    for (i = 0; i < STATE_MOTOR; i++) {
        for (j = 0; j < STATE_MOTOR; j++) {
            for (k = 0; k < STATE_DIST; k++) {
                for (l = 0; l < (int)ACTION_NUM; l++) {
                    qvalues[i][j][k][l] = random100();

                    if ((i == 0) || (j == 0)) {
                        qvalues[i][j][k][ACTION_SLOW_DOWN] = 0;
                    }
                    if ((((STATE_MOTOR - 1) == i) || ((STATE_MOTOR - 1) == j))
                        || (k < 1))
                    {
                        qvalues[i][j][k][ACTION_SPEED_UP] = 0;
                        qvalues[i][j][k][ACTION_TURN_RIGHT] = 0;
                        qvalues[i][j][k][ACTION_TURN_LEFT] = 0;
                        qvalues[i][j][k][ACTION_SPPED_KEEP] = 0;
                    }
                }
            }
        }
    }
}

/**
 * Select actoin by current state decided by motor output power and distance read from
 * ultrasonic sensor.
 *
 * @param   motor_state_1   Motor output power.
 * @param   motor_state_2   Motor output power.
 * @param   distance        Distance read from ultrasonic sensor.
 * 
 * @return  Action selected (decided) base on "Q" values specified by motor motor output
 *          power and distance.
 */
#define EPSILON     (3000)    //LSB : 0.01 - This maros is 30 in physical (mathematical?).
int select_action(int motor_state_1, int motor_state_2, int distance)
{
    int action = 0;

    if (random100() < EPSILON) {
        /*
         * Select action by e-Greedy method.
         */
        do {
            action = action_random();
            //printf("qvalues[%d][%d][%d][%d] = %d\n",
            //    motor_state_1, motor_state_2, distance, action,
            //    qvalues[motor_state_1][motor_state_2][distance][action]);
        } while (0 == qvalues[motor_state_1][motor_state_2][distance][action]);
    } else {
        action = select_action_by_q(motor_state_1, motor_state_2, distance);
    }

    return action;
}

int select_action_by_q(int motor_state_1, int motor_state_2, int distance)
{
	int action = 0;
	int maxq = 0;
	int max_action = 0;

	for (action = 0; action < ACTION_NUM; action++) {
		if (maxq < qvalues[motor_state_1][motor_state_2][distance][action]) {
			maxq = qvalues[motor_state_1][motor_state_2][distance][action];
			max_action = action;
		}
	}

	return max_action;
}

void next_state(int motor_state_1, int motor_state_2, int distance, int action,
	int *next_motor_state_1, int *next_motor_state_2)
{
    switch (action) {
        case ACTION_SPEED_UP:
            *next_motor_state_1 = motor_state_1 + 1;
            *next_motor_state_2 = motor_state_2 + 1;
            break;

        case ACTION_SLOW_DOWN:
            *next_motor_state_1 = motor_state_1 - 1;
            *next_motor_state_2 = motor_state_2 - 1;
            break;

        case ACTION_TURN_LEFT:
            *next_motor_state_1 = motor_state_1 + 1;
            break;

        case ACTION_TURN_RIGHT:
            *next_motor_state_2 = motor_state_2 + 1;
            break;

        case ACTION_SPPED_KEEP:
        default:
            *next_motor_state_1 = motor_state_1;
            *next_motor_state_2 = motor_state_2;
            break;
    }

    if (*next_motor_state_1 <= 0) {
        *next_motor_state_1 = 0;
    }
    if (*next_motor_state_2 <= 0) {
        *next_motor_state_2 = 0;
    }
    if (STATE_MOTOR <= *next_motor_state_1) {
        *next_motor_state_1 = STATE_MOTOR - 1;
    }
    if (STATE_MOTOR <= *next_motor_state_2) {
        *next_motor_state_2 = STATE_MOTOR - 1;
    }
}


void update_qvalues(int motor_state_1, int motor_state_2, int distance, int action,
	int next_motor_state_1, int next_motor_state_2, int isPenalty)
{
    int next_action = 0;
    int qvalue = 0;
    int next_qvalue = 0;
    int alpha_item = 0;
    int gannma_item = 0;

    if (0 == isPenalty) {
        next_action =
            select_action(next_motor_state_1, next_motor_state_2, distance);
        next_qvalue =
            qvalues[next_motor_state_1][next_motor_state_2][distance][next_action];
        qvalue = qvalues[motor_state_1][motor_state_2][distance][action];
        //Dividing 100 is to convert LSB from 0.0001 to 0.01.
        gannma_item = (int)(((long)next_qvalue * GANNMA) / 100);
        alpha_item = (int)((((long)gannma_item - (long)qvalue) * ALPHA) / 100);
        
        qvalues[motor_state_1][motor_state_2][distance][action] += alpha_item;
        if (Q_VALUEMAX <= qvalues[motor_state_1][motor_state_2][distance][action]) {
            qvalues[motor_state_1][motor_state_2][distance][action] = Q_VALUEMAX;
        }
        if (qvalues[motor_state_1][motor_state_2][distance][action] <= Q_VALUEMIN) {
            qvalues[motor_state_1][motor_state_2][distance][action] = Q_VALUEMIN;
        }
    }
}

/**
 * Select action randomly.
 */
int action_random(void)
{
    return (rand() % ACTION_NUM);
}

/**
 * Create random value in range 0 to 100, LSB is 0.01;
 */
int random100(void)
{
    return (rand() % 10001);
}

int power2state(int motor_power)
{
    int motor_state = 0;

    motor_state = motor_power + 41;
    if (motor_power <= -40) {
        motor_state = 0;
    } else if (40 <= motor_power) {
        motor_state = STATE_MOTOR - 1;
    }

    return motor_state;
}

int state2power(int motor_state)
{
    return motor_state - 41;
}