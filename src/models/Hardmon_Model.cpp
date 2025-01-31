//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Hardmon_Model.cpp
//
// Code generated for Simulink model 'Hardmon_Model'.
//
// Model version                  : 1.49
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Sat Mar 30 12:28:25 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <cmath>
#include <models/Hardmon_Model.hpp>
#include <stdint.h>

// Named constants for Chart: '<Root>/Chart'
const uint8_t Hard_IN_Ignition_State_Inactive{1U};

const uint8_t Hardm_IN_Invalid_State_Inactive{1U};

const uint8_t Hardmon_IN_ESTOP_State_Inactive{1U};

const uint8_t Hardmon_M_IN_MC_DISCHARGE_FAULT{6U};

const uint8_t Hardmon_Mod_IN_Normal_Operation{11U};

const uint8_t Hardmon_Mode_IN_LVSS_Powerdown1{4U};

const uint8_t Hardmon_Mode_IN_NO_ACTIVE_CHILD{0U};

const uint8_t Hardmon_Model_IN_ESTOP_Inactive{1U};

const uint8_t Hardmon_Model_IN_End_Powerdown{1U};

const uint8_t Hardmon_Model_IN_Enter_Fault{2U};

const uint8_t Hardmon_Model_IN_LVSS_Powerdown{3U};

const uint8_t Hardmon_Model_IN_MC_DISCHARGE{5U};

const uint8_t Hardmon_Model_IN_MC_FAULT_INIT{7U};

const uint8_t Hardmon_Model_IN_MC_POWERDOWN{8U};

const uint8_t Hardmon_Model_IN_MC_POWERDOWN1{9U};

const uint8_t Hardmon_Model_IN_MC_RESET_INIT{10U};

const uint8_t Hardmon_Model_IN_WD_High{1U};

const uint8_t Hardmon_Model_IN_WD_Low{2U};

const uint8_t Hardmon_Model_IN_count{2U};

namespace vcu {
// Function for Chart: '<Root>/Chart'
void Hardmon_Model::Hardmon__Invalid_State_Checking(const uint8_t* DataTypeConversion) {
    uint16_t tmp;

    // During 'Invalid_State_Checking': '<S2>:59'
    if (static_cast<uint16_t>(Hardmon_Model_DW.is_Invalid_State_Checking) == Hardm_IN_Invalid_State_Inactive) {
        // Inport: '<Root>/Forward_EN_CAN' incorporates:
        //   Inport: '<Root>/Ignition_3V3'

        // During 'Invalid_State_Inactive': '<S2>:53'
        if (((!Hardmon_Model_U.Forward_EN_CAN) || (!Hardmon_Model_U.Ignition_3V3)) && (static_cast<int16_t>(*DataTypeConversion) == 3)) {
            // Transition: '<S2>:55'
            Hardmon_Model_DW.temporalCounter_i3 = 0U;
            Hardmon_Model_DW.is_Invalid_State_Checking =
                Hardmon_Model_IN_count;

            // Entry 'count': '<S2>:58'
            tmp = static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Invalid_State_Counter) + 1U);
            if (static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Invalid_State_Counter)
                                      + 1U)
                > 255U) {
                tmp = 255U;
            }

            Hardmon_Model_DW.Invalid_State_Counter = static_cast<uint8_t>(tmp);
        }

        // End of Inport: '<Root>/Forward_EN_CAN'

        // During 'count': '<S2>:58'
    } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i3) >= 3U) {
        // Transition: '<S2>:57'
        Hardmon_Model_DW.temporalCounter_i3 = 0U;
        Hardmon_Model_DW.is_Invalid_State_Checking = Hardmon_Model_IN_count;

        // Entry 'count': '<S2>:58'
        tmp = static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Invalid_State_Counter) + 1U);
        if (static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Invalid_State_Counter) + 1U) > 255U) {
            tmp = 255U;
        }

        Hardmon_Model_DW.Invalid_State_Counter = static_cast<uint8_t>(tmp);
    } else if ((Hardmon_Model_U.Forward_EN_CAN && Hardmon_Model_U.Ignition_3V3) || (static_cast<int16_t>(*DataTypeConversion) != 3)) {
        // Transition: '<S2>:56'
        Hardmon_Model_DW.is_Invalid_State_Checking =
            Hardm_IN_Invalid_State_Inactive;

        // Entry 'Invalid_State_Inactive': '<S2>:53'
        Hardmon_Model_B.HM_Fault = false;
        Hardmon_Model_DW.Invalid_State_Counter = 0U;
        Hardmon_Model_B.nMC_Switch_EN = false;
        Hardmon_Model_B.nLVSS_Switch_EN = false;

        // Inport: '<Root>/LVSS_EN_uC'
        Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
    } else {
        // no actions
    }
}

