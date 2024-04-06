//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Hardmon_Model.h
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
#ifndef RTW_HEADER_Hardmon_Model_h_
#define RTW_HEADER_Hardmon_Model_h_
#include <stdbool.h>
#include <stdint.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model Hardmon_Model
namespace VCU
{
    class Hardmon_Model final
    {
        // public data and function members
      public:
        // Block signals (default storage)
        struct B_Hardmon_Model_T {
            bool nMC_Switch_EN;        // '<Root>/Chart'
            bool Inverter_DIS_CAN;     // '<Root>/Chart'
            bool MC_TOG_N_HM;          // '<Root>/Chart'
            bool MC_TOG_P_HM;          // '<Root>/Chart'
            bool nReset;               // '<Root>/Chart'
            bool LVSS_EN_HM;           // '<Root>/Chart'
            bool HM_Fault;             // '<Root>/Chart'
            bool nLVSS_Switch_EN;      // '<Root>/Chart'
        };

        // Block states (default storage) for system '<Root>'
        struct DW_Hardmon_Model_T {
            double pows[4];            // '<S3>/Binary to Scalar'
            uint8_t WD_Counter;        // '<Root>/Chart'
            uint8_t ESTOP_State_Counter;// '<Root>/Chart'
            uint8_t Ignition_State_Counter;// '<Root>/Chart'
            uint8_t Invalid_State_Counter;// '<Root>/Chart'
            uint8_t LVSS_Counter;      // '<Root>/Chart'
            uint8_t ESTOP_Counter;     // '<Root>/Chart'
            uint8_t is_active_c3_Hardmon_Model;// '<Root>/Chart'
            uint8_t is_c3_Hardmon_Model;// '<Root>/Chart'
            uint8_t is_ESTOP_Checking; // '<Root>/Chart'
            uint8_t is_Watchdog_Checking;// '<Root>/Chart'
            uint8_t is_Invalid_State_Checking;// '<Root>/Chart'
            uint8_t is_Ignition_Level_Checking;// '<Root>/Chart'
            uint8_t is_ESTOP_Level_Checking;// '<Root>/Chart'
            uint8_t is_LVSS_Checking;  // '<Root>/Chart'
            uint8_t temporalCounter_i1;// '<Root>/Chart'
            uint8_t temporalCounter_i2;// '<Root>/Chart'
            uint8_t temporalCounter_i3;// '<Root>/Chart'
            uint8_t temporalCounter_i4;// '<Root>/Chart'
            uint8_t temporalCounter_i5;// '<Root>/Chart'
            uint8_t temporalCounter_i6;// '<Root>/Chart'
            bool Memory_PreviousInput; // '<Root>/Memory'
            bool Memory7_PreviousInput;// '<Root>/Memory7'
            bool Memory6_PreviousInput;// '<Root>/Memory6'
            bool Memory1_PreviousInput;// '<Root>/Memory1'
            bool Memory2_PreviousInput;// '<Root>/Memory2'
            bool Memory3_PreviousInput;// '<Root>/Memory3'
            bool Memory4_PreviousInput;// '<Root>/Memory4'
            bool Memory5_PreviousInput;// '<Root>/Memory5'
        };

        // External inputs (root inport signals with default storage)
        struct ExtU_Hardmon_Model_T {
            bool Forward_EN_CAN;       // '<Root>/Forward_EN_CAN'
            bool Ignition_12V0;        // '<Root>/Ignition_12V0'
            bool Ignition_3V3;         // '<Root>/Ignition_3V3'
            bool LVSS_Status_3V3;      // '<Root>/LVSS_Status_3V3'
            bool MC_Status_3V3;        // '<Root>/MC_Status_3V3'
            uint8_t uC_State[4];       // '<Root>/uC_State'
            bool ESTOP_12V0;           // '<Root>/ESTOP_12V0'
            uint8_t Discharge_SM_CAN;  // '<Root>/Discharge_SM_CAN'
            bool Watchdog;             // '<Root>/Watchdog'
            bool ESTOP_3V3;            // '<Root>/ESTOP_3V3'
            bool LVSS_EN_uC;           // '<Root>/LVSS_EN_uC'
        };

