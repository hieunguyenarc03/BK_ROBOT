#include "follow_line.h"

#include "motor.h"
#include "line_sensor.h"
#include "encoder.h"

#define FOLLOW_LINE_SPEED 25

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    enum FollowLineState
    {
        FOLLOW_LINE,
        FOLLOW_LINE_TURN_RIGHT,
        FOLLOW_LINE_TURN_LEFT,
        FOLLOW_LINE_TURN_AROUND,
        FOLLOW_LINE_DISABLE
    };

    enum FollowLineState follow_line_state = FOLLOW_LINE;

    uint16_t cross_count = 0;
    uint16_t prev_cross_turn = 0;

    void followLine()
    {
        if (follow_line_state != FOLLOW_LINE)
        {
            return;
        }

        // These variables are used to detect the cross.
        static enum LineStatus prev_line_status = LINE_CROSS;
        static enum LineStatus curr_line_status = LINE_CROSS;

        prev_line_status = curr_line_status;
        curr_line_status = getLineStatus();

        switch (curr_line_status)
        {
        case LINE_CENTER:
        {
            moveForward(FOLLOW_LINE_SPEED);
            break;
        }
        case LINE_LEFT1:
        {
            motor1Rotates(FOLLOW_LINE_SPEED * 2 / 3);
            motor2Rotates(FOLLOW_LINE_SPEED * 2 / 3);

            motor3Rotates(FOLLOW_LINE_SPEED);
            motor4Rotates(FOLLOW_LINE_SPEED);

            break;
        }
        case LINE_LEFT2:
        {
            motor1Rotates(FOLLOW_LINE_SPEED * 1 / 3);
            motor2Rotates(FOLLOW_LINE_SPEED * 1 / 3);

            motor3Rotates(FOLLOW_LINE_SPEED);
            motor4Rotates(FOLLOW_LINE_SPEED);

            break;
        }
        case LINE_LEFT3:
        {
            motor1Rotates(0);
            motor2Rotates(0);

            motor3Rotates(FOLLOW_LINE_SPEED);
            motor4Rotates(FOLLOW_LINE_SPEED);

            break;
        }
        case LINE_RIGHT1:
        {
            motor1Rotates(FOLLOW_LINE_SPEED);
            motor2Rotates(FOLLOW_LINE_SPEED);

            motor3Rotates(FOLLOW_LINE_SPEED * 2 / 3);
            motor4Rotates(FOLLOW_LINE_SPEED * 2 / 3);

            break;
        }
        case LINE_RIGHT2:
        {
            motor1Rotates(FOLLOW_LINE_SPEED);
            motor2Rotates(FOLLOW_LINE_SPEED);

            motor3Rotates(FOLLOW_LINE_SPEED * 1 / 3);
            motor4Rotates(FOLLOW_LINE_SPEED * 1 / 3);

            break;
        }
        case LINE_RIGHT3:
        {
            motor1Rotates(FOLLOW_LINE_SPEED);
            motor2Rotates(FOLLOW_LINE_SPEED);

            motor3Rotates(0);
            motor4Rotates(0);

            break;
        }
        case LINE_CROSS:
        {
            if (prev_line_status != LINE_CROSS)
            {
                cross_count++;
            }
            break;
        }
        default:
        {
            return;
        }
        }
    }

    void turnRightAtCross(uint16_t cross_num)
    {
        static uint32_t pulse_count_enc_1 = 0;
        static uint32_t pulse_count_enc_2 = 0;
        static uint32_t pulse_count_enc_3 = 0;
        static uint32_t pulse_count_enc_4 = 0;

        if (cross_num == cross_count && follow_line_state == FOLLOW_LINE && prev_cross_turn != cross_num) // Execute only once
        {
            prev_cross_turn = cross_num;

            pulse_count_enc_1 = getPulseCount(ENCODER_1);
            pulse_count_enc_2 = getPulseCount(ENCODER_2);
            pulse_count_enc_3 = getPulseCount(ENCODER_3);
            pulse_count_enc_4 = getPulseCount(ENCODER_4);

            moveForward(FOLLOW_LINE_SPEED);

            follow_line_state = FOLLOW_LINE_TURN_RIGHT;
        }

        if (follow_line_state != FOLLOW_LINE_TURN_RIGHT)
        {
            return;
        }

        if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 4)
        {
            rotateRight(FOLLOW_LINE_SPEED);

            // if (getLineStatus() == LINE_RIGHT2)
            // {
            //     follow_line_state = FOLLOW_LINE;
            // }

            if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 2)
            {
                if (getLineStatus() == LINE_CENTER)
                {
                    follow_line_state = FOLLOW_LINE;
                }
            }
        }
        else
        {
            if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 4)
            {
                motor1Rotates(0);
            }
            if (getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 4)
            {
                motor2Rotates(0);
            }
            if (getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 4)
            {
                motor3Rotates(0);
            }
            if (getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 4)
            {
                motor4Rotates(0);
            }
        }
    }

    void turnLeftAtCross(uint16_t cross_num)
    {
        static uint32_t pulse_count_enc_1 = 0;
        static uint32_t pulse_count_enc_2 = 0;
        static uint32_t pulse_count_enc_3 = 0;
        static uint32_t pulse_count_enc_4 = 0;

        if (cross_num == cross_count && follow_line_state == FOLLOW_LINE && prev_cross_turn != cross_num) // Execute only once
        {
            prev_cross_turn = cross_num;

            pulse_count_enc_1 = getPulseCount(ENCODER_1);
            pulse_count_enc_2 = getPulseCount(ENCODER_2);
            pulse_count_enc_3 = getPulseCount(ENCODER_3);
            pulse_count_enc_4 = getPulseCount(ENCODER_4);

            moveForward(FOLLOW_LINE_SPEED);

            follow_line_state = FOLLOW_LINE_TURN_LEFT;
        }

        if (follow_line_state != FOLLOW_LINE_TURN_LEFT)
        {
            return;
        }

        if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 4 &&
            getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 4)
        {
            rotateLeft(FOLLOW_LINE_SPEED);

            // if (getLineStatus() == LINE_LEFT2)
            // {
            //     follow_line_state = FOLLOW_LINE;
            // }

            if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 2 &&
                getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 2)
            {
                if (getLineStatus() == LINE_CENTER)
                {
                    follow_line_state = FOLLOW_LINE;
                }
            }
        }
        else
        {
            if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION / 4)
            {
                motor1Rotates(0);
            }
            if (getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION / 4)
            {
                motor2Rotates(0);
            }
            if (getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION / 4)
            {
                motor3Rotates(0);
            }
            if (getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION / 4)
            {
                motor4Rotates(0);
            }
        }
    }

    void turnAroundAtCross(uint16_t cross_num)
    {
        static uint32_t pulse_count_enc_1 = 0;
        static uint32_t pulse_count_enc_2 = 0;
        static uint32_t pulse_count_enc_3 = 0;
        static uint32_t pulse_count_enc_4 = 0;

        if (cross_num == cross_count && follow_line_state == FOLLOW_LINE && prev_cross_turn != cross_num) // Execute only once
        {
            prev_cross_turn = cross_num;
            
            pulse_count_enc_1 = getPulseCount(ENCODER_1);
            pulse_count_enc_2 = getPulseCount(ENCODER_2);
            pulse_count_enc_3 = getPulseCount(ENCODER_3);
            pulse_count_enc_4 = getPulseCount(ENCODER_4);

            rotateRight(FOLLOW_LINE_SPEED);

            follow_line_state = FOLLOW_LINE_TURN_AROUND;
        }

        if (follow_line_state != FOLLOW_LINE_TURN_AROUND)
        {
            return;
        }

        if (getPulseCount(ENCODER_1) - pulse_count_enc_1 >= PULSE_PER_REVOLUTION &&
            getPulseCount(ENCODER_2) - pulse_count_enc_2 >= PULSE_PER_REVOLUTION &&
            getPulseCount(ENCODER_3) - pulse_count_enc_3 >= PULSE_PER_REVOLUTION &&
            getPulseCount(ENCODER_4) - pulse_count_enc_4 >= PULSE_PER_REVOLUTION)
        {
            if (getLineStatus() == LINE_CENTER)
            {
                follow_line_state = FOLLOW_LINE;
            }
        }
    }

    void disableFollowLine()
    {
        follow_line_state = FOLLOW_LINE_DISABLE;
    }
    void enableFollowLine()
    {
        follow_line_state = FOLLOW_LINE;
    }

    // void turnLeftAtCross(uint16_t cross_num)
    // {
    //     if (cross_count == cross_num)
    //     {
    //         static uint16_t prev_cross_num_turn = 0;
    //         static uint32_t pulse_count_temp = 0;

    //         if (prev_cross_num_turn != cross_num) // Execute only once
    //         {
    //             prev_cross_num_turn = cross_num;
    //             pulse_count_temp = getPulseCount(ENCODER_1);
    //             moveForward(FOLLOW_LINE_SPEED);
    //             follow_line_mode = FOLLOW_LINE_DISABLE;
    //         }

    //         if (follow_line_mode == FOLLOW_LINE_DISABLE)
    //         {
    //             if (getPulseCount(ENCODER_1) - pulse_count_temp >= PULSE_PER_REVOLUTION / 4)
    //             {
    //                 rotateLeft(FOLLOW_LINE_SPEED);

    //                 if (getLineStatus() == LINE_LEFT2)
    //                 {
    //                     follow_line_mode = FOLLOW_LINE_ENABLE;
    //                 }
    //             }
    //         }
    //     }
    // }

    // void turnLeftAtCross(uint16_t cross_num)
    // {
    //     if (cross_count == cross_num)
    //     {
    //         static uint16_t prev_cross_num_turn = 0;
    //         static uint32_t pulse_count_temp = 0;

    //         if (prev_cross_num_turn != cross_num) // Execute only once
    //         {
    //             prev_cross_num_turn = cross_num;
    //             pulse_count_temp = getPulseCount(ENCODER_1);
    //             moveForward(FOLLOW_LINE_SPEED);
    //             follow_line_mode = FOLLOW_LINE_DISABLE;
    //         }

    //         if (follow_line_mode == FOLLOW_LINE_DISABLE)
    //         {
    //             if (getPulseCount(ENCODER_1) - pulse_count_temp >= PULSE_PER_REVOLUTION / 4)
    //             {
    //                 rotateLeft(FOLLOW_LINE_SPEED);

    //                 if (getLineStatus() == LINE_LEFT2)
    //                 {
    //                     follow_line_mode = FOLLOW_LINE_ENABLE;
    //                 }
    //             }
    //         }
    //     }
    // }

    uint8_t getCrossCount()
    {
        return cross_count;
    }

    //     void moveFrontLeftLine()
    //     {
    //         motor1Rotates(5);
    //         motor2Rotates(10);
    //         motor3Rotates(10);
    //         motor4Rotates(5);
    //     }

    //     void moveFrontRightLine()
    //     {
    //         motor1Rotates(10);
    //         motor2Rotates(5);
    //         motor3Rotates(5);
    //         motor4Rotates(10);
    //     }

    //     void followLineForward()
    //     {
    //         enum Line_Status status_forward;
    //         status_forward = SensorGetLineStatus();

    //         switch (status_forward)
    //         {
    //         case LINE_END:
    //             motorStop();
    //             break;
    //         case LINE_CROSS:
    //             moveForward(12);
    //             break;
    //         case LINE_CENTER:
    //             moveForward(12);
    //             break;
    //         case LINE_LEFT1:
    //             //				moveFrontLeftLine();
    //             rotateRight(15);
    //             break;
    //         case LINE_LEFT2:
    //             //				moveFrontLeftLine();
    //             rotateRight(15);
    //             break;
    //         case LINE_LEFT3:
    //             //				moveFrontLeftLine();
    //             rotateRight(15);
    //             break;
    //         case LINE_RIGHT1:
    //             //				moveFrontRightLine();
    //             rotateLeft(15);
    //             break;
    //         case LINE_RIGHT2:
    //             //				moveFrontRightLine();
    //             rotateLeft(15);
    //             break;
    //         case LINE_RIGHT3:
    //             //				moveFrontRightLine();
    //             rotateLeft(15);
    //             break;
    //         default:
    //             motorStop();
    //             break;
    //         }
    //     }

    //     void followLineBackward()
    //     {
    //         enum Line_Status status_backward;
    //         status_backward = SensorGetLineStatus();

    //         switch (status_backward)
    //         {
    //         case LINE_END:
    //             motorStop();
    //             break;
    //         case LINE_CROSS:
    //             moveBackward(12);
    //             break;
    //         case LINE_CENTER:
    //             moveBackward(12);
    //             break;
    //         case LINE_LEFT1:
    //             //				moveFrontLeftLine();
    //             rotateLeft(15);
    //             break;
    //         case LINE_LEFT2:
    //             //				moveFrontLeftLine();
    //             rotateLeft(15);
    //             break;
    //         case LINE_LEFT3:
    //             //				moveFrontLeftLine();
    //             rotateLeft(15);
    //             break;
    //         case LINE_RIGHT1:
    //             //				moveFrontRightLine();
    //             rotateRight(15);
    //             break;
    //         case LINE_RIGHT2:
    //             //				moveFrontRightLine();
    //             rotateRight(15);
    //             break;
    //         case LINE_RIGHT3:
    //             //				moveFrontRightLine();
    //             rotateRight(15);
    //             break;
    //         default:
    //             motorStop();
    //             break;
    //         }
    //     }

    //     uint8_t followLineForwardUntilCross(void)
    //     {
    //         enum Line_Status status_forward;
    //         status_forward = SensorGetLineStatus();

    //         while (status_forward == LINE_CROSS)
    //         {
    //             status_forward = SensorGetLineStatus();
    //             moveForward(32);
    //             HAL_Delay(10);
    //         }

    //         motorStop();
    //         uint8_t state = 0;

    //         while (state != 1)
    //         {
    //             status_forward = SensorGetLineStatus();
    //             switch (status_forward)
    //             {
    //             case LINE_END:
    //                 motorStop();
    //                 break;
    //             case LINE_CROSS:
    //                 motorStop();
    //                 state = 1;
    //                 return 1;
    //                 break;
    //             case LINE_CENTER:
    //                 moveForward(32);
    //                 break;
    //             case LINE_LEFT1:
    //                 rotateRight(35);
    //                 break;
    //             case LINE_LEFT2:
    //                 rotateRight(35);
    //                 break;
    //             case LINE_LEFT3:
    //                 rotateRight(35);
    //                 break;
    //             case LINE_RIGHT1:
    //                 rotateLeft(35);
    //                 break;
    //             case LINE_RIGHT2:
    //                 rotateLeft(35);
    //                 break;
    //             case LINE_RIGHT3:
    //                 rotateLeft(35);
    //                 break;
    //             default:
    //                 followLineWithWeightError();
    //                 break;
    //             }
    //             HAL_Delay(10);
    //         }
    //     }

    //     void goToTheNthCross(uint8_t n)
    //     {
    //         uint8_t cross_nth = 0;
    //         while (cross_nth != n)
    //         {
    //             cross_nth += followLineForwardUntilCross();
    //         }
    //     }

    //     void goLeftUntilMeetLine(void)
    //     {
    //         enum Line_Status status;
    //         status = SensorGetLineStatus();
    //         while (status != LINE_LEFT1 && status != LINE_LEFT2 && status != LINE_CENTER && status != LINE_RIGHT1 && status != LINE_RIGHT2)
    //         {
    //             status = SensorGetLineStatus();
    //             moveLeft(45);
    //             HAL_Delay(10);
    //         }
    //         motorStop();
    //     }

    //     void goRightUntilMeetLine(void)
    //     {
    //         enum Line_Status status;
    //         status = SensorGetLineStatus();
    //         while (status != LINE_LEFT1 && status != LINE_LEFT2 && status != LINE_CENTER && status != LINE_RIGHT1 && status != LINE_RIGHT2)
    //         {
    //             status = SensorGetLineStatus();
    //             moveRight(45);
    //             HAL_Delay(10);
    //         }
    //         motorStop();
    //     }

    //     void goBackwardUntilMeetLine(void)
    //     {
    //         enum Line_Status status;
    //         status = SensorGetLineStatus();
    //         while (status == LINE_CROSS)
    //         {
    //             status = SensorGetLineStatus();
    //             moveBackward(35);
    //             HAL_Delay(10);
    //         }
    //         while (status != LINE_CROSS)
    //         {
    //             status = SensorGetLineStatus();
    //             moveBackward(35);
    //             HAL_Delay(10);
    //         }
    //         motorStop();
    //     }

    //     void followLineWithWeightError()
    //     {
    //         float weight_error = getWeightError();
    //         if (weight_error < 0)
    //         {
    //             rotateLeft(35);
    //         }
    //         else if (weight_error > 0)
    //         {
    //             rotateRight(35);
    //         }
    //         else
    //         {
    //             moveForward(32);
    //         }
    //     }

#ifdef __cplusplus
}
#endif /* __cplusplus */