//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Hardmon.h
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
#ifndef RTW_HEADER_Hardmon_h_
#define RTW_HEADER_Hardmon_h_
#include <stdbool.h>
#include <stdint.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef DEFINED_TYPEDEF_FOR_UC_State_
#define DEFINED_TYPEDEF_FOR_UC_State_

enum class UC_State
    : int16_t {
    MC_Off = 0,                        // Default value
    MC_Init,
    Contactor_Closed,
    MC_Active,
    Contactor_Open,
    MC_Discharging,
    Fault,
    Estop,
    Preset,
    LVSS_MC_Startup,
    LVSS_MC_Shutdown
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_MC_DC_State_
#define DEFINED_TYPEDEF_FOR_MC_DC_State_

enum class MC_DC_State
    : int16_t {
    Disabled = 0,                      // Default value
    Enabled,
    Speed_Check,
    Active,
    Complete
};

#endif

// Class declaration for model Hardmon
namespace vcu
{
    class Hardmon_Model final
    {
        // public data and function members
      public:
        // Block states (default storage) for system '<Root>'
        struct DW_Hardmon_T {
            double WD_Counter;         // '<Root>/Hardmon_Chart'
            double ESTOP_State_Counter;// '<Root>/Hardmon_Chart'
            double Ignition_State_Counter;// '<Root>/Hardmon_Chart'
            double Invalid_State_Counter;// '<Root>/Hardmon_Chart'
            double LVSS_Counter;       // '<Root>/Hardmon_Chart'
            double ESTOP_Counter;      // '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i1;// '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i2;// '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i3;// '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i4;// '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i5;// '<Root>/Hardmon_Chart'
            uint16_t temporalCounter_i6;// '<Root>/Hardmon_Chart'
            uint8_t is_active_c3_Hardmon;// '<Root>/Hardmon_Chart'
            uint8_t is_c3_Hardmon;     // '<Root>/Hardmon_Chart'
            uint8_t is_ESTOP_Checking; // '<Root>/Hardmon_Chart'
            uint8_t is_Watchdog_Checking;// '<Root>/Hardmon_Chart'
            uint8_t is_Invalid_State_Checking;// '<Root>/Hardmon_Chart'
            uint8_t is_Ignition_Level_Checking;// '<Root>/Hardmon_Chart'
            uint8_t is_ESTOP_Level_Checking;// '<Root>/Hardmon_Chart'
            uint8_t is_LVSS_Checking;  // '<Root>/Hardmon_Chart'
        };

        // External inputs (root inport signals with default storage)
        struct ExtU_Hardmon_T {
            bool Forward_EN;           // '<Root>/Forward_EN'
            bool Ignition_LS_B;        // '<Root>/Ignition_LS_B'
            bool Ignition_LS_A;        // '<Root>/Ignition_LS_A'
            bool LVSS_ON;              // '<Root>/LVSS_ON'
            bool MC_12V0;              // '<Root>/MC_12V0'
            UC_State uC_State;         // '<Root>/uC_State'
            bool ESTOP_LS_B;           // '<Root>/ESTOP_LS_B'
            MC_DC_State MC_DC_State_d; // '<Root>/MC_DC_State'
            bool Watchdog;             // '<Root>/Watchdog'
            bool ESTOP_LS_A;           // '<Root>/ESTOP_LS_A'
            bool LVSS_EN_uC;           // '<Root>/LVSS_EN_uC'
        };

        // External outputs (root outports fed by signals with default storage)
        struct ExtY_Hardmon_T {
            bool nMC_Switch_EN;        // '<Root>/nMC_Switch_EN'
            bool nLVSS_Switch_EN;      // '<Root>/nLVSS_Switch_EN'
            bool Inverter_DIS;         // '<Root>/Inverter_DIS'
            bool MC_EN_HM;             // '<Root>/MC_EN_HM'
            bool nReset;               // '<Root>/nReset'
            bool LVSS_EN_HM;           // '<Root>/LVSS_EN_HM'
            bool HM_Fault;             // '<Root>/HM_Fault'
        };

        // Real-time Model Data Structure
        struct RT_MODEL_Hardmon_T {
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
        Hardmon_Model::RT_MODEL_Hardmon_T * getRTM();

        // Root inports set method
        void setExternalInputs(const ExtU_Hardmon_T *pExtU_Hardmon_T)
        {
            Hardmon_U = *pExtU_Hardmon_T;
        }

        // Root outports get method
        const ExtY_Hardmon_T &getExternalOutputs() const
        {
            return Hardmon_Y;
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
        ExtU_Hardmon_T Hardmon_U;

        // External outputs
        ExtY_Hardmon_T Hardmon_Y;

        // Block states
        DW_Hardmon_T Hardmon_DW;

        // private member function(s) for subsystem '<Root>'
        void Hardmon_Invalid_State_Checking(void);
        void exit_internal_Normal_Operation(void);
        void Hardmon_Normal_Operation(void);
        void enter_internal_Normal_Operation(void);

        // Real-Time Model
        RT_MODEL_Hardmon_T Hardmon_M;
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
//  '<Root>' : 'Hardmon'
//  '<S1>'   : 'Hardmon/Hardmon_Chart'

#endif                                 // RTW_HEADER_Hardmon_h_

//
// File trailer for generated code.
//
// [EOF]
//
