#ifndef INC_LINE_SENSOR_H_
#define INC_LINE_SENSOR_H_

#include <stdint.h>
#include "global.h"

enum LineStatus
{
    LINE_END,
    LINE_CROSS,
    LINE_CENTER,
    LINE_RIGHT1,
    LINE_RIGHT2,
    LINE_RIGHT3,
    LINE_LEFT1,
    LINE_LEFT2,
    LINE_LEFT3,
    LINE_ERROR
};

enum ObstacleStatus
{
    OBSTACLE_NONE,
    OBSTACLE_LEFT,
    OBSTACLE_RIGHT,
    OBSTACLE_FRONT,
};

enum StatusCode calibSensor(void);
enum StatusCode scanSensor(void);

uint8_t getSensorState(void);
enum LineStatus getLineStatus(void);
enum ObstacleStatus getObstacleStatus(void);

const uint16_t *getSensorCalibValue(void);
const uint16_t *getSensorValue(void);

#endif /* INC_LINE_SENSOR_H_ */
