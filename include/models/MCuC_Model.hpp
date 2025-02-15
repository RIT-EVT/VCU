//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC.h
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
#ifndef RTW_HEADER_MCuC_h_
#define RTW_HEADER_MCuC_h_
#include <stdbool.h>
#include <stdint.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef DEFINED_TYPEDEF_FOR_MC_VSM_State_
#define DEFINED_TYPEDEF_FOR_MC_VSM_State_

enum class MC_VSM_State
    : int16_t {
    Start = 0,                         // Default value
    PC_Init = 1,
    PC_Active = 2,
    PC_Finish = 3,
    Wait = 4,
    Ready = 5,
    Motor_Running = 6,
    Fault = 7,
    Shutdown = 14,
    Recycle_Power = 15
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

// Class declaration for model MCuC
namespace vcu
{
    class MCuC_Model final
    {
        // public data and function members
      public:
        // Block states (default storage) for system '<Root>'
        struct DW_MCuC_T {
            uint8_t is_MCuC_Logic;     // '<Root>/MCuC_Chart'
            uint8_t is_MC_ST;          // '<Root>/MCuC_Chart'
            uint8_t is_Watchdog;       // '<Root>/MCuC_Chart'
            uint8_t temporalCounter_i1;// '<Root>/MCuC_Chart'
            bool uncharged;            // '<Root>/MCuC_Chart'
            bool ESTOP_activated;      // '<Root>/MCuC_Chart'
            bool Fault_Activated;      // '<Root>/MCuC_Chart'
            bool isNotInit;            // '<Root>/MCuC_Chart'
        };

        // External inputs (root inport signals with default storage)
        struct ExtU_MCuC_T {
            bool Ignition_LS_A;        // '<Root>/Ignition_LS_A'
            bool Start;                // '<Root>/Start'
            bool Brake;                // '<Root>/Brake'
            bool ESTOP_LS_A;           // '<Root>/ESTOP_LS_A'
            bool Forward_EN;           // '<Root>/Forward_EN'
            MC_VSM_State MC_VSM_State_g;// '<Root>/MC_VSM_State'
            MC_DC_State MC_DC_State_p; // '<Root>/MC_DC_State'
            bool nReset;               // '<Root>/nReset'
            bool HM_Fault;             // '<Root>/HM_Fault'
            int16_t Throttle;          // '<Root>/Throttle'
            bool LVSS_ON;              // '<Root>/LVSS_ON'
            bool MC_12V0;              // '<Root>/MC_12V0'
        };

        // External outputs (root outports fed by signals with default storage)
        struct ExtY_MCuC_T {
            bool LVSS_EN_uC;           // '<Root>/LVSS_EN_uC'
            bool Inverter_EN;          // '<Root>/Inverter_EN'
            bool Fault;                // '<Root>/Fault'
            bool Watchdog;             // '<Root>/Watchdog'
            UC_State uC_State;         // '<Root>/uC_State'
            bool Inverter_DIS;         // '<Root>/Inverter_DIS'
            bool MC_EN_uC;             // '<Root>/MC_EN_uC'
            int16_t Torque_Request;    // '<Root>/Torque_Request'
            bool MC_Self_Test;         // '<Root>/MC_Self_Test'
        };

        // Real-time Model Data Structure
        struct RT_MODEL_MCuC_T {
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
        MCuC_Model::RT_MODEL_MCuC_T * getRTM();

        // Root inports set method
        void setExternalInputs(const ExtU_MCuC_T *pExtU_MCuC_T)
        {
            MCuC_U = *pExtU_MCuC_T;
        }

        // Root outports get method
        const ExtY_MCuC_T &getExternalOutputs() const
        {
            return MCuC_Y;
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
        ExtU_MCuC_T MCuC_U;

        // External outputs
        ExtY_MCuC_T MCuC_Y;

        // Block states
        DW_MCuC_T MCuC_DW;

        // private member function(s) for subsystem '<Root>'
        void MCuC_Preset(void);
        void MCuC_exit_internal_MCuC_Logic(void);
        void MCuC_MCuC(void);

        // Real-Time Model
        RT_MODEL_MCuC_T MCuC_M;
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
//  '<Root>' : 'MCuC'
//  '<S1>'   : 'MCuC/MCuC_Chart'

#endif                                 // RTW_HEADER_MCuC_h_

//
// File trailer for generated code.
//
// [EOF]
//