        // External outputs (root outports fed by signals with default storage)
        struct ExtY_Hardmon_Model_T {
            bool nMC_Switch_EN;        // '<Root>/nMC_Switch_EN'
            bool nLVSS_Switch_EN;      // '<Root>/nLVSS_Switch_EN'
            bool Inverter_DIS_CAN;     // '<Root>/Inverter_DIS_CAN'
            bool MC_TOG_N_HM;          // '<Root>/MC_TOG_N_HM'
            bool MC_TOG_P_HM;          // '<Root>/MC_TOG_P_HM'
            bool nReset;               // '<Root>/nReset'
            bool LVSS_EN_HM;           // '<Root>/LVSS_EN_HM'
            bool HM_Fault;             // '<Root>/HM_Fault'
        };

        // Real-time Model Data Structure
        struct RT_MODEL_Hardmon_Model_T {
            const char * volatile errorStatus;
        };

        // Copy Constructor
        Hardmon_Model(Hardmon_Model const&) = delete;

        // Assignment Operator
        Hardmon_Model& operator= (Hardmon_Model const&) & = delete;

        // Move Constructor
        Hardmon_Model(Hardmon_Model &&) = delete;

        // Move Assignment Operator
        Hardmon_Model& operator= (Hardmon_Model &&) = delete;

        // Real-Time Model get method
        Hardmon_Model::RT_MODEL_Hardmon_Model_T * getRTM();

        // Root inports set method
        void setExternalInputs(const ExtU_Hardmon_Model_T *pExtU_Hardmon_Model_T)
        {
            Hardmon_Model_U = *pExtU_Hardmon_Model_T;
        }

        // Root outports get method
        const ExtY_Hardmon_Model_T &getExternalOutputs() const
        {
            return Hardmon_Model_Y;
        }

        // model initialize function
        void initialize();

        // model step function
        void step();

        // model terminate function
        static void terminate();

        // Constructor
        Hardmon_Model();

        // Destructor
        ~Hardmon_Model();

        // private data and function members
      private:
        // External inputs
        ExtU_Hardmon_Model_T Hardmon_Model_U;

        // External outputs
        ExtY_Hardmon_Model_T Hardmon_Model_Y;

        // Block signals
        B_Hardmon_Model_T Hardmon_Model_B;

        // Block states
        DW_Hardmon_Model_T Hardmon_Model_DW;

        // private member function(s) for subsystem '<Root>'
        void Hardmon__Invalid_State_Checking(const uint8_t *DataTypeConversion);
        void exit_internal_Normal_Operation(void);
        void Hardmon_Model_Normal_Operation(const uint8_t *DataTypeConversion);
        void enter_internal_Normal_Operation(void);

        // Real-Time Model
        RT_MODEL_Hardmon_Model_T Hardmon_Model_M;
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
//  '<Root>' : 'Hardmon_Model'
//  '<S1>'   : 'Hardmon_Model/Binary Vector Conversion'
//  '<S2>'   : 'Hardmon_Model/Chart'
//  '<S3>'   : 'Hardmon_Model/Binary Vector Conversion/From Unipolar'
//  '<S4>'   : 'Hardmon_Model/Binary Vector Conversion/From Unipolar/Binary to Scalar'
//  '<S5>'   : 'Hardmon_Model/Binary Vector Conversion/From Unipolar/InputInvert'
//  '<S6>'   : 'Hardmon_Model/Binary Vector Conversion/From Unipolar/InputInvert/off'

#endif                                 // RTW_HEADER_Hardmon_Model_h_

//
// File trailer for generated code.
//
// [EOF]
//
