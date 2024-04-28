//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.cpp
//
// Code generated for Simulink model 'MCuC_Model'.
//
// Model version                  : 1.14
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Thu Apr 25 19:50:25 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <models/MCuC_Model.hpp>

// Named constants for Chart: '<Root>/MCuC'
const uint8_t MCuC_Model_IN_A_CAN_fails{1U};

const uint8_t MCuC_Model_IN_A_Transmit{2U};

const uint8_t MCuC_Model_IN_CAN_A_ST{1U};

const uint8_t MCuC_Model_IN_Contactor_Closed{1U};

const uint8_t MCuC_Model_IN_Contactor_Open{2U};

const uint8_t MCuC_Model_IN_ESTOP{3U};

const uint8_t MCuC_Model_IN_ESTOP_Fails{1U};

const uint8_t MCuC_Model_IN_ESTOP_High{2U};

const uint8_t MCuC_Model_IN_ESTOP_Low{3U};

const uint8_t MCuC_Model_IN_ESTOP_ST{2U};

const uint8_t MCuC_Model_IN_Fault{4U};

const uint8_t MCuC_Model_IN_Fault_Ignit_H{5U};

const uint8_t MCuC_Model_IN_Ignition_Fails{1U};

const uint8_t MCuC_Model_IN_Ignition_High{2U};

const uint8_t MCuC_Model_IN_Ignition_Low{3U};

const uint8_t MCuC_Model_IN_Ignition_ST{3U};

const uint8_t MCuC_Model_IN_LVSS_MC_Shutdown{6U};

const uint8_t MCuC_Model_IN_LVSS_MC_Startup{7U};

const uint8_t MCuC_Model_IN_MC_Active{8U};

const uint8_t MCuC_Model_IN_MC_CAN_ST{4U};

const uint8_t MCuC_Model_IN_MC_CAN_fails{1U};

const uint8_t MCuC_Model_IN_MC_Discharging{9U};

const uint8_t MCuC_Model_IN_MC_Fails{1U};

const uint8_t MCuC_Model_IN_MC_Init{10U};

const uint8_t MCuC_Model_IN_MC_OFF{2U};

const uint8_t MCuC_Model_IN_MC_ON{3U};

const uint8_t MCuC_Model_IN_MC_Off{11U};

const uint8_t MCuC_Model_IN_MC_ST{5U};

const uint8_t MCuC_Model_IN_MC_Transmit{2U};

const uint8_t MCuC_Model_IN_NO_ACTIVE_CHILD{0U};

const uint8_t MCuC_Model_IN_Setup{12U};

const uint8_t MCuC_Model_IN_WD0{1U};

const uint8_t MCuC_Model_IN_WD1{2U};

