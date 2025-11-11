//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: MCuC_Model.h
//
// Code generated for Simulink model 'MCuC'.
//
// Model version                  : 5.8
// Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
// C/C++ source code generated on : Mon Nov 10 20:24:09 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef MCuC_Model_h_
    #define MCuC_Model_h_
    #include <stdbool.h>
    #include <stdint.h>
    #ifndef DEFINED_TYPEDEF_FOR_BMS_Contactor_Command_
        #define DEFINED_TYPEDEF_FOR_BMS_Contactor_Command_

enum class BMS_Contactor_Command : int16_t {
    Open_Contactor = 0, // Default value
    Close_Contactor,
    Stay
};

    #endif

    #ifndef DEFINED_TYPEDEF_FOR_UC_State_
        #define DEFINED_TYPEDEF_FOR_UC_State_

enum class UC_State : int16_t {
    Preset = 0, // Default value
    Key_Cycle,
    MC_Off,
    LVSS_MC_Startup,
    MC_Init,
    Contactor_Closed,
    MC_Ready,
    Contactor_Open,
    MC_Discharging,
    LVSS_MC_Shutdown,
    MC_Active,
    Estop,
    Fault,
    Super_Fault
};

    #endif

    #ifndef DEFINED_TYPEDEF_FOR_MC_VSM_State_
        #define DEFINED_TYPEDEF_FOR_MC_VSM_State_

enum class MC_VSM_State : int16_t {
    Start         = 0, // Default value
    PC_Init       = 1,
    PC_Active     = 2,
    PC_Finish     = 3,
    Wait          = 4,
    Ready         = 5,
    Motor_Running = 6,
    Fault         = 7,
    Shutdown      = 14,
    Recycle_Power = 15
};

    #endif

    #ifndef DEFINED_TYPEDEF_FOR_MC_DC_State_
        #define DEFINED_TYPEDEF_FOR_MC_DC_State_

enum class MC_DC_State : int16_t {
    Disabled = 0, // Default value
    Enabled,
    Speed_Check,
    Active,
    Complete
};

    #endif

// Class declaration for model MCuC
namespace vcu {
class MCuC_Model final {
    // public data and function members
public:
    // Block states (default storage) for system '<Root>'
    struct DW_MCuC_T {
        uint32_t Delay1_DSTATE[50];              // '<Root>/Delay1'
        BMS_Contactor_Command UnitDelay5_DSTATE; // '<Root>/Unit Delay5'
        UC_State UnitDelay6_DSTATE;              // '<Root>/Unit Delay6'
        int32_t clockTickCounter;                // '<Root>/Discrete Pulse Generator'
        bool UnitDelay4_DSTATE;                  // '<Root>/Unit Delay4'
        bool Delay_DSTATE[20];                   // '<Root>/Delay'
        uint8_t is_c2_MCuC;                      // '<Root>/MCuC_Chart'
        uint8_t is_Logic;                        // '<Root>/MCuC_Chart'
        uint8_t is_MC;                           // '<Root>/MCuC_Chart'
        uint8_t is_LS;                           // '<Root>/MCuC_Chart'
        uint8_t temporalCounter_i1;              // '<Root>/MCuC_Chart'
        uint8_t is_active_c3_MCuC;               // '<Root>/Cooling_Control_Chart'
        bool MC_Uncharged;                       // '<Root>/MCuC_Chart'
        bool CAN_ST_Complete;                    // '<Root>/MCuC_Chart'
        bool LS_ST_Complete;                     // '<Root>/MCuC_Chart'
        bool MC_ST_Complete;                     // '<Root>/MCuC_Chart'
        bool isNotInit;                          // '<Root>/MCuC_Chart'
    };

