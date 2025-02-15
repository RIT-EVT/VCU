//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Hardmon.cpp
//
// Code generated for Simulink model 'Hardmon'.
//
// Model version                  : 1.66
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Sat Feb 15 11:28:30 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <models/Hardmon_Model.hpp>
#include <stdint.h>

// Named constants for Chart: '<Root>/Hardmon_Chart'
const uint8_t Hard_IN_Ignition_State_Inactive{ 1U };

const uint8_t Hardm_IN_Invalid_State_Inactive{ 1U };

const uint8_t Hardmon_IN_ESTOP_Inactive{ 1U };

const uint8_t Hardmon_IN_ESTOP_State_Inactive{ 1U };

const uint8_t Hardmon_IN_End_Powerdown{ 1U };

const uint8_t Hardmon_IN_Enter_Fault{ 2U };

const uint8_t Hardmon_IN_LVSS_Powerdown{ 3U };

const uint8_t Hardmon_IN_LVSS_Powerdown1{ 4U };

const uint8_t Hardmon_IN_MC_DISCHARGE{ 5U };

const uint8_t Hardmon_IN_MC_DISCHARGE_FAULT{ 6U };

const uint8_t Hardmon_IN_MC_FAULT_INIT{ 7U };

const uint8_t Hardmon_IN_MC_POWERDOWN{ 8U };

const uint8_t Hardmon_IN_MC_POWERDOWN1{ 9U };

const uint8_t Hardmon_IN_MC_RESET_INIT{ 10U };

const uint8_t Hardmon_IN_NO_ACTIVE_CHILD{ 0U };

const uint8_t Hardmon_IN_Normal_Operation{ 11U };

const uint8_t Hardmon_IN_WD_High{ 1U };

const uint8_t Hardmon_IN_WD_Low{ 2U };

const uint8_t Hardmon_IN_count{ 2U };

namespace vcu
{
    // Function for Chart: '<Root>/Hardmon_Chart'
    void Hardmon_Model::Hardmon_Invalid_State_Checking(void)
    {
        // During 'Invalid_State_Checking': '<S1>:59'
        if (static_cast<uint16_t>(Hardmon_DW.is_Invalid_State_Checking) ==
                Hardm_IN_Invalid_State_Inactive) {
            // Inport: '<Root>/Forward_EN' incorporates:
            //   Inport: '<Root>/Ignition_LS_A'
            //   Inport: '<Root>/uC_State'

            // During 'Invalid_State_Inactive': '<S1>:53'
            if (((!Hardmon_U.Forward_EN) || (!Hardmon_U.Ignition_LS_A)) &&
                    (Hardmon_U.uC_State == UC_State::MC_Active)) {
                // Transition: '<S1>:55'
                Hardmon_DW.temporalCounter_i3 = 0U;
                Hardmon_DW.is_Invalid_State_Checking = Hardmon_IN_count;

                // Entry 'count': '<S1>:58'
                Hardmon_DW.Invalid_State_Counter++;
            }

            // End of Inport: '<Root>/Forward_EN'

            // During 'count': '<S1>:58'
        } else if (Hardmon_DW.temporalCounter_i3 >= 5000U) {
            // Transition: '<S1>:57'
            Hardmon_DW.temporalCounter_i3 = 0U;
            Hardmon_DW.is_Invalid_State_Checking = Hardmon_IN_count;

            // Entry 'count': '<S1>:58'
            Hardmon_DW.Invalid_State_Counter++;
        } else if ((Hardmon_U.Forward_EN && Hardmon_U.Ignition_LS_A) ||
                   (Hardmon_U.uC_State != UC_State::MC_Active)) {
            // Transition: '<S1>:56'
            Hardmon_DW.is_Invalid_State_Checking =
                Hardm_IN_Invalid_State_Inactive;

            // Outport: '<Root>/HM_Fault'
            // Entry 'Invalid_State_Inactive': '<S1>:53'
            Hardmon_Y.HM_Fault = false;
            Hardmon_DW.Invalid_State_Counter = 0.0;

            // Outport: '<Root>/nMC_Switch_EN'
            Hardmon_Y.nMC_Switch_EN = false;

            // Outport: '<Root>/nLVSS_Switch_EN'
            Hardmon_Y.nLVSS_Switch_EN = false;

            // Outport: '<Root>/LVSS_EN_HM' incorporates:
            //   Inport: '<Root>/LVSS_EN_uC'

            Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
        } else {
            // no actions
        }
    }

