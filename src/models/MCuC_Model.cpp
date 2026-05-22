//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.cpp
//
// Code generated for Simulink model 'MCuC'.
//
// Model version                  : 5.54
// Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
// C/C++ source code generated on : Fri Apr 24 15:28:40 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "models/MCuC_Model.hpp"
#include <stdbool.h>
#include <stdint.h>

// Named constants for Chart: '<Root>/MCuC_Chart'
const uint8_t MCuC_IN_Complete{1U};

const uint8_t MCuC_IN_Contactor_Closed{1U};

const uint8_t MCuC_IN_Contactor_Open{2U};

const uint8_t MCuC_IN_ESTOP{1U};

const uint8_t MCuC_IN_Fault{2U};

const uint8_t MCuC_IN_Key_Cycle{3U};

const uint8_t MCuC_IN_LVSS_MC_Shutdown{4U};

const uint8_t MCuC_IN_LVSS_MC_Startup{5U};

const uint8_t MCuC_IN_Logic{3U};

const uint8_t MCuC_IN_MC_Active{6U};

const uint8_t MCuC_IN_MC_Discharging{7U};

const uint8_t MCuC_IN_MC_EN_OFF{2U};

const uint8_t MCuC_IN_MC_EN_ON{3U};

const uint8_t MCuC_IN_MC_Init{8U};

const uint8_t MCuC_IN_MC_Off{9U};

const uint8_t MCuC_IN_MC_Ready{10U};

const uint8_t MCuC_IN_Preset{4U};

const uint8_t MCuC_IN_Set_High1{2U};

const uint8_t MCuC_IN_Set_Low{3U};

const uint8_t MCuC_IN_Shutdown{1U};

const uint8_t MCuC_IN_Super_Fault{5U};

const uint8_t MCuC_IN_Waiting{2U};

const uint8_t MCuC_IN_t0{11U};

const uint8_t MCuC_IN_t1{12U};

const uint8_t MCuC_IN_t10{13U};

const uint8_t MCuC_IN_t2{14U};

const uint8_t MCuC_IN_t3{15U};

const uint8_t MCuC_IN_t4{16U};

const uint8_t MCuC_IN_t5{17U};

const uint8_t MCuC_IN_t6{18U};

const uint8_t MCuC_IN_t7{19U};

const uint8_t MCuC_IN_t8{20U};

const uint8_t MCuC_IN_t9{21U};

const uint8_t MCuC_IN_tTBD{22U};

const uint8_t MCuC_IN_tTBD1{23U};

// Expression: const
//  Referenced by: '<S1>/Constant'

#define rtCP_Constant_Value (BMS_Contactor_Command::Stay)

// Expression: const
//  Referenced by: '<S5>/Constant'

#define rtCP_Constant_Value_j (MC_VSM_State::Motor_Running)

// Pooled Parameter (Expression: const)
//  Referenced by:
//    '<S2>/Constant'
//    '<S3>/Constant'
//    '<S4>/Constant'
//    '<S6>/Constant'

#define rtCP_pooled1 (UC_State::MC_Active)

// Computed Parameter: DiscretePulseGenerator_Period
//  Referenced by: '<Root>/Discrete Pulse Generator'

#define rtCP_DiscretePulseGenerator_Per ((static_cast<int32_t>(10L)))

// Computed Parameter: DiscretePulseGenerator_Duty
//  Referenced by: '<Root>/Discrete Pulse Generator'

#define rtCP_DiscretePulseGenerator_Dut ((static_cast<int32_t>(5L)))

// Computed Parameter: Delay1_InitialCondition
//  Referenced by: '<Root>/Delay1'

#define rtCP_Delay1_InitialCondition ((static_cast<uint32_t>(1UL)))

// Pooled Parameter (Expression: )
//  Referenced by:
//    '<S8>/Constant'
//    '<S9>/Constant'

#define rtCP_pooled2 (0)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/Constant'
//    '<Root>/Constant10'
//    '<Root>/Constant2'
//    '<Root>/Unit Delay4'

#define rtCP_pooled3 (false)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/BMS_EN'
//    '<Root>/GFDB_EN'
//    '<Root>/Discrete Pulse Generator'
//    '<Root>/Delay'

#define rtCP_pooled4 (true)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/Unit Delay7'
//    '<Root>/Unit Delay8'
//    '<S7>/Constant'

#define rtCP_pooled5 ((static_cast<uint8_t>(0U)))

namespace vcu {
// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_Contactor_Closed(void) {
    // Outport: '<Root>/Inverter_EN_uC_CAN'
    MCuC_Y.Inverter_EN_uC_CAN = false;

    // Inport: '<Root>/Ignition_LS_A' incorporates:
    //   Inport: '<Root>/Start_CAN'

    // During 'Contactor_Closed': '<S11>:528'
    if (!MCuC_U.Ignition_LS_A) {
        // Transition: '<S11>:488'
        MCuC_DW.is_Logic = MCuC_IN_tTBD;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'tTBD': '<S11>:590'
        MCuC_Y.uC_State = UC_State::Contactor_Open;
    } else if (MCuC_U.Start_CAN) {
        // Transition: '<S11>:503'
        MCuC_DW.is_Logic = MCuC_IN_t4;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't4': '<S11>:588'
        MCuC_Y.uC_State = UC_State::MC_Ready;
    } else {
        // no actions
    }

    // End of Inport: '<Root>/Ignition_LS_A'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_LVSS_MC_Shutdown(void) {
    // Outport: '<Root>/LVSS_EN_uC'
    MCuC_Y.LVSS_EN_uC = false;

    // Update for Outport: '<Root>/MC_EN_uC'
    MCuC_Y.MC_EN_uC = false;

    // Inport: '<Root>/MC_ON' incorporates:
    //   Inport: '<Root>/LVSS_ON_CAN'

    // During 'LVSS_MC_Shutdown': '<S11>:531'
    if ((!MCuC_U.MC_ON) && (!MCuC_U.LVSS_ON_CAN)) {
        // Transition: '<S11>:540'
        // Exit 'LVSS_MC_Shutdown': '<S11>:531'
        MCuC_DW.MC_Uncharged = true;
        MCuC_DW.is_Logic     = MCuC_IN_t10;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't10': '<S11>:609'
        MCuC_Y.uC_State = UC_State::Key_Cycle;
    }

    // End of Inport: '<Root>/MC_ON'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_LVSS_MC_Startup(void) {
    // Outport: '<Root>/LVSS_EN_uC'
    MCuC_Y.LVSS_EN_uC = true;

    // Update for Outport: '<Root>/MC_EN_uC'
    MCuC_Y.MC_EN_uC = true;

    // Inport: '<Root>/MC_ON' incorporates:
    //   Inport: '<Root>/LVSS_ON_CAN'

