//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC.cpp
//
// Code generated for Simulink model 'MCuC'.
//
// Model version                  : 1.18
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Sat Feb 15 11:21:59 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <models/MCuC_Model.hpp>
#include <stdbool.h>
#include <stdint.h>

// Named constants for Chart: '<Root>/MCuC_Chart'
const uint8_t MCuC_IN_Contactor_Closed{1U};

const uint8_t MCuC_IN_Contactor_Open{2U};

const uint8_t MCuC_IN_ESTOP{3U};

const uint8_t MCuC_IN_Fault{4U};

const uint8_t MCuC_IN_Fault_Ignit_H{5U};

const uint8_t MCuC_IN_LVSS_MC_Shutdown{6U};

const uint8_t MCuC_IN_LVSS_MC_Startup{7U};

const uint8_t MCuC_IN_MC_Active{8U};

const uint8_t MCuC_IN_MC_Discharging{9U};

const uint8_t MCuC_IN_MC_EN_OFF{1U};

const uint8_t MCuC_IN_MC_EN_ON{2U};

const uint8_t MCuC_IN_MC_Init{10U};

const uint8_t MCuC_IN_MC_Off{11U};

const uint8_t MCuC_IN_NO_ACTIVE_CHILD{0U};

const uint8_t MCuC_IN_Preset{12U};

const uint8_t MCuC_IN_WD0{1U};

const uint8_t MCuC_IN_WD1{2U};

namespace vcu {
// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_Preset(void) {
    // Outport: '<Root>/uC_State'
    MCuC_Y.uC_State = UC_State::Preset;

    // Inport: '<Root>/MC_12V0' incorporates:
    //   Inport: '<Root>/Ignition_LS_A'
    //   Inport: '<Root>/LVSS_ON'

    // During 'Preset': '<S1>:302'
    if ((!MCuC_U.MC_12V0) && (!MCuC_U.Ignition_LS_A) && (!MCuC_U.LVSS_ON)) {
        // Outport: '<Root>/MC_Self_Test'
        // Transition: '<S1>:320'
        MCuC_Y.MC_Self_Test = false;

        // Exit Internal 'Preset': '<S1>:302'
        // Exit Internal 'MC_ST': '<S1>:361'
        MCuC_DW.is_MC_ST = MCuC_IN_NO_ACTIVE_CHILD;
        MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Off;

        // Outport: '<Root>/LVSS_EN_uC'
        // Entry 'MC_Off': '<S1>:1'
        MCuC_Y.LVSS_EN_uC = false;

        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::MC_Off;
        MCuC_DW.uncharged = true;

        // Outport: '<Root>/Inverter_EN'
        MCuC_Y.Inverter_EN = false;

        // During 'MC_ST': '<S1>:361'
    } else if (static_cast<uint16_t>(MCuC_DW.is_MC_ST) == MCuC_IN_MC_EN_OFF) {
        // Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC = false;

        // During 'MC_EN_OFF': '<S1>:366'
    } else {
        // Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC = true;

        // During 'MC_EN_ON': '<S1>:364'
        if (MCuC_U.MC_12V0) {
            // Transition: '<S1>:368'
            MCuC_DW.is_MC_ST = MCuC_IN_MC_EN_OFF;

            // Outport: '<Root>/MC_EN_uC'
            // Entry 'MC_EN_OFF': '<S1>:366'
            MCuC_Y.MC_EN_uC = false;
        }
    }