    // External inputs (root inport signals with default storage)
    struct ExtU_MCuC_T {
        bool Ignition_LS_A;                 // '<Root>/Ignition_LS_A'
        bool Ignition_LS_B;                 // '<Root>/Ignition_LS_B'
        bool ESTOP_LS_A;                    // '<Root>/ESTOP_LS_A'
        bool ESTOP_LS_B;                    // '<Root>/ESTOP_LS_B'
        bool MC_ON;                         // '<Root>/MC_ON'
        bool Start_CAN;                     // '<Root>/Start_CAN'
        bool Interlock;                     // '<Root>/Interlock'
        bool LVSS_ON_CAN;                   // '<Root>/LVSS_ON_CAN'
        bool Brake_CAN;                     // '<Root>/Brake_CAN'
        bool Forward_EN_CAN;                // '<Root>/Forward_EN_CAN'
        MC_VSM_State MC_VSM_State_CAN;      // '<Root>/MC_VSM_State_CAN'
        MC_DC_State MC_DC_State_CAN;        // '<Root>/MC_DC_State_CAN'
        int16_t Throttle_CAN;               // '<Root>/Throttle_CAN'
        bool BMS_Contactor_Closed_CAN;      // '<Root>/BMS_Contactor_Closed_CAN'
        int32_t BMS_Cell_Temps_CAN[45];     // '<Root>/BMS_Cell_Temps_CAN'
        int16_t BMS_Cell_Voltages_CAN[100]; // '<Root>/BMS_Cell_Voltages_CAN'
        uint8_t GFDB_Isolation_State_CAN;   // '<Root>/GFDB_Isolation_State_CAN'
        uint8_t MC_PS_Present_CAN;          // '<Root>/MC_PS_Present_CAN'
        uint8_t Batt_PS_Present_CAN;        // '<Root>/Batt_PS_Present_CAN'
        int32_t Cooling_Loop_Temps_CAN[5];  // '<Root>/Cooling_Loop_Temps_CAN'
        int16_t MC_Cooling_FR_CAN;          // '<Root>/MC_Cooling_FR_CAN'
        int16_t Batt_Cooling_FR_CAN;        // '<Root>/Batt_Cooling_FR_CAN'
        bool HIB_Comparison_Fault_CAN;      // '<Root>/HIB_Comparison_Fault_CAN'
        bool HIB_ON_CAN;                    // '<Root>/HIB_ON_CAN'
        bool HUDL_ON_CAN;                   // '<Root>/HUDL_ON_CAN'
        bool TMS_ON_CAN;                    // '<Root>/TMS_ON_CAN'
        bool GUB_ON_CAN;                    // '<Root>/GUB_ON_CAN'
        bool Acc_ON_CAN;                    // '<Root>/Acc_ON_CAN'
        bool Batt_12V_ON_CAN;               // '<Root>/Batt_12V_ON_CAN'
        int16_t Vicor_Input_Current_CAN;    // '<Root>/Vicor_Input_Current_CAN'
        int32_t LVSS_Temps_CAN[6];          // '<Root>/LVSS_Temps_CAN'
        int16_t LVSS_Currents_CAN[6];       // '<Root>/LVSS_Currents_CAN'
        uint32_t Heartbeats_CAN[5];         // '<Root>/Heartbeats_CAN'
        int16_t Direction_Command1;         // '<Root>/Direction_Command1'
        int16_t Rolling_Counter1;           // '<Root>/Rolling_Counter1'
        int16_t Speed_Command1;             // '<Root>/Speed_Command1'
        int16_t Speed_Mode_Enable1;         // '<Root>/Speed_Mode_Enable1'
        int16_t Torque_Limit_Command1;      // '<Root>/Torque_Limit_Command1'
        bool LS_Self_Test_In_A;             // '<Root>/LS_Self_Test_In_A'
        bool LS_Self_Test_In_B;             // '<Root>/LS_Self_Test_In_B'
    };