    // During 'LVSS_MC_Startup': '<S11>:536'
    if (MCuC_U.MC_ON && MCuC_U.LVSS_ON_CAN) {
        // Transition: '<S11>:519'
        MCuC_DW.is_Logic = MCuC_IN_t2;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't2': '<S11>:583'
        MCuC_Y.uC_State = UC_State::MC_Init;
    }

    // End of Inport: '<Root>/MC_ON'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_MC_Active(void) {
    // Outport: '<Root>/Inverter_EN_uC_CAN'
    MCuC_Y.Inverter_EN_uC_CAN = true;

    // Inport: '<Root>/Forward_EN_CAN' incorporates:
    //   Inport: '<Root>/Ignition_LS_A'

    // During 'MC_Active': '<S11>:537'
    if (!MCuC_U.Forward_EN_CAN) {
        // Outport: '<Root>/Torque_Request_CAN'
        // Transition: '<S11>:491'
        // Exit 'MC_Active': '<S11>:537'
        MCuC_Y.Torque_Request_CAN = 0;
        MCuC_DW.is_Logic          = MCuC_IN_t6;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't6': '<S11>:594'
        MCuC_Y.uC_State = UC_State::MC_Ready;
    } else if (!MCuC_U.Ignition_LS_A) {
        // Outport: '<Root>/Torque_Request_CAN'
        // Transition: '<S11>:490'
        // Exit 'MC_Active': '<S11>:537'
        MCuC_Y.Torque_Request_CAN = 0;
        MCuC_DW.is_Logic          = MCuC_IN_t7;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't7': '<S11>:598'
        MCuC_Y.uC_State = UC_State::Contactor_Open;
    } else {
        // Outport: '<Root>/Torque_Request_CAN' incorporates:
        //   Inport: '<Root>/Throttle_CAN'

        MCuC_Y.Torque_Request_CAN = MCuC_U.Throttle_CAN;
    }

    // End of Inport: '<Root>/Forward_EN_CAN'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_MC_Discharging(const bool* LogicalOperator, const bool* OR) {
    // Inport: '<Root>/MC_DC_State_CAN' incorporates:
    //   Outport: '<Root>/Acc_EN_uC_CAN'
    //   Outport: '<Root>/Batt_12V_EN_uC_CAN'
    //   Outport: '<Root>/GUB_EN_uC_CAN'
    //   Outport: '<Root>/HIB_EN_uC_CAN'
    //   Outport: '<Root>/HUDL_EN_uC_CAN'
    //   Outport: '<Root>/Inverter_DC_uC_CAN'
    //   Outport: '<Root>/LVSS_EN_uC'
    //   Outport: '<Root>/MC_EN_uC'
    //   Outport: '<Root>/Shutdown_CAN'
    //   Outport: '<Root>/TMS_EN_uC_CAN'
    //   Outport: '<Root>/uC_State'