namespace VCU {
// Function for Chart: '<Root>/MCuC'
void MCuC_Model::MCuC_Model_Setup(void) {
    MCuC_Model_B.MC_Self_Test = true;

    // During 'Setup': '<S1>:302'
    switch (MCuC_Model_DW.is_Setup) {
    case MCuC_Model_IN_CAN_A_ST:
        // During 'CAN_A_ST': '<S1>:420'
        if (static_cast<uint16_t>(MCuC_Model_DW.is_CAN_A_ST) != MCuC_Model_IN_A_CAN_fails) {
            MCuC_Model_B.CAN_A_out = true;

            // Inport: '<Root>/A_CAN_ST' incorporates:
            //   Inport: '<Root>/Ignition_3V3'
            //   Inport: '<Root>/LVSS_Status_3V3'
            //   Inport: '<Root>/MC_Status_3V3'

            // During 'A_Transmit': '<S1>:430'
            if (MCuC_Model_U.A_CAN_ST && (!MCuC_Model_U.MC_Status_3V3) && MCuC_Model_U.Ignition_3V3 && (!MCuC_Model_U.LVSS_Status_3V3)) {
                // Transition: '<S1>:320'
                MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_Setup = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;

                // Entry 'MC_Off': '<S1>:1'
                MCuC_Model_B.LVSS_EN_3V3 = false;
                MCuC_Model_B.uC_State = 0U;
                MCuC_Model_DW.uncharged = true;
                MCuC_Model_B.Inverter_EN_CAN = false;
                MCuC_Model_B.MC_Self_Test = false;
            } else if ((!MCuC_Model_U.A_CAN_ST) && (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i1) >= 150U)) {
                // Transition: '<S1>:434'
                MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_A_CAN_fails;

                // Entry 'A_CAN_fails': '<S1>:433'
                MCuC_Model_B.uC_State = 11U;
            } else {
                // no actions
            }

            // End of Inport: '<Root>/A_CAN_ST'
        } else {
            // During 'A_CAN_fails': '<S1>:433'
        }
        break;

    case MCuC_Model_IN_ESTOP_ST:
        // During 'ESTOP_ST': '<S1>:389'
        switch (MCuC_Model_DW.is_ESTOP_ST) {
        case MCuC_Model_IN_ESTOP_Fails:
            // During 'ESTOP_Fails': '<S1>:408'
            break;

        case MCuC_Model_IN_ESTOP_High:
            MCuC_Model_B.ESTOP_Self_Test = false;

            // Inport: '<Root>/ESTOP_3V3'
            // During 'ESTOP_High': '<S1>:401'
            if (!MCuC_Model_U.ESTOP_3V3) {
                // Transition: '<S1>:407'
                MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Fails;

                // Entry 'ESTOP_Fails': '<S1>:408'
                MCuC_Model_B.uC_State = 13U;
            } else {
                // Transition: '<S1>:405'
                MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_CAN_ST;

                // Entry Internal 'MC_CAN_ST': '<S1>:419'
                // Transition: '<S1>:426'
                MCuC_Model_DW.temporalCounter_i1 = 0U;
                MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_MC_Transmit;

                // Entry 'MC_Transmit': '<S1>:421'
                MCuC_Model_B.MC_CAN_out = true;
            }
            break;

        default:
            MCuC_Model_B.ESTOP_Self_Test = true;

            // Inport: '<Root>/ESTOP_3V3'
            // During 'ESTOP_Low': '<S1>:403'
            if (!MCuC_Model_U.ESTOP_3V3) {
                // Transition: '<S1>:406'
                MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_High;

                // Entry 'ESTOP_High': '<S1>:401'
                MCuC_Model_B.ESTOP_Self_Test = false;
            } else {
                // Transition: '<S1>:402'
                MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Fails;

                // Entry 'ESTOP_Fails': '<S1>:408'
                MCuC_Model_B.uC_State = 13U;
            }
            break;
        }
        break;

    case MCuC_Model_IN_Ignition_ST:
        // During 'Ignition_ST': '<S1>:378'
        switch (MCuC_Model_DW.is_Ignition_ST) {
        case MCuC_Model_IN_Ignition_Fails:
            // During 'Ignition_Fails': '<S1>:384'
            break;

        case MCuC_Model_IN_Ignition_High:
            MCuC_Model_B.Ignition_Self_Test = false;

            // Inport: '<Root>/Ignition_3V3'
            // During 'Ignition_High': '<S1>:382'
            if (!MCuC_Model_U.Ignition_3V3) {
                // Transition: '<S1>:386'
                MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Fails;

                // Entry 'Ignition_Fails': '<S1>:384'
                MCuC_Model_B.uC_State = 14U;
            } else {
                // Transition: '<S1>:388'
                MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_Setup = MCuC_Model_IN_ESTOP_ST;

                // Entry Internal 'ESTOP_ST': '<S1>:389'
                // Transition: '<S1>:414'
                MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Low;

                // Entry 'ESTOP_Low': '<S1>:403'
                MCuC_Model_B.ESTOP_Self_Test = true;
            }
            break;

        default:
            MCuC_Model_B.Ignition_Self_Test = true;

            // Inport: '<Root>/Ignition_3V3'
            // During 'Ignition_Low': '<S1>:379'
            if (!MCuC_Model_U.Ignition_3V3) {
                // Transition: '<S1>:383'
                MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_High;

                // Entry 'Ignition_High': '<S1>:382'
                MCuC_Model_B.Ignition_Self_Test = false;
            } else {
                // Transition: '<S1>:385'
                MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Fails;

                // Entry 'Ignition_Fails': '<S1>:384'
                MCuC_Model_B.uC_State = 14U;
            }
            break;
        }
        break;

    case MCuC_Model_IN_MC_CAN_ST:
        // During 'MC_CAN_ST': '<S1>:419'
        if (static_cast<uint16_t>(MCuC_Model_DW.is_MC_CAN_ST) != MCuC_Model_IN_MC_CAN_fails) {
            MCuC_Model_B.MC_CAN_out = true;

            // Inport: '<Root>/MC_CAN_ST'
            // During 'MC_Transmit': '<S1>:421'
            if ((!MCuC_Model_U.MC_CAN_ST) && (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i1) >= 150U)) {
                // Transition: '<S1>:428'
                MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_MC_CAN_fails;

                // Entry 'MC_CAN_fails': '<S1>:427'
                MCuC_Model_B.uC_State = 12U;
            } else if (MCuC_Model_U.MC_CAN_ST) {
                // Transition: '<S1>:432'
                MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_Setup = MCuC_Model_IN_CAN_A_ST;

                // Entry Internal 'CAN_A_ST': '<S1>:420'
                // Transition: '<S1>:431'
                MCuC_Model_DW.temporalCounter_i1 = 0U;
                MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_A_Transmit;

                // Entry 'A_Transmit': '<S1>:430'
                MCuC_Model_B.CAN_A_out = true;
            } else {
                // no actions
            }

            // End of Inport: '<Root>/MC_CAN_ST'
        } else {
            // During 'MC_CAN_fails': '<S1>:427'
        }
        break;

