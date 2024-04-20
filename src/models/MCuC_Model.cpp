//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.cpp
//
// Code generated for Simulink model 'MCuC_Model'.
//
// Model version                  : 1.13
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Sat Apr 20 12:20:17 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <models/MCuC_Model.hpp>

// Named constants for Chart: '<Root>/MCuC'
const uint8_t MCuC_Model_IN_A_CAN_fails{ 1U };

const uint8_t MCuC_Model_IN_A_Transmit{ 2U };

const uint8_t MCuC_Model_IN_CAN_A_ST{ 1U };

const uint8_t MCuC_Model_IN_Contactor_Closed{ 1U };

const uint8_t MCuC_Model_IN_Contactor_Open{ 2U };

const uint8_t MCuC_Model_IN_ESTOP{ 3U };

const uint8_t MCuC_Model_IN_ESTOP_Fails{ 1U };

const uint8_t MCuC_Model_IN_ESTOP_High{ 2U };

const uint8_t MCuC_Model_IN_ESTOP_Low{ 3U };

const uint8_t MCuC_Model_IN_ESTOP_ST{ 2U };

const uint8_t MCuC_Model_IN_Fault{ 4U };

const uint8_t MCuC_Model_IN_Fault_Ignit_H{ 5U };

const uint8_t MCuC_Model_IN_Ignition_Fails{ 1U };

const uint8_t MCuC_Model_IN_Ignition_High{ 2U };

const uint8_t MCuC_Model_IN_Ignition_Low{ 3U };

const uint8_t MCuC_Model_IN_Ignition_ST{ 3U };

const uint8_t MCuC_Model_IN_LVSS_MC_Shutdown{ 6U };

const uint8_t MCuC_Model_IN_LVSS_MC_Startup{ 7U };

const uint8_t MCuC_Model_IN_MC_Active{ 8U };

const uint8_t MCuC_Model_IN_MC_CAN_ST{ 4U };

const uint8_t MCuC_Model_IN_MC_CAN_fails{ 1U };

const uint8_t MCuC_Model_IN_MC_Discharging{ 9U };

const uint8_t MCuC_Model_IN_MC_Fails{ 1U };

const uint8_t MCuC_Model_IN_MC_Init{ 10U };

const uint8_t MCuC_Model_IN_MC_OFF{ 2U };

const uint8_t MCuC_Model_IN_MC_ON{ 3U };

const uint8_t MCuC_Model_IN_MC_Off{ 11U };

const uint8_t MCuC_Model_IN_MC_ST{ 5U };

const uint8_t MCuC_Model_IN_MC_Transmit{ 2U };

const uint8_t MCuC_Model_IN_NO_ACTIVE_CHILD{ 0U };

const uint8_t MCuC_Model_IN_Setup{ 12U };

const uint8_t MCuC_Model_IN_WD0{ 1U };

const uint8_t MCuC_Model_IN_WD1{ 2U };