    // During 'MC_Discharging': '<S11>:522'
    if ((MCuC_U.MC_DC_State_CAN == MC_DC_State::Complete) && (!MCuC_Y.Shutdown_CAN)) {
        // Transition: '<S11>:527'
        // Exit Internal 'MC_Discharging': '<S11>:522'
        MCuC_DW.is_MC_Discharging = 0;

        // Exit 'MC_Discharging': '<S11>:522'
        MCuC_Y.Inverter_DC_uC_CAN = false;
        MCuC_DW.is_Logic          = MCuC_IN_t9;

        // Entry 't9': '<S11>:604'
        MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;
    } else if (static_cast<uint16_t>(MCuC_DW.is_MC_Discharging) == MCuC_IN_Shutdown) {
        MCuC_Y.Acc_EN_uC_CAN      = false;
        MCuC_Y.GUB_EN_uC_CAN      = false;
        MCuC_Y.HUDL_EN_uC_CAN     = false;
        MCuC_Y.HIB_EN_uC_CAN      = false;
        MCuC_Y.TMS_EN_uC_CAN      = false;
        MCuC_Y.Batt_12V_EN_uC_CAN = false;
        MCuC_Y.Shutdown_CAN       = false;

        // During 'Shutdown': '<S11>:521'

        // During 'Waiting': '<S11>:523'
    } else if ((MCuC_U.MC_DC_State_CAN == MC_DC_State::Complete) && ((*OR) || (!*LogicalOperator))) {
        // Transition: '<S11>:529'
        MCuC_DW.is_MC_Discharging = 0;

        // Exit 'MC_Discharging': '<S11>:522'
        MCuC_Y.Inverter_DC_uC_CAN = false;
        MCuC_DW.is_Logic          = MCuC_IN_LVSS_MC_Shutdown;

        // Entry 'LVSS_MC_Shutdown': '<S11>:531'
        MCuC_Y.LVSS_EN_uC = false;
        MCuC_Y.MC_EN_uC   = false;
    } else if (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) >= 1) {
        // Transition: '<S11>:524'
        MCuC_DW.is_MC_Discharging = MCuC_IN_Shutdown;

        // Entry 'Shutdown': '<S11>:521'
        MCuC_Y.Acc_EN_uC_CAN      = false;
        MCuC_Y.GUB_EN_uC_CAN      = false;
        MCuC_Y.HUDL_EN_uC_CAN     = false;
        MCuC_Y.HIB_EN_uC_CAN      = false;
        MCuC_Y.TMS_EN_uC_CAN      = false;
        MCuC_Y.Batt_12V_EN_uC_CAN = false;
        MCuC_Y.Shutdown_CAN       = false;
    } else {
        // no actions
    }

    // End of Inport: '<Root>/MC_DC_State_CAN'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_MC_Init(void) {
    // Outport: '<Root>/Batt_12V_EN_uC_CAN'
    MCuC_Y.Batt_12V_EN_uC_CAN = true;

    // Outport: '<Root>/TMS_EN_uC_CAN'
    MCuC_Y.TMS_EN_uC_CAN = true;

    // Outport: '<Root>/HIB_EN_uC_CAN'
    MCuC_Y.HIB_EN_uC_CAN = true;

    // Outport: '<Root>/HUDL_EN_uC_CAN'
    MCuC_Y.HUDL_EN_uC_CAN = true;

    // Outport: '<Root>/GUB_EN_uC_CAN'
    MCuC_Y.GUB_EN_uC_CAN = true;

    // Outport: '<Root>/Acc_EN_uC_CAN'
    MCuC_Y.Acc_EN_uC_CAN = true;

    // Inport: '<Root>/Ignition_LS_A' incorporates:
    //   Inport: '<Root>/MC_VSM_State_CAN'

    // During 'MC_Init': '<S11>:535'
    if (!MCuC_U.Ignition_LS_A) {
        // Transition: '<S11>:493'
        MCuC_DW.is_Logic = MCuC_IN_tTBD;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'tTBD': '<S11>:590'
        MCuC_Y.uC_State = UC_State::Contactor_Open;
    } else if (MCuC_U.MC_VSM_State_CAN == MC_VSM_State::Ready) {
        // Transition: '<S11>:487'
        MCuC_DW.is_Logic = MCuC_IN_t3;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't3': '<S11>:585'
        MCuC_Y.uC_State = UC_State::Contactor_Closed;
    } else {
        // no actions
    }

    // End of Inport: '<Root>/Ignition_LS_A'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_MC_Ready(const bool* AND) {
    // Update for Outport: '<Root>/uC_State'
    MCuC_Y.uC_State = UC_State::MC_Ready;

    // Outport: '<Root>/Inverter_EN_uC_CAN'
    // Inport: '<Root>/Ignition_LS_A'
    MCuC_Y.Inverter_EN_uC_CAN = false;

    // During 'MC_Ready': '<S11>:542'
    if (*AND) {
        // Transition: '<S11>:505'
        MCuC_DW.is_Logic = MCuC_IN_t5;

        // Update for Outport: '<Root>/uC_State'
        // Entry 't5': '<S11>:592'
        MCuC_Y.uC_State = UC_State::MC_Active;
    } else if (!MCuC_U.Ignition_LS_A) {
        // Transition: '<S11>:489'
        MCuC_DW.is_Logic = MCuC_IN_tTBD;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'tTBD': '<S11>:590'
        MCuC_Y.uC_State = UC_State::Contactor_Open;
    } else {
        // no actions
    }

    // End of Inport: '<Root>/Ignition_LS_A'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_t0(void) {
    // Update for Outport: '<Root>/uC_State'
    MCuC_Y.uC_State = UC_State::MC_Off;

    // Inport: '<Root>/State_Handshake'
    // During 't0': '<S11>:580'
    if (MCuC_U.State_Handshake) {
        // Transition: '<S11>:582'
        MCuC_DW.is_Logic = MCuC_IN_MC_Off;

        // Update for Outport: '<Root>/MC_EN_uC'
        // Entry 'MC_Off': '<S11>:533'
        MCuC_Y.MC_EN_uC = false;

        // Outport: '<Root>/LVSS_EN_uC'
        MCuC_Y.LVSS_EN_uC = false;

        // Outport: '<Root>/Inverter_EN_uC_CAN'
        MCuC_Y.Inverter_EN_uC_CAN = false;
        MCuC_DW.MC_Uncharged      = true;
    }

    // End of Inport: '<Root>/State_Handshake'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_t2(void) {
    // Update for Outport: '<Root>/uC_State'
    MCuC_Y.uC_State = UC_State::MC_Init;

    // Inport: '<Root>/State_Handshake'
    // During 't2': '<S11>:583'
    if (MCuC_U.State_Handshake) {
        // Transition: '<S11>:584'
        MCuC_DW.is_Logic = MCuC_IN_MC_Init;

        // Outport: '<Root>/Batt_12V_EN_uC_CAN'
        // Entry 'MC_Init': '<S11>:535'
        MCuC_Y.Batt_12V_EN_uC_CAN = true;

        // Outport: '<Root>/TMS_EN_uC_CAN'
        MCuC_Y.TMS_EN_uC_CAN = true;

        // Outport: '<Root>/HIB_EN_uC_CAN'
        MCuC_Y.HIB_EN_uC_CAN = true;

        // Outport: '<Root>/HUDL_EN_uC_CAN'
        MCuC_Y.HUDL_EN_uC_CAN = true;

        // Outport: '<Root>/GUB_EN_uC_CAN'
        MCuC_Y.GUB_EN_uC_CAN = true;

        // Outport: '<Root>/Acc_EN_uC_CAN'
        MCuC_Y.Acc_EN_uC_CAN = true;
    }

    // End of Inport: '<Root>/State_Handshake'
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_exit_internal_Logic(void) {
    // Exit Internal 'Logic': '<S11>:539'
    switch (MCuC_DW.is_Logic) {
    case MCuC_IN_LVSS_MC_Shutdown:
        // Exit 'LVSS_MC_Shutdown': '<S11>:531'
        MCuC_DW.MC_Uncharged = true;
        MCuC_DW.is_Logic     = 0;
        break;

    case MCuC_IN_MC_Active:
        // Outport: '<Root>/Torque_Request_CAN'
        // Exit 'MC_Active': '<S11>:537'
        MCuC_Y.Torque_Request_CAN = 0;
        MCuC_DW.is_Logic          = 0;
        break;

    case MCuC_IN_MC_Discharging:
        // Exit Internal 'MC_Discharging': '<S11>:522'
        MCuC_DW.is_MC_Discharging = 0;

        // Outport: '<Root>/Inverter_DC_uC_CAN'
        // Exit 'MC_Discharging': '<S11>:522'
        MCuC_Y.Inverter_DC_uC_CAN = false;
        MCuC_DW.is_Logic          = 0;
        break;

    default:
        MCuC_DW.is_Logic = 0;
        break;
    }
}