    default:
        // During 'MC_ST': '<S1>:361'
        switch (MCuC_Model_DW.is_MC_ST) {
        case MCuC_Model_IN_MC_Fails:
            // During 'MC_Fails': '<S1>:372'
            break;

        case MCuC_Model_IN_MC_OFF:
            // Inport: '<Root>/MC_Status_3V3'
            // During 'MC_OFF': '<S1>:370'
            if (MCuC_Model_U.MC_Status_3V3 && (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i1) >= 10U)) {
                // Transition: '<S1>:374'
                MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_Fails;

                // Entry 'MC_Fails': '<S1>:372'
                MCuC_Model_B.uC_State = 15U;
                MCuC_Model_B.MC_TOG_P_uC = false;
                MCuC_Model_B.MC_TOG_N_uC = false;
            } else if (!MCuC_Model_U.MC_Status_3V3) {
                // Transition: '<S1>:376'
                MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
                MCuC_Model_DW.is_Setup = MCuC_Model_IN_Ignition_ST;

                // Entry Internal 'Ignition_ST': '<S1>:378'
                // Transition: '<S1>:380'
                MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Low;

                // Entry 'Ignition_Low': '<S1>:379'
                MCuC_Model_B.Ignition_Self_Test = true;
            } else {
                // no actions
            }
            break;

        default:
            // Inport: '<Root>/MC_Status_3V3'
            // During 'MC_ON': '<S1>:363'
            if (MCuC_Model_U.MC_Status_3V3) {
                // Transition: '<S1>:371'
                MCuC_Model_DW.temporalCounter_i1 = 0U;
                MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_OFF;

                // Entry 'MC_OFF': '<S1>:370'
                MCuC_Model_B.MC_TOG_P_uC = false;
                MCuC_Model_B.MC_TOG_N_uC = true;
            } else if ((!MCuC_Model_U.MC_Status_3V3) && (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i1) >= 10U)) {
                // Transition: '<S1>:373'
                MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_Fails;

                // Entry 'MC_Fails': '<S1>:372'
                MCuC_Model_B.uC_State = 15U;
                MCuC_Model_B.MC_TOG_P_uC = false;
                MCuC_Model_B.MC_TOG_N_uC = false;
            } else {
                // no actions
            }
            break;
        }
        break;
    }
}

// Function for Chart: '<Root>/MCuC'
void MCuC_Model::MCuC_M_exit_internal_MCuC_Logic(void) {
    // Exit Internal 'MCuC_Logic': '<S1>:197'
    switch (MCuC_Model_DW.is_MCuC_Logic) {
    case MCuC_Model_IN_Fault:
        // Exit 'Fault': '<S1>:3'
        MCuC_Model_B.Fault = false;
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
        break;

    case MCuC_Model_IN_LVSS_MC_Shutdown:
        // Exit 'LVSS_MC_Shutdown': '<S1>:353'
        MCuC_Model_B.MC_TOG_P_uC = false;
        MCuC_Model_B.MC_TOG_N_uC = false;
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
        break;

    case MCuC_Model_IN_LVSS_MC_Startup:
        // Exit 'LVSS_MC_Startup': '<S1>:350'
        MCuC_Model_B.MC_TOG_P_uC = false;
        MCuC_Model_B.MC_TOG_N_uC = false;
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
        break;

    case MCuC_Model_IN_MC_Active:
        // Outport: '<Root>/Torque_Request'
        // Exit 'MC_Active': '<S1>:9'
        MCuC_Model_Y.Torque_Request = 0;
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
        break;

    default:
        // Exit Internal 'Setup': '<S1>:302'
        // Exit Internal 'CAN_A_ST': '<S1>:420'
        MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'ESTOP_ST': '<S1>:389'
        MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'Ignition_ST': '<S1>:378'
        MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'MC_CAN_ST': '<S1>:419'
        MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;

        // Exit Internal 'MC_ST': '<S1>:361'
        MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
        MCuC_Model_DW.is_Setup = MCuC_Model_IN_NO_ACTIVE_CHILD;
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
        break;
    }
}

