#ifndef INC_ROBOT_FIRMWARE_H_
#define INC_ROBOT_FIRMWARE_H_

#include <stdlib.h>
#include <stdint.h>

enum Mode
{
    MODE_IDLE,

    // Test Modes
    MODE_TEST_DEBUG_INDICATORS,
    MODE_TEST_COMM,
    MODE_TEST_MOTORS,
    MODE_TEST_ENCODERS,
    MODE_TEST_PID,
    MODE_TEST_SERVO_MOTORS,
    MODE_TEST_STEP_MOTORS,
    MODE_TEST_SENSORS,
    MODE_TEST_GAMEPAD,
    MODE_TEST_MODULE_MKE,
    MODE_TEST_FULL,

    // Contest Modes
    MODE_CONTEST_AUTONOMOUS_1,
    MODE_CONTEST_AUTONOMOUS_2,

    MODE_CONTEST_CONTROLLED_1,
    MODE_CONTEST_CONTROLLED_2,
};

enum Mode getModeId(void);
enum StatusCode setupRobot(void);
enum StatusCode processRobot(void);

#endif /* INC_ROBOT_FIRMWARE_H_ */