// Function for Chart: '<Root>/Chart'
void Hardmon_Model::exit_internal_Normal_Operation(void) {
    // Exit Internal 'Normal_Operation': '<S2>:29'
    // Exit Internal 'LVSS_Checking': '<S2>:85'
    Hardmon_Model_DW.is_LVSS_Checking = Hardmon_Mode_IN_NO_ACTIVE_CHILD;

    // Exit Internal 'ESTOP_Level_Checking': '<S2>:76'
    Hardmon_Model_DW.is_ESTOP_Level_Checking =
        Hardmon_Mode_IN_NO_ACTIVE_CHILD;

    // Exit Internal 'Ignition_Level_Checking': '<S2>:68'
    Hardmon_Model_DW.is_Ignition_Level_Checking =
        Hardmon_Mode_IN_NO_ACTIVE_CHILD;

    // Exit Internal 'Invalid_State_Checking': '<S2>:59'
    Hardmon_Model_DW.is_Invalid_State_Checking =
        Hardmon_Mode_IN_NO_ACTIVE_CHILD;

    // Exit Internal 'Watchdog_Checking': '<S2>:36'
    // Exit Internal 'WD_High': '<S2>:42'
    // Exit Internal 'WD_Low': '<S2>:41'
    Hardmon_Model_DW.is_Watchdog_Checking = Hardmon_Mode_IN_NO_ACTIVE_CHILD;

    // Exit Internal 'ESTOP_Checking': '<S2>:28'
    Hardmon_Model_DW.is_ESTOP_Checking = Hardmon_Mode_IN_NO_ACTIVE_CHILD;
}