namespace VCU
{
  // Function for Chart: '<Root>/MCuC'
  void MCuC_Model::MCuC_Model_Setup(void)
  {
    MCuC_Model_B.MC_Self_Test = true;
    switch (MCuC_Model_DW.is_Setup) {
     case MCuC_Model_IN_CAN_A_ST:
      switch (MCuC_Model_DW.is_CAN_A_ST) {
       case MCuC_Model_IN_A_CAN_fails:
        break;

       default:
        // case IN_A_Transmit:
        MCuC_Model_B.CAN_A_out = true;

        // Inport: '<Root>/A_CAN_ST' incorporates:
        //   Inport: '<Root>/Ignition_3V3'
        //   Inport: '<Root>/LVSS_Status_3V3'
        //   Inport: '<Root>/MC_Status_3V3'

        if (MCuC_Model_U.A_CAN_ST && (!MCuC_Model_U.MC_Status_3V3) &&
            MCuC_Model_U.Ignition_3V3 && (!MCuC_Model_U.LVSS_Status_3V3)) {
          MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_Setup = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;
          MCuC_Model_B.LVSS_EN_3V3 = false;
          MCuC_Model_B.uC_State = 0U;
          MCuC_Model_DW.uncharged = true;
          MCuC_Model_B.Inverter_EN_CAN = false;
          MCuC_Model_B.MC_Self_Test = false;
        } else if ((!MCuC_Model_U.A_CAN_ST) && (MCuC_Model_DW.temporalCounter_i1
                    >= 150U)) {
          MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_A_CAN_fails;
          MCuC_Model_B.uC_State = 11U;
        }

        // End of Inport: '<Root>/A_CAN_ST'
        break;
      }
      break;

     case MCuC_Model_IN_ESTOP_ST:
      switch (MCuC_Model_DW.is_ESTOP_ST) {
       case MCuC_Model_IN_ESTOP_Fails:
        break;

       case MCuC_Model_IN_ESTOP_High:
        MCuC_Model_B.ESTOP_Self_Test = false;

        // Inport: '<Root>/ESTOP_3V3'
        if (!MCuC_Model_U.ESTOP_3V3) {
          MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Fails;
          MCuC_Model_B.uC_State = 13U;
        } else {
          MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_CAN_ST;
          MCuC_Model_DW.temporalCounter_i1 = 0U;
          MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_MC_Transmit;
          MCuC_Model_B.MC_CAN_out = true;
        }
        break;

       default:
        // case IN_ESTOP_Low:
        MCuC_Model_B.ESTOP_Self_Test = true;

        // Inport: '<Root>/ESTOP_3V3'
        if (!MCuC_Model_U.ESTOP_3V3) {
          MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_High;
          MCuC_Model_B.ESTOP_Self_Test = false;
        } else {
          MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Fails;
          MCuC_Model_B.uC_State = 13U;
        }
        break;
      }
      break;

     case MCuC_Model_IN_Ignition_ST:
      switch (MCuC_Model_DW.is_Ignition_ST) {
       case MCuC_Model_IN_Ignition_Fails:
        break;

       case MCuC_Model_IN_Ignition_High:
        MCuC_Model_B.Ignition_Self_Test = false;

        // Inport: '<Root>/Ignition_3V3'
        if (!MCuC_Model_U.Ignition_3V3) {
          MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Fails;
          MCuC_Model_B.uC_State = 14U;
        } else {
          MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_Setup = MCuC_Model_IN_ESTOP_ST;
          MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_ESTOP_Low;
          MCuC_Model_B.ESTOP_Self_Test = true;
        }
        break;

       default:
        // case IN_Ignition_Low:
        MCuC_Model_B.Ignition_Self_Test = true;

        // Inport: '<Root>/Ignition_3V3'
        if (!MCuC_Model_U.Ignition_3V3) {
          MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_High;
          MCuC_Model_B.Ignition_Self_Test = false;
        } else {
          MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Fails;
          MCuC_Model_B.uC_State = 14U;
        }
        break;
      }
      break;

     case MCuC_Model_IN_MC_CAN_ST:
      switch (MCuC_Model_DW.is_MC_CAN_ST) {
       case MCuC_Model_IN_MC_CAN_fails:
        break;

       default:
        // case IN_MC_Transmit:
        MCuC_Model_B.MC_CAN_out = true;

        // Inport: '<Root>/MC_CAN_ST'
        if ((!MCuC_Model_U.MC_CAN_ST) && (MCuC_Model_DW.temporalCounter_i1 >=
             150U)) {
          MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_MC_CAN_fails;
          MCuC_Model_B.uC_State = 12U;
        } else if (MCuC_Model_U.MC_CAN_ST) {
          MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_Setup = MCuC_Model_IN_CAN_A_ST;
          MCuC_Model_DW.temporalCounter_i1 = 0U;
          MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_A_Transmit;
          MCuC_Model_B.CAN_A_out = true;
        }

        // End of Inport: '<Root>/MC_CAN_ST'
        break;
      }
      break;

     default:
      // case IN_MC_ST:
      switch (MCuC_Model_DW.is_MC_ST) {
       case MCuC_Model_IN_MC_Fails:
        break;

       case MCuC_Model_IN_MC_OFF:
        // Inport: '<Root>/MC_Status_3V3'
        if (MCuC_Model_U.MC_Status_3V3 && (MCuC_Model_DW.temporalCounter_i1 >=
             10U)) {
          MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_Fails;
          MCuC_Model_B.uC_State = 15U;
          MCuC_Model_B.MC_TOG_P_uC = false;
          MCuC_Model_B.MC_TOG_N_uC = false;
        } else if (!MCuC_Model_U.MC_Status_3V3) {
          MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
          MCuC_Model_DW.is_Setup = MCuC_Model_IN_Ignition_ST;
          MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_Ignition_Low;
          MCuC_Model_B.Ignition_Self_Test = true;
        }
        break;

       default:
        // Inport: '<Root>/MC_Status_3V3'
        // case IN_MC_ON:
        if (MCuC_Model_U.MC_Status_3V3) {
          MCuC_Model_DW.temporalCounter_i1 = 0U;
          MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_OFF;
          MCuC_Model_B.MC_TOG_P_uC = false;
          MCuC_Model_B.MC_TOG_N_uC = true;
        } else if ((!MCuC_Model_U.MC_Status_3V3) &&
                   (MCuC_Model_DW.temporalCounter_i1 >= 10U)) {
          MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_Fails;
          MCuC_Model_B.uC_State = 15U;
          MCuC_Model_B.MC_TOG_P_uC = false;
          MCuC_Model_B.MC_TOG_N_uC = false;
        }
        break;
      }
      break;
    }
  }