// Function for Chart: '<Root>/MCuC'
void MCuC_Model::MCuC_Model_MCuC(bool* Watchdog) {
    bool guard1;
    bool tmp;
    bool tmp_0;
    bool tmp_1;

    // During 'MCuC': '<S1>:206'
    // During 'MCuC_Logic': '<S1>:197'
    tmp = !MCuC_Model_DW.uncharged;

    // Inport: '<Root>/Ignition_3V3'
    tmp_1 = !MCuC_Model_U.Ignition_3V3;

    // Inport: '<Root>/HM_Fault'
    guard1 = false;
    if (MCuC_Model_U.HM_Fault && tmp_1 && tmp && (!MCuC_Model_DW.Fault_Activated)) {
        // Transition: '<S1>:356'
        MCuC_Model_DW.Fault_Activated = true;
        guard1 = true;
    } else {
        // Inport: '<Root>/ESTOP_3V3'
        tmp_0 = !MCuC_Model_U.ESTOP_3V3;
        if (tmp_0 && tmp && (!MCuC_Model_DW.ESTOP_activated)) {
            // Transition: '<S1>:199'
            MCuC_Model_DW.ESTOP_activated = true;
            guard1 = true;

            // Inport: '<Root>/nReset'
        } else if (tmp_0 && MCuC_Model_DW.uncharged && MCuC_Model_U.nReset && (static_cast<int16_t>(MCuC_Model_B.uC_State) != 10)) {
            // Transition: '<S1>:200'
            MCuC_M_exit_internal_MCuC_Logic();
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_ESTOP;

            // Entry 'ESTOP': '<S1>:4'
            MCuC_Model_B.uC_State = 7U;
            MCuC_Model_B.LVSS_EN_3V3 = false;
            MCuC_Model_DW.ESTOP_activated = false;
        } else if (MCuC_Model_U.HM_Fault && MCuC_Model_DW.uncharged && (static_cast<int16_t>(MCuC_Model_B.uC_State) != 10) && MCuC_Model_U.nReset) {
            // Transition: '<S1>:201'
            MCuC_M_exit_internal_MCuC_Logic();
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Fault;

            // Entry 'Fault': '<S1>:3'
            MCuC_Model_B.uC_State = 6U;
            MCuC_Model_DW.Fault_Activated = false;
            MCuC_Model_B.Fault = true;
        } else if (!MCuC_Model_U.nReset) {
            // Transition: '<S1>:304'
            MCuC_M_exit_internal_MCuC_Logic();
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Setup;

            // Entry 'Setup': '<S1>:302'
            MCuC_Model_B.uC_State = 10U;
            MCuC_Model_B.MC_Self_Test = true;

            // Entry Internal 'Setup': '<S1>:302'
            // Transition: '<S1>:368'
            MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_ST;

            // Entry 'MC_ST': '<S1>:361'
            // Entry Internal 'MC_ST': '<S1>:361'
            // Transition: '<S1>:369'
            MCuC_Model_DW.temporalCounter_i1 = 0U;
            MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_ON;

            // Entry 'MC_ON': '<S1>:363'
            MCuC_Model_B.MC_TOG_P_uC = true;
            MCuC_Model_B.MC_TOG_N_uC = false;
        } else {
            switch (MCuC_Model_DW.is_MCuC_Logic) {
            case MCuC_Model_IN_Contactor_Closed:
                MCuC_Model_B.Inverter_EN_CAN = false;

                // Inport: '<Root>/Start_CAN' incorporates:
                //   Inport: '<Root>/Brake_CAN'
                //   Inport: '<Root>/Forward_EN_CAN'
                //   Inport: '<Root>/Ignition_3V3'
                //   Inport: '<Root>/Throttle_CAN'

                // During 'Contactor_Closed': '<S1>:8'
                if (MCuC_Model_U.Start_CAN && MCuC_Model_U.Forward_EN_CAN && MCuC_Model_U.Brake_CAN && (MCuC_Model_U.Throttle_CAN == 0)) {
                    // Transition: '<S1>:17'
                    MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Active;

                    // Entry 'MC_Active': '<S1>:9'
                    MCuC_Model_B.Inverter_EN_CAN = true;
                    MCuC_Model_B.uC_State = 3U;

                    // Outport: '<Root>/Torque_Request'
                    MCuC_Model_Y.Torque_Request = 0;
                } else if (MCuC_Model_U.Ignition_3V3) {
                    // Transition: '<S1>:195'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Contactor_Open;

                    // Entry 'Contactor_Open': '<S1>:7'
                    MCuC_Model_B.Inverter_DIS_CAN = true;
                    MCuC_Model_B.uC_State = 4U;
                } else {
                    // no actions
                }

                // End of Inport: '<Root>/Start_CAN'
                break;

            case MCuC_Model_IN_Contactor_Open:
                MCuC_Model_B.Inverter_EN_CAN = false;
                MCuC_Model_B.Inverter_DIS_CAN = true;

                // Inport: '<Root>/Discharge_SM_CAN'
                // During 'Contactor_Open': '<S1>:7'
                if (static_cast<int16_t>(MCuC_Model_U.Discharge_SM_CAN) != 0) {
                    // Transition: '<S1>:42'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_MC_Discharging;

                    // Entry 'MC_Discharging': '<S1>:5'
                    MCuC_Model_B.uC_State = 5U;
                }
                break;

            case MCuC_Model_IN_ESTOP:
                MCuC_Model_B.LVSS_EN_3V3 = false;

                // Inport: '<Root>/ESTOP_3V3'
                // During 'ESTOP': '<S1>:4'
                if (MCuC_Model_U.ESTOP_3V3) {
                    // Transition: '<S1>:21'
                    MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;

                    // Entry 'MC_Off': '<S1>:1'
                    MCuC_Model_B.uC_State = 0U;
                    MCuC_Model_DW.uncharged = true;
                    MCuC_Model_B.Inverter_EN_CAN = false;
                    MCuC_Model_B.MC_Self_Test = false;
                }
                break;

            case MCuC_Model_IN_Fault:
                // Inport: '<Root>/MC_Status_3V3'
                // During 'Fault': '<S1>:3'
                if ((!MCuC_Model_U.MC_Status_3V3) && tmp_1) {
                    // Transition: '<S1>:359'
                    // Exit 'Fault': '<S1>:3'
                    MCuC_Model_B.Fault = false;
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Fault_Ignit_H;
                }
                break;

            case MCuC_Model_IN_Fault_Ignit_H:
                // Inport: '<Root>/Ignition_3V3'
                // During 'Fault_Ignit_H': '<S1>:358'
                if (MCuC_Model_U.Ignition_3V3) {
                    // Transition: '<S1>:287'
                    MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;

                    // Entry 'MC_Off': '<S1>:1'
                    MCuC_Model_B.LVSS_EN_3V3 = false;
                    MCuC_Model_B.uC_State = 0U;
                    MCuC_Model_DW.uncharged = true;
                    MCuC_Model_B.Inverter_EN_CAN = false;
                    MCuC_Model_B.MC_Self_Test = false;
                }
                break;

            case MCuC_Model_IN_LVSS_MC_Shutdown:
                MCuC_Model_B.LVSS_EN_3V3 = false;
                MCuC_Model_B.Inverter_DIS_CAN = false;

                // Inport: '<Root>/MC_Status_3V3' incorporates:
                //   Inport: '<Root>/LVSS_Status_3V3'

                // During 'LVSS_MC_Shutdown': '<S1>:353'
                if ((!MCuC_Model_U.MC_Status_3V3) && (!MCuC_Model_U.LVSS_Status_3V3)) {
                    // Transition: '<S1>:354'
                    // Exit 'LVSS_MC_Shutdown': '<S1>:353'
                    MCuC_Model_B.MC_TOG_P_uC = false;
                    MCuC_Model_B.MC_TOG_N_uC = false;
                    MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;

                    // Entry 'MC_Off': '<S1>:1'
                    MCuC_Model_B.uC_State = 0U;
                    MCuC_Model_DW.uncharged = true;
                    MCuC_Model_B.Inverter_EN_CAN = false;
                    MCuC_Model_B.MC_Self_Test = false;
                }
                break;

            case MCuC_Model_IN_LVSS_MC_Startup:
                MCuC_Model_B.LVSS_EN_3V3 = true;

                // Inport: '<Root>/MC_Status_3V3' incorporates:
                //   Inport: '<Root>/LVSS_Status_3V3'

                // During 'LVSS_MC_Startup': '<S1>:350'
                if (MCuC_Model_U.MC_Status_3V3 && MCuC_Model_U.LVSS_Status_3V3) {
                    // Transition: '<S1>:351'
                    // Exit 'LVSS_MC_Startup': '<S1>:350'
                    MCuC_Model_B.MC_TOG_P_uC = false;
                    MCuC_Model_B.MC_TOG_N_uC = false;
                    MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Init;

                    // Entry 'MC_Init': '<S1>:6'
                    MCuC_Model_B.uC_State = 1U;
                }
                break;

            case MCuC_Model_IN_MC_Active:
                MCuC_Model_B.Inverter_EN_CAN = true;

                // Inport: '<Root>/Forward_EN_CAN' incorporates:
                //   Inport: '<Root>/Ignition_3V3'

                // During 'MC_Active': '<S1>:9'
                if (!MCuC_Model_U.Forward_EN_CAN) {
                    // Outport: '<Root>/Torque_Request'
                    // Transition: '<S1>:243'
                    // Exit 'MC_Active': '<S1>:9'
                    MCuC_Model_Y.Torque_Request = 0;
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Contactor_Closed;

                    // Entry 'Contactor_Closed': '<S1>:8'
                    MCuC_Model_B.uC_State = 2U;
                    MCuC_Model_B.Inverter_EN_CAN = false;
                } else if (MCuC_Model_U.Ignition_3V3) {
                    // Outport: '<Root>/Torque_Request'
                    // Transition: '<S1>:242'
                    // Exit 'MC_Active': '<S1>:9'
                    MCuC_Model_Y.Torque_Request = 0;
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Contactor_Open;

                    // Entry 'Contactor_Open': '<S1>:7'
                    MCuC_Model_B.Inverter_EN_CAN = false;
                    MCuC_Model_B.Inverter_DIS_CAN = true;
                    MCuC_Model_B.uC_State = 4U;
                } else {
                    // Outport: '<Root>/Torque_Request' incorporates:
                    //   Inport: '<Root>/Throttle_CAN'

                    MCuC_Model_Y.Torque_Request = MCuC_Model_U.Throttle_CAN;
                }
                break;

            case MCuC_Model_IN_MC_Discharging:
                // Inport: '<Root>/Discharge_SM_CAN'
                // During 'MC_Discharging': '<S1>:5'
                if (static_cast<int16_t>(MCuC_Model_U.Discharge_SM_CAN) == 4) {
                    // Transition: '<S1>:250'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_LVSS_MC_Shutdown;

                    // Entry 'LVSS_MC_Shutdown': '<S1>:353'
                    MCuC_Model_B.uC_State = 9U;
                    MCuC_Model_B.LVSS_EN_3V3 = false;
                    MCuC_Model_B.Inverter_DIS_CAN = false;
                    MCuC_Model_B.MC_TOG_P_uC = false;
                    MCuC_Model_B.MC_TOG_N_uC = true;
                }
                break;

            case MCuC_Model_IN_MC_Init:
                // Inport: '<Root>/Ignition_3V3' incorporates:
                //   Inport: '<Root>/MC_SM_CAN'

                // During 'MC_Init': '<S1>:6'
                if (MCuC_Model_U.Ignition_3V3) {
                    // Transition: '<S1>:15'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Contactor_Open;

                    // Entry 'Contactor_Open': '<S1>:7'
                    MCuC_Model_B.Inverter_EN_CAN = false;
                    MCuC_Model_B.Inverter_DIS_CAN = true;
                    MCuC_Model_B.uC_State = 4U;
                } else if (static_cast<int16_t>(MCuC_Model_U.MC_SM_CAN) == 5) {
                    // Transition: '<S1>:12'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_Contactor_Closed;

                    // Entry 'Contactor_Closed': '<S1>:8'
                    MCuC_Model_B.uC_State = 2U;
                    MCuC_Model_B.Inverter_EN_CAN = false;
                } else {
                    // no actions
                }
                break;

            case MCuC_Model_IN_MC_Off:
                MCuC_Model_B.LVSS_EN_3V3 = false;
                MCuC_Model_B.Inverter_EN_CAN = false;
                MCuC_Model_B.MC_Self_Test = false;

                // During 'MC_Off': '<S1>:1'
                if (tmp_1) {
                    // Transition: '<S1>:11'
                    MCuC_Model_DW.is_MCuC_Logic =
                        MCuC_Model_IN_LVSS_MC_Startup;

                    // Entry 'LVSS_MC_Startup': '<S1>:350'
                    MCuC_Model_DW.uncharged = false;
                    MCuC_Model_B.uC_State = 8U;
                    MCuC_Model_B.LVSS_EN_3V3 = true;
                    MCuC_Model_B.MC_TOG_P_uC = true;
                    MCuC_Model_B.MC_TOG_N_uC = false;
                }
                break;

            default:
                MCuC_Model_Setup();
                break;
            }

            // End of Inport: '<Root>/nReset'
        }
    }

    if (guard1) {
        MCuC_M_exit_internal_MCuC_Logic();
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Open;

        // Entry 'Contactor_Open': '<S1>:7'
        MCuC_Model_B.Inverter_EN_CAN = false;
        MCuC_Model_B.Inverter_DIS_CAN = true;
        MCuC_Model_B.uC_State = 4U;
    }

    // End of Inport: '<Root>/HM_Fault'
    // During 'Watchdog': '<S1>:211'
    if (static_cast<uint16_t>(MCuC_Model_DW.is_Watchdog) == MCuC_Model_IN_WD0) {
        *Watchdog = false;

        // During 'WD0': '<S1>:210'
        if (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i2) >= 3U) {
            // Transition: '<S1>:208'
            MCuC_Model_DW.temporalCounter_i2 = 0U;
            MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD1;

            // Entry 'WD1': '<S1>:209'
            *Watchdog = true;
        }
    } else {
        *Watchdog = true;

        // During 'WD1': '<S1>:209'
        if (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i2) >= 3U) {
            // Transition: '<S1>:207'
            MCuC_Model_DW.temporalCounter_i2 = 0U;
            MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD0;

            // Entry 'WD0': '<S1>:210'
            *Watchdog = false;
        }
    }
}
}// namespace VCU