    // End of Inport: '<Root>/MC_12V0'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_exit_internal_MCuC_Logic(void) {
    // Exit Internal 'MCuC_Logic': '<S1>:197'
    switch (MCuC_DW.is_MCuC_Logic) {
    case MCuC_IN_Fault:
        // Outport: '<Root>/Fault'
        // Exit 'Fault': '<S1>:3'
        MCuC_Y.Fault = false;
        MCuC_DW.is_MCuC_Logic = MCuC_IN_NO_ACTIVE_CHILD;
        break;

    case MCuC_IN_MC_Active:
        // Outport: '<Root>/Torque_Request'
        // Exit 'MC_Active': '<S1>:9'
        MCuC_Y.Torque_Request = 0;
        MCuC_DW.is_MCuC_Logic = MCuC_IN_NO_ACTIVE_CHILD;
        break;

    default:
        // Exit Internal 'Preset': '<S1>:302'
        // Exit Internal 'MC_ST': '<S1>:361'
        MCuC_DW.is_MC_ST = MCuC_IN_NO_ACTIVE_CHILD;
        MCuC_DW.is_MCuC_Logic = MCuC_IN_NO_ACTIVE_CHILD;
        break;
    }
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_MCuC(void) {
    bool guard1;
    bool tmp;

    // During 'MCuC': '<S1>:206'
    // During 'MCuC_Logic': '<S1>:197'
    tmp = !MCuC_DW.uncharged;

    // Inport: '<Root>/HM_Fault' incorporates:
    //   Inport: '<Root>/Ignition_LS_A'

    guard1 = false;
    if (MCuC_U.HM_Fault && MCuC_U.Ignition_LS_A && tmp && (!MCuC_DW.Fault_Activated)) {
        // Transition: '<S1>:356'
        MCuC_DW.Fault_Activated = true;
        guard1 = true;
    } else if (MCuC_U.ESTOP_LS_A && tmp && (!MCuC_DW.ESTOP_activated)) {
        // Transition: '<S1>:199'
        MCuC_DW.ESTOP_activated = true;
        guard1 = true;

        // Inport: '<Root>/ESTOP_LS_A' incorporates:
        //   Inport: '<Root>/Brake'
        //   Inport: '<Root>/Forward_EN'
        //   Inport: '<Root>/LVSS_ON'
        //   Inport: '<Root>/MC_12V0'
        //   Inport: '<Root>/MC_DC_State'
        //   Inport: '<Root>/MC_VSM_State'
        //   Inport: '<Root>/Start'
        //   Inport: '<Root>/Throttle'
        //   Inport: '<Root>/nReset'
        //   Outport: '<Root>/Fault'
        //   Outport: '<Root>/Inverter_DIS'
        //   Outport: '<Root>/Inverter_EN'
        //   Outport: '<Root>/LVSS_EN_uC'
        //   Outport: '<Root>/MC_EN_uC'
        //   Outport: '<Root>/MC_Self_Test'
        //   Outport: '<Root>/Torque_Request'
        //   Outport: '<Root>/uC_State'

    } else if (MCuC_U.ESTOP_LS_A && MCuC_DW.uncharged && MCuC_U.nReset && (MCuC_Y.uC_State != UC_State::Preset)) {
        // Transition: '<S1>:200'
        MCuC_exit_internal_MCuC_Logic();
        MCuC_DW.is_MCuC_Logic = MCuC_IN_ESTOP;

        // Entry 'ESTOP': '<S1>:4'
        MCuC_Y.uC_State = UC_State::Estop;
        MCuC_Y.LVSS_EN_uC = false;
        MCuC_DW.ESTOP_activated = false;
    } else if (MCuC_U.HM_Fault && MCuC_DW.uncharged && (MCuC_Y.uC_State != UC_State::Preset) && MCuC_U.nReset) {
        // Transition: '<S1>:201'
        MCuC_exit_internal_MCuC_Logic();
        MCuC_DW.is_MCuC_Logic = MCuC_IN_Fault;

        // Entry 'Fault': '<S1>:3'
        MCuC_Y.uC_State = UC_State::Fault;
        MCuC_DW.Fault_Activated = false;
        MCuC_Y.Fault = true;
    } else if (!MCuC_U.nReset) {
        // Transition: '<S1>:304'
        MCuC_exit_internal_MCuC_Logic();
        MCuC_DW.is_MCuC_Logic = MCuC_IN_Preset;

        // Entry 'Preset': '<S1>:302'
        MCuC_Y.uC_State = UC_State::Preset;

        // Entry Internal 'Preset': '<S1>:302'
        // Transition: '<S1>:362'
        // Entry 'MC_ST': '<S1>:361'
        MCuC_Y.MC_Self_Test = true;

        // Entry Internal 'MC_ST': '<S1>:361'
        // Transition: '<S1>:365'
        MCuC_DW.is_MC_ST = MCuC_IN_MC_EN_ON;

        // Entry 'MC_EN_ON': '<S1>:364'
        MCuC_Y.MC_EN_uC = true;
    } else {
        switch (MCuC_DW.is_MCuC_Logic) {
        case MCuC_IN_Contactor_Closed:
            MCuC_Y.uC_State = UC_State::Contactor_Closed;
            MCuC_Y.Inverter_EN = false;

            // During 'Contactor_Closed': '<S1>:8'
            if (MCuC_U.Start && MCuC_U.Forward_EN && MCuC_U.Brake && (MCuC_U.Throttle == 0)) {
                // Transition: '<S1>:17'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Active;

                // Entry 'MC_Active': '<S1>:9'
                MCuC_Y.Inverter_EN = true;
                MCuC_Y.uC_State = UC_State::MC_Active;
                MCuC_Y.Torque_Request = 0;
            } else if (!MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:195'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Open;

                // Entry 'Contactor_Open': '<S1>:7'
                MCuC_Y.Inverter_DIS = true;
                MCuC_Y.uC_State = UC_State::Contactor_Open;
            } else {
                // no actions
            }
            break;

        case MCuC_IN_Contactor_Open:
            MCuC_Y.Inverter_EN = false;
            MCuC_Y.Inverter_DIS = true;
            MCuC_Y.uC_State = UC_State::Contactor_Open;

            // During 'Contactor_Open': '<S1>:7'
            if (MCuC_U.MC_DC_State_p != MC_DC_State::Disabled) {
                // Transition: '<S1>:42'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Discharging;

                // Entry 'MC_Discharging': '<S1>:5'
                MCuC_Y.uC_State = UC_State::MC_Discharging;
            }
            break;

        case MCuC_IN_ESTOP:
            MCuC_Y.uC_State = UC_State::Estop;
            MCuC_Y.LVSS_EN_uC = false;

            // During 'ESTOP': '<S1>:4'
            if (!MCuC_U.ESTOP_LS_A) {
                // Transition: '<S1>:21'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Off;

                // Entry 'MC_Off': '<S1>:1'
                MCuC_Y.uC_State = UC_State::MC_Off;
                MCuC_DW.uncharged = true;
                MCuC_Y.Inverter_EN = false;
            }
            break;

        case MCuC_IN_Fault:
            MCuC_Y.uC_State = UC_State::Fault;

            // During 'Fault': '<S1>:3'
            if ((!MCuC_U.MC_12V0) && MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:359'
                // Exit 'Fault': '<S1>:3'
                MCuC_Y.Fault = false;
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Fault_Ignit_H;
            }
            break;

        case MCuC_IN_Fault_Ignit_H:
            // During 'Fault_Ignit_H': '<S1>:358'
            if (!MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:287'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Off;

                // Entry 'MC_Off': '<S1>:1'
                MCuC_Y.LVSS_EN_uC = false;
                MCuC_Y.uC_State = UC_State::MC_Off;
                MCuC_DW.uncharged = true;
                MCuC_Y.Inverter_EN = false;
            }
            break;

        case MCuC_IN_LVSS_MC_Shutdown:
            MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;
            MCuC_Y.LVSS_EN_uC = false;
            MCuC_Y.Inverter_DIS = false;
            MCuC_Y.MC_EN_uC = false;

            // During 'LVSS_MC_Shutdown': '<S1>:353'
            if ((!MCuC_U.MC_12V0) && (!MCuC_U.LVSS_ON)) {
                // Transition: '<S1>:354'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Off;

                // Entry 'MC_Off': '<S1>:1'
                MCuC_Y.uC_State = UC_State::MC_Off;
                MCuC_DW.uncharged = true;
                MCuC_Y.Inverter_EN = false;
            }
            break;

        case MCuC_IN_LVSS_MC_Startup:
            MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;
            MCuC_Y.LVSS_EN_uC = true;
            MCuC_Y.MC_EN_uC = true;

            // During 'LVSS_MC_Startup': '<S1>:350'
            if (MCuC_U.MC_12V0 && MCuC_U.LVSS_ON) {
                // Transition: '<S1>:351'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_MC_Init;

                // Entry 'MC_Init': '<S1>:6'
                MCuC_Y.uC_State = UC_State::MC_Init;
            }
            break;

        case MCuC_IN_MC_Active:
            MCuC_Y.Inverter_EN = true;
            MCuC_Y.uC_State = UC_State::MC_Active;

            // During 'MC_Active': '<S1>:9'
            if (!MCuC_U.Forward_EN) {
                // Transition: '<S1>:243'
                // Exit 'MC_Active': '<S1>:9'
                MCuC_Y.Torque_Request = 0;
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Closed;

                // Entry 'Contactor_Closed': '<S1>:8'
                MCuC_Y.uC_State = UC_State::Contactor_Closed;
                MCuC_Y.Inverter_EN = false;
            } else if (!MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:242'
                // Exit 'MC_Active': '<S1>:9'
                MCuC_Y.Torque_Request = 0;
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Open;

                // Entry 'Contactor_Open': '<S1>:7'
                MCuC_Y.Inverter_EN = false;
                MCuC_Y.Inverter_DIS = true;
                MCuC_Y.uC_State = UC_State::Contactor_Open;
            } else {
                MCuC_Y.Torque_Request = MCuC_U.Throttle;
            }
            break;

        case MCuC_IN_MC_Discharging:
            MCuC_Y.uC_State = UC_State::MC_Discharging;

            // During 'MC_Discharging': '<S1>:5'
            if (MCuC_U.MC_DC_State_p == MC_DC_State::Complete) {
                // Transition: '<S1>:250'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_LVSS_MC_Shutdown;

                // Entry 'LVSS_MC_Shutdown': '<S1>:353'
                MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;
                MCuC_Y.LVSS_EN_uC = false;
                MCuC_Y.Inverter_DIS = false;
                MCuC_Y.MC_EN_uC = false;
            }
            break;

        case MCuC_IN_MC_Init:
            MCuC_Y.uC_State = UC_State::MC_Init;

            // During 'MC_Init': '<S1>:6'
            if (!MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:15'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Open;

                // Entry 'Contactor_Open': '<S1>:7'
                MCuC_Y.Inverter_EN = false;
                MCuC_Y.Inverter_DIS = true;
                MCuC_Y.uC_State = UC_State::Contactor_Open;
            } else if (MCuC_U.MC_VSM_State_g == MC_VSM_State::Ready) {
                // Transition: '<S1>:12'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Closed;

                // Entry 'Contactor_Closed': '<S1>:8'
                MCuC_Y.uC_State = UC_State::Contactor_Closed;
                MCuC_Y.Inverter_EN = false;
            } else {
                // no actions
            }
            break;

        case MCuC_IN_MC_Off:
            MCuC_Y.LVSS_EN_uC = false;
            MCuC_Y.uC_State = UC_State::MC_Off;
            MCuC_Y.Inverter_EN = false;

            // During 'MC_Off': '<S1>:1'
            if (MCuC_U.Ignition_LS_A) {
                // Transition: '<S1>:11'
                MCuC_DW.is_MCuC_Logic = MCuC_IN_LVSS_MC_Startup;

                // Entry 'LVSS_MC_Startup': '<S1>:350'
                MCuC_DW.uncharged = false;
                MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;
                MCuC_Y.LVSS_EN_uC = true;
                MCuC_Y.MC_EN_uC = true;
            }
            break;

        default:
            MCuC_Preset();
            break;
        }

        // End of Inport: '<Root>/ESTOP_LS_A'
    }

    if (guard1) {
        MCuC_exit_internal_MCuC_Logic();
        MCuC_DW.is_MCuC_Logic = MCuC_IN_Contactor_Open;

        // Outport: '<Root>/Inverter_EN'
        // Entry 'Contactor_Open': '<S1>:7'
        MCuC_Y.Inverter_EN = false;

        // Outport: '<Root>/Inverter_DIS'
        MCuC_Y.Inverter_DIS = true;

        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Contactor_Open;
    }

    // End of Inport: '<Root>/HM_Fault'
    // During 'Watchdog': '<S1>:211'
    if (static_cast<uint16_t>(MCuC_DW.is_Watchdog) == MCuC_IN_WD0) {
        // Outport: '<Root>/Watchdog'
        MCuC_Y.Watchdog = false;

        // During 'WD0': '<S1>:210'
        if (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) >= 3U) {
            // Transition: '<S1>:208'
            MCuC_DW.temporalCounter_i1 = 0U;
            MCuC_DW.is_Watchdog = MCuC_IN_WD1;

            // Outport: '<Root>/Watchdog'
            // Entry 'WD1': '<S1>:209'
            MCuC_Y.Watchdog = true;
        }
    } else {
        // Outport: '<Root>/Watchdog'
        MCuC_Y.Watchdog = true;

        // During 'WD1': '<S1>:209'
        if (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) >= 3U) {
            // Transition: '<S1>:207'
            MCuC_DW.temporalCounter_i1 = 0U;
            MCuC_DW.is_Watchdog = MCuC_IN_WD0;

            // Outport: '<Root>/Watchdog'
            // Entry 'WD0': '<S1>:210'
            MCuC_Y.Watchdog = false;
        }
    }
}
}// namespace vcu