  // Function for Chart: '<Root>/MCuC'
  void MCuC_Model::MCuC_M_exit_internal_MCuC_Logic(void)
  {
    switch (MCuC_Model_DW.is_MCuC_Logic) {
     case MCuC_Model_IN_Fault:
      MCuC_Model_B.Fault = false;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
      break;

     case MCuC_Model_IN_LVSS_MC_Shutdown:
      MCuC_Model_B.MC_TOG_P_uC = false;
      MCuC_Model_B.MC_TOG_N_uC = false;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
      break;

     case MCuC_Model_IN_LVSS_MC_Startup:
      MCuC_Model_B.MC_TOG_P_uC = false;
      MCuC_Model_B.MC_TOG_N_uC = false;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
      break;

     case MCuC_Model_IN_MC_Active:
      // Outport: '<Root>/Torque_Request'
      MCuC_Model_Y.Torque_Request = 0U;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
      break;

     default:
      MCuC_Model_DW.is_CAN_A_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_ESTOP_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_Ignition_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_MC_CAN_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_Setup = MCuC_Model_IN_NO_ACTIVE_CHILD;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_NO_ACTIVE_CHILD;
      break;
    }
  }

  // Function for Chart: '<Root>/MCuC'
  void MCuC_Model::MCuC_Model_MCuC(bool *Watchdog)
  {
    bool guard1;
    bool tmp;
    bool tmp_0;
    bool tmp_1;
    tmp = !MCuC_Model_DW.uncharged;

    // Inport: '<Root>/Ignition_3V3'
    tmp_1 = !MCuC_Model_U.Ignition_3V3;

    // Inport: '<Root>/HM_Fault'
    guard1 = false;
    if (MCuC_Model_U.HM_Fault && tmp_1 && tmp && (!MCuC_Model_DW.Fault_Activated))
    {
      MCuC_Model_DW.Fault_Activated = true;
      guard1 = true;
    } else {
      // Inport: '<Root>/ESTOP_3V3'
      tmp_0 = !MCuC_Model_U.ESTOP_3V3;
      if (tmp_0 && tmp && (!MCuC_Model_DW.ESTOP_activated)) {
        MCuC_Model_DW.ESTOP_activated = true;
        guard1 = true;

        // Inport: '<Root>/nReset'
      } else if (tmp_0 && MCuC_Model_DW.uncharged && MCuC_Model_U.nReset &&
                 (MCuC_Model_B.uC_State != 10)) {
        MCuC_M_exit_internal_MCuC_Logic();
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_ESTOP;
        MCuC_Model_B.uC_State = 7U;
        MCuC_Model_B.LVSS_EN_3V3 = false;
        MCuC_Model_DW.ESTOP_activated = false;
      } else if (MCuC_Model_U.HM_Fault && MCuC_Model_DW.uncharged &&
                 (MCuC_Model_B.uC_State != 10) && MCuC_Model_U.nReset) {
        MCuC_M_exit_internal_MCuC_Logic();
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Fault;
        MCuC_Model_B.uC_State = 6U;
        MCuC_Model_DW.Fault_Activated = false;
        MCuC_Model_B.Fault = true;
      } else if (!MCuC_Model_U.nReset) {
        MCuC_M_exit_internal_MCuC_Logic();
        MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Setup;
        MCuC_Model_B.uC_State = 10U;
        MCuC_Model_B.MC_Self_Test = true;
        MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_ST;
        MCuC_Model_DW.temporalCounter_i1 = 0U;
        MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_ON;
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

          if (MCuC_Model_U.Start_CAN && MCuC_Model_U.Forward_EN_CAN &&
              MCuC_Model_U.Brake_CAN && (MCuC_Model_U.Throttle_CAN == 0)) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Active;
            MCuC_Model_B.Inverter_EN_CAN = true;
            MCuC_Model_B.uC_State = 3U;

            // Outport: '<Root>/Torque_Request'
            MCuC_Model_Y.Torque_Request = 0U;
          } else if (MCuC_Model_U.Ignition_3V3) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Open;
            MCuC_Model_B.Inverter_DIS_CAN = true;
            MCuC_Model_B.uC_State = 4U;
          }

          // End of Inport: '<Root>/Start_CAN'
          break;

         case MCuC_Model_IN_Contactor_Open:
          MCuC_Model_B.Inverter_EN_CAN = false;
          MCuC_Model_B.Inverter_DIS_CAN = true;

          // Inport: '<Root>/Discharge_SM_CAN'
          if (MCuC_Model_U.Discharge_SM_CAN != 0) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Discharging;
            MCuC_Model_B.uC_State = 5U;
          }
          break;