    // Function for Chart: '<Root>/Hardmon_Chart'
    void Hardmon_Model::exit_internal_Normal_Operation(void)
    {
        // Exit Internal 'Normal_Operation': '<S1>:29'
        // Exit Internal 'LVSS_Checking': '<S1>:85'
        Hardmon_DW.is_LVSS_Checking = Hardmon_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'ESTOP_Level_Checking': '<S1>:76'
        Hardmon_DW.is_ESTOP_Level_Checking = Hardmon_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'Ignition_Level_Checking': '<S1>:68'
        Hardmon_DW.is_Ignition_Level_Checking = Hardmon_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'Invalid_State_Checking': '<S1>:59'
        Hardmon_DW.is_Invalid_State_Checking = Hardmon_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'Watchdog_Checking': '<S1>:36'
        // Exit Internal 'WD_High': '<S1>:42'
        // Exit Internal 'WD_Low': '<S1>:41'
        Hardmon_DW.is_Watchdog_Checking = Hardmon_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'ESTOP_Checking': '<S1>:28'
        Hardmon_DW.is_ESTOP_Checking = Hardmon_IN_NO_ACTIVE_CHILD;
    }

    // Function for Chart: '<Root>/Hardmon_Chart'
    void Hardmon_Model::Hardmon_Normal_Operation(void)
    {
        // Inport: '<Root>/uC_State' incorporates:
        //   Inport: '<Root>/ESTOP_LS_A'
        //   Inport: '<Root>/ESTOP_LS_B'
        //   Inport: '<Root>/Ignition_LS_A'
        //   Inport: '<Root>/Ignition_LS_B'
        //   Inport: '<Root>/LVSS_EN_uC'
        //   Inport: '<Root>/LVSS_ON'

        // During 'Normal_Operation': '<S1>:29'
        if ((Hardmon_DW.WD_Counter >= 4.0) && (Hardmon_U.uC_State != UC_State::
                Preset)) {
            // Transition: '<S1>:8'
            exit_internal_Normal_Operation();
            Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_RESET_INIT;

            // Outport: '<Root>/nMC_Switch_EN'
            // Entry 'MC_RESET_INIT': '<S1>:125'
            Hardmon_Y.nMC_Switch_EN = true;

            // Outport: '<Root>/nLVSS_Switch_EN'
            Hardmon_Y.nLVSS_Switch_EN = true;
        } else if ((Hardmon_DW.Invalid_State_Counter >= 4.0) ||
                   (Hardmon_DW.Ignition_State_Counter >= 4.0) ||
                   (Hardmon_DW.LVSS_Counter >= 4.0) || (Hardmon_DW.ESTOP_Counter
                    >= 4.0) || (Hardmon_DW.ESTOP_State_Counter >= 4.0)) {
            // Transition: '<S1>:118'
            exit_internal_Normal_Operation();
            Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_FAULT_INIT;

            // Outport: '<Root>/nMC_Switch_EN'
            // Entry 'MC_FAULT_INIT': '<S1>:119'
            Hardmon_Y.nMC_Switch_EN = true;

            // Outport: '<Root>/nLVSS_Switch_EN'
            Hardmon_Y.nLVSS_Switch_EN = true;
        } else {
            // During 'ESTOP_Checking': '<S1>:28'
            if (static_cast<uint16_t>(Hardmon_DW.is_ESTOP_Checking) ==
                    Hardmon_IN_ESTOP_Inactive) {
                // Inport: '<Root>/ESTOP_LS_A'
                // During 'ESTOP_Inactive': '<S1>:1'
                if (Hardmon_U.ESTOP_LS_A && (Hardmon_U.uC_State != UC_State::
                        Fault) && (Hardmon_U.uC_State != UC_State::Estop)) {
                    // Transition: '<S1>:5'
                    Hardmon_DW.temporalCounter_i1 = 0U;
                    Hardmon_DW.is_ESTOP_Checking = Hardmon_IN_count;

                    // Entry 'count': '<S1>:6'
                    Hardmon_DW.ESTOP_Counter++;
                }

                // During 'count': '<S1>:6'
            } else if (Hardmon_DW.temporalCounter_i1 >= 1000U) {
                // Transition: '<S1>:7'
                Hardmon_DW.temporalCounter_i1 = 0U;
                Hardmon_DW.is_ESTOP_Checking = Hardmon_IN_count;

                // Entry 'count': '<S1>:6'
                Hardmon_DW.ESTOP_Counter++;
            } else if (Hardmon_U.ESTOP_LS_A || (Hardmon_U.uC_State == UC_State::
                        Fault) || (Hardmon_U.uC_State == UC_State::Estop)) {
                // Transition: '<S1>:14'
                Hardmon_DW.is_ESTOP_Checking = Hardmon_IN_ESTOP_Inactive;

                // Outport: '<Root>/HM_Fault'
                // Entry 'ESTOP_Inactive': '<S1>:1'
                Hardmon_Y.HM_Fault = false;
                Hardmon_DW.ESTOP_Counter = 0.0;

                // Outport: '<Root>/nMC_Switch_EN'
                Hardmon_Y.nMC_Switch_EN = false;

                // Outport: '<Root>/nLVSS_Switch_EN'
                Hardmon_Y.nLVSS_Switch_EN = false;

                // Outport: '<Root>/LVSS_EN_HM' incorporates:
                //   Inport: '<Root>/LVSS_EN_uC'

                Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
            } else {
                // no actions
            }

            // During 'Watchdog_Checking': '<S1>:36'
            if (static_cast<uint16_t>(Hardmon_DW.is_Watchdog_Checking) ==
                    Hardmon_IN_WD_High) {
                // Inport: '<Root>/Watchdog'
                // During 'WD_High': '<S1>:42'
                // During 'Counting': '<S1>:49'
                if (!Hardmon_U.Watchdog) {
                    // Transition: '<S1>:46'
                    Hardmon_DW.is_Watchdog_Checking = Hardmon_IN_WD_Low;

                    // Entry 'WD_Low': '<S1>:41'
                    // Entry Internal 'WD_Low': '<S1>:41'
                    Hardmon_DW.temporalCounter_i2 = 0U;

                    // Entry 'Counting': '<S1>:48'
                    Hardmon_DW.WD_Counter = 1.0;
                } else if (Hardmon_DW.temporalCounter_i2 >= 5000U) {
                    // Transition: '<S1>:50'
                    Hardmon_DW.temporalCounter_i2 = 0U;

                    // Entry 'Counting': '<S1>:49'
                    Hardmon_DW.WD_Counter++;
                } else {
                    // no actions
                }

                // Inport: '<Root>/Watchdog'
                // During 'WD_Low': '<S1>:41'
                // During 'Counting': '<S1>:48'
            } else if (Hardmon_U.Watchdog) {
                // Transition: '<S1>:43'
                Hardmon_DW.is_Watchdog_Checking = Hardmon_IN_WD_High;

                // Entry 'WD_High': '<S1>:42'
                // Entry Internal 'WD_High': '<S1>:42'
                Hardmon_DW.temporalCounter_i2 = 0U;

                // Entry 'Counting': '<S1>:49'
                Hardmon_DW.WD_Counter = 1.0;
            } else if (Hardmon_DW.temporalCounter_i2 >= 5000U) {
                // Transition: '<S1>:40'
                Hardmon_DW.temporalCounter_i2 = 0U;

                // Entry 'Counting': '<S1>:48'
                Hardmon_DW.WD_Counter++;
            } else {
                // no actions
            }

            Hardmon_Invalid_State_Checking();

            // During 'Ignition_Level_Checking': '<S1>:68'
            if (static_cast<uint16_t>(Hardmon_DW.is_Ignition_Level_Checking) ==
                    Hard_IN_Ignition_State_Inactive) {
                // Inport: '<Root>/Ignition_LS_A' incorporates:
                //   Inport: '<Root>/Ignition_LS_B'

                // During 'Ignition_State_Inactive': '<S1>:65'
                if ((Hardmon_U.Ignition_LS_A != Hardmon_U.Ignition_LS_B) &&
                        (Hardmon_U.uC_State != UC_State::Fault)) {
                    // Transition: '<S1>:62'
                    Hardmon_DW.temporalCounter_i4 = 0U;
                    Hardmon_DW.is_Ignition_Level_Checking = Hardmon_IN_count;

                    // Entry 'count': '<S1>:66'
                    Hardmon_DW.Ignition_State_Counter++;
                }

                // During 'count': '<S1>:66'
            } else if (Hardmon_DW.temporalCounter_i4 >= 5000U) {
                // Transition: '<S1>:64'
                Hardmon_DW.temporalCounter_i4 = 0U;
                Hardmon_DW.is_Ignition_Level_Checking = Hardmon_IN_count;

                // Entry 'count': '<S1>:66'
                Hardmon_DW.Ignition_State_Counter++;
            } else if ((Hardmon_U.Ignition_LS_A == Hardmon_U.Ignition_LS_B) ||
                       (Hardmon_U.uC_State == UC_State::Fault)) {
                // Transition: '<S1>:63'
                Hardmon_DW.is_Ignition_Level_Checking =
                    Hard_IN_Ignition_State_Inactive;

                // Outport: '<Root>/HM_Fault'
                // Entry 'Ignition_State_Inactive': '<S1>:65'
                Hardmon_Y.HM_Fault = false;
                Hardmon_DW.Ignition_State_Counter = 0.0;

                // Outport: '<Root>/nMC_Switch_EN'
                Hardmon_Y.nMC_Switch_EN = false;

                // Outport: '<Root>/nLVSS_Switch_EN'
                Hardmon_Y.nLVSS_Switch_EN = false;

                // Outport: '<Root>/LVSS_EN_HM' incorporates:
                //   Inport: '<Root>/LVSS_EN_uC'

                Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
            } else {
                // no actions
            }

            // During 'ESTOP_Level_Checking': '<S1>:76'
            if (static_cast<uint16_t>(Hardmon_DW.is_ESTOP_Level_Checking) ==
                    Hardmon_IN_ESTOP_State_Inactive) {
                // Inport: '<Root>/ESTOP_LS_A' incorporates:
                //   Inport: '<Root>/ESTOP_LS_B'

                // During 'ESTOP_State_Inactive': '<S1>:73'
                if ((Hardmon_U.ESTOP_LS_A != Hardmon_U.ESTOP_LS_B) &&
                        (Hardmon_U.uC_State != UC_State::Fault)) {
                    // Transition: '<S1>:70'
                    Hardmon_DW.temporalCounter_i5 = 0U;
                    Hardmon_DW.is_ESTOP_Level_Checking = Hardmon_IN_count;

                    // Entry 'count': '<S1>:75'
                    Hardmon_DW.ESTOP_State_Counter++;
                }

                // During 'count': '<S1>:75'
            } else if (Hardmon_DW.temporalCounter_i5 >= 5000U) {
                // Transition: '<S1>:72'
                Hardmon_DW.temporalCounter_i5 = 0U;
                Hardmon_DW.is_ESTOP_Level_Checking = Hardmon_IN_count;

                // Entry 'count': '<S1>:75'
                Hardmon_DW.ESTOP_State_Counter++;
            } else if ((Hardmon_U.ESTOP_LS_A == Hardmon_U.ESTOP_LS_B) ||
                       (Hardmon_U.uC_State == UC_State::Fault)) {
                // Transition: '<S1>:71'
                Hardmon_DW.is_ESTOP_Level_Checking =
                    Hardmon_IN_ESTOP_State_Inactive;

                // Outport: '<Root>/HM_Fault'
                // Entry 'ESTOP_State_Inactive': '<S1>:73'
                Hardmon_Y.HM_Fault = false;
                Hardmon_DW.ESTOP_State_Counter = 0.0;

                // Outport: '<Root>/nMC_Switch_EN'
                Hardmon_Y.nMC_Switch_EN = false;

                // Outport: '<Root>/nLVSS_Switch_EN'
                Hardmon_Y.nLVSS_Switch_EN = false;

                // Outport: '<Root>/LVSS_EN_HM' incorporates:
                //   Inport: '<Root>/LVSS_EN_uC'

                Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
            } else {
                // no actions
            }

            // During 'LVSS_Checking': '<S1>:85'
            if (static_cast<uint16_t>(Hardmon_DW.is_LVSS_Checking) ==
                    Hardm_IN_Invalid_State_Inactive) {
                // Inport: '<Root>/LVSS_ON' incorporates:
                //   Inport: '<Root>/LVSS_EN_uC'

                // During 'Invalid_State_Inactive': '<S1>:82'
                if ((Hardmon_U.LVSS_ON != Hardmon_U.LVSS_EN_uC) &&
                        (Hardmon_U.uC_State != UC_State::Fault)) {
                    // Transition: '<S1>:79'
                    Hardmon_DW.temporalCounter_i6 = 0U;
                    Hardmon_DW.is_LVSS_Checking = Hardmon_IN_count;

                    // Entry 'count': '<S1>:84'
                    Hardmon_DW.LVSS_Counter++;
                }

                // During 'count': '<S1>:84'
            } else if (Hardmon_DW.temporalCounter_i6 >= 5000U) {
                // Transition: '<S1>:81'
                Hardmon_DW.temporalCounter_i6 = 0U;
                Hardmon_DW.is_LVSS_Checking = Hardmon_IN_count;

                // Entry 'count': '<S1>:84'
                Hardmon_DW.LVSS_Counter++;
            } else if ((Hardmon_U.LVSS_ON == Hardmon_U.LVSS_EN_uC) ||
                       (Hardmon_U.uC_State == UC_State::Fault)) {
                // Transition: '<S1>:80'
                Hardmon_DW.is_LVSS_Checking = Hardm_IN_Invalid_State_Inactive;

                // Outport: '<Root>/HM_Fault'
                // Entry 'Invalid_State_Inactive': '<S1>:82'
                Hardmon_Y.HM_Fault = false;
                Hardmon_DW.LVSS_Counter = 0.0;

                // Outport: '<Root>/nMC_Switch_EN'
                Hardmon_Y.nMC_Switch_EN = false;

                // Outport: '<Root>/nLVSS_Switch_EN'
                Hardmon_Y.nLVSS_Switch_EN = false;

                // Outport: '<Root>/LVSS_EN_HM' incorporates:
                //   Inport: '<Root>/LVSS_EN_uC'

                Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
            } else {
                // no actions
            }
        }

        // End of Inport: '<Root>/uC_State'
    }

