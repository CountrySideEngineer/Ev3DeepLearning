#pragma once

//Macros about map
#define STATE_MOTOR     (81)    //-41 to 40
#define STATE_DIST      (251)   //  0 to 250
#define Q_VALUEMAX      (100) //  0 to 100 / LSB:0.01
#define Q_VALUEMIN      (0)     //  0 to 100 / LSB:0.01

//Enumrator of actions.
enum {
    ACTION_SPEED_UP = 0,
    ACTION_SLOW_DOWN,
    ACTION_TURN_RIGHT,
    ACTION_TURN_LEFT,
    ACTION_SPPED_KEEP,
    ACTION_NUM
};

//External function.
extern void init_qlearning(void);
extern int select_action(int motor_state_1, int motor_state_2, int distance);
extern void next_state(int motor_state_1,
    int motor_state_2,
    int distance,
    int action,
    int *next_motor_state_1,
    int *next_motor_state_2);
extern void update_qvalues(int motor_state_1,
    int motor_state_2,
    int distance,
    int action,
	int next_motor_state_1,
    int next_motor_state_2,
    int isPenalty);
extern int power2state(int motor_power);
extern int state2power(int motor_state);
