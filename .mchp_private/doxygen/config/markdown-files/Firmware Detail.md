
# Firmware Overview

The firmware overview indicates that the power controller state machine and fault handler are executed every 100 microseconds by the scheduler. Additionally, the GUI handler operates every 10 milliseconds, while the LED and fan operations, along with temperature checks, are performed every 100 milliseconds.

The system has a single interrupt source, the ControlLoop_Interrupt_CallBack(), which is executed every 10 microseconds. This callback measures voltages and currents on the DAB board using an ADC and feeds the data to three control loops. The Voltage and Power loops operate at 10 kHz, interleaved, while the Current Loop operates at 100 kHz.<p>
  <center>
    <img src="images/tasks-diagram.png" alt="firmware-0" width="800">
    <br>
    Firmware overview.
  </center>
</p>

The Microchip Code Configurator (MCC) is utilized to set up peripherals. This configuration occurs at run-time at the beginning of the main() function, prior to the initiation of the background loop.

The project files are organized as follows:

- CAN Communication:

    - app_PBV_CAN.c: Manages CAN communication objects and functions.
    - app_PBV_UART.c: Manages UART communication objects and functions.
    - app_PBV_config.c: Configures functions for CAN and UART communication for the Power Board Visualizer (PBV).
    - app_PBV_dab_frame_map.c: User configuration for data transmission and reception with PBV.
    - app_PBV_interface.c: Contains generic communication functions for PBV.

- Device Management:
    - dev_current_sensor.c: Measures current sensor offset.
    - dev_fan.c: Initializes and controls fan speed.
    - dev_led.c: Provides LED functions to indicate the power converter's state.
    - dev_temp.c: Indicates and calculates the board temperature.

- Fault Handling:
    - dev_fault.c: Manages fault initialization, execution, and handling.
    - dev_fault_common.c: Contains generic fault handling functions.

- Power Control:
    - dev_pwrctrl.c: Manages power control initialization, control loop, and execution.
    - dev_pwrctrl_isr.c: Handles control loop interrupt callbacks, ADC data acquisition, and PWM distribution.
    - dev_pwrctrl_pwm.c: Calculates DAB control phase and manages PWM distribution.
    - dev_pwrctrl_sm.c: Executes the power control state machine every 100 microseconds.
    - dev_pwrctrl_utils.c: Contains functions for power control ramp up/down and averaging.
<br>
<p>
  <center>
    <img src="images/dab-block-diagram.jpg" alt="firmware-0" width="850">
    <br>
    Firmware block diagram.
  </center>
</p>

The firmware block diagram illustrates the use of the dspic33C for a Dual Active Bridge converter. The following provides a detailed explanation of the firmware development for this application.

---

## Converter State Machine

The main power controller state machine, executed every 100 microseconds, is detailed in the function Dev_PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance) located in pwrctrl/dev_pwrctrl_sm.c. The state machine progresses through a series of steps in a specific chronological order during its execution.

### Operating States
The state machine for the power control system (PCS) operates as follows:

- PCS_INIT_handler(): Resets conditional flag bits, disables PWM output, and runs initial current calibration offset. Then, it checks the fault handler.
- PCS_WAIT_IF_FAULT_ACTIVE_handler(): Checks for fault events. If none are detected, it transitions to the StandBy state.
- PCS_STANDBY_handler(): Waits for no fault events and the power control enable bit to be set. Once set, it resets fault status bits, PWM control settings, enables the power control running bit and PWM output, initializes control loop references, and moves to the soft start state.
- PCS_SOFT_START_handler(): Gradually adjusts power control references until the desired control reference is reached, then transitions to the online state.
- PCS_UP_AND_RUNNING_handler(): Enters constant regulation mode after successful startup, maintaining this mode until shutdown or suspension. It monitors and adjusts the control reference as needed using a defined transition ramp.

<p>
  <center>
    <img src="images/dab-state-machine.jpg" alt="state-machine" width="450">
    <br>
    Power supply state machine.
  </center>
</p>

<span id="soft-starting-the-converter"><a name="soft-starting-the-converter"> </a></span>

## Power Control Data Structure

Before utilizing the state machine, it is essential to define and initialize at least one Power control data object within the power control code. The following outlines the data structure of POWER_CONTROL_t and its usage.
```c
struct POWER_CONTROL_s
{
    STATUS_FLAGS_t          Status; ///< Power Supply status flags
    PWR_CTRL_STATE_t        State;  ///< Power Control State ID
    PWR_CTRL_PROPERTIES_t   Properties; ///< Power Control properties    
    SWITCH_NODE_t           Pwm;    ///< Switch node settings
    FEEDBACK_SETTINGS_t     Data;   ///< Feedback channel settings
    FAULT_SETTINGS_t        Fault;  ///< Fault flags and settings 
    START_UP_RAMP_t         VRamp;  ///< Voltage ramp-up settings
    START_UP_RAMP_t         IRamp;  ///< Current ramp-up settings
    START_UP_RAMP_t         PRamp;  ///< Power ramp-up settings
    CONTROLLER_t            ILoop;  ///< structure for current controller data
    CONTROLLER_t            VLoop;  ///< structure for voltage controller data
    CONTROLLER_t            PLoop;  ///< structure for power controller data
    PWR_CTRL_CHARGE_STATE_t    PowerDirection;  ///< defines if the power converter is in charging or discharging mode   
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;
```

##### Declaration Examples

```c
POWER_CONTROL_t dab;    // Declare DAB converter data structure
```

---

## Fault Protection