         case MCuC_Model_IN_ESTOP:
          MCuC_Model_B.LVSS_EN_3V3 = false;

          // Inport: '<Root>/ESTOP_3V3'
          if (MCuC_Model_U.ESTOP_3V3) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;
            MCuC_Model_B.uC_State = 0U;
            MCuC_Model_DW.uncharged = true;
            MCuC_Model_B.Inverter_EN_CAN = false;
            MCuC_Model_B.MC_Self_Test = false;
          }
          break;

         case MCuC_Model_IN_Fault:
          // Inport: '<Root>/MC_Status_3V3'
          if ((!MCuC_Model_U.MC_Status_3V3) && tmp_1) {
            MCuC_Model_B.Fault = false;
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Fault_Ignit_H;
          }
          break;

         case MCuC_Model_IN_Fault_Ignit_H:
          // Inport: '<Root>/Ignition_3V3'
          if (MCuC_Model_U.Ignition_3V3) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;
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

          if ((!MCuC_Model_U.MC_Status_3V3) && (!MCuC_Model_U.LVSS_Status_3V3))
          {
            MCuC_Model_B.MC_TOG_P_uC = false;
            MCuC_Model_B.MC_TOG_N_uC = false;
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Off;
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

          if (MCuC_Model_U.MC_Status_3V3 && MCuC_Model_U.LVSS_Status_3V3) {
            MCuC_Model_B.MC_TOG_P_uC = false;
            MCuC_Model_B.MC_TOG_N_uC = false;
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_MC_Init;
            MCuC_Model_B.uC_State = 1U;
          }
          break;

         case MCuC_Model_IN_MC_Active:
          MCuC_Model_B.Inverter_EN_CAN = true;

          // Inport: '<Root>/Forward_EN_CAN' incorporates:
          //   Inport: '<Root>/Ignition_3V3'

          if (!MCuC_Model_U.Forward_EN_CAN) {
            // Outport: '<Root>/Torque_Request'
            MCuC_Model_Y.Torque_Request = 0U;
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Closed;
            MCuC_Model_B.uC_State = 2U;
            MCuC_Model_B.Inverter_EN_CAN = false;
          } else if (MCuC_Model_U.Ignition_3V3) {
            // Outport: '<Root>/Torque_Request'
            MCuC_Model_Y.Torque_Request = 0U;
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Open;
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
          if (MCuC_Model_U.Discharge_SM_CAN == 4) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_LVSS_MC_Shutdown;
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

          if (MCuC_Model_U.Ignition_3V3) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Open;
            MCuC_Model_B.Inverter_EN_CAN = false;
            MCuC_Model_B.Inverter_DIS_CAN = true;
            MCuC_Model_B.uC_State = 4U;
          } else if (MCuC_Model_U.MC_SM_CAN == 5) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Closed;
            MCuC_Model_B.uC_State = 2U;
            MCuC_Model_B.Inverter_EN_CAN = false;
          }
          break;

         case MCuC_Model_IN_MC_Off:
          MCuC_Model_B.LVSS_EN_3V3 = false;
          MCuC_Model_B.Inverter_EN_CAN = false;
          MCuC_Model_B.MC_Self_Test = false;
          if (tmp_1) {
            MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_LVSS_MC_Startup;
            MCuC_Model_DW.uncharged = false;
            MCuC_Model_B.uC_State = 8U;
            MCuC_Model_B.LVSS_EN_3V3 = true;
            MCuC_Model_B.MC_TOG_P_uC = true;
            MCuC_Model_B.MC_TOG_N_uC = false;
          }
          break;

         default:
          // case IN_Setup:
          MCuC_Model_Setup();
          break;
        }

        // End of Inport: '<Root>/nReset'
      }
    }

    if (guard1) {
      MCuC_M_exit_internal_MCuC_Logic();
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Contactor_Open;
      MCuC_Model_B.Inverter_EN_CAN = false;
      MCuC_Model_B.Inverter_DIS_CAN = true;
      MCuC_Model_B.uC_State = 4U;
    }

    // End of Inport: '<Root>/HM_Fault'
    if (MCuC_Model_DW.is_Watchdog == MCuC_Model_IN_WD0) {
      *Watchdog = false;
      if (MCuC_Model_DW.temporalCounter_i2 >= 3U) {
        MCuC_Model_DW.temporalCounter_i2 = 0U;
        MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD1;
        *Watchdog = true;
      }
    } else {
      // case IN_WD1:
      *Watchdog = true;
      if (MCuC_Model_DW.temporalCounter_i2 >= 3U) {
        MCuC_Model_DW.temporalCounter_i2 = 0U;
        MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD0;
        *Watchdog = false;
      }
    }
  }
}

