//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.cpp
//
// Code generated for Simulink model 'MCuC'.
//
// Model version                  : 5.3
// Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
// C/C++ source code generated on : Mon Nov  3 19:58:40 2025
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
const uint8_t MCuC_IN_Contactor_Closed{ 1U };

const uint8_t MCuC_IN_Contactor_Open{ 2U };

const uint8_t MCuC_IN_ESTOP{ 3U };

const uint8_t MCuC_IN_Fault{ 4U };

const uint8_t MCuC_IN_Key_Cycle{ 5U };

const uint8_t MCuC_IN_LVSS_MC_Shutdown{ 6U };

const uint8_t MCuC_IN_LVSS_MC_Startup{ 7U };

const uint8_t MCuC_IN_MC_Active{ 8U };

const uint8_t MCuC_IN_MC_Discharging{ 9U };

const uint8_t MCuC_IN_MC_EN_OFF{ 1U };

const uint8_t MCuC_IN_MC_EN_ON{ 2U };

const uint8_t MCuC_IN_MC_Init{ 10U };

const uint8_t MCuC_IN_MC_Off{ 11U };

const uint8_t MCuC_IN_MC_Ready{ 12U };

const uint8_t MCuC_IN_Preset{ 13U };

const uint8_t MCuC_IN_Super_Fault{ 14U };

// Expression: const
//  Referenced by: '<S1>/Constant'

#define rtCP_Constant_Value            (BMS_Contactor_Command::Stay)

// Expression: const
//  Referenced by: '<S5>/Constant'

#define rtCP_Constant_Value_j          (MC_VSM_State::Motor_Running)

// Pooled Parameter (Expression: const)
//  Referenced by:
//    '<S2>/Constant'
//    '<S3>/Constant'
//    '<S4>/Constant'
//    '<S6>/Constant'

#define rtCP_pooled1                   (UC_State::MC_Active)

// Computed Parameter: DiscretePulseGenerator_Period
//  Referenced by: '<Root>/Discrete Pulse Generator'

#define rtCP_DiscretePulseGenerator_Per ((static_cast<int32_t>(10L)))

// Computed Parameter: DiscretePulseGenerator_Duty
//  Referenced by: '<Root>/Discrete Pulse Generator'

#define rtCP_DiscretePulseGenerator_Dut ((static_cast<int32_t>(5L)))

// Computed Parameter: Delay1_InitialCondition
//  Referenced by: '<Root>/Delay1'

#define rtCP_Delay1_InitialCondition   ((static_cast<uint32_t>(1UL)))

// Computed Parameter: Constant_Value_c
//  Referenced by: '<S8>/Constant'

#define rtCP_Constant_Value_c          (0)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/Constant'
//    '<Root>/Constant10'
//    '<Root>/Constant2'
//    '<Root>/Constant3'
//    '<Root>/Constant4'
//    '<Root>/Constant5'
//    '<Root>/Unit Delay4'

#define rtCP_pooled2                   (false)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/BMS_EN'
//    '<Root>/GFDB_EN'
//    '<Root>/Discrete Pulse Generator'
//    '<Root>/Delay'

#define rtCP_pooled3                   (true)

// Pooled Parameter (Mixed Expressions)
//  Referenced by:
//    '<Root>/Unit Delay7'
//    '<Root>/Unit Delay8'
//    '<S7>/Constant'

#define rtCP_pooled4                   ((static_cast<uint8_t>(0U)))