    // Function for Chart: '<Root>/Hardmon_Chart'
    void Hardmon_Model::enter_internal_Normal_Operation(void)
    {
        // Entry Internal 'Normal_Operation': '<S1>:29'
        // Entry Internal 'ESTOP_Checking': '<S1>:28'
        // Transition: '<S1>:2'
        Hardmon_DW.is_ESTOP_Checking = Hardmon_IN_ESTOP_Inactive;

        // Entry 'ESTOP_Inactive': '<S1>:1'
        Hardmon_DW.ESTOP_Counter = 0.0;

        // Entry Internal 'Watchdog_Checking': '<S1>:36'
        // Transition: '<S1>:44'
        Hardmon_DW.is_Watchdog_Checking = Hardmon_IN_WD_Low;

        // Entry 'WD_Low': '<S1>:41'
        // Entry Internal 'WD_Low': '<S1>:41'
        Hardmon_DW.temporalCounter_i2 = 0U;

        // Entry 'Counting': '<S1>:48'
        Hardmon_DW.WD_Counter = 1.0;

        // Entry Internal 'Invalid_State_Checking': '<S1>:59'
        // Transition: '<S1>:54'
        Hardmon_DW.is_Invalid_State_Checking = Hardm_IN_Invalid_State_Inactive;

        // Entry 'Invalid_State_Inactive': '<S1>:53'
        Hardmon_DW.Invalid_State_Counter = 0.0;

        // Entry Internal 'Ignition_Level_Checking': '<S1>:68'
        // Transition: '<S1>:67'
        Hardmon_DW.is_Ignition_Level_Checking = Hard_IN_Ignition_State_Inactive;

        // Entry 'Ignition_State_Inactive': '<S1>:65'
        Hardmon_DW.Ignition_State_Counter = 0.0;

        // Entry Internal 'ESTOP_Level_Checking': '<S1>:76'
        // Transition: '<S1>:74'
        Hardmon_DW.is_ESTOP_Level_Checking = Hardmon_IN_ESTOP_State_Inactive;

        // Entry 'ESTOP_State_Inactive': '<S1>:73'
        Hardmon_DW.ESTOP_State_Counter = 0.0;

        // Entry Internal 'LVSS_Checking': '<S1>:85'
        // Transition: '<S1>:83'
        Hardmon_DW.is_LVSS_Checking = Hardm_IN_Invalid_State_Inactive;

        // Outport: '<Root>/HM_Fault'
        // Entry 'Invalid_State_Inactive': '<S1>:82'
        Hardmon_Y.HM_Fault = false;
        Hardmon_DW.LVSS_Counter = 0.0;

        // Outport: '<Root>/nMC_Switch_EN'
        Hardmon_Y.nMC_Switch_EN = false;

        // Outport: '<Root>/nLVSS_Switch_EN'
        Hardmon_Y.nLVSS_Switch_EN = false;

        // Outport: '<Root>/LVSS_EN_HM' incorporates:
        //   Inport: '<Root>/LVSS_EN_uC'

        Hardmon_Y.LVSS_EN_HM = Hardmon_U.LVSS_EN_uC;
    }
}