// Function for Chart: '<Root>/Chart'
void Hardmon_Model::Hardmon_Model_Normal_Operation(const uint8_t* DataTypeConversion) {
    uint16_t tmp;

    // During 'Normal_Operation': '<S2>:29'
    if ((static_cast<int16_t>(Hardmon_Model_DW.WD_Counter) >= 4) && (static_cast<int16_t>(*DataTypeConversion) != 8)) {
        // Transition: '<S2>:8'
        exit_internal_Normal_Operation();
        Hardmon_Model_DW.is_c3_Hardmon_Model =
            Hardmon_Model_IN_MC_RESET_INIT;

        // Entry 'MC_RESET_INIT': '<S2>:125'
        Hardmon_Model_B.nMC_Switch_EN = true;
        Hardmon_Model_B.nLVSS_Switch_EN = true;
    } else if ((static_cast<int16_t>(Hardmon_Model_DW.Invalid_State_Counter)
                >= 4)
               || (static_cast<int16_t>(Hardmon_Model_DW.Ignition_State_Counter) >= 4) || (static_cast<int16_t>(Hardmon_Model_DW.LVSS_Counter) >= 4) || (static_cast<int16_t>(Hardmon_Model_DW.ESTOP_Counter) >= 4) || (static_cast<int16_t>(Hardmon_Model_DW.ESTOP_State_Counter) >= 4)) {
        // Transition: '<S2>:118'
        exit_internal_Normal_Operation();
        Hardmon_Model_DW.is_c3_Hardmon_Model =
            Hardmon_Model_IN_MC_FAULT_INIT;

        // Entry 'MC_FAULT_INIT': '<S2>:119'
        Hardmon_Model_B.nMC_Switch_EN = true;
        Hardmon_Model_B.nLVSS_Switch_EN = true;
    } else {
        // During 'ESTOP_Checking': '<S2>:28'
        if (static_cast<uint16_t>(Hardmon_Model_DW.is_ESTOP_Checking) == Hardmon_Model_IN_ESTOP_Inactive) {
            // Inport: '<Root>/ESTOP_3V3'
            // During 'ESTOP_Inactive': '<S2>:1'
            if (Hardmon_Model_U.ESTOP_3V3 && (static_cast<int16_t>(*DataTypeConversion) != 7) && (static_cast<int16_t>(*DataTypeConversion) != 6)) {
                // Transition: '<S2>:5'
                Hardmon_Model_DW.temporalCounter_i1 = 0U;
                Hardmon_Model_DW.is_ESTOP_Checking = Hardmon_Model_IN_count;

                // Entry 'count': '<S2>:6'
                Hardmon_Model_DW.ESTOP_Counter = static_cast<uint8_t>(
                    static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.ESTOP_Counter) + 1U));
            }

            // During 'count': '<S2>:6'
        } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i1)
                   >= 1U) {
            // Transition: '<S2>:7'
            Hardmon_Model_DW.temporalCounter_i1 = 0U;
            Hardmon_Model_DW.is_ESTOP_Checking = Hardmon_Model_IN_count;

            // Entry 'count': '<S2>:6'
            Hardmon_Model_DW.ESTOP_Counter = static_cast<uint8_t>(
                static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.ESTOP_Counter) + 1U));
        } else if (Hardmon_Model_U.ESTOP_3V3 || (static_cast<int16_t>(*DataTypeConversion) == 7) || (static_cast<int16_t>(*DataTypeConversion) == 6)) {
            // Transition: '<S2>:14'
            Hardmon_Model_DW.is_ESTOP_Checking =
                Hardmon_Model_IN_ESTOP_Inactive;

            // Entry 'ESTOP_Inactive': '<S2>:1'
            Hardmon_Model_B.HM_Fault = false;
            Hardmon_Model_DW.ESTOP_Counter = 0U;
            Hardmon_Model_B.nMC_Switch_EN = false;
            Hardmon_Model_B.nLVSS_Switch_EN = false;

            // Inport: '<Root>/LVSS_EN_uC'
            Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
        } else {
            // no actions
        }

        // During 'Watchdog_Checking': '<S2>:36'
        if (static_cast<uint16_t>(Hardmon_Model_DW.is_Watchdog_Checking) == Hardmon_Model_IN_WD_High) {
            // Inport: '<Root>/Watchdog'
            // During 'WD_High': '<S2>:42'
            // During 'Counting': '<S2>:49'
            if (!Hardmon_Model_U.Watchdog) {
                // Transition: '<S2>:46'
                Hardmon_Model_DW.is_Watchdog_Checking =
                    Hardmon_Model_IN_WD_Low;

                // Entry 'WD_Low': '<S2>:41'
                // Entry Internal 'WD_Low': '<S2>:41'
                Hardmon_Model_DW.temporalCounter_i2 = 0U;

                // Entry 'Counting': '<S2>:48'
                Hardmon_Model_DW.WD_Counter = 1U;
            } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i2) >= 3U) {
                // Transition: '<S2>:50'
                Hardmon_Model_DW.temporalCounter_i2 = 0U;

                // Entry 'Counting': '<S2>:49'
                tmp = static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.WD_Counter) + 1U);
                if (static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.WD_Counter) + 1U)
                    > 255U) {
                    tmp = 255U;
                }

                Hardmon_Model_DW.WD_Counter = static_cast<uint8_t>(tmp);
            } else {
                // no actions
            }

            // Inport: '<Root>/Watchdog'
            // During 'WD_Low': '<S2>:41'
            // During 'Counting': '<S2>:48'
        } else if (Hardmon_Model_U.Watchdog) {
            // Transition: '<S2>:43'
            Hardmon_Model_DW.is_Watchdog_Checking = Hardmon_Model_IN_WD_High;

            // Entry 'WD_High': '<S2>:42'
            // Entry Internal 'WD_High': '<S2>:42'
            Hardmon_Model_DW.temporalCounter_i2 = 0U;

            // Entry 'Counting': '<S2>:49'
            Hardmon_Model_DW.WD_Counter = 1U;
        } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i2)
                   >= 3U) {
            // Transition: '<S2>:40'
            Hardmon_Model_DW.temporalCounter_i2 = 0U;

            // Entry 'Counting': '<S2>:48'
            tmp = static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.WD_Counter) + 1U);
            if (static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.WD_Counter) + 1U) > 255U) {
                tmp = 255U;
            }

            Hardmon_Model_DW.WD_Counter = static_cast<uint8_t>(tmp);
        } else {
            // no actions
        }

        Hardmon__Invalid_State_Checking(DataTypeConversion);

        // During 'Ignition_Level_Checking': '<S2>:68'
        if (static_cast<uint16_t>(Hardmon_Model_DW.is_Ignition_Level_Checking) == Hard_IN_Ignition_State_Inactive) {
            // Inport: '<Root>/Ignition_3V3' incorporates:
            //   Inport: '<Root>/Ignition_12V0'

            // During 'Ignition_State_Inactive': '<S2>:65'
            if ((Hardmon_Model_U.Ignition_3V3 != Hardmon_Model_U.Ignition_12V0) && (static_cast<int16_t>(*DataTypeConversion) != 6)) {
                // Transition: '<S2>:62'
                Hardmon_Model_DW.temporalCounter_i4 = 0U;
                Hardmon_Model_DW.is_Ignition_Level_Checking =
                    Hardmon_Model_IN_count;

                // Entry 'count': '<S2>:66'
                Hardmon_Model_DW.Ignition_State_Counter =
                    static_cast<uint8_t>(static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Ignition_State_Counter) + 1U));
            }

            // End of Inport: '<Root>/Ignition_3V3'

            // During 'count': '<S2>:66'
        } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i4)
                   >= 3U) {
            // Transition: '<S2>:64'
            Hardmon_Model_DW.temporalCounter_i4 = 0U;
            Hardmon_Model_DW.is_Ignition_Level_Checking =
                Hardmon_Model_IN_count;

            // Entry 'count': '<S2>:66'
            Hardmon_Model_DW.Ignition_State_Counter = static_cast<uint8_t>(
                static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.Ignition_State_Counter) + 1U));
        } else if ((Hardmon_Model_U.Ignition_3V3 == Hardmon_Model_U.Ignition_12V0) || (static_cast<int16_t>(*DataTypeConversion) == 6)) {
            // Transition: '<S2>:63'
            Hardmon_Model_DW.is_Ignition_Level_Checking =
                Hard_IN_Ignition_State_Inactive;

            // Entry 'Ignition_State_Inactive': '<S2>:65'
            Hardmon_Model_B.HM_Fault = false;
            Hardmon_Model_DW.Ignition_State_Counter = 0U;
            Hardmon_Model_B.nMC_Switch_EN = false;
            Hardmon_Model_B.nLVSS_Switch_EN = false;

            // Inport: '<Root>/LVSS_EN_uC'
            Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
        } else {
            // no actions
        }

        // During 'ESTOP_Level_Checking': '<S2>:76'
        if (static_cast<uint16_t>(Hardmon_Model_DW.is_ESTOP_Level_Checking) == Hardmon_IN_ESTOP_State_Inactive) {
            // Inport: '<Root>/ESTOP_3V3' incorporates:
            //   Inport: '<Root>/ESTOP_12V0'

            // During 'ESTOP_State_Inactive': '<S2>:73'
            if ((Hardmon_Model_U.ESTOP_3V3 != Hardmon_Model_U.ESTOP_12V0) && (static_cast<int16_t>(*DataTypeConversion) != 6)) {
                // Transition: '<S2>:70'
                Hardmon_Model_DW.temporalCounter_i5 = 0U;
                Hardmon_Model_DW.is_ESTOP_Level_Checking =
                    Hardmon_Model_IN_count;

                // Entry 'count': '<S2>:75'
                Hardmon_Model_DW.ESTOP_State_Counter = static_cast<uint8_t>(
                    static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.ESTOP_State_Counter) + 1U));
            }

            // During 'count': '<S2>:75'
        } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i5)
                   >= 3U) {
            // Transition: '<S2>:72'
            Hardmon_Model_DW.temporalCounter_i5 = 0U;
            Hardmon_Model_DW.is_ESTOP_Level_Checking =
                Hardmon_Model_IN_count;

            // Entry 'count': '<S2>:75'
            Hardmon_Model_DW.ESTOP_State_Counter = static_cast<uint8_t>(
                static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.ESTOP_State_Counter) + 1U));
        } else if ((Hardmon_Model_U.ESTOP_3V3 == Hardmon_Model_U.ESTOP_12V0)
                   || (static_cast<int16_t>(*DataTypeConversion) == 6)) {
            // Transition: '<S2>:71'
            Hardmon_Model_DW.is_ESTOP_Level_Checking =
                Hardmon_IN_ESTOP_State_Inactive;

            // Entry 'ESTOP_State_Inactive': '<S2>:73'
            Hardmon_Model_B.HM_Fault = false;
            Hardmon_Model_DW.ESTOP_State_Counter = 0U;
            Hardmon_Model_B.nMC_Switch_EN = false;
            Hardmon_Model_B.nLVSS_Switch_EN = false;

            // Inport: '<Root>/LVSS_EN_uC'
            Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
        } else {
            // no actions
        }

        // During 'LVSS_Checking': '<S2>:85'
        if (static_cast<uint16_t>(Hardmon_Model_DW.is_LVSS_Checking) == Hardm_IN_Invalid_State_Inactive) {
            // Inport: '<Root>/LVSS_Status_3V3' incorporates:
            //   Inport: '<Root>/LVSS_EN_uC'

            // During 'Invalid_State_Inactive': '<S2>:82'
            if ((Hardmon_Model_U.LVSS_Status_3V3 != Hardmon_Model_U.LVSS_EN_uC) && (static_cast<int16_t>(*DataTypeConversion) != 6)) {
                // Transition: '<S2>:79'
                Hardmon_Model_DW.temporalCounter_i6 = 0U;
                Hardmon_Model_DW.is_LVSS_Checking = Hardmon_Model_IN_count;

                // Entry 'count': '<S2>:84'
                Hardmon_Model_DW.LVSS_Counter = static_cast<uint8_t>(
                    static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.LVSS_Counter) + 1U));
            }

            // End of Inport: '<Root>/LVSS_Status_3V3'

            // During 'count': '<S2>:84'
        } else if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i6)
                   >= 3U) {
            // Transition: '<S2>:81'
            Hardmon_Model_DW.temporalCounter_i6 = 0U;
            Hardmon_Model_DW.is_LVSS_Checking = Hardmon_Model_IN_count;

            // Entry 'count': '<S2>:84'
            Hardmon_Model_DW.LVSS_Counter = static_cast<uint8_t>(
                static_cast<uint16_t>(static_cast<uint16_t>(Hardmon_Model_DW.LVSS_Counter) + 1U));
        } else if ((Hardmon_Model_U.LVSS_Status_3V3 == Hardmon_Model_U.LVSS_EN_uC) || (static_cast<int16_t>(*DataTypeConversion) == 6)) {
            // Transition: '<S2>:80'
            Hardmon_Model_DW.is_LVSS_Checking =
                Hardm_IN_Invalid_State_Inactive;

            // Entry 'Invalid_State_Inactive': '<S2>:82'
            Hardmon_Model_B.HM_Fault = false;
            Hardmon_Model_DW.LVSS_Counter = 0U;
            Hardmon_Model_B.nMC_Switch_EN = false;
            Hardmon_Model_B.nLVSS_Switch_EN = false;

            // Inport: '<Root>/LVSS_EN_uC'
            Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
        } else {
            // no actions
        }
    }
}