namespace vcu
{
    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_Contactor_Closed(void)
    {
        // Update for Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Contactor_Closed;

        // Outport: '<Root>/Inverter_EN_uC_CAN'
        MCuC_Y.Inverter_EN_uC_CAN = false;

        // Inport: '<Root>/Ignition_LS_A' incorporates:
        //   Inport: '<Root>/Start_CAN'

        // During 'Contactor_Closed': '<S10>:8'
        if (!MCuC_U.Ignition_LS_A) {
            // Transition: '<S10>:195'
            MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

            // Outport: '<Root>/Inverter_DC_uC_CAN'
            // Entry 'Contactor_Open': '<S10>:7'
            MCuC_Y.Inverter_DC_uC_CAN = true;

            // Update for Outport: '<Root>/uC_State'
            MCuC_Y.uC_State = UC_State::Contactor_Open;

            // Outport: '<Root>/Acc_EN_uC_CAN'
            MCuC_Y.Acc_EN_uC_CAN = false;

            // Outport: '<Root>/GUB_EN_uC_CAN'
            MCuC_Y.GUB_EN_uC_CAN = false;

            // Outport: '<Root>/HUDL_EN_uC_CAN'
            MCuC_Y.HUDL_EN_uC_CAN = false;

            // Outport: '<Root>/HIB_EN_uC_CAN'
            MCuC_Y.HIB_EN_uC_CAN = false;

            // Outport: '<Root>/TMS_EN_uC_CAN'
            MCuC_Y.TMS_EN_uC_CAN = false;

            // Outport: '<Root>/Batt_12V_EN_uC_CAN'
            MCuC_Y.Batt_12V_EN_uC_CAN = false;
        } else if (MCuC_U.Start_CAN) {
            // Transition: '<S10>:437'
            MCuC_DW.is_Logic = MCuC_IN_MC_Ready;

            // Update for Outport: '<Root>/uC_State'
            // Entry 'MC_Ready': '<S10>:436'
            MCuC_Y.uC_State = UC_State::MC_Ready;
        } else {
            // no actions
        }

        // End of Inport: '<Root>/Ignition_LS_A'
    }

    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_MC_Active(void)
    {
        // Outport: '<Root>/Inverter_EN_uC_CAN'
        MCuC_Y.Inverter_EN_uC_CAN = true;

        // Update for Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::MC_Active;

        // Inport: '<Root>/Forward_EN_CAN' incorporates:
        //   Inport: '<Root>/Ignition_LS_A'

        // During 'MC_Active': '<S10>:9'
        if (!MCuC_U.Forward_EN_CAN) {
            // Outport: '<Root>/Torque_Request_CAN'
            // Transition: '<S10>:243'
            // Exit 'MC_Active': '<S10>:9'
            MCuC_Y.Torque_Request_CAN = 0;
            MCuC_DW.is_Logic = MCuC_IN_MC_Ready;

            // Update for Outport: '<Root>/uC_State'
            // Entry 'MC_Ready': '<S10>:436'
            MCuC_Y.uC_State = UC_State::MC_Ready;

            // Outport: '<Root>/Inverter_EN_uC_CAN'
            MCuC_Y.Inverter_EN_uC_CAN = false;
        } else if (!MCuC_U.Ignition_LS_A) {
            // Outport: '<Root>/Torque_Request_CAN'
            // Transition: '<S10>:242'
            // Exit 'MC_Active': '<S10>:9'
            MCuC_Y.Torque_Request_CAN = 0;
            MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

            // Outport: '<Root>/Inverter_EN_uC_CAN'
            // Entry 'Contactor_Open': '<S10>:7'
            MCuC_Y.Inverter_EN_uC_CAN = false;

            // Outport: '<Root>/Inverter_DC_uC_CAN'
            MCuC_Y.Inverter_DC_uC_CAN = true;

            // Update for Outport: '<Root>/uC_State'
            MCuC_Y.uC_State = UC_State::Contactor_Open;

            // Outport: '<Root>/Acc_EN_uC_CAN'
            MCuC_Y.Acc_EN_uC_CAN = false;

            // Outport: '<Root>/GUB_EN_uC_CAN'
            MCuC_Y.GUB_EN_uC_CAN = false;

            // Outport: '<Root>/HUDL_EN_uC_CAN'
            MCuC_Y.HUDL_EN_uC_CAN = false;

            // Outport: '<Root>/HIB_EN_uC_CAN'
            MCuC_Y.HIB_EN_uC_CAN = false;

            // Outport: '<Root>/TMS_EN_uC_CAN'
            MCuC_Y.TMS_EN_uC_CAN = false;

            // Outport: '<Root>/Batt_12V_EN_uC_CAN'
            MCuC_Y.Batt_12V_EN_uC_CAN = false;
        } else {
            // Outport: '<Root>/Torque_Request_CAN' incorporates:
            //   Inport: '<Root>/Throttle_CAN'

            MCuC_Y.Torque_Request_CAN = MCuC_U.Throttle_CAN;
        }

        // End of Inport: '<Root>/Forward_EN_CAN'
    }

    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_MC_Init(void)
    {
        // Update for Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::MC_Init;

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

        // During 'MC_Init': '<S10>:6'
        if (!MCuC_U.Ignition_LS_A) {
            // Transition: '<S10>:15'
            MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

            // Outport: '<Root>/Inverter_EN_uC_CAN'
            // Entry 'Contactor_Open': '<S10>:7'
            MCuC_Y.Inverter_EN_uC_CAN = false;

            // Outport: '<Root>/Inverter_DC_uC_CAN'
            MCuC_Y.Inverter_DC_uC_CAN = true;

            // Update for Outport: '<Root>/uC_State'
            MCuC_Y.uC_State = UC_State::Contactor_Open;

            // Outport: '<Root>/Acc_EN_uC_CAN'
            MCuC_Y.Acc_EN_uC_CAN = false;

            // Outport: '<Root>/GUB_EN_uC_CAN'
            MCuC_Y.GUB_EN_uC_CAN = false;

            // Outport: '<Root>/HUDL_EN_uC_CAN'
            MCuC_Y.HUDL_EN_uC_CAN = false;

            // Outport: '<Root>/HIB_EN_uC_CAN'
            MCuC_Y.HIB_EN_uC_CAN = false;

            // Outport: '<Root>/TMS_EN_uC_CAN'
            MCuC_Y.TMS_EN_uC_CAN = false;

            // Outport: '<Root>/Batt_12V_EN_uC_CAN'
            MCuC_Y.Batt_12V_EN_uC_CAN = false;
        } else if (MCuC_U.MC_VSM_State_CAN == MC_VSM_State::Ready) {
            // Transition: '<S10>:12'
            MCuC_DW.is_Logic = MCuC_IN_Contactor_Closed;

            // Update for Outport: '<Root>/uC_State'
            // Entry 'Contactor_Closed': '<S10>:8'
            MCuC_Y.uC_State = UC_State::Contactor_Closed;

            // Outport: '<Root>/Inverter_EN_uC_CAN'
            MCuC_Y.Inverter_EN_uC_CAN = false;
        } else {
            // no actions
        }

        // End of Inport: '<Root>/Ignition_LS_A'
    }

    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_MC_Ready(const bool *AND)
    {
        // Update for Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::MC_Ready;

        // Outport: '<Root>/Inverter_EN_uC_CAN'
        MCuC_Y.Inverter_EN_uC_CAN = false;

        // Inport: '<Root>/Ignition_LS_A'
        // During 'MC_Ready': '<S10>:436'
        if (!MCuC_U.Ignition_LS_A) {
            // Transition: '<S10>:452'
            MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

            // Outport: '<Root>/Inverter_DC_uC_CAN'
            // Entry 'Contactor_Open': '<S10>:7'
            MCuC_Y.Inverter_DC_uC_CAN = true;

            // Update for Outport: '<Root>/uC_State'
            MCuC_Y.uC_State = UC_State::Contactor_Open;

            // Outport: '<Root>/Acc_EN_uC_CAN'
            MCuC_Y.Acc_EN_uC_CAN = false;

            // Outport: '<Root>/GUB_EN_uC_CAN'
            MCuC_Y.GUB_EN_uC_CAN = false;

            // Outport: '<Root>/HUDL_EN_uC_CAN'
            MCuC_Y.HUDL_EN_uC_CAN = false;

            // Outport: '<Root>/HIB_EN_uC_CAN'
            MCuC_Y.HIB_EN_uC_CAN = false;

            // Outport: '<Root>/TMS_EN_uC_CAN'
            MCuC_Y.TMS_EN_uC_CAN = false;

            // Outport: '<Root>/Batt_12V_EN_uC_CAN'
            MCuC_Y.Batt_12V_EN_uC_CAN = false;
        } else if (*AND) {
            // Transition: '<S10>:17'
            MCuC_DW.is_Logic = MCuC_IN_MC_Active;

            // Outport: '<Root>/Inverter_EN_uC_CAN'
            // Entry 'MC_Active': '<S10>:9'
            MCuC_Y.Inverter_EN_uC_CAN = true;

            // Update for Outport: '<Root>/uC_State'
            MCuC_Y.uC_State = UC_State::MC_Active;

            // Outport: '<Root>/Torque_Request_CAN'
            MCuC_Y.Torque_Request_CAN = 0;
        } else {
            // no actions
        }

        // End of Inport: '<Root>/Ignition_LS_A'
    }

    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_Preset(const bool *NOR)
    {
        // Update for Outport: '<Root>/uC_State'
        MCuC_Y.uC_State = UC_State::Preset;

        // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
        MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::
            Open_Contactor;

        // During 'Preset': '<S10>:302'
        // During 'MC_ST': '<S10>:361'
        if (static_cast<uint16_t>(MCuC_DW.is_MC_ST) == MCuC_IN_MC_EN_OFF) {
            // Update for Outport: '<Root>/MC_EN_uC'
            MCuC_Y.MC_EN_uC = false;

            // During 'MC_EN_OFF': '<S10>:366'
            if (*NOR) {
                // Transition: '<S10>:320'
                MCuC_DW.is_MC_ST = 0;

                // Exit 'Preset': '<S10>:302'
                MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

                // Update for Outport: '<Root>/uC_State'
                // Entry 'Key_Cycle': '<S10>:372'
                MCuC_Y.uC_State = UC_State::Key_Cycle;

                // Update for Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::
                    Close_Contactor;
            }
        } else {
            // Update for Outport: '<Root>/MC_EN_uC'
            MCuC_Y.MC_EN_uC = true;

            // Inport: '<Root>/MC_ON'
            // During 'MC_EN_ON': '<S10>:364'
            if (MCuC_U.MC_ON) {
                // Transition: '<S10>:368'
                MCuC_DW.is_MC_ST = MCuC_IN_MC_EN_OFF;

                // Update for Outport: '<Root>/MC_EN_uC'
                // Entry 'MC_EN_OFF': '<S10>:366'
                MCuC_Y.MC_EN_uC = false;
            }

            // End of Inport: '<Root>/MC_ON'
        }
    }

