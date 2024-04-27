//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.h
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
#ifndef RTW_HEADER_MCuC_Model_h_
#define RTW_HEADER_MCuC_Model_h_
#include <cstdint>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model MCuC_Model
namespace VCU
{
    class MCuC_Model final
    {
        // public data and function members
      public:
        // Block signals (default storage)
        struct B_MCuC_Model_T {
            uint8_t uC_State;          // '<Root>/MCuC'
            bool Fault;                // '<Root>/MCuC'
            bool Inverter_EN_CAN;      // '<Root>/MCuC'
            bool LVSS_EN_3V3;          // '<Root>/MCuC'
            bool Inverter_DIS_CAN;     // '<Root>/MCuC'
            bool MC_TOG_N_uC;          // '<Root>/MCuC'
            bool MC_TOG_P_uC;          // '<Root>/MCuC'
            bool MC_Self_Test;         // '<Root>/MCuC'
            bool ESTOP_Self_Test;      // '<Root>/MCuC'
            bool Ignition_Self_Test;   // '<Root>/MCuC'
            bool CAN_A_out;            // '<Root>/MCuC'
            bool MC_CAN_out;           // '<Root>/MCuC'
        };

        // Block states (default storage) for system '<Root>'
        struct DW_MCuC_Model_T {
            uint8_t Memory4_PreviousInput;// '<Root>/Memory4'
            uint8_t is_active_c2_MCuC_Model;// '<Root>/MCuC'
            uint8_t is_MCuC_Logic;     // '<Root>/MCuC'
            uint8_t is_Setup;          // '<Root>/MCuC'
            uint8_t is_CAN_A_ST;       // '<Root>/MCuC'
            uint8_t is_ESTOP_ST;       // '<Root>/MCuC'
            uint8_t is_Ignition_ST;    // '<Root>/MCuC'
            uint8_t is_MC_CAN_ST;      // '<Root>/MCuC'
            uint8_t is_MC_ST;          // '<Root>/MCuC'
            uint8_t is_Watchdog;       // '<Root>/MCuC'
            uint8_t temporalCounter_i1;// '<Root>/MCuC'
            uint8_t temporalCounter_i2;// '<Root>/MCuC'
            bool Memory5_PreviousInput;// '<Root>/Memory5'
            bool Memory2_PreviousInput;// '<Root>/Memory2'
            bool Memory_PreviousInput; // '<Root>/Memory'
            bool Memory6_PreviousInput;// '<Root>/Memory6'
            bool Memory1_PreviousInput;// '<Root>/Memory1'
            bool Memory8_PreviousInput;// '<Root>/Memory8'
            bool Memory3_PreviousInput;// '<Root>/Memory3'
            bool Memory7_PreviousInput;// '<Root>/Memory7'
            bool Memory9_PreviousInput;// '<Root>/Memory9'
            bool Memory10_PreviousInput;// '<Root>/Memory10'
            bool Memory11_PreviousInput;// '<Root>/Memory11'
            bool Memory12_PreviousInput;// '<Root>/Memory12'
            bool uncharged;            // '<Root>/MCuC'
            bool ESTOP_activated;      // '<Root>/MCuC'
            bool Fault_Activated;      // '<Root>/MCuC'
        };

        // External inputs (root inport signals with default storage)
        struct ExtU_MCuC_Model_T {
            bool Ignition_3V3;         // '<Root>/Ignition_3V3'
            bool Start_CAN;            // '<Root>/Start_CAN'
            bool Brake_CAN;            // '<Root>/Brake_CAN'
            bool ESTOP_3V3;            // '<Root>/ESTOP_3V3'
            bool Forward_EN_CAN;       // '<Root>/Forward_EN_CAN'
            uint8_t MC_SM_CAN;         // '<Root>/MC_SM_CAN'
            uint8_t Discharge_SM_CAN;  // '<Root>/Discharge_SM_CAN'
            bool nReset;               // '<Root>/nReset'
            bool HM_Fault;             // '<Root>/HM_Fault'
            int16_t Throttle_CAN;      // '<Root>/Throttle_CAN'
            bool LVSS_Status_3V3;      // '<Root>/LVSS_Status_3V3'
            bool MC_Status_3V3;        // '<Root>/MC_Status_3V3'
            bool MC_CAN_ST;            // '<Root>/MC_CAN_ST'
            bool A_CAN_ST;             // '<Root>/A_CAN_ST'
        };

        // External outputs (root outports fed by signals with default storage)
        struct ExtY_MCuC_Model_T {
            bool LVSS_EN_uC;           // '<Root>/LVSS_EN_uC'
            bool Inverter_EN_CAN;      // '<Root>/Inverter_EN_CAN'
            bool Fault;                // '<Root>/Fault'
            bool Watchdog;             // '<Root>/Watchdog'
            uint8_t uC_State;          // '<Root>/uC_State'
            bool Inverter_DIS_CAN;     // '<Root>/Inverter_DIS_CAN'
            bool MC_TOGp_uC;           // '<Root>/MC_TOGp_uC'
            bool MC_TOGn_uC;           // '<Root>/MC_TOGn_uC'
            int16_t Torque_Request;    // '<Root>/Torque_Request'
            bool MC_Self_Test;         // '<Root>/MC_Self_Test'
            bool ESTOP_Self_Test;      // '<Root>/ESTOP_Self_Test'
            bool Ignition_Self_Test1;  // '<Root>/Ignition_Self_Test1'
            bool CAN_A_out;            // '<Root>/CAN_A_out'
            bool MC_CAN_out;           // '<Root>/MC_CAN_out'
        };

        // Real-time Model Data Structure
        struct RT_MODEL_MCuC_Model_T {
            const char * volatile errorStatus;
        };

        // Copy Constructor
        MCuC_Model(MCuC_Model const&) = delete;

        // Assignment Operator
        MCuC_Model& operator= (MCuC_Model const&) & = delete;

        // Move Constructor
        MCuC_Model(MCuC_Model &&) = delete;

        // Move Assignment Operator
        MCuC_Model& operator= (MCuC_Model &&) = delete;

        // Real-Time Model get method
        MCuC_Model::RT_MODEL_MCuC_Model_T * getRTM();

        // Root inports set method
        void setExternalInputs(const ExtU_MCuC_Model_T *pExtU_MCuC_Model_T)
        {
            MCuC_Model_U = *pExtU_MCuC_Model_T;
        }

        // Root outports get method
        const ExtY_MCuC_Model_T &getExternalOutputs() const
        {
            return MCuC_Model_Y;
        }

        // model initialize function
        void initialize();

        // model step function
        void step();

        // model terminate function
        static void terminate();

        // Constructor
        MCuC_Model();

        // Destructor
        ~MCuC_Model();

        // private data and function members
      private:
        // External inputs
        ExtU_MCuC_Model_T MCuC_Model_U;

        // External outputs
        ExtY_MCuC_Model_T MCuC_Model_Y;

        // Block signals
        B_MCuC_Model_T MCuC_Model_B;

        // Block states
        DW_MCuC_Model_T MCuC_Model_DW;

        // private member function(s) for subsystem '<Root>'
        void MCuC_Model_Setup(void);
        void MCuC_M_exit_internal_MCuC_Logic(void);
        void MCuC_Model_MCuC(bool *Watchdog);

        // Real-Time Model
        RT_MODEL_MCuC_Model_T MCuC_Model_M;
    };
}

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'MCuC_Model'
//  '<S1>'   : 'MCuC_Model/MCuC'

#endif                                 // RTW_HEADER_MCuC_Model_h_

//
// File trailer for generated code.
//
// [EOF]
//