namespace VCU
{
  // Model step function
  void MCuC_Model::step()
  {
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
    if (MCuC_Model_DW.temporalCounter_i1 < 255U) {
      MCuC_Model_DW.temporalCounter_i1 = static_cast<uint8_t>
        (MCuC_Model_DW.temporalCounter_i1 + 1);
    }

    if (MCuC_Model_DW.temporalCounter_i2 < 3U) {
      MCuC_Model_DW.temporalCounter_i2 = static_cast<uint8_t>
        (MCuC_Model_DW.temporalCounter_i2 + 1);
    }

    if (MCuC_Model_DW.is_active_c2_MCuC_Model == 0U) {
      MCuC_Model_DW.is_active_c2_MCuC_Model = 1U;
      MCuC_Model_DW.is_MCuC_Logic = MCuC_Model_IN_Setup;
      MCuC_Model_B.uC_State = 10U;
      MCuC_Model_B.MC_Self_Test = true;
      MCuC_Model_DW.is_Setup = MCuC_Model_IN_MC_ST;
      MCuC_Model_DW.temporalCounter_i1 = 0U;
      MCuC_Model_DW.is_MC_ST = MCuC_Model_IN_MC_ON;
      MCuC_Model_B.MC_TOG_P_uC = true;
      MCuC_Model_B.MC_TOG_N_uC = false;
      MCuC_Model_DW.temporalCounter_i2 = 0U;
      MCuC_Model_DW.is_Watchdog = MCuC_Model_IN_WD0;

      // Update for Memory: '<Root>/Memory6'
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
  void MCuC_Model::initialize()
  {
    // SystemInitialize for Chart: '<Root>/MCuC'
    MCuC_Model_DW.uncharged = true;
  }

  // Model terminate function
  void MCuC_Model::terminate()
  {
    // (no terminate code required)
  }

  // Constructor
  MCuC_Model::MCuC_Model() :
    MCuC_Model_U(),
    MCuC_Model_Y(),
    MCuC_Model_B(),
    MCuC_Model_DW(),
    MCuC_Model_M()
  {
    // Currently there is no constructor body generated.
  }

  // Destructor
  // Currently there is no destructor body generated.
  MCuC_Model::~MCuC_Model() = default;

  // Real-Time Model get method
  MCuC_Model::RT_MODEL_MCuC_Model_T * MCuC_Model::getRTM()
  {
    return (&MCuC_Model_M);
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