    // Function for Chart: '<Root>/MCuC_Chart'
    void MCuC_Model::MCuC_exit_internal_Logic(void)
    {
        // Exit Internal 'Logic': '<S10>:441'
        switch (MCuC_DW.is_Logic) {
          case MCuC_IN_Fault:
            // Outport: '<Root>/Fault_to_MC_CAN'
            // Exit 'Fault': '<S10>:3'
            MCuC_Y.Fault_to_MC_CAN = false;
            MCuC_DW.is_Logic = 0;
            break;

          case MCuC_IN_MC_Active:
            // Outport: '<Root>/Torque_Request_CAN'
            // Exit 'MC_Active': '<S10>:9'
            MCuC_Y.Torque_Request_CAN = 0;
            MCuC_DW.is_Logic = 0;
            break;

          case MCuC_IN_MC_Discharging:
            // Exit 'MC_Discharging': '<S10>:5'
            MCuC_DW.MC_Uncharged = true;

            // Outport: '<Root>/Inverter_DC_uC_CAN'
            MCuC_Y.Inverter_DC_uC_CAN = false;
            MCuC_DW.is_Logic = 0;
            break;

          case MCuC_IN_Preset:
            // Exit Internal 'Preset': '<S10>:302'
            // Exit Internal 'MC_ST': '<S10>:361'
            MCuC_DW.is_MC_ST = 0;

            // Exit 'Preset': '<S10>:302'
            MCuC_DW.is_Logic = 0;
            break;

          default:
            MCuC_DW.is_Logic = 0;
            break;
        }
    }
}