// Function for Chart: '<Root>/MCuC_Chart'
void MCuC_Model::MCuC_Logic(const bool* LogicalOperator, const bool* OR, const bool* AND) {
    bool guard1;
    bool tmp;

    // During 'Logic': '<S11>:539'
    if ((*OR) && MCuC_DW.MC_Uncharged) {
        // Transition: '<S11>:512'
        MCuC_exit_internal_Logic();
        MCuC_DW.is_c2_MCuC = MCuC_IN_Super_Fault;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'Super_Fault': '<S11>:532'
        MCuC_Y.uC_State = UC_State::Super_Fault;

        // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Open_Contactor;
    } else if ((!*LogicalOperator) && MCuC_DW.MC_Uncharged) {
        // Transition: '<S11>:514'
        MCuC_exit_internal_Logic();
        MCuC_DW.is_c2_MCuC = MCuC_IN_Fault;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'Fault': '<S11>:520'
        MCuC_Y.uC_State        = UC_State::Fault;
        MCuC_DW.Activate_Fault = false;

        // Outport: '<Root>/Fault_to_MC_CAN'
        MCuC_Y.Fault_to_MC_CAN = true;

        // Outport: '<Root>/LVSS_EN_uC'
        MCuC_Y.LVSS_EN_uC = false;

        // Update for Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC = false;

        // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Stay;
    } else if (MCuC_U.ESTOP_LS_A && MCuC_DW.MC_Uncharged && (MCuC_Y.uC_State != UC_State::Preset)) {
        // Transition: '<S11>:518'
        MCuC_exit_internal_Logic();
        MCuC_DW.is_c2_MCuC = MCuC_IN_ESTOP;

        // Update for Outport: '<Root>/uC_State'
        // Entry 'ESTOP': '<S11>:530'
        MCuC_Y.uC_State = UC_State::Estop;

        // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Open_Contactor;

        // Outport: '<Root>/LVSS_EN_uC'
        MCuC_Y.LVSS_EN_uC = false;

        // Update for Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC        = false;
        MCuC_DW.Activate_ESTOP = false;
    } else {
        tmp    = !MCuC_DW.MC_Uncharged;
        guard1 = false;
        if ((*OR) && tmp && (!MCuC_DW.Activate_Super_Fault)) {
            // Transition: '<S11>:498'
            MCuC_DW.Activate_Super_Fault = true;
            guard1                       = true;
        } else if (MCuC_U.ESTOP_LS_A && tmp && (!MCuC_DW.Activate_ESTOP)) {
            // Transition: '<S11>:508'
            MCuC_DW.Activate_ESTOP = true;
            guard1                 = true;
        } else if ((!*LogicalOperator) && tmp && (!MCuC_DW.Activate_Fault)) {
            // Transition: '<S11>:501'
            MCuC_DW.Activate_Fault = true;
            guard1                 = true;
        } else {
            switch (MCuC_DW.is_Logic) {
            case MCuC_IN_Contactor_Closed:
                MCuC_Contactor_Closed();
                break;

            case MCuC_IN_Contactor_Open:
                // Outport: '<Root>/Inverter_EN_uC_CAN'
                MCuC_Y.Inverter_EN_uC_CAN = false;

                // Outport: '<Root>/Shutdown_CAN'
                MCuC_Y.Shutdown_CAN = true;

                // Inport: '<Root>/MC_VSM_State_CAN'
                // During 'Contactor_Open': '<S11>:525'
                if (MCuC_U.MC_VSM_State_CAN != MC_VSM_State::Motor_Running) {
                    // Transition: '<S11>:502'
                    MCuC_DW.is_Logic = MCuC_IN_t8;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 't8': '<S11>:602'
                    MCuC_Y.uC_State = UC_State::MC_Discharging;
                }

                // End of Inport: '<Root>/MC_VSM_State_CAN'
                break;

            case MCuC_IN_Key_Cycle:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Key_Cycle;

                // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Close_Contactor;

                // Inport: '<Root>/Ignition_LS_A'
                // During 'Key_Cycle': '<S11>:534'
                if (!MCuC_U.Ignition_LS_A) {
                    // Transition: '<S11>:526'
                    MCuC_DW.is_Logic = MCuC_IN_t0;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 't0': '<S11>:580'
                    MCuC_Y.uC_State = UC_State::MC_Off;
                }
                break;

            case MCuC_IN_LVSS_MC_Shutdown:
                MCuC_LVSS_MC_Shutdown();
                break;

            case MCuC_IN_LVSS_MC_Startup:
                MCuC_LVSS_MC_Startup();
                break;

            case MCuC_IN_MC_Active:
                MCuC_MC_Active();
                break;

            case MCuC_IN_MC_Discharging:
                MCuC_MC_Discharging(LogicalOperator, OR);
                break;

            case MCuC_IN_MC_Init:
                MCuC_MC_Init();
                break;

            case MCuC_IN_MC_Off:
                // Update for Outport: '<Root>/MC_EN_uC'
                MCuC_Y.MC_EN_uC = false;

                // Outport: '<Root>/LVSS_EN_uC'
                MCuC_Y.LVSS_EN_uC = false;

                // Outport: '<Root>/Inverter_EN_uC_CAN'
                MCuC_Y.Inverter_EN_uC_CAN = false;

                // Inport: '<Root>/Ignition_LS_A'
                // During 'MC_Off': '<S11>:533'
                if (MCuC_U.Ignition_LS_A) {
                    // Transition: '<S11>:544'
                    MCuC_DW.is_Logic = MCuC_IN_t1;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 't1': '<S11>:575'
                    MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;
                }
                break;

            case MCuC_IN_MC_Ready:
                MCuC_MC_Ready(AND);
                break;

            case MCuC_IN_t0:
                MCuC_t0();
                break;

            case MCuC_IN_t1:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;

                // Inport: '<Root>/State_Handshake'
                // During 't1': '<S11>:575'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:576'
                    MCuC_DW.is_Logic = MCuC_IN_LVSS_MC_Startup;

                    // Entry 'LVSS_MC_Startup': '<S11>:536'
                    MCuC_DW.MC_Uncharged = false;

                    // Outport: '<Root>/LVSS_EN_uC'
                    MCuC_Y.LVSS_EN_uC = true;

                    // Update for Outport: '<Root>/MC_EN_uC'
                    MCuC_Y.MC_EN_uC = true;
                }
                break;

            case MCuC_IN_t10:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Key_Cycle;

                // Inport: '<Root>/State_Handshake'
                // During 't10': '<S11>:609'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:610'
                    MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 'Key_Cycle': '<S11>:534'
                    MCuC_Y.uC_State = UC_State::Key_Cycle;

                    // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                    MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Close_Contactor;
                }
                break;

            case MCuC_IN_t2:
                MCuC_t2();
                break;

            case MCuC_IN_t3:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Contactor_Closed;

                // Inport: '<Root>/State_Handshake'
                // During 't3': '<S11>:585'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:587'
                    MCuC_DW.is_Logic = MCuC_IN_Contactor_Closed;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    // Entry 'Contactor_Closed': '<S11>:528'
                    MCuC_Y.Inverter_EN_uC_CAN = false;
                }
                break;

            case MCuC_IN_t4:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::MC_Ready;

                // Inport: '<Root>/State_Handshake'
                // During 't4': '<S11>:588'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:589'
                    MCuC_DW.is_Logic = MCuC_IN_MC_Ready;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 'MC_Ready': '<S11>:542'
                    MCuC_Y.uC_State = UC_State::MC_Ready;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    MCuC_Y.Inverter_EN_uC_CAN = false;
                }
                break;

            case MCuC_IN_t5:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::MC_Active;

                // Inport: '<Root>/State_Handshake'
                // During 't5': '<S11>:592'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:593'
                    MCuC_DW.is_Logic = MCuC_IN_MC_Active;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    // Entry 'MC_Active': '<S11>:537'
                    MCuC_Y.Inverter_EN_uC_CAN = true;

                    // Outport: '<Root>/Torque_Request_CAN'
                    MCuC_Y.Torque_Request_CAN = 0;
                }
                break;

            case MCuC_IN_t6:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::MC_Ready;

                // Inport: '<Root>/State_Handshake'
                // During 't6': '<S11>:594'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:595'
                    MCuC_DW.is_Logic = MCuC_IN_MC_Ready;

                    // Update for Outport: '<Root>/uC_State'
                    // Entry 'MC_Ready': '<S11>:542'
                    MCuC_Y.uC_State = UC_State::MC_Ready;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    MCuC_Y.Inverter_EN_uC_CAN = false;
                }
                break;

            case MCuC_IN_t7:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Contactor_Open;

                // Inport: '<Root>/State_Handshake'
                // During 't7': '<S11>:598'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:599'
                    MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    // Entry 'Contactor_Open': '<S11>:525'
                    MCuC_Y.Inverter_EN_uC_CAN = false;

                    // Outport: '<Root>/Shutdown_CAN'
                    MCuC_Y.Shutdown_CAN = true;
                }
                break;

            case MCuC_IN_t8:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::MC_Discharging;

                // Inport: '<Root>/State_Handshake'
                // During 't8': '<S11>:602'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:603'
                    MCuC_DW.is_Logic = MCuC_IN_MC_Discharging;

                    // Outport: '<Root>/Inverter_DC_uC_CAN'
                    // Entry 'MC_Discharging': '<S11>:522'
                    MCuC_Y.Inverter_DC_uC_CAN = true;

                    // Entry Internal 'MC_Discharging': '<S11>:522'
                    // Transition: '<S11>:541'
                    MCuC_DW.temporalCounter_i1 = 0U;
                    MCuC_DW.is_MC_Discharging  = MCuC_IN_Waiting;
                }
                break;

            case MCuC_IN_t9:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;

                // Inport: '<Root>/State_Handshake'
                // During 't9': '<S11>:604'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:606'
                    MCuC_DW.is_Logic = MCuC_IN_LVSS_MC_Shutdown;

                    // Outport: '<Root>/LVSS_EN_uC'
                    // Entry 'LVSS_MC_Shutdown': '<S11>:531'
                    MCuC_Y.LVSS_EN_uC = false;

                    // Update for Outport: '<Root>/MC_EN_uC'
                    MCuC_Y.MC_EN_uC = false;
                }
                break;

            case MCuC_IN_tTBD:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Contactor_Open;

                // Inport: '<Root>/State_Handshake'
                // During 'tTBD': '<S11>:590'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:600'
                    MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    // Entry 'Contactor_Open': '<S11>:525'
                    MCuC_Y.Inverter_EN_uC_CAN = false;

                    // Outport: '<Root>/Shutdown_CAN'
                    MCuC_Y.Shutdown_CAN = true;
                }
                break;

            default:
                // Update for Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Contactor_Open;

                // Inport: '<Root>/State_Handshake'
                // During 'tTBD1': '<S11>:612'
                if (MCuC_U.State_Handshake) {
                    // Transition: '<S11>:613'
                    MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

                    // Outport: '<Root>/Inverter_EN_uC_CAN'
                    // Entry 'Contactor_Open': '<S11>:525'
                    MCuC_Y.Inverter_EN_uC_CAN = false;

                    // Outport: '<Root>/Shutdown_CAN'
                    MCuC_Y.Shutdown_CAN = true;
                }
                break;
            }
        }

        if (guard1) {
            MCuC_exit_internal_Logic();
            MCuC_DW.is_Logic = MCuC_IN_tTBD1;

            // Update for Outport: '<Root>/uC_State'
            // Entry 'tTBD1': '<S11>:612'
            MCuC_Y.uC_State = UC_State::Contactor_Open;
        }
    }
}
} // namespace vcu

