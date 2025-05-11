#include "robot_firmware.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "global.h"
#include "software_timer.h"

#include "servo.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "step_motor.h"
#include "line_sensor.h"
#include "gamepad.h"
#include "module_mke_m15.h"
#include "follow_line.h"

#include "button.h"
#include "switch.h"

#include "display.h"
#include "buzzer.h"

#include "uart.h"
#include "utils.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    enum Mode mode_id = MODE_IDLE;

    enum Mode getModeId()
    {
        mode_id = (enum Mode)((getSwitchStatus(SWITCH_1) << 3) | (getSwitchStatus(SWITCH_2) << 2) | (getSwitchStatus(SWITCH_3) << 1) | getSwitchStatus(SWITCH_4));

        return mode_id;
    }

    void initSystem()
    {
        (void)initTimer2();

        (void)initServo(SERVO_1);
        (void)initServo(SERVO_2);
        (void)initServo(SERVO_3);

        (void)initMotor(MOTOR_1);
        (void)initMotor(MOTOR_2);
        (void)initMotor(MOTOR_3);
        (void)initMotor(MOTOR_4);

        (void)initGamepad();
        (void)initModuleMKE(&huart1);

        (void)initButton();
        (void)initSwitch();
        (void)initBuzzer();
        (void)initDisplay();
    }

    enum StatusCode setupRobot()
    {
        switch (mode_id)
        {
        case MODE_IDLE:
        case MODE_TEST_DEBUG_INDICATORS:
        case MODE_TEST_COMM:
        case MODE_TEST_MOTORS:
        case MODE_TEST_ENCODERS:
        // case MODE_TEST_PID: // PID is not ready to be used
        case MODE_TEST_SERVO_MOTORS:
        case MODE_TEST_STEP_MOTORS:
        case MODE_TEST_SENSORS:
        case MODE_TEST_GAMEPAD:
        case MODE_TEST_MODULE_MKE:
        case MODE_TEST_FULL:
        {
            uartTransmitString(&huart2, "Begin initialize process\r\n");

            if (initTimer2() != STATUS_OK)
            {
                uartTransmitString(&huart2, "[ERR] Timer initialized fail\r\n");
            }

            if ((initServo(SERVO_1) != STATUS_OK) || (initServo(SERVO_2) != STATUS_OK) || (initServo(SERVO_3) != STATUS_OK))
            {
                uartTransmitString(&huart2, "[ERR] Servo initialized fail\r\n");
            }

            if ((initMotor(MOTOR_1) != STATUS_OK) || (initMotor(MOTOR_2) != STATUS_OK) || (initMotor(MOTOR_3) != STATUS_OK) || (initMotor(MOTOR_4) != STATUS_OK))
            {
                uartTransmitString(&huart2, "[ERR] Motor initialized fail\r\n");
            }

            if (initGamepad() != STATUS_OK)
            {
                uartTransmitString(&huart2, "[ERR] Gamepad initialized fail\r\n");
            }
            if (initModuleMKE(&huart1) != STATUS_OK)
            {
                uartTransmitString(&huart2, "[ERR] Module MKE m15 initialized fail\r\n");
            }

            if ((initButton() != STATUS_OK) || (initSwitch() != STATUS_OK))
            {
                uartTransmitString(&huart2, "[ERR] Buttons & Switches initialized fail\r\n");
            }

            if ((initBuzzer() != STATUS_OK) || (initDisplay() != STATUS_OK))
            {
                uartTransmitString(&huart2, "[ERR] Buzzer & Display initialized fail\r\n");
            }

            uartTransmitString(&huart2, "\r\nFinished initialize process\r\n");

            sTimer2Set(500, 50);
            displayLeds(0b00000000);

            break;
        }

        case MODE_CONTEST_AUTONOMOUS_1:
        case MODE_CONTEST_AUTONOMOUS_2:
        {
            initSystem();
            sTimer2Set(500, 50);
            displayLeds(0b00000000);

            moveStepMotor(UP, 5);

            break;
        }

        case MODE_CONTEST_CONTROLLED_1:
        case MODE_CONTEST_CONTROLLED_2:
        {
            initSystem();
            sTimer2Set(500, 10);
            displayLeds(0b00000000);

            break;
        }

        default:
        {
            return STATUS_ERROR;
            break;
        }
        }
        return STATUS_OK;
    }

    enum StatusCode processRobot()
    {
        switch (mode_id)
        {
        case MODE_IDLE:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
            }

            break;
        }

        case MODE_TEST_DEBUG_INDICATORS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                static uint8_t byte_displayed = 0b00000000;

                if (isButtonPressed(BUTTON_1))
                {
                    byte_displayed = (byte_displayed << 1) | 0b00000001;
                    displayLeds(byte_displayed);
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    byte_displayed = byte_displayed >> 1;
                    displayLeds(byte_displayed);
                }
                else if (isButtonPressed(BUTTON_3))
                {
                    setFreqBuzzer(byte_displayed);
                }
                else if (isButtonPressed(BUTTON_4))
                {
                    stopBuzzer();
                }
            }
            break;
        }

        case MODE_TEST_COMM:
        {
            if (getFlagUart1())
            {
                (void)readSWGamepadData();
            }

            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    if (readGamepadData() == STATUS_OK)
                    {
                        (void)uartTransmit8b(&huart2, getGamepadData(), 30);
                    }
                    else
                    {
                        (void)uartTransmitString(&huart2, "[ERR] Gamepad data read error\r\n");
                    }
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    (void)uartTransmit8b(&huart2, getSWGamepadData(), 8);
                }
            }
            break;
        }

        case MODE_TEST_MOTORS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                static uint8_t counter = 0;

                if (counter <= 0)
                {
                    if (isButtonPressed(BUTTON_1))
                    {
                        moveDirection(FORWARD, 30);
                        counter = 60;
                    }
                    else if (isButtonPressed(BUTTON_2))
                    {
                        moveDirection(BACKWARD, 30);
                        counter = 60;
                    }
                    else if (isButtonPressed(BUTTON_3))
                    {
                        moveDirection(LEFT, 30);
                        counter = 60;
                    }
                    else if (isButtonPressed(BUTTON_4))
                    {
                        moveDirection(RIGHT, 30);
                        counter = 60;
                    }
                }
                else
                {
                    --counter;
                    displayLeds(counter);
                    if (counter == 0)
                    {
                        motorStop();
                        displayLeds(0b00000000);
                    }
                }
            }
            break;
        }

        case MODE_TEST_ENCODERS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    uint32_t encoder_1_pulse = getPulseCount(ENCODER_1);
                    uint8_t transmit_buffer[4] = {0};
                    uint32_to_bytes(&encoder_1_pulse, transmit_buffer, 1);
                    uartTransmit8b(&huart2, transmit_buffer, 4);
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    uint32_t encoder_2_pulse = getPulseCount(ENCODER_2);
                    uint8_t transmit_buffer[4] = {0};
                    uint32_to_bytes(&encoder_2_pulse, transmit_buffer, 1);
                    uartTransmit8b(&huart2, transmit_buffer, 4);
                }
                else if (isButtonPressed(BUTTON_3))
                {
                    uint32_t encoder_3_pulse = getPulseCount(ENCODER_3);
                    uint8_t transmit_buffer[4] = {0};
                    uint32_to_bytes(&encoder_3_pulse, transmit_buffer, 1);
                    uartTransmit8b(&huart2, transmit_buffer, 4);
                }
                else if (isButtonPressed(BUTTON_4))
                {
                    uint32_t encoder_4_pulse = getPulseCount(ENCODER_4);
                    uint8_t transmit_buffer[4] = {0};
                    uint32_to_bytes(&encoder_4_pulse, transmit_buffer, 1);
                    uartTransmit8b(&huart2, transmit_buffer, 4);
                }
            }
            break;
        }

        case MODE_TEST_PID:
        {
            break;
        }

        case MODE_TEST_SERVO_MOTORS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();

                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    servoSetAngle(SERVO_1, 0);
                    servoSetAngle(SERVO_2, 0);
                    servoSetAngle(SERVO_3, 0);

                    (void)uartTransmitString(&huart2, "Button 1 Pressed\r\n");
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    servoSetAngle(SERVO_1, 14);
                    servoSetAngle(SERVO_2, 14);
                    servoSetAngle(SERVO_3, 14);

                    (void)uartTransmitString(&huart2, "Button 2 Pressed\r\n");
                }
                else if (isButtonPressed(BUTTON_3))
                {
                    servoSetAngle(SERVO_1, 28);
                    servoSetAngle(SERVO_2, 28);
                    servoSetAngle(SERVO_3, 28);

                    (void)uartTransmitString(&huart2, "Button 3 Pressed\r\n");
                }
                else if (isButtonPressed(BUTTON_4))
                {
                    servoSetAngle(SERVO_1, MAX_SERVO_ANGLE);
                    servoSetAngle(SERVO_2, MAX_SERVO_ANGLE);
                    servoSetAngle(SERVO_3, MAX_SERVO_ANGLE);

                    (void)uartTransmitString(&huart2, "Button 4 Pressed\r\n");
                }
            }
            break;
        }

        case MODE_TEST_STEP_MOTORS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    moveStepMotor(UP, 1);
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    moveStepMotor(DOWN, 1);
                }
                else if (isButtonPressed(BUTTON_3))
                {
                    moveStepMotor(UP, 5);
                }
                else if (isButtonPressed(BUTTON_4))
                {
                    stopStepMotor();
                }
            }
            break;
        }

        case MODE_TEST_SENSORS:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();
                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    (void)calibSensor();
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    (void)scanSensor();
                    displayLeds(getSensorState());
                }
                else if (isButtonPressed(BUTTON_3))
                {
                    uint8_t transmit_buffer[16] = {0};
                    uint16_to_bytes(getSensorCalibValue(), transmit_buffer, 8);
                    (void)uartTransmit8b(&huart2, transmit_buffer, 16);
                }
                else if (isButtonPressed(BUTTON_4))
                {
                    uint8_t transmit_buffer[16] = {0};
                    uint16_to_bytes(getSensorValue(), transmit_buffer, 8);
                    (void)uartTransmit8b(&huart2, transmit_buffer, 16);
                }
            }
            break;
        }

        case MODE_TEST_FULL:
        {
            break;
        }

        case MODE_CONTEST_AUTONOMOUS_1:
        {
            if (sTimer2GetFlag())
            {
                static enum StatusCode is_robot_ready = STATUS_ERROR;

                (void)scanButton();

                if (isButtonPressed(BUTTON_1))
                {
                    (void)calibSensor();
                }
                else if (isButtonPressed(BUTTON_2))
                {
                    servoSetAngle(SERVO_3, 55);
                    is_robot_ready = STATUS_OK;
                }

                (void)scanSensor();
                displayLeds(getSensorState());

                if (is_robot_ready == STATUS_OK)
                {
                    toggleLedDebug();

                    static uint16_t previous_cross_count = 0;
                    static uint16_t counter = 0;

                    followLine();
                    turnRightAtCross(1);
                    turnLeftAtCross(2);
                    turnRightAtCross(4);
                    turnLeftAtCross(5);
                    turnRightAtCross(10);

                    switch (getCrossCount())
                    {
                    case 7:
                    {
                        if (previous_cross_count != getCrossCount())
                        {
                            previous_cross_count = getCrossCount();

                            servoSetAngle(SERVO_3, 0);
                            moveStepMotor(DOWN, 10);
                        }

                        break;
                    }
                    case 8:
                    {
                        if (previous_cross_count != getCrossCount())
                        {
                            previous_cross_count = getCrossCount();

                            disableFollowLine();
                            motorStop();

                            servoSetAngle(SERVO_3, 55);
                            moveStepMotor(UP, 5);
                        }
                        else
                        {
                            counter = (counter + 1) % 40;
                            if (counter == 0)
                            {
                                enableFollowLine();
                                moveForward(20);
                            }
                        }
                        break;
                    }
                    case 9:
                    {
                        if (previous_cross_count != getCrossCount())
                        {
                            previous_cross_count = getCrossCount();

                            disableFollowLine();
                            motorStop();

                            moveStepMotor(DOWN, 5);
                        }
                        else
                        {
                            counter = (counter + 1) % 60; // 1 seconds
                            if (counter == 20)
                            {
                                servoSetAngle(SERVO_3, 0);
                            }
                            else if (counter == 40)
                            {
                                moveStepMotor(UP, 10);
                            }
                            else if (counter == 60)
                            {
                                enableFollowLine();
                                turnAroundAtCross(10);
                            }
                        }
                        break;
                    }

                    default:
                        break;
                    }

                    // else if (getCrossCount() == 8)
                    // {
                    //     servoSetAngle(SERVO_3, 55);
                    // }
                    // else if (getCrossCount() == 9)
                    // {
                    //     servoSetAngle(SERVO_3, 0);
                    // }

                    // if (getCrossCount() == 8)
                    // {
                    //     motorStop();
                    //     servoSetAngle(SERVO_3, 0);
                    //     is_robot_ready = STATUS_ERROR;
                    // }
                    // else if (curr_line_status == LINE_END)
                    // {
                    //     motorStop();
                    // }
                    // else if (curr_line_status == LINE_ERROR)
                    // {
                    //     setFreqBuzzer(1000);
                    // }
                }
            }
            break;
        }

        case MODE_CONTEST_AUTONOMOUS_2:
        {
            break;
        }

        case MODE_TEST_GAMEPAD:
        case MODE_CONTEST_CONTROLLED_1:
        {
            if (sTimer2GetFlag())
            {
                toggleLedDebug();

                if (readGamepadData() == STATUS_OK)
                {
                    uint8_t speed = 25;

                    if (isGamepadButtonPressed(BUTTON_TRIANGLE))
                    {
                        speed *= 2;
                    }
                    else if (isGamepadButtonPressed(BUTTON_CROSS))
                    {
                        speed /= 2;
                    }

                    if (isGamepadButtonPressed(BUTTON_UP))
                    {
                        moveDirection(FORWARD, speed);
                    }
                    else if (isGamepadButtonPressed(BUTTON_DOWN))
                    {
                        moveDirection(BACKWARD, speed);
                    }
                    else if (isGamepadButtonPressed(BUTTON_LEFT))
                    {
                        moveDirection(LEFT, speed);
                    }
                    else if (isGamepadButtonPressed(BUTTON_RIGHT))
                    {
                        moveDirection(RIGHT, speed);
                    }
                    else if (isGamepadButtonPressed(BUTTON_SQUARE))
                    {
                        moveDirection(ROTATE_LEFT, speed);
                    }
                    else if (isGamepadButtonPressed(BUTTON_CIRCLE))
                    {
                        moveDirection(ROTATE_RIGHT, speed);
                    }
                    else
                    {
                        motorStop();
                    }

                    if (isGamepadButtonPressed(BUTTON_L1))
                    {
                        moveStepMotor(UP, 1);
                    }
                    else if (isGamepadButtonPressed(BUTTON_L2))
                    {
                        moveStepMotor(DOWN, 1);
                    }
                    else
                    {
                        stopStepMotor();
                    }

                    static uint8_t angle = 0;
                    static uint8_t angle_step = 5;
                    if (isGamepadButtonPressed(BUTTON_R1))
                    {
                        angle = (angle < MAX_SERVO_ANGLE) ? (angle + angle_step) : MAX_SERVO_ANGLE;

                        servoSetAngle(SERVO_1, angle);
                        servoSetAngle(SERVO_2, angle);
                        servoSetAngle(SERVO_3, angle);
                    }
                    else if (isGamepadButtonPressed(BUTTON_R2))
                    {
                        angle = (angle > 0) ? (angle - angle_step) : 0;

                        servoSetAngle(SERVO_1, angle);
                        servoSetAngle(SERVO_2, angle);
                        servoSetAngle(SERVO_3, angle);
                    }
                }
            }

            break;
        }

        case MODE_TEST_MODULE_MKE:
        case MODE_CONTEST_CONTROLLED_2:
        {
            if (getFlagUart1())
            {
                (void)readSWGamepadData();
            }
            if (sTimer2GetFlag())
            {
                toggleLedDebug();

                uint8_t speed = 25;

                if (isSWGamepadButtonPressed(SW_BUTTON_START))
                {
                    speed *= 2;
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_SELECT))
                {
                    speed /= 2;
                }

                if (isSWGamepadButtonPressed(SW_BUTTON_UP))
                {
                    moveDirection(FORWARD, speed);
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_DOWN))
                {
                    moveDirection(BACKWARD, speed);
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_LEFT))
                {
                    moveDirection(LEFT, speed);
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_RIGHT))
                {
                    moveDirection(RIGHT, speed);
                }
                else
                {
                    motorStop();
                }

                if (isSWGamepadButtonPressed(SW_BUTTON_CROSS))
                {
                    moveStepMotor(DOWN, 1);
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_TRIANGLE))
                {
                    moveStepMotor(UP, 1);
                }
                else
                {
                    stopStepMotor();
                }

                static uint8_t angle = 0;
                static uint8_t angle_step = 5;
                if (isSWGamepadButtonPressed(SW_BUTTON_CIRCLE))
                {
                    angle = (angle < MAX_SERVO_ANGLE) ? (angle + angle_step) : MAX_SERVO_ANGLE;

                    servoSetAngle(SERVO_1, angle);
                    servoSetAngle(SERVO_2, angle);
                    servoSetAngle(SERVO_3, angle);
                }
                else if (isSWGamepadButtonPressed(SW_BUTTON_SQUARE))
                {
                    angle = (angle > 0) ? (angle - angle_step) : 0;

                    servoSetAngle(SERVO_1, angle);
                    servoSetAngle(SERVO_2, angle);
                    servoSetAngle(SERVO_3, angle);
                }
            }
            break;
        }

        default:
        {
            return STATUS_ERROR;
            break;
        }
        }
        return STATUS_OK;
    }
#ifdef __cplusplus
}
#endif /* __cplusplus */