namespace vcu
{
    // Model step function
    void MCuC_Model::step()
    {
        int16_t i;
        bool rtb_VectorConcatenate1[5];
        bool AND;
        bool LogicalOperator;
        bool NOR;
        bool NOR_tmp;
        bool NOR_tmp_0;
        bool OR;
        bool guard1;
        bool rtb_NOR1;
        bool tmp;

        // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
        //   Inport: '<Root>/LVSS_ON_CAN'

        rtb_VectorConcatenate1[0] = MCuC_U.LVSS_ON_CAN;

        // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
        //   Inport: '<Root>/TMS_ON_CAN'

        rtb_VectorConcatenate1[1] = MCuC_U.TMS_ON_CAN;

        // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
        //   Constant: '<Root>/BMS_EN'

        rtb_VectorConcatenate1[2] = rtCP_pooled3;

        // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
        //   Constant: '<Root>/GFDB_EN'

        rtb_VectorConcatenate1[3] = rtCP_pooled3;

        // SignalConversion generated from: '<Root>/Vector Concatenate1' incorporates:
        //   Inport: '<Root>/HIB_ON_CAN'

        rtb_VectorConcatenate1[4] = MCuC_U.HIB_ON_CAN;

        // Logic: '<Root>/AND1' incorporates:
        //   Concatenate: '<Root>/Vector Concatenate1'
        //   Delay: '<Root>/Delay1'
        //   Inport: '<Root>/Heartbeats_CAN'
        //   RelationalOperator: '<Root>/Relational Operator'

        for (i = 0; i < 5; i++) {
            rtb_VectorConcatenate1[i] = ((MCuC_U.Heartbeats_CAN[i] <=
                MCuC_DW.Delay1_DSTATE[i]) && rtb_VectorConcatenate1[i]);
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

        rtb_NOR1 = ((!MCuC_U.HIB_Comparison_Fault_CAN) &&
                    (MCuC_DW.UnitDelay4_DSTATE == MCuC_U.MC_ON) &&
                    ((!rtb_VectorConcatenate1[0]) && (!rtb_VectorConcatenate1[1])
                     && (!rtb_VectorConcatenate1[2]) &&
                     (!rtb_VectorConcatenate1[3]) && (!rtb_VectorConcatenate1[4]))
                    && ((MCuC_Y.MC_PS_Request_uC_CAN == MCuC_U.MC_PS_Present_CAN)
                        && (MCuC_Y.Batt_PS_Request_uC_CAN ==
                            MCuC_U.Batt_PS_Present_CAN)));

        // Logic: '<Root>/Logical Operator' incorporates:
        //   Delay: '<Root>/Delay'

        LogicalOperator = (rtb_NOR1 || MCuC_DW.Delay_DSTATE[0]);

        // Logic: '<Root>/NOR' incorporates:
        //   Chart: '<Root>/MCuC_Chart'
        //   Inport: '<Root>/LVSS_ON_CAN'
        //   Inport: '<Root>/MC_ON'

        NOR_tmp = !MCuC_U.MC_ON;
        NOR_tmp_0 = !MCuC_U.LVSS_ON_CAN;

        // Logic: '<Root>/NOR' incorporates:
        //   Inport: '<Root>/BMS_Contactor_Closed_CAN'

        NOR = (NOR_tmp_0 && NOR_tmp && (!MCuC_U.BMS_Contactor_Closed_CAN));

        // Switch: '<Root>/Switch' incorporates:
        //   Constant: '<Root>/Constant'
        //   Constant: '<S1>/Constant'
        //   DataTypeConversion: '<Root>/Cast To Boolean1'
        //   Inport: '<Root>/BMS_Contactor_Closed_CAN'
        //   Logic: '<Root>/XOR3'
        //   RelationalOperator: '<S1>/Compare'
        //   UnitDelay: '<Root>/Unit Delay5'

        if (MCuC_DW.UnitDelay5_DSTATE == rtCP_Constant_Value) {
            tmp = rtCP_pooled2;
        } else {
            tmp = (static_cast<int16_t>((static_cast<int16_t>
                     (MCuC_DW.UnitDelay5_DSTATE) != 0) !=
                    MCuC_U.BMS_Contactor_Closed_CAN) != 0);
        }

        // Logic: '<Root>/OR' incorporates:
        //   Constant: '<S5>/Constant'
        //   Constant: '<S6>/Constant'
        //   Constant: '<S7>/Constant'
        //   Inport: '<Root>/GFDB_Isolation_State_CAN'
        //   Inport: '<Root>/Interlock'
        //   Inport: '<Root>/MC_VSM_State_CAN'
        //   Logic: '<Root>/NOT1'
        //   Logic: '<Root>/NXOR1'
        //   RelationalOperator: '<S5>/Compare'
        //   RelationalOperator: '<S6>/Compare'
        //   RelationalOperator: '<S7>/Compare'
        //   Switch: '<Root>/Switch'
        //   UnitDelay: '<Root>/Unit Delay6'

        OR = ((MCuC_U.GFDB_Isolation_State_CAN != rtCP_pooled4) ||
              (!MCuC_U.Interlock) || tmp || ((MCuC_DW.UnitDelay6_DSTATE !=
                rtCP_pooled1) && (MCuC_U.MC_VSM_State_CAN ==
                rtCP_Constant_Value_j)));

        // Logic: '<Root>/AND' incorporates:
        //   Constant: '<S8>/Constant'
        //   Inport: '<Root>/Brake_CAN'
        //   Inport: '<Root>/Forward_EN_CAN'
        //   Inport: '<Root>/Throttle_CAN'
        //   RelationalOperator: '<S8>/Compare'

        AND = (MCuC_U.Brake_CAN && MCuC_U.Forward_EN_CAN && (MCuC_U.Throttle_CAN
                == rtCP_Constant_Value_c));

        // Chart: '<Root>/MCuC_Chart' incorporates:
        //   Inport: '<Root>/ESTOP_LS_A'
        //   Inport: '<Root>/Ignition_LS_A'
        //   Inport: '<Root>/LVSS_ON_CAN'
        //   Inport: '<Root>/MC_DC_State_CAN'
        //   Inport: '<Root>/MC_ON'

        // Gateway: MCuC_Chart
        // During: MCuC_Chart
        // During 'Logic': '<S10>:441'
        if ((OR && MCuC_DW.MC_Uncharged) || MCuC_DW.Super_Super_Fault) {
            // Transition: '<S10>:391'
            MCuC_exit_internal_Logic();
            MCuC_DW.is_Logic = MCuC_IN_Super_Fault;

            // Outport: '<Root>/uC_State'
            // Entry 'Super_Fault': '<S10>:388'
            MCuC_Y.uC_State = UC_State::Super_Fault;

            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
            MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::
                Open_Contactor;
            MCuC_DW.Super_Super_Fault = true;
        } else {
            tmp = !MCuC_DW.MC_Uncharged;
            guard1 = false;
            if (OR && tmp && (!MCuC_DW.Activate_Super_Fault)) {
                // Transition: '<S10>:439'
                MCuC_DW.Activate_Super_Fault = true;
                guard1 = true;
            } else if (MCuC_U.ESTOP_LS_A && MCuC_DW.MC_Uncharged &&
                       (MCuC_Y.uC_State != UC_State::Preset)) {
                // Transition: '<S10>:200'
                MCuC_exit_internal_Logic();
                MCuC_DW.is_Logic = MCuC_IN_ESTOP;

                // Outport: '<Root>/uC_State'
                // Entry 'ESTOP': '<S10>:4'
                MCuC_Y.uC_State = UC_State::Estop;

                // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::
                    Open_Contactor;

                // Outport: '<Root>/LVSS_EN_uC'
                MCuC_Y.LVSS_EN_uC = false;
                MCuC_DW.Activate_ESTOP = false;
            } else if (MCuC_U.ESTOP_LS_A && tmp && (!MCuC_DW.Activate_ESTOP)) {
                // Transition: '<S10>:199'
                MCuC_DW.Activate_ESTOP = true;
                guard1 = true;
            } else {
                OR = !LogicalOperator;
                if (OR && MCuC_DW.MC_Uncharged) {
                    // Transition: '<S10>:435'
                    MCuC_exit_internal_Logic();
                    MCuC_DW.is_Logic = MCuC_IN_Fault;

                    // Outport: '<Root>/uC_State'
                    // Entry 'Fault': '<S10>:3'
                    MCuC_Y.uC_State = UC_State::Fault;
                    MCuC_DW.Activate_Fault = false;

                    // Outport: '<Root>/Fault_to_MC_CAN'
                    MCuC_Y.Fault_to_MC_CAN = true;

                    // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                    MCuC_Y.BMS_Contactor_Command_uC_CAN = BMS_Contactor_Command::
                        Stay;
                } else if (OR && tmp && (!MCuC_DW.Activate_Fault)) {
                    // Transition: '<S10>:400'
                    MCuC_DW.Activate_Fault = true;
                    guard1 = true;
                } else {
                    switch (MCuC_DW.is_Logic) {
                      case MCuC_IN_Contactor_Closed:
                        MCuC_Contactor_Closed();
                        break;

                      case MCuC_IN_Contactor_Open:
                        // Outport: '<Root>/Inverter_EN_uC_CAN'
                        MCuC_Y.Inverter_EN_uC_CAN = false;

                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::Contactor_Open;

                        // Outport: '<Root>/Acc_EN_uC_CAN'
                        MCuC_Y.Acc_EN_uC_CAN = false;

                        // Outport: '<Root>/GUB_EN_uC_CAN'
                        MCuC_Y.GUB_EN_uC_CAN = false;

                        // Outport: '<Root>/HUDL_EN_uC_CAN'
                        MCuC_Y.HUDL_EN_uC_CAN = false;

                        // Outport: '<Root>/HIB_EN_uC_CAN'
                        MCuC_Y.HIB_EN_uC_CAN = false;

                        // Outport: '<Root>/TMS_EN_uC_CAN'
                        MCuC_Y.TMS_EN_uC_CAN = false;

                        // Outport: '<Root>/Batt_12V_EN_uC_CAN'
                        MCuC_Y.Batt_12V_EN_uC_CAN = false;

                        // During 'Contactor_Open': '<S10>:7'
                        if (MCuC_U.MC_DC_State_CAN != MC_DC_State::Disabled) {
                            // Transition: '<S10>:42'
                            MCuC_DW.is_Logic = MCuC_IN_MC_Discharging;

                            // Outport: '<Root>/uC_State'
                            // Entry 'MC_Discharging': '<S10>:5'
                            MCuC_Y.uC_State = UC_State::MC_Discharging;
                        }
                        break;

                      case MCuC_IN_ESTOP:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::Estop;

                        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                        MCuC_Y.BMS_Contactor_Command_uC_CAN =
                            BMS_Contactor_Command::Open_Contactor;

                        // Outport: '<Root>/LVSS_EN_uC'
                        MCuC_Y.LVSS_EN_uC = false;

                        // During 'ESTOP': '<S10>:4'
                        if (!MCuC_U.ESTOP_LS_A) {
                            // Transition: '<S10>:21'
                            MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

                            // Outport: '<Root>/uC_State'
                            // Entry 'Key_Cycle': '<S10>:372'
                            MCuC_Y.uC_State = UC_State::Key_Cycle;

                            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                            MCuC_Y.BMS_Contactor_Command_uC_CAN =
                                BMS_Contactor_Command::Close_Contactor;
                        }
                        break;

                      case MCuC_IN_Fault:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::Fault;

                        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                        MCuC_Y.BMS_Contactor_Command_uC_CAN =
                            BMS_Contactor_Command::Stay;

                        // During 'Fault': '<S10>:3'
                        if (LogicalOperator) {
                            // Outport: '<Root>/Fault_to_MC_CAN'
                            // Transition: '<S10>:387'
                            // Exit 'Fault': '<S10>:3'
                            MCuC_Y.Fault_to_MC_CAN = false;
                            MCuC_DW.is_Logic = MCuC_IN_Key_Cycle;

                            // Outport: '<Root>/uC_State'
                            // Entry 'Key_Cycle': '<S10>:372'
                            MCuC_Y.uC_State = UC_State::Key_Cycle;

                            // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                            MCuC_Y.BMS_Contactor_Command_uC_CAN =
                                BMS_Contactor_Command::Close_Contactor;
                        }
                        break;

                      case MCuC_IN_Key_Cycle:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::Key_Cycle;

                        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                        MCuC_Y.BMS_Contactor_Command_uC_CAN =
                            BMS_Contactor_Command::Close_Contactor;

                        // During 'Key_Cycle': '<S10>:372'
                        if (!MCuC_U.Ignition_LS_A) {
                            // Transition: '<S10>:374'
                            MCuC_DW.is_Logic = MCuC_IN_MC_Off;

                            // Outport: '<Root>/MC_EN_uC'
                            // Entry 'MC_Off': '<S10>:1'
                            MCuC_Y.MC_EN_uC = false;

                            // Outport: '<Root>/LVSS_EN_uC'
                            MCuC_Y.LVSS_EN_uC = false;

                            // Outport: '<Root>/uC_State'
                            MCuC_Y.uC_State = UC_State::MC_Off;
                            MCuC_DW.MC_Uncharged = true;

                            // Outport: '<Root>/Inverter_EN_uC_CAN'
                            MCuC_Y.Inverter_EN_uC_CAN = false;
                        }
                        break;

                      case MCuC_IN_LVSS_MC_Shutdown:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;

                        // Outport: '<Root>/LVSS_EN_uC'
                        MCuC_Y.LVSS_EN_uC = false;

                        // Outport: '<Root>/MC_EN_uC'
                        MCuC_Y.MC_EN_uC = false;

                        // During 'LVSS_MC_Shutdown': '<S10>:353'
                        if (NOR_tmp && NOR_tmp_0) {
                            // Transition: '<S10>:354'
                            MCuC_DW.is_Logic = MCuC_IN_MC_Off;

                            // Outport: '<Root>/uC_State'
                            // Entry 'MC_Off': '<S10>:1'
                            MCuC_Y.uC_State = UC_State::MC_Off;
                            MCuC_DW.MC_Uncharged = true;

                            // Outport: '<Root>/Inverter_EN_uC_CAN'
                            MCuC_Y.Inverter_EN_uC_CAN = false;
                        }
                        break;

                      case MCuC_IN_LVSS_MC_Startup:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;

                        // Outport: '<Root>/LVSS_EN_uC'
                        MCuC_Y.LVSS_EN_uC = true;

                        // Outport: '<Root>/MC_EN_uC'
                        MCuC_Y.MC_EN_uC = true;

                        // During 'LVSS_MC_Startup': '<S10>:350'
                        if (MCuC_U.MC_ON && MCuC_U.LVSS_ON_CAN) {
                            // Transition: '<S10>:351'
                            MCuC_DW.is_Logic = MCuC_IN_MC_Init;

                            // Outport: '<Root>/uC_State'
                            // Entry 'MC_Init': '<S10>:6'
                            MCuC_Y.uC_State = UC_State::MC_Init;

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
                        }
                        break;

                      case MCuC_IN_MC_Active:
                        MCuC_MC_Active();
                        break;

                      case MCuC_IN_MC_Discharging:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::MC_Discharging;

                        // During 'MC_Discharging': '<S10>:5'
                        if (MCuC_U.MC_DC_State_CAN == MC_DC_State::Complete) {
                            // Transition: '<S10>:250'
                            // Exit 'MC_Discharging': '<S10>:5'
                            MCuC_DW.MC_Uncharged = true;

                            // Outport: '<Root>/Inverter_DC_uC_CAN'
                            MCuC_Y.Inverter_DC_uC_CAN = false;
                            MCuC_DW.is_Logic = MCuC_IN_LVSS_MC_Shutdown;

                            // Outport: '<Root>/uC_State'
                            // Entry 'LVSS_MC_Shutdown': '<S10>:353'
                            MCuC_Y.uC_State = UC_State::LVSS_MC_Shutdown;

                            // Outport: '<Root>/LVSS_EN_uC'
                            MCuC_Y.LVSS_EN_uC = false;

                            // Outport: '<Root>/MC_EN_uC'
                            MCuC_Y.MC_EN_uC = false;
                        }
                        break;

                      case MCuC_IN_MC_Init:
                        MCuC_MC_Init();
                        break;

                      case MCuC_IN_MC_Off:
                        // Outport: '<Root>/MC_EN_uC'
                        MCuC_Y.MC_EN_uC = false;

                        // Outport: '<Root>/LVSS_EN_uC'
                        MCuC_Y.LVSS_EN_uC = false;

                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::MC_Off;

                        // Outport: '<Root>/Inverter_EN_uC_CAN'
                        MCuC_Y.Inverter_EN_uC_CAN = false;

                        // During 'MC_Off': '<S10>:1'
                        if (MCuC_U.Ignition_LS_A) {
                            // Transition: '<S10>:11'
                            MCuC_DW.is_Logic = MCuC_IN_LVSS_MC_Startup;

                            // Entry 'LVSS_MC_Startup': '<S10>:350'
                            MCuC_DW.MC_Uncharged = false;

                            // Outport: '<Root>/uC_State'
                            MCuC_Y.uC_State = UC_State::LVSS_MC_Startup;

                            // Outport: '<Root>/LVSS_EN_uC'
                            MCuC_Y.LVSS_EN_uC = true;

                            // Outport: '<Root>/MC_EN_uC'
                            MCuC_Y.MC_EN_uC = true;
                        }
                        break;

                      case MCuC_IN_MC_Ready:
                        MCuC_MC_Ready(&AND);
                        break;

                      case MCuC_IN_Preset:
                        MCuC_Preset(&NOR);
                        break;

                      default:
                        // Outport: '<Root>/uC_State'
                        MCuC_Y.uC_State = UC_State::Super_Fault;

                        // Outport: '<Root>/BMS_Contactor_Command_uC_CAN'
                        MCuC_Y.BMS_Contactor_Command_uC_CAN =
                            BMS_Contactor_Command::Open_Contactor;

                        // During 'Super_Fault': '<S10>:388'
                        break;
                    }
                }
            }

            if (guard1) {
                MCuC_exit_internal_Logic();
                MCuC_DW.is_Logic = MCuC_IN_Contactor_Open;

                // Outport: '<Root>/Inverter_EN_uC_CAN'
                // Entry 'Contactor_Open': '<S10>:7'
                MCuC_Y.Inverter_EN_uC_CAN = false;

                // Outport: '<Root>/Inverter_DC_uC_CAN'
                MCuC_Y.Inverter_DC_uC_CAN = true;

                // Outport: '<Root>/uC_State'
                MCuC_Y.uC_State = UC_State::Contactor_Open;

                // Outport: '<Root>/Acc_EN_uC_CAN'
                MCuC_Y.Acc_EN_uC_CAN = false;

                // Outport: '<Root>/GUB_EN_uC_CAN'
                MCuC_Y.GUB_EN_uC_CAN = false;

                // Outport: '<Root>/HUDL_EN_uC_CAN'
                MCuC_Y.HUDL_EN_uC_CAN = false;

                // Outport: '<Root>/HIB_EN_uC_CAN'
                MCuC_Y.HIB_EN_uC_CAN = false;

                // Outport: '<Root>/TMS_EN_uC_CAN'
                MCuC_Y.TMS_EN_uC_CAN = false;

                // Outport: '<Root>/Batt_12V_EN_uC_CAN'
                MCuC_Y.Batt_12V_EN_uC_CAN = false;
            }
        }

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
            // Transition: '<S9>:9'
            // Entry 'NOT_IMPLEMENTED': '<S9>:8'
            MCuC_Y.MC_PS_Request_uC_CAN = MCuC_U.MC_PS_Present_CAN;

            // Outport: '<Root>/Batt_PS_Request_uC_CAN' incorporates:
            //   Inport: '<Root>/Batt_PS_Present_CAN'

            MCuC_Y.Batt_PS_Request_uC_CAN = MCuC_U.Batt_PS_Present_CAN;
        } else {
            // During 'NOT_IMPLEMENTED': '<S9>:8'
        }

        // End of Chart: '<Root>/Cooling_Control_Chart'

        // Outport: '<Root>/Watchdog' incorporates:
        //   DiscretePulseGenerator: '<Root>/Discrete Pulse Generator'

        MCuC_Y.Watchdog = ((MCuC_DW.clockTickCounter <
                            rtCP_DiscretePulseGenerator_Dut) &&
                           (MCuC_DW.clockTickCounter >= 0L) && rtCP_pooled3);

        // DiscretePulseGenerator: '<Root>/Discrete Pulse Generator'
        if (MCuC_DW.clockTickCounter >= static_cast<int32_t>
                (rtCP_DiscretePulseGenerator_Per - 1L)) {
            MCuC_DW.clockTickCounter = 0L;
        } else {
            MCuC_DW.clockTickCounter = static_cast<int32_t>
                (MCuC_DW.clockTickCounter + 1L);
        }

        // Outport: '<Root>/Direction_Command_uC_CAN' incorporates:
        //   Inport: '<Root>/Direction_Command1'

        MCuC_Y.Direction_Command_uC_CAN = MCuC_U.Direction_Command1;

        // Outport: '<Root>/Rolling_Counter_uC_CAN' incorporates:
        //   Inport: '<Root>/Rolling_Counter1'

        MCuC_Y.Rolling_Counter_uC_CAN = MCuC_U.Rolling_Counter1;

        // Outport: '<Root>/Speed_Command_uC_CAN' incorporates:
        //   Inport: '<Root>/Speed_Command1'

        MCuC_Y.Speed_Command_uC_CAN = MCuC_U.Speed_Command1;

        // Outport: '<Root>/Speed_Mode_Enable_uC_CAN' incorporates:
        //   Inport: '<Root>/Speed_Mode_Enable1'

        MCuC_Y.Speed_Mode_Enable_uC_CAN = MCuC_U.Speed_Mode_Enable1;

        // Outport: '<Root>/Torque_Limit_Command_uC_CAN' incorporates:
        //   Inport: '<Root>/Torque_Limit_Command1'

        MCuC_Y.Torque_Limit_Command_uC_CAN = MCuC_U.Torque_Limit_Command1;

        // Update for Delay: '<Root>/Delay1' incorporates:
        //   Inport: '<Root>/Heartbeats_CAN'

        for (i = 0; i < 45; i++) {
            MCuC_DW.Delay1_DSTATE[i] = MCuC_DW.Delay1_DSTATE[static_cast<int16_t>
                (i + 5)];
        }

        for (i = 0; i < 5; i++) {
            MCuC_DW.Delay1_DSTATE[static_cast<int16_t>(i + 45)] =
                MCuC_U.Heartbeats_CAN[i];
        }

        // End of Update for Delay: '<Root>/Delay1'

        // Update for UnitDelay: '<Root>/Unit Delay4' incorporates:
        //   Outport: '<Root>/MC_EN_uC'

        MCuC_DW.UnitDelay4_DSTATE = MCuC_Y.MC_EN_uC;

        // Update for Delay: '<Root>/Delay'
        for (i = 0; i < 19; i++) {
            MCuC_DW.Delay_DSTATE[i] = MCuC_DW.Delay_DSTATE[static_cast<int16_t>
                (i + 1)];
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
    void MCuC_Model::initialize()
    {
        {
            int16_t i;

            // InitializeConditions for Delay: '<Root>/Delay1'
            for (i = 0; i < 50; i++) {
                MCuC_DW.Delay1_DSTATE[i] = rtCP_Delay1_InitialCondition;
            }

            // End of InitializeConditions for Delay: '<Root>/Delay1'

            // InitializeConditions for Delay: '<Root>/Delay'
            for (i = 0; i < 20; i++) {
                MCuC_DW.Delay_DSTATE[i] = rtCP_pooled3;
            }

            // End of InitializeConditions for Delay: '<Root>/Delay'

            // SystemInitialize for Chart: '<Root>/MCuC_Chart'
            MCuC_DW.MC_Uncharged = true;

            // Chart: '<Root>/MCuC_Chart'
            // Entry: MCuC_Chart
            // Entry Internal: MCuC_Chart
            // Entry Internal 'Logic': '<S10>:441'
            // Transition: '<S10>:2'
            MCuC_DW.is_Logic = MCuC_IN_Preset;

            // Entry 'Preset': '<S10>:302'
            // Entry Internal 'Preset': '<S10>:302'
            // Transition: '<S10>:362'
            // Entry 'MC_ST': '<S10>:361'
            // Entry Internal 'MC_ST': '<S10>:361'
            // Transition: '<S10>:365'
            MCuC_DW.is_MC_ST = MCuC_IN_MC_EN_ON;

            // Outport: '<Root>/MC_EN_uC' incorporates:
            //   Chart: '<Root>/MCuC_Chart'
            //
            // Entry 'MC_EN_ON': '<S10>:364'
            MCuC_Y.MC_EN_uC = true;

            // ConstCode for Outport: '<Root>/Fault' incorporates:
            //   Constant: '<Root>/Constant10'

            MCuC_Y.Fault = rtCP_pooled2;

            // ConstCode for Outport: '<Root>/Super_Fault' incorporates:
            //   Constant: '<Root>/Constant2'

            MCuC_Y.Super_Fault = rtCP_pooled2;

            // ConstCode for Outport: '<Root>/LS_Self_Test_Out' incorporates:
            //   Constant: '<Root>/Constant3'

            MCuC_Y.LS_Self_Test_Out = rtCP_pooled2;

            // ConstCode for Outport: '<Root>/CAN_Self_Test' incorporates:
            //   Constant: '<Root>/Constant4'

            MCuC_Y.CAN_Self_Test = rtCP_pooled2;

            // ConstCode for Outport: '<Root>/MC_Self_Test' incorporates:
            //   Constant: '<Root>/Constant5'

            MCuC_Y.MC_Self_Test = rtCP_pooled2;
        }
    }

    // Model terminate function
    void MCuC_Model::terminate()
    {
        // (no terminate code required)
    }

    const char* MCuC_Model::RT_MODEL_MCuC_T::getErrorStatus() const
    {
        return (errorStatus);
    }

    void MCuC_Model::RT_MODEL_MCuC_T::setErrorStatus(const char* const volatile
        aErrorStatus)
    {
        (errorStatus = aErrorStatus);
    }

    // Constructor
    MCuC_Model::MCuC_Model() :
        MCuC_U(),
        MCuC_Y(),
        MCuC_DW(),
        MCuC_M()
    {
        // Currently there is no constructor body generated.
    }

    // Destructor
    // Currently there is no destructor body generated.
    MCuC_Model::~MCuC_Model() = default;

    // Real-Time Model get method
    MCuC_Model::RT_MODEL_MCuC_T * MCuC_Model::getRTM()
    {
        return (&MCuC_M);
    }
}

//
// File trailer for generated code.
//
// [EOF]
//