namespace vcu {
// Model step function
void MCuC_Model::step() {
    int16_t i;
    bool rtb_VectorConcatenate1[5];
    bool AND;
    bool LogicalOperator;
    bool OR;
    bool rtb_NOR1;

    // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
    //   Inport: '<Root>/LVSS_ON_CAN'

    rtb_VectorConcatenate1[0] = MCuC_U.LVSS_ON_CAN;

    // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
    //   Inport: '<Root>/TMS_ON_CAN'

    rtb_VectorConcatenate1[1] = MCuC_U.TMS_ON_CAN;

    // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
    //   Constant: '<Root>/BMS_EN'

    rtb_VectorConcatenate1[2] = rtCP_pooled4;

    // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
    //   Constant: '<Root>/GFDB_EN'

    rtb_VectorConcatenate1[3] = rtCP_pooled4;

    // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
    //   Inport: '<Root>/HIB_ON_CAN'

    rtb_VectorConcatenate1[4] = MCuC_U.HIB_ON_CAN;

    // Logic: '<Root>/AND1' incorporates:
    //   Concatenate: '<Root>/Vector Concatenate1'
    //   Delay: '<Root>/Delay1'
    //   Inport: '<Root>/Heartbeats_CAN'
    //   RelationalOperator: '<Root>/Relational Operator'

    for (i = 0; i < 5; i++) {
        rtb_VectorConcatenate1[i] =
            ((MCuC_U.Heartbeats_CAN[i] <= MCuC_DW.Delay1_DSTATE[i]) && rtb_VectorConcatenate1[i]);
    }

    // End of Logic: '<Root>/AND1'

    // Logic: '<Root>/NOR1' incorporates:
    //   Inport: '<Root>/Batt_PS_Present_CAN'
    //   Inport: '<Root>/HIB_Comparison_Fault_CAN'
    //   Inport: '<Root>/MC_ON'
    //   Inport: '<Root>/MC_PS_Present_CAN'
    //   Logic: '<Root>/OR1'
    //   Logic: '<Root>/OR2'
    //   Logic: '<Root>/XOR4'
    //   RelationalOperator: '<Root>/Equal'
    //   RelationalOperator: '<Root>/Equal1'
    //   UnitDelay: '<Root>/Unit Delay4'
    //   UnitDelay: '<Root>/Unit Delay7'
    //   UnitDelay: '<Root>/Unit Delay8'

    rtb_NOR1 = ((!MCuC_U.HIB_Comparison_Fault_CAN) && (MCuC_DW.UnitDelay4_DSTATE == MCuC_U.MC_ON)
                && ((!rtb_VectorConcatenate1[0]) && (!rtb_VectorConcatenate1[1]) && (!rtb_VectorConcatenate1[2])
                    && (!rtb_VectorConcatenate1[3]) && (!rtb_VectorConcatenate1[4]))
                && ((MCuC_Y.MC_PS_Request_uC_CAN == MCuC_U.MC_PS_Present_CAN)
                    && (MCuC_Y.Batt_PS_Request_uC_CAN == MCuC_U.Batt_PS_Present_CAN)));

    // Logic: '<Root>/Logical Operator' incorporates:
    //   Delay: '<Root>/Delay'

    LogicalOperator = (rtb_NOR1 || MCuC_DW.Delay_DSTATE[0]);

    // Switch: '<Root>/Switch' incorporates:
    //   Constant: '<Root>/Constant'
    //   Constant: '<S1>/Constant'
    //   DataTypeConversion: '<Root>/Cast To Boolean1'
    //   Inport: '<Root>/BMS_Contactor_Closed_CAN'
    //   Logic: '<Root>/XOR3'
    //   RelationalOperator: '<S1>/Compare'
    //   UnitDelay: '<Root>/Unit Delay5'

    if (MCuC_DW.UnitDelay5_DSTATE == rtCP_Constant_Value) {
        OR = rtCP_pooled3;
    } else {
        OR = (static_cast<int16_t>((static_cast<int16_t>(MCuC_DW.UnitDelay5_DSTATE) != 0)
                                   != MCuC_U.BMS_Contactor_Closed_CAN)
              != 0);
    }

    // Logic: '<Root>/OR' incorporates:
    //   Constant: '<S5>/Constant'
    //   Constant: '<S6>/Constant'
    //   Constant: '<S7>/Constant'
    //   Inport: '<Root>/ESTOP_LS_A'
    //   Inport: '<Root>/ESTOP_LS_B'
    //   Inport: '<Root>/GFDB_Isolation_State_CAN'
    //   Inport: '<Root>/Ignition_LS_A'
    //   Inport: '<Root>/Ignition_LS_B'
    //   Inport: '<Root>/Interlock'
    //   Inport: '<Root>/LS_Self_Test_In_A'
    //   Inport: '<Root>/LS_Self_Test_In_B'
    //   Inport: '<Root>/MC_VSM_State_CAN'
    //   Logic: '<Root>/NOT1'
    //   Logic: '<Root>/NXOR1'
    //   Logic: '<Root>/OR3'
    //   Logic: '<Root>/XOR1'
    //   Logic: '<Root>/XOR2'
    //   Logic: '<Root>/XOR5'
    //   RelationalOperator: '<S5>/Compare'
    //   RelationalOperator: '<S6>/Compare'
    //   RelationalOperator: '<S7>/Compare'
    //   Switch: '<Root>/Switch'
    //   UnitDelay: '<Root>/Unit Delay6'

    OR = ((MCuC_U.GFDB_Isolation_State_CAN != rtCP_pooled5) || (!MCuC_U.Interlock) || OR
          || ((MCuC_DW.UnitDelay6_DSTATE != rtCP_pooled1) && (MCuC_U.MC_VSM_State_CAN == rtCP_Constant_Value_j))
          || ((static_cast<int16_t>(MCuC_U.Ignition_LS_A != MCuC_U.Ignition_LS_B) != 0)
              || (static_cast<int16_t>(MCuC_U.ESTOP_LS_A != MCuC_U.ESTOP_LS_B) != 0)
              || (static_cast<int16_t>(MCuC_U.LS_Self_Test_In_A != MCuC_U.LS_Self_Test_In_B) != 0)));

    // Logic: '<Root>/AND' incorporates:
    //   Constant: '<S8>/Constant'
    //   Constant: '<S9>/Constant'
    //   Inport: '<Root>/Brake_CAN'
    //   Inport: '<Root>/Forward_EN_CAN'
    //   Inport: '<Root>/Throttle_CAN'
    //   RelationalOperator: '<S8>/Compare'
    //   RelationalOperator: '<S9>/Compare'

    AND = ((MCuC_U.Brake_CAN > rtCP_pooled2) && MCuC_U.Forward_EN_CAN && (MCuC_U.Throttle_CAN == rtCP_pooled2));

    // Chart: '<Root>/MCuC_Chart' incorporates:
    //   Inport: '<Root>/ESTOP_LS_A'
    //   Inport: '<Root>/LS_Self_Test_In_A'
    //   Inport: '<Root>/LS_Self_Test_In_B'
    //   Inport: '<Root>/MC_ON'

    // Gateway: MCuC_Chart
    if (MCuC_DW.isNotInit && (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) < 127)) {
        MCuC_DW.temporalCounter_i1 =
            static_cast<uint8_t>(static_cast<int16_t>(static_cast<int16_t>(MCuC_DW.temporalCounter_i1) + 1));
    }