namespace VCU {
// Model step function
void MCuC_Model::step() {
    // Outport: '<Root>/LVSS_EN_uC' incorporates:
    //   Memory: '<Root>/Memory5'

    MCuC_Model_Y.LVSS_EN_uC = MCuC_Model_DW.Memory5_PreviousInput;

    // Outport: '<Root>/Inverter_EN_CAN' incorporates:
    //   Memory: '<Root>/Memory2'

    MCuC_Model_Y.Inverter_EN_CAN = MCuC_Model_DW.Memory2_PreviousInput;

    // Outport: '<Root>/Fault' incorporates:
    //   Memory: '<Root>/Memory'

    MCuC_Model_Y.Fault = MCuC_Model_DW.Memory_PreviousInput;

    // Outport: '<Root>/Watchdog' incorporates:
    //   Memory: '<Root>/Memory6'

    MCuC_Model_Y.Watchdog = MCuC_Model_DW.Memory6_PreviousInput;

    // Outport: '<Root>/uC_State' incorporates:
    //   Memory: '<Root>/Memory4'

    MCuC_Model_Y.uC_State = MCuC_Model_DW.Memory4_PreviousInput;

    // Outport: '<Root>/Inverter_DIS_CAN' incorporates:
    //   Memory: '<Root>/Memory1'

    MCuC_Model_Y.Inverter_DIS_CAN = MCuC_Model_DW.Memory1_PreviousInput;

    // Outport: '<Root>/MC_TOGp_uC' incorporates:
    //   Memory: '<Root>/Memory8'

    MCuC_Model_Y.MC_TOGp_uC = MCuC_Model_DW.Memory8_PreviousInput;

    // Outport: '<Root>/MC_TOGn_uC' incorporates:
    //   Memory: '<Root>/Memory3'

    MCuC_Model_Y.MC_TOGn_uC = MCuC_Model_DW.Memory3_PreviousInput;

    // Chart: '<Root>/MCuC'
    // Gateway: MCuC
    if (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i1) < 255U) {
        MCuC_Model_DW.temporalCounter_i1 = static_cast<uint8_t>(static_cast<int16_t>(static_cast<int16_t>(MCuC_Model_DW.temporalCounter_i1) + 1));
    }