// Function for Chart: '<Root>/Chart'
void Hardmon_Model::enter_internal_Normal_Operation(void) {
    // Entry Internal 'Normal_Operation': '<S2>:29'
    // Entry Internal 'ESTOP_Checking': '<S2>:28'
    // Transition: '<S2>:2'
    Hardmon_Model_DW.is_ESTOP_Checking = Hardmon_Model_IN_ESTOP_Inactive;

    // Entry 'ESTOP_Inactive': '<S2>:1'
    Hardmon_Model_DW.ESTOP_Counter = 0U;

    // Entry Internal 'Watchdog_Checking': '<S2>:36'
    // Transition: '<S2>:44'
    Hardmon_Model_DW.is_Watchdog_Checking = Hardmon_Model_IN_WD_Low;

    // Entry 'WD_Low': '<S2>:41'
    // Entry Internal 'WD_Low': '<S2>:41'
    Hardmon_Model_DW.temporalCounter_i2 = 0U;

    // Entry 'Counting': '<S2>:48'
    Hardmon_Model_DW.WD_Counter = 1U;

    // Entry Internal 'Invalid_State_Checking': '<S2>:59'
    // Transition: '<S2>:54'
    Hardmon_Model_DW.is_Invalid_State_Checking =
        Hardm_IN_Invalid_State_Inactive;

    // Entry 'Invalid_State_Inactive': '<S2>:53'
    Hardmon_Model_DW.Invalid_State_Counter = 0U;

    // Entry Internal 'Ignition_Level_Checking': '<S2>:68'
    // Transition: '<S2>:67'
    Hardmon_Model_DW.is_Ignition_Level_Checking =
        Hard_IN_Ignition_State_Inactive;

    // Entry 'Ignition_State_Inactive': '<S2>:65'
    Hardmon_Model_DW.Ignition_State_Counter = 0U;

    // Entry Internal 'ESTOP_Level_Checking': '<S2>:76'
    // Transition: '<S2>:74'
    Hardmon_Model_DW.is_ESTOP_Level_Checking =
        Hardmon_IN_ESTOP_State_Inactive;

    // Entry 'ESTOP_State_Inactive': '<S2>:73'
    Hardmon_Model_DW.ESTOP_State_Counter = 0U;

    // Entry Internal 'LVSS_Checking': '<S2>:85'
    // Transition: '<S2>:83'
    Hardmon_Model_DW.is_LVSS_Checking = Hardm_IN_Invalid_State_Inactive;

    // Entry 'Invalid_State_Inactive': '<S2>:82'
    Hardmon_Model_B.HM_Fault = false;
    Hardmon_Model_DW.LVSS_Counter = 0U;
    Hardmon_Model_B.nMC_Switch_EN = false;
    Hardmon_Model_B.nLVSS_Switch_EN = false;

    // Inport: '<Root>/LVSS_EN_uC'
    Hardmon_Model_B.LVSS_EN_HM = Hardmon_Model_U.LVSS_EN_uC;
}
}// namespace vcu