The fault protection code, executed every 10 microseconds within the interrupt service routine in the function Dev_Fault_Execute(), is located in the file device/fault/dev_fault.c. There are two types of protection: firmware fault protection, implemented on the dsPIC on the DP-PIM, and hardware fault protection, implemented on the DAB power board. The primary purpose of these protections is to prevent catastrophic board damage, especially from input and output overcurrent events.

---

### Firmware Fault Protection

All our firmware fault protection systems operate with the same functionality, incorporating a trigger threshold, a clear threshold, a fault blanking time, and a fault clear time.

For instance, in the case of a fault with a "max" threshold (e.g., output over-voltage protection), the fault is triggered when the source exceeds the threshold. A timer starts once this threshold is breached. If the source remains above the threshold beyond the fault blanking time, the fault becomes active, causing the PWMs to switch off and the converter to enter the "FAULT ACTIVE" state. If the source drops below the threshold before the blanking time expires, the timer resets.

When the fault is active, it will be cleared if the source stays below the clear threshold for the fault clear time. Once all faults are cleared, the converter will either restart or be ready to start again.
<p>
  <center>
    <img src="images/fault.png" alt="fault-protection" width="800">
    <br>
    Firmware Fault protection.
  </center>
</p>

The accompanying flowchart illustrates the process in detail. If "fault active" is true, the fault is active, and the converter is disabled. Conversely, if "fault active" is false, the converter is permitted to attempt startup.
<p>
  <center>
    <img src="images/fault-diagram.png" alt="fault-protection" width="750">
    <br>
    Flowchart illustrating the firmware fault protection.
  </center>
</p>

The table below lists all faults that are protected by our firmware, which executes fault protection every 10 microseconds.
<p>
  <center>
    <img src="images/dab-fault-threshold.jpg" alt="fault-protection" width="700">
    <br>
    DAB faults with firmware protection.
  </center>
</p>

---

### Hardware fault protection

The hardware fault protection system is designed to prevent severe board damage, especially from input or output overcurrent. It activates instantly without any delay, setting all PWM drive signals to zero and shutting down the converter. This system operates independently of the dsPIC, ensuring that any drive signals from the dsPIC are overridden by the hardware protection before reaching the FET drivers.
<p>
  <center>
    <img src="images/" alt="fault-protection" width="400">
    <br>
    DAB faults with hardware protection.
  </center>
</p>

If the hardware fault protection is triggered, the red LED will turn on. The protection is latched, meaning that once triggered it will not clear itself, it needs to be manually cleared.

If you want to re-run the board, you need to

- disable all PWMs first, either by holding down the RESET push button, or erasing the dsPIC firmware (we recommend the second option as it is safer)
- then short press the "RESET protection" push button on the HMI interface.

On the dsPIC, output over current protection using comparators and DACs is also implemented as follows:

- Current transformer in high voltage Bridge sense tied to CMP1DAC 
- Current transformer in low voltage Bridge sense tied to CMP3DAC 

Either of these comparators tripping will trigger the highest priority interrupt, which disables all PWM drive signals and puts the converter in the "FAULT ACTIVE" state.
Like the hardware fault protection, this fault protection is also latched, meaning that the dsPIC needs to be reset to restart the converter. If this fault protection is triggered, the RESET flag in the Power Board Visualizer GUI will be set, as shown below, indicating that the dsPIC needs to be reset to re-start the DAB converter.

<p>
  <center>
    <img src="images/" alt="fault-protection" width="250">
    <br>
    DAB faults with hardware protection.
  </center>
</p>

---

## PWM Setup

The PWM setup for the DAB application is primarily configured using MCC-generated initialization functions in the main function, with additional custom configurations applied as needed. The application employs a dual phase shift (DPS) modulation scheme, which adjusts the phases in both primary and secondary bridges. This approach offers advantages over traditional phase shift (TPS) control by reducing current stress and enhancing system efficiency.
 
<p>
  <center>
    <img src="images/digital-pwm-signal.jpg" alt="PWM signal" width="800"></p>
    DAB Dual phase shift control
  </center>
</p>

- - -

### PWM Routing

In this application, the primary bridge of the DAB is driven by PWM1 (for P1 and P2) and PWM3 (for P3 and P4), while the secondary bridge is driven by PWM2 (for S1 and S2) and PWM4 (for S3 and S4). Each PWM operates in complementary mode, with PWM3 and PWM4 having swapped outputs.
<p>
  <center>
<img src="images/dab-schematic.jpg" width="800"></p>
    Simplified DAB schematic
  </center>
</p>

<p>
  <center>
<img src="images/pwm-soc.jpg" width="800"></p>
    Cascaded PWM modules 
  </center>
</p>

In a cascaded PWM configuration, the first PWM triggers subsequent PWMs in sequence, ensuring synchronized updates across all PWMs within the same cycle. In the DAB application, PWM1 acts as the master, while PWM2, PWM3, and PWM4 are secondary PWMs that follow PWM1. The configuration ensures that the start of each PWM cycle is dependent on the preceding PWM (e.g., PWM2 starts after PWM1, PWM3 after PWM2, and PWM4 after PWM3). PWM4, being the last in the sequence, broadcasts the update signal to all PWMs, ensuring synchronized data updates.

<p>
  <center>
    <img src="images/pwm-configuration.jpg" alt="PWM config MCC" width="1200">
    <br>
    PWM configuration in MCC Melody.
  </center>
</p>

---
## Compensator Settings

mention DCDT configuration
relevant files for dcdt


---

&copy; 2024, Microchip Technology Inc.

---