    if (static_cast<uint16_t>(MCuC_Model_DW.temporalCounter_i2) < 3U) {
        MCuC_Model_DW.temporalCounter_i2 = static_cast<uint8_t>(static_cast<int16_t>(static_cast<int16_t>(MCuC_Model_DW.temporalCounter_i2) + 1));
    }

    // During: MCuC
    if (static_cast<uint16_t>(MCuC_Model_DW.is_active_c2_MCuC_Model) == 0U) {
        // Entry: MCuC
        MCuC_Model_DW.is_active_c2_MCuC_Model = 1U;

        // Entry Internal: MCuC
        // Entry Internal 'MCuC': '<S1>:206'
        // Entry Internal 'MCuC_Logic': '<S1>:197'
        // Transition: '<S1>:2'
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Setup;

        // Entry 'Setup': '<S1>:302'
        MCuC_Model_B.uC_State = 10U;
        MCuC_Model_B.MC_Self_Test = true;

        // Entry Internal 'Setup': '<S1>:302'
        // Transition: '<S1>:368'
        MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_ST;

        // Entry 'MC_ST': '<S1>:361'
        // Entry Internal 'MC_ST': '<S1>:361'
        // Transition: '<S1>:369'
        MCuC_Model_DW.temporalCounter_i1 = 0U;
        MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_ON;

        // Entry 'MC_ON': '<S1>:363'
        MCuC_Model_B.MC_TOG_P_uC = true;
        MCuC_Model_B.MC_TOG_N_uC = false;

        // Entry Internal 'Watchdog': '<S1>:211'
        // Transition: '<S1>:212'
        MCuC_Model_DW.temporalCounter_i2 = 0U;
        MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD0;

        // Update for Memory: '<Root>/Memory6'
        // Entry 'WD0': '<S1>:210'
        MCuC_Model_DW.Memory6_PreviousInput = false;
    } else {
        // Update for Memory: '<Root>/Memory6'
        MCuC_Model_MCuC(&MCuC_Model_DW.Memory6_PreviousInput);
    }

