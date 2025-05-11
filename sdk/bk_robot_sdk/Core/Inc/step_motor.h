#ifndef INC_STEP_MOTOR_H_
#define INC_STEP_MOTOR_H_

#include <stdint.h>
#include "global.h"

enum Step_Direction
{
    DOWN,
    UP
};

enum StatusCode moveStepMotor(enum Step_Direction direction, uint8_t cycle);
enum StatusCode stopStepMotor(void);

#endif /* INC_STEP_MOTOR_H_ */