namespace vcu {
// Model step function
void MCuC_Model::step() {
    // Chart: '<Root>/MCuC_Chart'
    // Gateway: MCuC_Chart
    if (MCuC_DW.isNotInit && (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) < 3U)) {
        MCuC_DW.temporalCounter_i1 = static_cast<uint8_t>(static_cast<int16_t>(static_cast<int16_t>(MCuC_DW.temporalCounter_i1) + 1));
    }

    MCuC_DW.isNotInit = true;

    // During: MCuC_Chart
    MCuC_MCuC();

    // End of Chart: '<Root>/MCuC_Chart'
}

// Model initialize function
void MCuC_Model::initialize() {
    // SystemInitialize for Chart: '<Root>/MCuC_Chart'
    MCuC_DW.uncharged = true;

    // Chart: '<Root>/MCuC_Chart'
    // Entry: MCuC_Chart
    // Entry Internal: MCuC_Chart
    // Entry Internal 'MCuC': '<S1>:206'
    // Entry Internal 'MCuC_Logic': '<S1>:197'
    // Transition: '<S1>:2'
    MCuC_DW.is_MCuC_Logic = MCuC_IN_Preset;

    // Outport: '<Root>/uC_State' incorporates:
    //   Chart: '<Root>/MCuC_Chart'
    //
    // Entry 'Preset': '<S1>:302'
    MCuC_Y.uC_State = UC_State::Preset;

    // Outport: '<Root>/MC_Self_Test' incorporates:
    //   Chart: '<Root>/MCuC_Chart'
    //
    // Entry Internal 'Preset': '<S1>:302'
    // Transition: '<S1>:362'
    // Entry 'MC_ST': '<S1>:361'
    MCuC_Y.MC_Self_Test = true;

    // Chart: '<Root>/MCuC_Chart'
    // Entry Internal 'MC_ST': '<S1>:361'
    // Transition: '<S1>:365'
    MCuC_DW.is_MC_ST = MCuC_IN_MC_EN_ON;

    // Outport: '<Root>/MC_EN_uC' incorporates:
    //   Chart: '<Root>/MCuC_Chart'
    //
    // Entry 'MC_EN_ON': '<S1>:364'
    MCuC_Y.MC_EN_uC = true;

    // Chart: '<Root>/MCuC_Chart'
    // Entry Internal 'Watchdog': '<S1>:211'
    // Transition: '<S1>:212'
    MCuC_DW.is_Watchdog = MCuC_IN_WD0;

    // Entry 'WD0': '<S1>:210'
}

// Model terminate function
void MCuC_Model::terminate() {
    // (no terminate code required)
}

// Constructor
MCuC_Model::MCuC_Model() : MCuC_U(),
                           MCuC_Y(),
                           MCuC_DW(),
                           MCuC_M() {
    // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
MCuC_Model::~MCuC_Model() = default;

// Real-Time Model get method
MCuC_Model::RT_MODEL_MCuC_T* MCuC_Model::getRTM() {
    return (&MCuC_M);
}
}// namespace vcu

//
// File trailer for generated code.
//
// [EOF]
//