    // End of Chart: '<Root>/MCuC'

    // Outport: '<Root>/MC_Self_Test' incorporates:
    //   Memory: '<Root>/Memory7'

    MCuC_Model_Y.MC_Self_Test = MCuC_Model_DW.Memory7_PreviousInput;

    // Outport: '<Root>/ESTOP_Self_Test' incorporates:
    //   Memory: '<Root>/Memory9'

    MCuC_Model_Y.ESTOP_Self_Test = MCuC_Model_DW.Memory9_PreviousInput;

    // Outport: '<Root>/Ignition_Self_Test1' incorporates:
    //   Memory: '<Root>/Memory10'

    MCuC_Model_Y.Ignition_Self_Test1 = MCuC_Model_DW.Memory10_PreviousInput;

    // Outport: '<Root>/CAN_A_out' incorporates:
    //   Memory: '<Root>/Memory11'

    MCuC_Model_Y.CAN_A_out = MCuC_Model_DW.Memory11_PreviousInput;

    // Outport: '<Root>/MC_CAN_out' incorporates:
    //   Memory: '<Root>/Memory12'

    MCuC_Model_Y.MC_CAN_out = MCuC_Model_DW.Memory12_PreviousInput;

    // Update for Memory: '<Root>/Memory5'
    MCuC_Model_DW.Memory5_PreviousInput = MCuC_Model_B.LVSS_EN_3V3;