    // External outputs (root outports fed by signals with default storage)
    struct ExtY_MCuC_T {
        bool LVSS_EN_uC;                                    // '<Root>/LVSS_EN_uC'
        bool Fault;                                         // '<Root>/Fault'
        bool Super_Fault;                                   // '<Root>/Super_Fault'
        bool Watchdog;                                      // '<Root>/Watchdog'
        UC_State uC_State;                                  // '<Root>/uC_State'
        bool MC_EN_uC;                                      // '<Root>/MC_EN_uC'
        bool LED[3];                                        // '<Root>/LED'
        bool LS_Self_Test_Out;                              // '<Root>/LS_Self_Test_Out'
        bool CAN_Self_Test;                                 // '<Root>/CAN_Self_Test'
        bool MC_Self_Test;                                  // '<Root>/MC_Self_Test'
        bool Inverter_EN_uC_CAN;                            // '<Root>/Inverter_EN_uC_CAN'
        bool Fault_to_MC_CAN;                               // '<Root>/Fault_to_MC_CAN'
        bool Inverter_DC_uC_CAN;                            // '<Root>/Inverter_DC_uC_CAN'
        int16_t Torque_Request_CAN;                         // '<Root>/Torque_Request_CAN'
        BMS_Contactor_Command BMS_Contactor_Command_uC_CAN; // '<Root>/BMS_Contactor_Command_uC_CAN'
        uint8_t MC_PS_Request_uC_CAN;                       // '<Root>/MC_PS_Request_uC_CAN'
        uint8_t Batt_PS_Request_uC_CAN;                     // '<Root>/Batt_PS_Request_uC_CAN'
        bool HIB_EN_uC_CAN;                                 // '<Root>/HIB_EN_uC_CAN'
        bool HUDL_EN_uC_CAN;                                // '<Root>/HUDL_EN_uC_CAN'
        bool TMS_EN_uC_CAN;                                 // '<Root>/TMS_EN_uC_CAN'
        bool GUB_EN_uC_CAN;                                 // '<Root>/GUB_EN_uC_CAN'
        bool Acc_EN_uC_CAN;                                 // '<Root>/Acc_EN_uC_CAN'
        bool Batt_12V_EN_uC_CAN;                            // '<Root>/Batt_12V_EN_uC_CAN'
        int16_t Direction_Command_uC_CAN;                   // '<Root>/Direction_Command_uC_CAN'
        int16_t Rolling_Counter_uC_CAN;                     // '<Root>/Rolling_Counter_uC_CAN'
        int16_t Speed_Command_uC_CAN;                       // '<Root>/Speed_Command_uC_CAN'
        int16_t Speed_Mode_Enable_uC_CAN;                   // '<Root>/Speed_Mode_Enable_uC_CAN'
        int16_t Torque_Limit_Command_uC_CAN;                // '<Root>/Torque_Limit_Command_uC_CAN'
    };

    // Real-time Model Data Structure
    struct RT_MODEL_MCuC_T {
        const char* volatile errorStatus;
        const char* getErrorStatus() const;
        void setErrorStatus(const char* const volatile aErrorStatus);
    };

    // Copy Constructor
    MCuC_Model(MCuC_Model const&) = delete;

    // Assignment Operator
    MCuC_Model& operator=(MCuC_Model const&) & = delete;

    // Move Constructor
    MCuC_Model(MCuC_Model&&) = delete;

    // Move Assignment Operator
    MCuC_Model& operator=(MCuC_Model&&) = delete;

    // Real-Time Model get method
    MCuC_Model::RT_MODEL_MCuC_T* getRTM();

    // Root inports set method
    void setExternalInputs(const ExtU_MCuC_T* pExtU_MCuC_T) {
        MCuC_U = *pExtU_MCuC_T;
    }

    // Root outports get method
    const ExtY_MCuC_T& getExternalOutputs() const {
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
    void MCuC_MC_Ready(const bool* AND);
    void MCuC_MC_Active(void);
    void MCuC_MC_Init(void);
    void MCuC_exit_internal_Logic(void);
    void MCuC_Logic(const bool* LogicalOperator, const bool* OR, const bool* AND);

    // Real-Time Model
    RT_MODEL_MCuC_T MCuC_M;
};
} // namespace vcu

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Cast To Boolean' : Eliminate redundant data type conversion

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
//  '<S1>'   : 'MCuC/Compare To Constant'
//  '<S2>'   : 'MCuC/Compare To Constant1'
//  '<S3>'   : 'MCuC/Compare To Constant2'
//  '<S4>'   : 'MCuC/Compare To Constant3'
//  '<S5>'   : 'MCuC/Compare To Constant4'
//  '<S6>'   : 'MCuC/Compare To Constant5'
//  '<S7>'   : 'MCuC/Compare To Zero'
//  '<S8>'   : 'MCuC/Compare To Zero1'
//  '<S9>'   : 'MCuC/Cooling_Control_Chart'
//  '<S10>'  : 'MCuC/MCuC_Chart'

#endif // MCuC_Model_h_

//
// File trailer for generated code.
//
// [EOF]
//