    MCuC_DW.isNotInit = true;

    // During: MCuC_Chart
    switch (MCuC_DW.is_c2_MCuC) {
    case MCuC_IN_ESTOP:
        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Estop;

        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Open_Contactor;

        // Outport: '<Root>/LVSS_EN_uC'
        MCuC_Y.LVSS_EN_uC = false;

        // Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC = false;

        // During 'ESTOP': '<S11>:530'
        if (!MCuC_U.ESTOP_LS_A) {
            // Transition: '<S11>:515'
            MCuC_DW.is_c2_MCuC = MCuC_IN_Logic;

            // Entry Internal 'Logic': '<S11>:539'
            // Transition: '<S11>:538'
            MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

            // Outport: '<Root>/uC_State'
            // Entry 'Key_Cycle': '<S11>:534'
            MCuC_Y.uC_State = UC_State::Key_Cycle;

            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
            MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Close_Contactor;
        }
        break;

    case MCuC_IN_Fault:
        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Fault;

        // Outport: '<Root>/LVSS_EN_uC'
        MCuC_Y.LVSS_EN_uC = false;

        // Outport: '<Root>/MC_EN_uC'
        MCuC_Y.MC_EN_uC = false;

        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Stay;

        // During 'Fault': '<S11>:520'
        if (LogicalOperator) {
            // Outport: '<Root>/Fault_to_MC_CAN'
            // Transition: '<S11>:516'
            // Exit 'Fault': '<S11>:520'
            MCuC_Y.Fault_to_MC_CAN = false;
            MCuC_DW.is_c2_MCuC     = MCuC_IN_Logic;

            // Entry Internal 'Logic': '<S11>:539'
            // Transition: '<S11>:538'
            MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

            // Outport: '<Root>/uC_State'
            // Entry 'Key_Cycle': '<S11>:534'
            MCuC_Y.uC_State = UC_State::Key_Cycle;

            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
            MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Close_Contactor;
        }
        break;

    case MCuC_IN_Logic:
        MCuC_Logic(&LogicalOperator, &OR, &AND);
        break;

    case MCuC_IN_Preset:
        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Preset;

        // During 'Preset': '<S11>:543'
        if (static_cast<uint16_t>(MCuC_DW.temporalCounter_i1) >= 100) {
            // Transition: '<S11>:504'
            // Exit Internal 'Preset': '<S11>:543'
            // Exit Internal 'LS': '<S11>:496'
            MCuC_DW.is_LS = 0;

            // Exit Internal 'MC': '<S11>:499'
            MCuC_DW.is_MC = 0;

            // Outport: '<Root>/MC_Self_Test'
            // Exit 'MC': '<S11>:499'
            MCuC_Y.MC_Self_Test = false;
            MCuC_DW.is_c2_MCuC  = MCuC_IN_Super_Fault;

            // Outport: '<Root>/uC_State'
            // Entry 'Super_Fault': '<S11>:532'
            MCuC_Y.uC_State = UC_State::Super_Fault;

            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
            MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Open_Contactor;
        } else if (MCuC_DW.MC_ST_Complete) {
            // Transition: '<S11>:513'
            // Exit Internal 'Preset': '<S11>:543'
            // Exit Internal 'LS': '<S11>:496'
            MCuC_DW.is_LS = 0;

            // Exit Internal 'MC': '<S11>:499'
            MCuC_DW.is_MC = 0;

            // Outport: '<Root>/MC_Self_Test'
            // Exit 'MC': '<S11>:499'
            MCuC_Y.MC_Self_Test = false;
            MCuC_DW.is_c2_MCuC  = MCuC_IN_Logic;

            // Entry Internal 'Logic': '<S11>:539'
            // Transition: '<S11>:538'
            MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

            // Outport: '<Root>/uC_State'
            // Entry 'Key_Cycle': '<S11>:534'
            MCuC_Y.uC_State = UC_State::Key_Cycle;

            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
            MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Close_Contactor;
        } else {
            // During 'MC': '<S11>:499'
            switch (MCuC_DW.is_MC) {
            case MCuC_IN_Complete:
                // During 'Complete': '<S11>:494'
                break;

            case MCuC_IN_MC_EN_OFF:
                // Outport: '<Root>/MC_EN_uC'
                MCuC_Y.MC_EN_uC = false;

                // During 'MC_EN_OFF': '<S11>:506'
                if (!MCuC_U.MC_ON) {
                    // Transition: '<S11>:510'
                    MCuC_DW.is_MC = MCuC_IN_Complete;

                    // Entry 'Complete': '<S11>:494'
                    MCuC_DW.MC_ST_Complete = true;
                }
                break;

            default:
                // Outport: '<Root>/MC_EN_uC'
                MCuC_Y.MC_EN_uC = true;

                // During 'MC_EN_ON': '<S11>:492'
                if (MCuC_U.MC_ON) {
                    // Transition: '<S11>:507'
                    MCuC_DW.is_MC = MCuC_IN_MC_EN_OFF;

                    // Outport: '<Root>/MC_EN_uC'
                    // Entry 'MC_EN_OFF': '<S11>:506'
                    MCuC_Y.MC_EN_uC = false;
                }
                break;
            }

            // During 'LS': '<S11>:496'
            switch (MCuC_DW.is_LS) {
            case MCuC_IN_Complete:
                // During 'Complete': '<S11>:497'
                break;

            case MCuC_IN_Set_High1:
                // Outport: '<Root>/LS_Self_Test_Out'
                MCuC_Y.LS_Self_Test_Out = true;

                // During 'Set_High1': '<S11>:484'
                if (MCuC_U.LS_Self_Test_In_A && MCuC_U.LS_Self_Test_In_B) {
                    // Transition: '<S11>:517'
                    MCuC_DW.is_LS = MCuC_IN_Complete;

                    // Entry 'Complete': '<S11>:497'
                }
                break;

            default:
                // Outport: '<Root>/LS_Self_Test_Out'
                MCuC_Y.LS_Self_Test_Out = false;

                // During 'Set_Low': '<S11>:509'
                if ((!MCuC_U.LS_Self_Test_In_A) && (!MCuC_U.LS_Self_Test_In_B)) {
                    // Transition: '<S11>:486'
                    MCuC_DW.is_LS = MCuC_IN_Set_High1;

                    // Outport: '<Root>/LS_Self_Test_Out'
                    // Entry 'Set_High1': '<S11>:484'
                    MCuC_Y.LS_Self_Test_Out = true;
                }
                break;
            }
        }
        break;

    default:
        // Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Super_Fault;

        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::Open_Contactor;

        // During 'Super_Fault': '<S11>:532'
        break;
    }

