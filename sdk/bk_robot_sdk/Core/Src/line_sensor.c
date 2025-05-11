#include "line_sensor.h"

#include "dma.h"
#include "adc.h"

#define NUMBER_OF_LINE_SENSORS 6
#define NUMBER_OF_OBSTACLE_SENSORS 2
#define NUMBER_OF_SENSORS (NUMBER_OF_LINE_SENSORS + NUMBER_OF_OBSTACLE_SENSORS)

#define SENSOR_LINE_DELTA 200
#define SENSOR_OBSTACLE_DELTA 500

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	uint16_t sensor_calib[NUMBER_OF_SENSORS] = {0};
	uint16_t sensor_buffer[NUMBER_OF_SENSORS] = {0};

	const uint8_t sensor_map[NUMBER_OF_SENSORS] = {0b00000001, 0b00001000, 0b00100000, 0b01000000, 0b00000010, 0b00000100, 0b00010000, 0b10000000}; // sensor_map is used to map the sensor index to the corresponding bit in the sensor_states variable

	uint8_t sensor_states = 0b00000000;

	uint8_t processSensorBuffer(const uint16_t *sensor_bf);

	/**
	 * @brief Sensor calibration function
	 * @note This function should be called once to calibrate the sensors
	 * @note This function should be called when the sensors are in black square
	 */
	enum StatusCode calibSensor()
	{
		for (uint8_t attempt = 0; attempt < MAX_RETRY_COUNT; ++attempt)
		{
			if (HAL_ADC_Start_DMA(&hadc1, (void *)sensor_calib, 8) == HAL_OK)
			{
				return STATUS_OK;
			}
			HAL_Delay(50);
		}
		return STATUS_ERROR;
	}

	/**
	 * @brief Scan the sensors
	 * @note This function should be called after calibSensor()
	 * @note This function should be called in a loop
	 */

	enum StatusCode scanSensor()
	{
		for (uint8_t attempt = 0; attempt < MAX_RETRY_COUNT; ++attempt)
		{
			if (HAL_ADC_Start_DMA(&hadc1, (void *)sensor_buffer, 8) == HAL_OK)
			{
				sensor_states = processSensorBuffer(sensor_buffer);

				return STATUS_OK;
			}
			HAL_Delay(50);
		}
		return STATUS_ERROR;
	}

	/**
	 * @brief Get the line status
	 * @return LineStatus
	 * @note This function should be called after SensorScan()
	 */
	enum LineStatus getLineStatus()
	{
		enum LineStatus line_status = LINE_END;

		switch (sensor_states & 0b01111110)
		{
		case 0b01111110:
		{
			line_status = LINE_CROSS;
			break;
		}
		case 0b00000000:
		{
			line_status = LINE_END;
			break;
		}
		case 0b00011000:
		{
			line_status = LINE_CENTER;
			break;
		}

		case 0b00001100:
		case 0b00011100:
		{
			line_status = LINE_RIGHT1;
			break;
		}

		case 0b00000110:
		{
			line_status = LINE_RIGHT2;
			break;
		}
		case 0b00001110:
		{
			line_status = LINE_RIGHT2;
			break;
		}

		case 0b00000010:
		{
			line_status = LINE_RIGHT3;
			break;
		}

		case 0b00110000:
		case 0b00111000:
		{
			line_status = LINE_LEFT1;
			break;
		}

		case 0b01100000:
		{
			line_status = LINE_LEFT2;
			break;
		}
		case 0b01110000:
		{
			line_status = LINE_LEFT2;
			break;
		}
		case 0b01000000:
		{
			line_status = LINE_LEFT3;
			break;
		}
		default:
		{
			line_status = LINE_ERROR;
			break;
		}
		}

		return line_status;
	}

	/**
	 * @brief Get the obstacle status
	 * @return obstacle status
	 * @note This function should be called after SensorScan()
	 */
	enum ObstacleStatus getObstacleStatus()
	{
		enum ObstacleStatus obstacle_status = OBSTACLE_NONE;

		switch (sensor_states & 0b10000001)
		{
		case 0b00000000:
		{
			obstacle_status = OBSTACLE_NONE;
			break;
		}
		case 0b10000000:
		{
			obstacle_status = OBSTACLE_LEFT;
			break;
		}
		case 0b00000001:
		{
			obstacle_status = OBSTACLE_RIGHT;
			break;
		}
		case 0b10000001:
		{
			obstacle_status = OBSTACLE_FRONT;
			break;
		}
		default:
		{
			obstacle_status = OBSTACLE_NONE;
			break;
		}
		}

		return obstacle_status;
	}

	/**
	 * @brief These function should be used for debugging line sensor
	 */
	uint8_t getSensorState()
	{
		return sensor_states;
	}
	const uint16_t *getSensorCalibValue()
	{
		return sensor_calib;
	}
	const uint16_t *getSensorValue()
	{
		return sensor_buffer;
	}

	/**
	 * @brief Private function to process the sensor buffer
	 */
	uint8_t processSensorBuffer(const uint16_t *sensor_bf)
	{
		uint8_t states = 0b00000000;

		for (int i = 0; i < NUMBER_OF_SENSORS; ++i)
		{
			if ((sensor_bf[i] < (sensor_calib[i] - SENSOR_LINE_DELTA)) || (sensor_bf[i] > (sensor_calib[i] + SENSOR_LINE_DELTA)))
			{
				states |= sensor_map[i];
			}
		}

		return states;
	}

#ifdef __cplusplus
}
#endif /* __cplusplus */