namespace vcu {
// Model step function
void Hardmon_Model::step() {
    double x_idx_0;
    double x_idx_1;
    double x_idx_2;
    uint8_t DataTypeConversion;

    // Outport: '<Root>/nMC_Switch_EN' incorporates:
    //   Memory: '<Root>/Memory'

    Hardmon_Model_Y.nMC_Switch_EN = Hardmon_Model_DW.Memory_PreviousInput;

    // Outport: '<Root>/nLVSS_Switch_EN' incorporates:
    //   Memory: '<Root>/Memory7'

    Hardmon_Model_Y.nLVSS_Switch_EN = Hardmon_Model_DW.Memory7_PreviousInput;

    // Outport: '<Root>/Inverter_DIS_CAN' incorporates:
    //   Memory: '<Root>/Memory6'

    Hardmon_Model_Y.Inverter_DIS_CAN =
        Hardmon_Model_DW.Memory6_PreviousInput;

    // Outport: '<Root>/MC_TOG_N_HM' incorporates:
    //   Memory: '<Root>/Memory1'

    Hardmon_Model_Y.MC_TOG_N_HM = Hardmon_Model_DW.Memory1_PreviousInput;

    // Outport: '<Root>/MC_TOG_P_HM' incorporates:
    //   Memory: '<Root>/Memory2'

    Hardmon_Model_Y.MC_TOG_P_HM = Hardmon_Model_DW.Memory2_PreviousInput;

    // Outport: '<Root>/nReset' incorporates:
    //   Memory: '<Root>/Memory3'

    Hardmon_Model_Y.nReset = Hardmon_Model_DW.Memory3_PreviousInput;

    // Outport: '<Root>/LVSS_EN_HM' incorporates:
    //   Memory: '<Root>/Memory4'

    Hardmon_Model_Y.LVSS_EN_HM = Hardmon_Model_DW.Memory4_PreviousInput;

    // Outport: '<Root>/HM_Fault' incorporates:
    //   Memory: '<Root>/Memory5'

    Hardmon_Model_Y.HM_Fault = Hardmon_Model_DW.Memory5_PreviousInput;

    // MATLAB Function: '<S3>/Binary to Scalar' incorporates:
    //   Inport: '<Root>/uC_State'

    // binvec2num Converts a binary vector to a decimal scalar value
    //    This function assumes bit 0 is in position 1, bit 1 in position 2, and
    //    so on.
    // MATLAB Function 'Binary Vector Conversion/From Unipolar/Binary to Scalar': '<S4>:1'
    //    Copyright 2018-2020 The MathWorks, Inc.
    // '<S4>:1:12'
    // '<S4>:1:19'
    DataTypeConversion = Hardmon_Model_U.uC_State[0];
    if (static_cast<int16_t>(Hardmon_Model_U.uC_State[0]) > 0) {
        // '<S4>:1:13'
        // '<S4>:1:14'
        DataTypeConversion = 1U;
    } else {
        // '<S4>:1:16'
    }

    x_idx_0 = Hardmon_Model_DW.pows[0] * static_cast<double>(DataTypeConversion);
    DataTypeConversion = Hardmon_Model_U.uC_State[1];
    if (static_cast<int16_t>(Hardmon_Model_U.uC_State[1]) > 0) {
        // '<S4>:1:13'
        // '<S4>:1:14'
        DataTypeConversion = 1U;
    } else {
        // '<S4>:1:16'
    }

    x_idx_1 = Hardmon_Model_DW.pows[1] * static_cast<double>(DataTypeConversion);
    DataTypeConversion = Hardmon_Model_U.uC_State[2];
    if (static_cast<int16_t>(Hardmon_Model_U.uC_State[2]) > 0) {
        // '<S4>:1:13'
        // '<S4>:1:14'
        DataTypeConversion = 1U;
    } else {
        // '<S4>:1:16'
    }

    x_idx_2 = Hardmon_Model_DW.pows[2] * static_cast<double>(DataTypeConversion);
    DataTypeConversion = Hardmon_Model_U.uC_State[3];
    if (static_cast<int16_t>(Hardmon_Model_U.uC_State[3]) > 0) {
        // '<S4>:1:13'
        // '<S4>:1:14'
        DataTypeConversion = 1U;
    } else {
        // '<S4>:1:16'
    }

    // DataTypeConversion: '<S3>/Data Type Conversion' incorporates:
    //   MATLAB Function: '<S3>/Binary to Scalar'

    x_idx_0 = std::fmod(std::floor(((x_idx_0 + x_idx_1) + x_idx_2) + Hardmon_Model_DW.pows[3] * static_cast<double>(DataTypeConversion)), 256.0);

    // DataTypeConversion: '<S3>/Data Type Conversion'
    DataTypeConversion = static_cast<uint8_t>(x_idx_0 < 0.0 ? static_cast<
                                                  int16_t>(static_cast<uint8_t>(static_cast<int16_t>(-static_cast<int16_t>(static_cast<int8_t>(static_cast<uint8_t>(
                                                  static_cast<double>(-x_idx_0))))))) :
                                                              static_cast<int16_t>(
                                                                  static_cast<uint8_t>(x_idx_0)));

    // Chart: '<Root>/Chart' incorporates:
    //   Inport: '<Root>/Discharge_SM_CAN'
    //   Inport: '<Root>/LVSS_Status_3V3'
    //   Inport: '<Root>/MC_Status_3V3'

    // Gateway: Chart
    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i1) < 1U) {
        Hardmon_Model_DW.temporalCounter_i1 = 1U;
    }

    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i2) < 3U) {
        Hardmon_Model_DW.temporalCounter_i2 = static_cast<uint8_t>(
            static_cast<int16_t>(static_cast<int16_t>(Hardmon_Model_DW.temporalCounter_i2) + 1));
    }

    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i3) < 3U) {
        Hardmon_Model_DW.temporalCounter_i3 = static_cast<uint8_t>(
            static_cast<int16_t>(static_cast<int16_t>(Hardmon_Model_DW.temporalCounter_i3) + 1));
    }

    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i4) < 3U) {
        Hardmon_Model_DW.temporalCounter_i4 = static_cast<uint8_t>(
            static_cast<int16_t>(static_cast<int16_t>(Hardmon_Model_DW.temporalCounter_i4) + 1));
    }

    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i5) < 3U) {
        Hardmon_Model_DW.temporalCounter_i5 = static_cast<uint8_t>(
            static_cast<int16_t>(static_cast<int16_t>(Hardmon_Model_DW.temporalCounter_i5) + 1));
    }

    if (static_cast<uint16_t>(Hardmon_Model_DW.temporalCounter_i6) < 3U) {
        Hardmon_Model_DW.temporalCounter_i6 = static_cast<uint8_t>(
            static_cast<int16_t>(static_cast<int16_t>(Hardmon_Model_DW.temporalCounter_i6) + 1));
    }

    // During: Chart
    if (static_cast<uint16_t>(Hardmon_Model_DW.is_active_c3_Hardmon_Model) == 0U) {
        // Entry: Chart
        Hardmon_Model_DW.is_active_c3_Hardmon_Model = 1U;

        // Entry Internal: Chart
        // Transition: '<S2>:31'
        Hardmon_Model_DW.is_c3_Hardmon_Model =
            Hardmon_Mod_IN_Normal_Operation;
        enter_internal_Normal_Operation();
    } else {
        switch (Hardmon_Model_DW.is_c3_Hardmon_Model) {
        case Hardmon_Model_IN_End_Powerdown:
            // During 'End_Powerdown': '<S2>:116'
            if (static_cast<int16_t>(DataTypeConversion) == 6) {
                // Transition: '<S2>:113'
                // Exit 'End_Powerdown': '<S2>:116'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Mod_IN_Normal_Operation;
                enter_internal_Normal_Operation();
            }
            break;

        case Hardmon_Model_IN_Enter_Fault:
            // During 'Enter_Fault': '<S2>:23'
            if (static_cast<int16_t>(DataTypeConversion) == 8) {
                // Transition: '<S2>:32'
                // Exit 'Enter_Fault': '<S2>:23'
                Hardmon_Model_B.nReset = true;
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Mod_IN_Normal_Operation;
                enter_internal_Normal_Operation();
            }
            break;

        case Hardmon_Model_IN_LVSS_Powerdown:
            // During 'LVSS_Powerdown': '<S2>:16'
            if (!Hardmon_Model_U.LVSS_Status_3V3) {
                // Transition: '<S2>:24'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_Enter_Fault;

                // Entry 'Enter_Fault': '<S2>:23'
                Hardmon_Model_B.nReset = false;
                Hardmon_Model_B.HM_Fault = true;
            }
            break;

        case Hardmon_Mode_IN_LVSS_Powerdown1:
            // During 'LVSS_Powerdown1': '<S2>:114'
            if (!Hardmon_Model_U.LVSS_Status_3V3) {
                // Transition: '<S2>:117'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_End_Powerdown;

                // Entry 'End_Powerdown': '<S2>:116'
                Hardmon_Model_B.nMC_Switch_EN = false;
                Hardmon_Model_B.nLVSS_Switch_EN = false;
            }
            break;

        case Hardmon_Model_IN_MC_DISCHARGE:
            Hardmon_Model_B.Inverter_DIS_CAN = true;

            // During 'MC_DISCHARGE': '<S2>:33'
            if (static_cast<int16_t>(Hardmon_Model_U.Discharge_SM_CAN) == 4) {
                // Transition: '<S2>:34'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_MC_POWERDOWN;

                // Entry 'MC_POWERDOWN': '<S2>:15'
                Hardmon_Model_B.nReset = true;
                Hardmon_Model_B.Inverter_DIS_CAN = false;
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = true;
            }
            break;

        case Hardmon_M_IN_MC_DISCHARGE_FAULT:
            Hardmon_Model_B.Inverter_DIS_CAN = true;

            // During 'MC_DISCHARGE_FAULT': '<S2>:110'
            if (static_cast<int16_t>(Hardmon_Model_U.Discharge_SM_CAN) == 4) {
                // Transition: '<S2>:112'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_MC_POWERDOWN1;

                // Entry 'MC_POWERDOWN1': '<S2>:111'
                Hardmon_Model_B.HM_Fault = true;
                Hardmon_Model_B.Inverter_DIS_CAN = false;
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = true;
            }
            break;

        case Hardmon_Model_IN_MC_FAULT_INIT:
            // During 'MC_FAULT_INIT': '<S2>:119'
            if ((static_cast<int16_t>(DataTypeConversion) != 0) && (static_cast<int16_t>(DataTypeConversion) != 8)) {
                // Transition: '<S2>:121'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_M_IN_MC_DISCHARGE_FAULT;

                // Entry 'MC_DISCHARGE_FAULT': '<S2>:110'
                Hardmon_Model_B.HM_Fault = true;
                Hardmon_Model_B.Inverter_DIS_CAN = true;
            } else if ((static_cast<int16_t>(DataTypeConversion) == 0) || (static_cast<int16_t>(DataTypeConversion) == 8)) {
                // Transition: '<S2>:123'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_MC_POWERDOWN1;

                // Entry 'MC_POWERDOWN1': '<S2>:111'
                Hardmon_Model_B.HM_Fault = true;
                Hardmon_Model_B.Inverter_DIS_CAN = false;
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = true;
            } else {
                // no actions
            }
            break;

        case Hardmon_Model_IN_MC_POWERDOWN:
            Hardmon_Model_B.Inverter_DIS_CAN = false;

            // During 'MC_POWERDOWN': '<S2>:15'
            if (!Hardmon_Model_U.MC_Status_3V3) {
                // Transition: '<S2>:17'
                // Exit 'MC_POWERDOWN': '<S2>:15'
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = false;
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_LVSS_Powerdown;

                // Entry 'LVSS_Powerdown': '<S2>:16'
                Hardmon_Model_B.LVSS_EN_HM = false;
            }
            break;

        case Hardmon_Model_IN_MC_POWERDOWN1:
            Hardmon_Model_B.Inverter_DIS_CAN = false;

            // During 'MC_POWERDOWN1': '<S2>:111'
            if (!Hardmon_Model_U.MC_Status_3V3) {
                // Transition: '<S2>:115'
                // Exit 'MC_POWERDOWN1': '<S2>:111'
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = false;
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Mode_IN_LVSS_Powerdown1;

                // Entry 'LVSS_Powerdown1': '<S2>:114'
                Hardmon_Model_B.LVSS_EN_HM = false;
            }
            break;

        case Hardmon_Model_IN_MC_RESET_INIT:
            // During 'MC_RESET_INIT': '<S2>:125'
            if ((static_cast<int16_t>(DataTypeConversion) != 0) && (static_cast<int16_t>(DataTypeConversion) != 8)) {
                // Transition: '<S2>:126'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_MC_DISCHARGE;

                // Entry 'MC_DISCHARGE': '<S2>:33'
                Hardmon_Model_B.HM_Fault = true;
                Hardmon_Model_B.Inverter_DIS_CAN = true;
            } else if ((static_cast<int16_t>(DataTypeConversion) == 0) || (static_cast<int16_t>(DataTypeConversion) == 8)) {
                // Transition: '<S2>:127'
                Hardmon_Model_DW.is_c3_Hardmon_Model =
                    Hardmon_Model_IN_MC_POWERDOWN;

                // Entry 'MC_POWERDOWN': '<S2>:15'
                Hardmon_Model_B.nReset = true;
                Hardmon_Model_B.Inverter_DIS_CAN = false;
                Hardmon_Model_B.MC_TOG_P_HM = false;
                Hardmon_Model_B.MC_TOG_N_HM = true;
            } else {
                // no actions
            }
            break;

        default:
            Hardmon_Model_Normal_Operation(&DataTypeConversion);
            break;
        }
    }

    // End of Chart: '<Root>/Chart'

    // Update for Memory: '<Root>/Memory'
    Hardmon_Model_DW.Memory_PreviousInput = Hardmon_Model_B.nMC_Switch_EN;

    // Update for Memory: '<Root>/Memory7'
    Hardmon_Model_DW.Memory7_PreviousInput = Hardmon_Model_B.nLVSS_Switch_EN;

    // Update for Memory: '<Root>/Memory6'
    Hardmon_Model_DW.Memory6_PreviousInput =
        Hardmon_Model_B.Inverter_DIS_CAN;

    // Update for Memory: '<Root>/Memory1'
    Hardmon_Model_DW.Memory1_PreviousInput = Hardmon_Model_B.MC_TOG_N_HM;

    // Update for Memory: '<Root>/Memory2'
    Hardmon_Model_DW.Memory2_PreviousInput = Hardmon_Model_B.MC_TOG_P_HM;

    // Update for Memory: '<Root>/Memory3'
    Hardmon_Model_DW.Memory3_PreviousInput = Hardmon_Model_B.nReset;

    // Update for Memory: '<Root>/Memory4'
    Hardmon_Model_DW.Memory4_PreviousInput = Hardmon_Model_B.LVSS_EN_HM;

    // Update for Memory: '<Root>/Memory5'
    Hardmon_Model_DW.Memory5_PreviousInput = Hardmon_Model_B.HM_Fault;
}

// Model initialize function
void Hardmon_Model::initialize() {
    // SystemInitialize for MATLAB Function: '<S3>/Binary to Scalar'
    Hardmon_Model_DW.pows[0] = 1.0;
    Hardmon_Model_DW.pows[1] = 2.0;
    Hardmon_Model_DW.pows[2] = 4.0;
    Hardmon_Model_DW.pows[3] = 8.0;

    // SystemInitialize for Chart: '<Root>/Chart'
    Hardmon_Model_B.nReset = true;
}

// Model terminate function
void Hardmon_Model::terminate() {
    // (no terminate code required)
}

// Constructor
Hardmon_Model::Hardmon_Model() : Hardmon_Model_U(),
                                 Hardmon_Model_Y(),
                                 Hardmon_Model_B(),
                                 Hardmon_Model_DW(),
                                 Hardmon_Model_M() {
    // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Hardmon_Model::~Hardmon_Model() = default;

// Real-Time Model get method
Hardmon_Model::RT_MODEL_Hardmon_Model_T* Hardmon_Model::getRTM() {
    return (&Hardmon_Model_M);
}
}// namespace vcu

//
// File trailer for generated code.
//
// [EOF]
//
