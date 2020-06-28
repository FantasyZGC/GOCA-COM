//==============================================================================
//
// Title:       G_comlib.h
// Purpose:     Port communication command library of GOCA
//
// Created on:  2020/3/5 by ZhaoGuoChen.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __constant_H__
#define __constant_H__

#ifdef __cplusplus
extern "C" 
{
#endif
    
    const int getGOCA = 0xA00000;

    const int toMBC_h = 0xA1;
    const int toCLD_h = 0xA2;
    const int toLLD_h = 0xA3;

    const int ADC1_getPower = 0xA40000;
    const int ADC1_getOrigin = 0xA40001;
    const int ADC2_getPower = 0xA50000;
    const int ADC2_getOrigin = 0xA50001;
    const int ADC3_getPower = 0xA60000;
    const int ADC3_getOrigin = 0xA60001;
    const int ADC_getCorrection = 0xAB0000;
    const int ADC_setCorrection_h = 0xAB; //0xAB(000A~00C8)

    const int DAC1_Output_h = 0xA7; //0xA7(1000~1FFF)
    const int DAC2_Output_h = 0xA8; //0xA8(1000~1FFF)

    const int getTemp = 0xA90000;

    const int Buzzer_close = 0xA90100;
    const int Buzzer_open = 0xA90101;

    const int OS1_close = 0xA90200;
    const int OS1_open = 0xA90201;
    const int OS1_getStatus = 0xA90202;
    const int OS2_close = 0xA90300;
    const int OS2_open = 0xA90301;
    const int OS2_getStatus = 0xA90302;
    const int OS3_close = 0xA90400;
    const int OS3_open = 0xA90401;
    const int OS3_getStatus = 0xA90402;

    const int LD_OFF_1310 = 0xA90500;
    const int LD_ON_1310 = 0xA90501;
    const int LD_getStatus_1310 = 0xA90502;

    const int CLD_reset = 0xA90601;

    const int MBC_OFF = 0xA90700;
    const int MBC_ON = 0xA90701;
    const int MBC_reset = 0xA90702;
    const int MBC_to1550 = 0xA90703;
    const int MBC_to1310 = 0xA90704;

    const int LED_close_OuterLaser = 0xA90800;
    const int LED_open_OuterLaser = 0xA90801;
    const int LED_close_InnerLaser = 0xA90900;
    const int LED_open_InnerLaser = 0xA90901;
    const int LED_close_ModulationLaser = 0xA9A000;
    const int LED_open_ModulationLaser = 0xA9A001;
    const int LED_close_ProbeLaser = 0xA9A100;
    const int LED_open_ProbeLaser = 0xA9A101;

    const int PCB_powerOFF = 0xA9A200;
    const int PCB_powerON = 0xA9A201;

    const int OS4_close = 0xA9A300;
    const int OS4_open = 0xA9A301;
    const int OS4_getStatus = 0xA9A302;

    const int MBC_getStatus = 0xA9A402;

    const int LLD_reset = 0xA9A501;

    const int switchMode_inner_C_M = 0xAA0000;
    const int switchMode_inner_C_D = 0xAA0001;
    const int switchMode_inner_O_M = 0xAA0002;
    const int switchMode_inner_O_D = 0xAA0003;
    const int switchMode_outer_CSL_M = 0xAA0004;
    const int switchMode_outer_O_M = 0xAA0005;
    const int switchMode_inner_L_M = 0xAA0006;
    const int switchMode_inner_L_D = 0xAA0007;
    const int switchMode_outer_OCSL_D = 0xAA0008;

    const int resetAll = 0xAA0009;

    const int PC8_OFF = 0xAC0000;
    const int PC8_switch_O = 0xAC0001;
    const int PC8_switch_C = 0xAC0002;
    const int PC8_switch_L = 0xAC0003;
    

#ifdef __cplusplus
}
#endif

#endif 