namespace vcu
{
    // Model step function
    void Hardmon_Model::step()
    {
        // Chart: '<Root>/Hardmon_Chart' incorporates:
        //   Inport: '<Root>/LVSS_ON'
        //   Inport: '<Root>/MC_12V0'
        //   Inport: '<Root>/MC_DC_State'
        //   Inport: '<Root>/uC_State'

        // Gateway: Hardmon_Chart
        if (Hardmon_DW.temporalCounter_i1 < 1023U) {
            Hardmon_DW.temporalCounter_i1 = static_cast<uint16_t>(static_cast<
                int16_t>(static_cast<int16_t>(Hardmon_DW.temporalCounter_i1) + 1));
        }

        if (Hardmon_DW.temporalCounter_i2 < 8191U) {
            Hardmon_DW.temporalCounter_i2 = static_cast<uint16_t>
                (static_cast<int16_t>(static_cast<int16_t>
                  (Hardmon_DW.temporalCounter_i2) + 1));
        }

        if (Hardmon_DW.temporalCounter_i3 < 8191U) {
            Hardmon_DW.temporalCounter_i3 = static_cast<uint16_t>
                (static_cast<int16_t>(static_cast<int16_t>
                  (Hardmon_DW.temporalCounter_i3) + 1));
        }

        if (Hardmon_DW.temporalCounter_i4 < 8191U) {
            Hardmon_DW.temporalCounter_i4 = static_cast<uint16_t>
                (static_cast<int16_t>(static_cast<int16_t>
                  (Hardmon_DW.temporalCounter_i4) + 1));
        }

        if (Hardmon_DW.temporalCounter_i5 < 8191U) {
            Hardmon_DW.temporalCounter_i5 = static_cast<uint16_t>
                (static_cast<int16_t>(static_cast<int16_t>
                  (Hardmon_DW.temporalCounter_i5) + 1));
        }

        if (Hardmon_DW.temporalCounter_i6 < 8191U) {
            Hardmon_DW.temporalCounter_i6 = static_cast<uint16_t>
                (static_cast<int16_t>(static_cast<int16_t>
                  (Hardmon_DW.temporalCounter_i6) + 1));
        }

        // During: Hardmon_Chart
        if (static_cast<uint16_t>(Hardmon_DW.is_active_c3_Hardmon) == 0U) {
            // Entry: Hardmon_Chart
            Hardmon_DW.is_active_c3_Hardmon = 1U;

            // Entry Internal: Hardmon_Chart
            // Transition: '<S1>:31'
            Hardmon_DW.is_c3_Hardmon = Hardmon_IN_Normal_Operation;
            enter_internal_Normal_Operation();
        } else {
            switch (Hardmon_DW.is_c3_Hardmon) {
              case Hardmon_IN_End_Powerdown:
                // During 'End_Powerdown': '<S1>:116'
                if (static_cast<int16_t>(Hardmon_U.uC_State) == 6) {
                    // Transition: '<S1>:113'
                    // Exit 'End_Powerdown': '<S1>:116'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_Normal_Operation;
                    enter_internal_Normal_Operation();
                }
                break;

              case Hardmon_IN_Enter_Fault:
                // During 'Enter_Fault': '<S1>:23'
                if (static_cast<int16_t>(Hardmon_U.uC_State) == 8) {
                    // Outport: '<Root>/nReset'
                    // Transition: '<S1>:32'
                    // Exit 'Enter_Fault': '<S1>:23'
                    Hardmon_Y.nReset = false;
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_Normal_Operation;
                    enter_internal_Normal_Operation();
                }
                break;

              case Hardmon_IN_LVSS_Powerdown:
                // During 'LVSS_Powerdown': '<S1>:16'
                if (!Hardmon_U.LVSS_ON) {
                    // Transition: '<S1>:24'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_Enter_Fault;

                    // Outport: '<Root>/nReset'
                    // Entry 'Enter_Fault': '<S1>:23'
                    Hardmon_Y.nReset = true;

                    // Outport: '<Root>/HM_Fault'
                    Hardmon_Y.HM_Fault = true;
                }
                break;

              case Hardmon_IN_LVSS_Powerdown1:
                // During 'LVSS_Powerdown1': '<S1>:114'
                if (!Hardmon_U.LVSS_ON) {
                    // Transition: '<S1>:117'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_End_Powerdown;

                    // Outport: '<Root>/nMC_Switch_EN'
                    // Entry 'End_Powerdown': '<S1>:116'
                    Hardmon_Y.nMC_Switch_EN = false;

                    // Outport: '<Root>/nLVSS_Switch_EN'
                    Hardmon_Y.nLVSS_Switch_EN = false;
                }
                break;

              case Hardmon_IN_MC_DISCHARGE:
                // Outport: '<Root>/Inverter_DIS'
                Hardmon_Y.Inverter_DIS = true;

                // During 'MC_DISCHARGE': '<S1>:33'
                if (static_cast<int16_t>(Hardmon_U.MC_DC_State_d) == 4) {
                    // Transition: '<S1>:34'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_POWERDOWN;

                    // Outport: '<Root>/nReset'
                    // Entry 'MC_POWERDOWN': '<S1>:15'
                    Hardmon_Y.nReset = false;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = false;

                    // Outport: '<Root>/MC_EN_HM'
                    Hardmon_Y.MC_EN_HM = false;
                }
                break;

              case Hardmon_IN_MC_DISCHARGE_FAULT:
                // Outport: '<Root>/Inverter_DIS'
                Hardmon_Y.Inverter_DIS = true;

                // During 'MC_DISCHARGE_FAULT': '<S1>:110'
                if (static_cast<int16_t>(Hardmon_U.MC_DC_State_d) == 4) {
                    // Transition: '<S1>:112'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_POWERDOWN1;

                    // Outport: '<Root>/HM_Fault'
                    // Entry 'MC_POWERDOWN1': '<S1>:111'
                    Hardmon_Y.HM_Fault = true;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = false;

                    // Outport: '<Root>/MC_EN_HM'
                    Hardmon_Y.MC_EN_HM = false;
                }
                break;

              case Hardmon_IN_MC_FAULT_INIT:
                // During 'MC_FAULT_INIT': '<S1>:119'
                if ((Hardmon_U.uC_State != UC_State::MC_Off) &&
                        (Hardmon_U.uC_State != UC_State::Preset)) {
                    // Transition: '<S1>:121'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_DISCHARGE_FAULT;

                    // Outport: '<Root>/HM_Fault'
                    // Entry 'MC_DISCHARGE_FAULT': '<S1>:110'
                    Hardmon_Y.HM_Fault = true;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = true;
                } else if ((Hardmon_U.uC_State == UC_State::MC_Off) ||
                           (Hardmon_U.uC_State == UC_State::Preset)) {
                    // Transition: '<S1>:123'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_POWERDOWN1;

                    // Outport: '<Root>/HM_Fault'
                    // Entry 'MC_POWERDOWN1': '<S1>:111'
                    Hardmon_Y.HM_Fault = true;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = false;

                    // Outport: '<Root>/MC_EN_HM'
                    Hardmon_Y.MC_EN_HM = false;
                } else {
                    // no actions
                }
                break;

              case Hardmon_IN_MC_POWERDOWN:
                // Outport: '<Root>/Inverter_DIS'
                Hardmon_Y.Inverter_DIS = false;

                // Outport: '<Root>/MC_EN_HM'
                Hardmon_Y.MC_EN_HM = false;

                // During 'MC_POWERDOWN': '<S1>:15'
                if (!Hardmon_U.MC_12V0) {
                    // Transition: '<S1>:17'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_LVSS_Powerdown;

                    // Outport: '<Root>/LVSS_EN_HM'
                    // Entry 'LVSS_Powerdown': '<S1>:16'
                    Hardmon_Y.LVSS_EN_HM = false;
                }
                break;

              case Hardmon_IN_MC_POWERDOWN1:
                // Outport: '<Root>/Inverter_DIS'
                Hardmon_Y.Inverter_DIS = false;

                // Outport: '<Root>/MC_EN_HM'
                Hardmon_Y.MC_EN_HM = false;

                // During 'MC_POWERDOWN1': '<S1>:111'
                if (!Hardmon_U.MC_12V0) {
                    // Transition: '<S1>:115'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_LVSS_Powerdown1;

                    // Outport: '<Root>/LVSS_EN_HM'
                    // Entry 'LVSS_Powerdown1': '<S1>:114'
                    Hardmon_Y.LVSS_EN_HM = false;
                }
                break;

              case Hardmon_IN_MC_RESET_INIT:
                // During 'MC_RESET_INIT': '<S1>:125'
                if ((Hardmon_U.uC_State != UC_State::MC_Off) &&
                        (Hardmon_U.uC_State != UC_State::Preset)) {
                    // Transition: '<S1>:126'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_DISCHARGE;

                    // Outport: '<Root>/HM_Fault'
                    // Entry 'MC_DISCHARGE': '<S1>:33'
                    Hardmon_Y.HM_Fault = true;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = true;
                } else if ((Hardmon_U.uC_State == UC_State::MC_Off) ||
                           (Hardmon_U.uC_State == UC_State::Preset)) {
                    // Transition: '<S1>:127'
                    Hardmon_DW.is_c3_Hardmon = Hardmon_IN_MC_POWERDOWN;

                    // Outport: '<Root>/nReset'
                    // Entry 'MC_POWERDOWN': '<S1>:15'
                    Hardmon_Y.nReset = false;

                    // Outport: '<Root>/Inverter_DIS'
                    Hardmon_Y.Inverter_DIS = false;

                    // Outport: '<Root>/MC_EN_HM'
                    Hardmon_Y.MC_EN_HM = false;
                } else {
                    // no actions
                }
                break;

              default:
                Hardmon_Normal_Operation();
                break;
            }
        }

        // End of Chart: '<Root>/Hardmon_Chart'
    }

    // Model initialize function
    void Hardmon_Model::initialize()
    {
        // SystemInitialize for Outport: '<Root>/nReset' incorporates:
        //   Chart: '<Root>/Hardmon_Chart'

        Hardmon_Y.nReset = true;
    }

    // Model terminate function
    void Hardmon_Model::terminate()
    {
        // (no terminate code required)
    }

    // Constructor
    Hardmon_Model::Hardmon_Model() :
        Hardmon_U(),
        Hardmon_Y(),
        Hardmon_DW(),
        Hardmon_M()
    {
        // Currently there is no constructor body generated.
    }

    // Destructor
    // Currently there is no destructor body generated.
    Hardmon_Model::~Hardmon_Model() = default;

    // Real-Time Model get method
    Hardmon_Model::RT_MODEL_Hardmon_T * Hardmon_Model::getRTM()
    {
        return (&Hardmon_M);
    }
}

//
// File trailer for generated code.
//
// [EOF]
//