    // End of Chart: '<Root>/MCuC_Chart'

    // RelationalOperator: '<S4>/Compare' incorporates:
    //   Constant: '<S4>/Constant'
    //   Outport: '<Root>/LED'
    //   Outport: '<Root>/uC_State'

    MCuC_Y.LED[0] = (MCuC_Y.uC_State == rtCP_pooled1);

    // RelationalOperator: '<S2>/Compare' incorporates:
    //   Constant: '<S2>/Constant'
    //   Outport: '<Root>/LED'
    //   Outport: '<Root>/uC_State'

    MCuC_Y.LED[1] = (MCuC_Y.uC_State < rtCP_pooled1);

    // RelationalOperator: '<S3>/Compare' incorporates:
    //   Constant: '<S3>/Constant'
    //   Outport: '<Root>/LED'
    //   Outport: '<Root>/uC_State'

    MCuC_Y.LED[2] = (MCuC_Y.uC_State > rtCP_pooled1);

    // Chart: '<Root>/Cooling_Control_Chart'
    // Gateway: Cooling_Control_Chart
    // During: Cooling_Control_Chart
    if (static_cast<uint16_t>(MCuC_DW.is_active_c3_MCuC) == 0) {
        // Entry: Cooling_Control_Chart
        MCuC_DW.is_active_c3_MCuC = 1U;

        // Outport: '<Root>/MC_PS_Request_uC_CAN' incorporates:
        //   Inport: '<Root>/MC_PS_Present_CAN'

        // Entry Internal: Cooling_Control_Chart
        // Transition: '<S10>:9'
        // Entry 'NOT_IMPLEMENTED': '<S10>:8'
        MCuC_Y.MC_PS_Request_uC_CAN = MCuC_U.MC_PS_Present_CAN;

        // Outport: '<Root>/Batt_PS_Request_uC_CAN' incorporates:
        //   Inport: '<Root>/Batt_PS_Present_CAN'

        MCuC_Y.Batt_PS_Request_uC_CAN = MCuC_U.Batt_PS_Present_CAN;
    } else {
        // During 'NOT_IMPLEMENTED': '<S10>:8'
    }

    // End of Chart: '<Root>/Cooling_Control_Chart'

    // Outport: '<Root>/Watchdog' incorporates:
    //   DiscretePulseGenerator: '<Root>/Discrete Pulse Generator'

    MCuC_Y.Watchdog = ((MCuC_DW.clockTickCounter < rtCP_DiscretePulseGenerator_Dut) && (MCuC_DW.clockTickCounter >= 0L)
                       && rtCP_pooled4);