    // Update for Memory: '<Root>/Memory2'
    MCuC_Model_DW.Memory2_PreviousInput = MCuC_Model_B.Inverter_EN_CAN;

    // Update for Memory: '<Root>/Memory'
    MCuC_Model_DW.Memory_PreviousInput = MCuC_Model_B.Fault;

    // Update for Memory: '<Root>/Memory4'
    MCuC_Model_DW.Memory4_PreviousInput = MCuC_Model_B.uC_State;

    // Update for Memory: '<Root>/Memory1'
    MCuC_Model_DW.Memory1_PreviousInput = MCuC_Model_B.Inverter_DIS_CAN;

    // Update for Memory: '<Root>/Memory8'
    MCuC_Model_DW.Memory8_PreviousInput = MCuC_Model_B.MC_TOG_P_uC;

    // Update for Memory: '<Root>/Memory3'
    MCuC_Model_DW.Memory3_PreviousInput = MCuC_Model_B.MC_TOG_N_uC;

    // Update for Memory: '<Root>/Memory7'
    MCuC_Model_DW.Memory7_PreviousInput = MCuC_Model_B.MC_Self_Test;

    // Update for Memory: '<Root>/Memory9'
    MCuC_Model_DW.Memory9_PreviousInput = MCuC_Model_B.ESTOP_Self_Test;

    // Update for Memory: '<Root>/Memory10'
    MCuC_Model_DW.Memory10_PreviousInput = MCuC_Model_B.Ignition_Self_Test;

    // Update for Memory: '<Root>/Memory11'
    MCuC_Model_DW.Memory11_PreviousInput = MCuC_Model_B.CAN_A_out;

    // Update for Memory: '<Root>/Memory12'
    MCuC_Model_DW.Memory12_PreviousInput = MCuC_Model_B.MC_CAN_out;
}

// Model initialize function
void MCuC_Model::initialize() {
    // SystemInitialize for Chart: '<Root>/MCuC'
    MCuC_Model_DW.uncharged = true;
}

// Model terminate function
void MCuC_Model::terminate() {
    // (no terminate code required)
}

// Constructor
MCuC_Model::MCuC_Model() : MCuC_Model_U(),
                           MCuC_Model_Y(),
                           MCuC_Model_B(),
                           MCuC_Model_DW(),
                           MCuC_Model_M() {
    // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
MCuC_Model::~MCuC_Model() = default;

// Real-Time Model get method
MCuC_Model::RT_MODEL_MCuC_Model_T* MCuC_Model::getRTM() {
    return (&MCuC_Model_M);
}
}// namespace VCU

//
// File trailer for generated code.
//
// [EOF]
//
