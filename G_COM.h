//==============================================================================
//
// Title:		G_COM.h
// Purpose:		All of comPort control functions of GOCA.
//
// Created on:	2020/3/5 by ZhaoGuoChen.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Device_H__
#define __Device_H__

#ifdef __cplusplus
extern "C" 
{
#endif
    
    int G_initCOM(int comPort);
    int G_inputCOM(int comPort, int command);
    int G_getCOCA(int *comPort);
    int G_toOtherCom(int comPort, int com_id, int command);
    int G_getPowerOfADC(int comPort, int ADC_id, char *power);
    int G_getOriginOfADC(int comPort, int ADC_id, char *origin);
    int G_getCorrectionOfADC(int comPort, char *correction);
    int G_setCorrectionOfADC(int comPort, int command);
    int G_setOutputOfDAC(int comPort, int command, int DAC_id);
    int G_getTemp(int comPort, char *temp);
    int G_switchBuzzer(int comPort, int status);
    int G_getStatusOfOS(int comPort, int OS_id, int *status);
    int G_switchOS(int comPort, int OS_id, int status);
    int G_getStatusOfLD1310(int comPort, int *status);
    int G_switchLD1310(int comPort, int status);
    int G_resetCLD(int comPort);
    int G_switchMBC(int comPort, int status);
    int G_resetMBC(int comPort);
    int G_switchMBCToLD(int comPort, int status);
    int G_switchLED(int comPort, int LED_id, int status);
    int G_switchPCB(int comPort, int status);
    int G_getStatusOfMBC(int comPort, int *status);
    int G_resetLLD(int comPort);
    int G_switchMode(int comPort, int mode);
    int G_resetAll(int comPort);
    int G_setPC8(int comPort, int status);
    
#ifdef __cplusplus
}
#endif

#endif 
