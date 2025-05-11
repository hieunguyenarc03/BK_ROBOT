# BK_ROBOT

This repository provides a programming support library for mini robots, which are used in the BACH KHOA ROBOT CONTEST.

## Getting Started
To clone this repository, run the following command in your terminal:
```sh
git clone https://github.comhieunguyen3092003/BK_ROBOT.git
```
After cloning, open STM32CubeIDE and import the project files located in `./BK_ROBOT/sdk/bk_robot_sdk` to start working with the SDK.

## Project Structure

The main folders in this repository are organized as follows:

```
BK_ROBOT/
├── sdk/                # Programming support library (SDK) for mini robots
│   └── bk_robot_sdk/   # STM32CubeIDE project files for the SDK
├── docs/               # Documentation (data frames, ICs datasheet,...)
├── schematic/          # Schematic files
├── CHANGELOG.md        # Project changelog
└── README.md           # Project introduction
```

## Sample program instructions
### Setup Robot
**Before using the sample firmware, make sure your robot is connected to either [Ohstem's Bluetooth Module](https://ohstem.vn/product/module-nhan-tin-hieu-gamepad/) or [MakerLab's MKE-M15 Bluetooth Module](https://hshop.vn/mach-thu-phat-mke-m15-bluetooth-3-0-spp-ble-4-2-dual-mode-module).**

### Selecting Robot Modes
To use different modes on the robot, you need to set the mode number using the 4 DIP switches on the control board:

1. **Identify the 4 DIP switches** on your robot's control board. Each switch represents a bit in the mode number (from 0 to 15).
2. **Set the switches** to represent the desired mode in binary.  
   - Switch 1 = Most Significant Bit (MSB)
   - Switch 4 = Least Significant Bit (LSB)
3. **Example:**  
   - To select `MODE_TEST_COMM` (mode number 2), set the switches to `0010` (Switch 3 ON, others OFF).
   - To select `MODE_TEST_MOTORS` (mode number 3), set the switches to `0011` (Switch 3 and 4 ON, others OFF).

### Available Robot Modes

The following modes are available for selection:

- `MODE_IDLE`
- `MODE_TEST_DEBUG_INDICATORS`
- `MODE_TEST_COMM`
- `MODE_TEST_MOTORS`
- `MODE_TEST_ENCODERS`
- `MODE_TEST_PID`
- `MODE_TEST_SERVO_MOTORS`
- `MODE_TEST_STEP_MOTORS`
- `MODE_TEST_SENSORS`
- `MODE_TEST_GAMEPAD`
- `MODE_TEST_MODULE_MKE`
- `MODE_TEST_FULL`
- `MODE_CONTEST_AUTONOMOUS_1`
- `MODE_CONTEST_AUTONOMOUS_2`
- `MODE_CONTEST_CONTROLLED_1`
- `MODE_CONTEST_CONTROLLED_2`

### Robot Mode Descriptions

---

#### `MODE_IDLE`  
The robot remains idle. Only the debug LED toggles periodically to indicate the system is running.

---

#### `MODE_TEST_DEBUG_INDICATORS`  
Use the buttons to test LEDs and buzzer:
- Button 1: Shift LED pattern on LEDs array left.
- Button 2: Shift LED pattern LEDs array right.
- Button 3: Set buzzer frequency corresponding to display LEDs.
- Button 4: Stop buzzer.

---

#### `MODE_TEST_COMM`  
Test UART communication:

> **Note:** 
> You should connect simultaneously the MKE-M15 Bluetooth Module to UART1, the Ohstem's Bluetooth module to the I2C port and USB ttl to UART2.

- Button 1: Read and transmit gamepad data via UART2.
- Button 2: Transmit software gamepad data via UART2.

---

#### `MODE_TEST_MOTORS`
Use buttons to test motor movement:
- Button 1: Move forward for 3s then stop.
- Button 2: Move backward for 3s then stop.
- Button 3: Move left for 3s then stop.
- Button 4: Move right for 3s and then stop.

---

#### `MODE_TEST_ENCODERS`  
Use buttons to read encoder values:
- Button 1–4: Transmit pulse count of encoder 1–4 via UART2.

---

#### `MODE_TEST_PID`  
_This feature is still unreleased._

---

#### `MODE_TEST_SERVO_MOTORS`  
Use buttons to set servo angles:
- Button 1: Set angle 0° (fully opened).
- Button 2: Set angle 14°.
- Button 3: Set angle 28°.
- Button 4: Set angle 55° (fully closed).

---

#### `MODE_TEST_STEP_MOTORS`  
Use buttons to move stepper motors:

> **Note:** 
> Before running this mode, make sure to lower the robot's gripper to its lowest position.

- Button 1: Move up (1 step).
- Button 2: Move down (1 step).
- Button 3: Move up (0 steps).
- Button 4: Move up (3 steps).

---

#### `MODE_TEST_SENSORS`  
Use buttons to test sensors:
- Button 1: Calibrate sensors.
- Button 2: Scan and display sensor state on LEDs.
- Button 3: Transmit sensor calibration values via UART2.
- Button 4: Transmit sensor values via UART2.

---

#### `MODE_TEST_GAMEPAD`  
Control the robot using a physical gamepad:

> **Note:** 
> 1. Before running this mode, make sure to lower the robot's gripper to its lowest position. 
> 2. Connect the Ohstem's Bluetooth module to the I2C port.

- D-pad: Move robot (forward, backward, left, right).
- SQUARE/CIRCLE: Rotate left/right.
- TRIANGLE: Double speed.
- CROSS: Halve speed.
- L1/R1: Move stepper up/down.
- L2/R2: Adjust robot gripper angle.

---

#### `MODE_TEST_MODULE_MKE`  
Control the robot using the software gamepad via the MKE module:

> **Note:** 
> 1. Before running this mode, make sure to lower the robot's gripper to its lowest position.
> 2. Connect the the MKE-M15 Bluetooth Module to UART1.

- D-pad: Move robot.
- START/SELECT: Double/halve speed.
- TRIANGLE/CROSS: Stepper up/down.
- CIRCLE/SQUARE: Adjust servo angle.

---

#### `MODE_TEST_FULL`  
Control the robot using the software gamepad via the MKE module:

---

#### `MODE_CONTEST_AUTONOMOUS_1`
Autonomous contest modes (user should implement autonomous routines).

---

#### `MODE_CONTEST_AUTONOMOUS_2`  
Autonomous contest modes (user should implement autonomous routines).

---

#### `MODE_CONTEST_CONTROLLED_1`  
Controlled contest modes (user should implement manual or semi-autonomous routines).

---

#### `MODE_CONTEST_CONTROLLED_2`  
Controlled contest modes (user should implement manual or semi-autonomous routines).