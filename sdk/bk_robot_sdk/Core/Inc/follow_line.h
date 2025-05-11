#ifndef INC_FOLLOW_LINE_H_
#define INC_FOLLOW_LINE_H_

#include <stdint.h>

void followLine(void);
void turnRightAtCross(uint16_t cross_num);
void turnLeftAtCross(uint16_t cross_num);
void turnAroundAtCross(uint16_t cross_num);
void disableFollowLine(void);
void enableFollowLine(void);

uint8_t getCrossCount(void);

// // for follow line
// void followLineForward(void);
// void followLineBackward(void);
// void goLeftUntilMeetLine(void);
// void goRightUntilMeetLine(void);
// uint8_t followLineForwardUntilCross(void);
// void goToTheNthCross(uint8_t n);
// void goBackwardUntilMeetLine(void);
// void followLineWithWeightError(void);
// // int followLineUntilCross();

#endif /* INC_FOLLOW_LINE_H_ */