    // DiscretePulseGenerator: '<Root>/Discrete Pulse Generator'
    if (MCuC_DW.clockTickCounter >= static_cast<int32_t>(rtCP_DiscretePulseGenerator_Per - 1L)) {
        MCuC_DW.clockTickCounter = 0L;
    } else {
        MCuC_DW.clockTickCounter = static_cast<int32_t>(MCuC_DW.clockTickCounter + 1L);
    }

    // Outport: '<Root>/Direction_Command_uC_CAN' incorporates:
    //   Inport: '<Root>/Direction_Command'

    MCuC_Y.Direction_Command_uC_CAN = MCuC_U.Direction_Command;

    // Outport: '<Root>/Rolling_Counter_uC_CAN' incorporates:
    //   Inport: '<Root>/Rolling_Counter'

    MCuC_Y.Rolling_Counter_uC_CAN = MCuC_U.Rolling_Counter;

    // Outport: '<Root>/Speed_Command_uC_CAN' incorporates:
    //   Inport: '<Root>/Speed_Command'

    MCuC_Y.Speed_Command_uC_CAN = MCuC_U.Speed_Command;

    // Outport: '<Root>/Speed_Mode_Enable_uC_CAN' incorporates:
    //   Inport: '<Root>/Speed_Mode_Enable'

    MCuC_Y.Speed_Mode_Enable_uC_CAN = MCuC_U.Speed_Mode_Enable;

    // Outport: '<Root>/Torque_Limit_Command_uC_CAN' incorporates:
    //   Inport: '<Root>/Torque_Limit_Command'

    MCuC_Y.Torque_Limit_Command_uC_CAN = MCuC_U.Torque_Limit_Command;

    // Update for Delay: '<Root>/Delay1' incorporates:
    //   Inport: '<Root>/Heartbeats_CAN'

    for (i = 0; i < 45; i++) {
        MCuC_DW.Delay1_DSTATE[i] = MCuC_DW.Delay1_DSTATE[static_cast<int16_t>(i + 5)];
    }

    for (i = 0; i < 5; i++) {
        MCuC_DW.Delay1_DSTATE[static_cast<int16_t>(i + 45)] = MCuC_U.Heartbeats_CAN[i];
    }

    // End of Update for Delay: '<Root>/Delay1'

    // Update for UnitDelay: '<Root>/Unit Delay4' incorporates:
    //   Outport: '<Root>/MC_EN_uC'

    MCuC_DW.UnitDelay4_DSTATE = MCuC_Y.MC_EN_uC;

    // Update for Delay: '<Root>/Delay'
    for (i = 0; i < 19; i++) {
        MCuC_DW.Delay_DSTATE[i] = MCuC_DW.Delay_DSTATE[static_cast<int16_t>(i + 1)];
    }

    MCuC_DW.Delay_DSTATE[19] = rtb_NOR1;

    // End of Update for Delay: '<Root>/Delay'

    // Update for UnitDelay: '<Root>/Unit Delay5' incorporates:
    //   Outport: '<Root>/BMS_Contactor_Command_uC_CAN'

    MCuC_DW.UnitDelay5_DSTATE = MCuC_Y.BMS_Contactor_Command_uC_CAN;

    // Update for UnitDelay: '<Root>/Unit Delay6' incorporates:
    //   Outport: '<Root>/uC_State'

    MCuC_DW.UnitDelay6_DSTATE = MCuC_Y.uC_State;
}

// Model initialize function
void MCuC_Model::initialize() {
    {
        int16_t i;

        // InitializeConditions for Delay: '<Root>/Delay1'
        for (i = 0; i < 50; i++) {
            MCuC_DW.Delay1_DSTATE[i] = rtCP_Delay1_InitialCondition;
        }

        // End of InitializeConditions for Delay: '<Root>/Delay1'

        // InitializeConditions for Delay: '<Root>/Delay'
        for (i = 0; i < 20; i++) {
            MCuC_DW.Delay_DSTATE[i] = rtCP_pooled4;
        }

        // End of InitializeConditions for Delay: '<Root>/Delay'

        // SystemInitialize for Chart: '<Root>/MCuC_Chart'
        MCuC_DW.MC_Uncharged = true;

        // Chart: '<Root>/MCuC_Chart'
        // Entry: MCuC_Chart
        // Entry Internal: MCuC_Chart
        // Transition: '<S11>:511'
        MCuC_DW.is_c2_MCuC = MCuC_IN_Preset;

        // Outport: '<Root>/MC_Self_Test' incorporates:
        //   Chart: '<Root>/MCuC_Chart'
        //
        // Entry 'Preset': '<S11>:543'
        // Entry Internal 'Preset': '<S11>:543'
        // Entry 'MC': '<S11>:499'
        MCuC_Y.MC_Self_Test = true;

        // Chart: '<Root>/MCuC_Chart'
        // Entry Internal 'MC': '<S11>:499'
        // Transition: '<S11>:500'
        MCuC_DW.is_MC = MCuC_IN_MC_EN_ON;

        // Outport: '<Root>/MC_EN_uC' incorporates:
        //   Chart: '<Root>/MCuC_Chart'
        //
        // Entry 'MC_EN_ON': '<S11>:492'
        MCuC_Y.MC_EN_uC = true;

        // Chart: '<Root>/MCuC_Chart'
        // Entry Internal 'LS': '<S11>:496'
        // Transition: '<S11>:485'
        MCuC_DW.is_LS = MCuC_IN_Set_Low;

        // ConstCode for Outport: '<Root>/Fault' incorporates:
        //   Constant: '<Root>/Constant10'

        // Entry 'Set_Low': '<S11>:509'
        MCuC_Y.Fault = rtCP_pooled3;

        // ConstCode for Outport: '<Root>/Super_Fault' incorporates:
        //   Constant: '<Root>/Constant2'

        MCuC_Y.Super_Fault = rtCP_pooled3;
    }
}

// Model terminate function
void MCuC_Model::terminate() {
    // (no terminate code required)
}

const char* MCuC_Model::RT_MODEL_MCuC_T::getErrorStatus() const {
    return (errorStatus);
}

void MCuC_Model::RT_MODEL_MCuC_T::setErrorStatus(const char* const volatile aErrorStatus) {
    (errorStatus = aErrorStatus);
}

// Constructor
MCuC_Model::MCuC_Model() : MCuC_U(), MCuC_Y(), MCuC_DW(), MCuC_M() {
    // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
MCuC_Model::~MCuC_Model() = default;

// Real-Time Model get method
MCuC_Model::RT_MODEL_MCuC_T* MCuC_Model::getRTM() {
    return (&MCuC_M);
}
} // namespace vcu

//
// File trailer for generated code.
//
// [EOF]
//
