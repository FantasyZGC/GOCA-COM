//==============================================================================
//
// Title:       G_comlib.h
// Purpose:     Port communication command library of GOCA
//
// Created on:  2020/3/5 by ZhaoGuoChen.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#include "G_comlib.h"
#include "G_COM.h"

/*
    initialize the port normally
*/
int G_initCOM(int comPort)
{
    int error;
    error = OpenComConfig(comPort, "", 9600, 0, 8, 1, 1, 4);
    if (error != 0)
        return error;
    FlushInQ(comPort);
    FlushOutQ(comPort);
    return 0;
}

/*
    send command to port normally
*/
int G_inputCOM(int comPort, int command)
{
    int error = -1;
    error = G_initCOM(comPort);
    if (error != 0)
    {
        return error;
    }
    ComWrtByte(comPort, command);
    Delay(0.001);
    //	CloseCom (comPort);
    return 0;
}


/*
    search the goca-Port and return it
    getGOCA = 0xA00000
*/
int G_getCOCA(int *comPort)
{
    int command = getGOCA,
        error = 0,
        isGOCA = 1;
    char getString[18] = {0};
    char GOCA[6] = "GOCA-";
    for (int i = 1; i < 21; i++)
    {
        error = G_initCOM(i);
        if (error == 0)
        {
            ComWrtByte(i, command);
            Delay(0.001);
            ComRd(i, getString, 18);
            isGOCA = strncmp(GOCA, getString, 5);
            if (isGOCA == 0)
            {
                *comPort = i;
                CloseCom(i);
                return 0;
            }
        }
    }
    return -1;
}

// ==============================================================================
/*  
    Forward to the MBC/CLD/LLD port
    comID: 1->MBC, 2->CLD, 3->LLD
    you shoule input 0xA1/A2/A3 + other number + 0d0a
*/
int G_toOtherCom(int comPort, int com_id, int command)
{
    int check_up = 0xFF,
        check_down = 0,
        check_head = 0,
        error = -1,
        head = command;

    // choose MBC/CLD/LLD
    switch (com_id)
    {
    case 1:
        check_head = toMBC_h;
        break;
    case 2:
        check_head = toCLD_h;
        break;
    case 3:
        check_head = toLLD_h;
        break;
    // it is error
    default:
        return error = -1;
    }

    // check the head of command is 0xA1/A2/A3
    while (1)
    {
        if (head <= check_up && head >= check_down && head == check_head)
        {
            error = G_inputCOM(comPort, command);
            break;
        }
        else
        {
            head = head / 0x100;
            if (head <= 0)
            {
                break;
            }
        }
    }
    return error;
}

/*
    get the power of ADC
    (char *Power) is the power of an ADC like +XX.XX
    ADC1_getPower = 0xA40000
    ADC2_getPower = 0xA50000
    ADC3_getPower = 0xA60000
*/
int G_getPowerOfADC(int comPort, int ADC_id, char *power)
{
    int command,
        error = -1;

    // intermediate variable needed to read the return data
    int x = 0;
    char y[2];

    switch (ADC_id)
    {
    case 1:
        command = ADC1_getPower;
        break;
    case 2:
        command = ADC2_getPower;
        break;
    case 3:
        command = ADC3_getPower;
        break;
    default:
        return error;
    }

    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 6; i++)
        {
            x = ComRdByte(comPort);
            sprintf(y, "%c", x);
            *(power + i) = y[0];
        }
    }
    return error;
}


/*
    get the original data of ADC
    (char *Power) is the original data of an ADC which is between 0~65535
    ADC1_getOrigin= 0xA40001
    ADC2_getOrigin= 0xA50001
    ADC3_getOrigin= 0xA60001
*/
int G_getOriginOfADC(int comPort, int ADC_id, char *origin)
{
    int command,
        error = -1;

    // intermediate variable needed to read the return data
    int x = 0;
    char y[2];

    switch (ADC_id)
    {
    case 1:
        command = ADC1_getOrigin;
        break;
    case 2:
        command = ADC2_getOrigin;
        break;
    case 3:
        command = ADC3_getOrigin;
        break;
    default:
        return error;
    }

    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            x = ComRdByte(comPort);
            sprintf(y, "%c", x);
            *(origin + i) = y[0];
        }
    }
    return error;
}

/*
    get the correction factor of all of ADC
    ADC_getCorrection = 0xAB0000
    (char *correction) is the correction factor Of ADC like X.XX
*/
int G_getCorrectionOfADC(int comPort, char *correction)
{
    int error = -1,
        command = ADC_getCorrection;

    // intermediate variable needed to read the return data
    int x = 0;
    char y[2];
    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            x = ComRdByte(comPort);
            sprintf(y, "%c", x);
            *(correction + i) = y[0];
        }
    }
    return error;
}


/*
    set the correction factor of all of ADC
    the command should between 0xAB(000A~00C8)
*/
int G_setCorrectionOfADC(int comPort, int command)
{
    int error = -1;
    if (command >= 0xAB000A && command <= 0xAB00C8)
    {
        error = G_inputCOM(comPort, command);
    }
    return error;
}

/*
    set the output power through the DAC
    DAC1:0xA7(1000~1FFF)
    DAC2:0xA8(1000~1FFF)
*/
int G_setOutputOfDAC(int comPort, int command, int DAC_id)
{
    int error = -1,
        whichDAC,
        check_up,
        check_down;

    switch (DAC_id)
    {
    case 1:
        check_down = 0xA71000;
        check_up = 0xA71FFF;
        break;
    case 2:
        check_down = 0xA81000;
        check_up = 0xA81FFF;
        break;
    default:
        return error;
    }
    if (command >= check_down && command <= check_up)
    {
        error = G_inputCOM(comPort, command);
    }
    return error;
}

/* 
    get temp like +XX.XX
    getTemp = 0xA90000
*/
int G_getTemp(int comPort, char *temp)
{
    int error = -1,
        command = getTemp;

    int x = 0;
    char y[2];

    error = G_inputCOM(comPort, command);
    if (error = 0)
    {
        for (int i = 0; i < 6; i++)
        {
            x = ComRdByte(comPort);
            sprintf(y, "%c", x);
            *(temp + i) = y[0];
        }
    }
    return error;
}

/*
    switch the status of Buzzer
    open = 0xA90100
    close = 0xA90101
*/
int G_switchBuzzer(int comPort, int status)
{
    int error = -1,
        command;

    switch (status)
    {
    case 0:
        command = Buzzer_close;
        break;
    case 1:
        command = Buzzer_open;
        break;
    default:
        return error;
    }

    error = G_inputCOM(comPort, command);
    return error;
}

/*
    get the status of any OS, OS_id can be input as 1/2/3/4
    OS1_getStatus = 0xA90202
    OS2_getStatus = 0xA90302
    OS3_getStatus = 0xA90402
    OS4_getStatus = 0xA9A302
    status == 1 -> ON, 0 -> OFF
*/
int G_getStatusOfOS(int comPort, int OS_id, int *status)
{
    int error = -1,
        data[3] = {0},
        command;
    switch (OS_id)
    {
    case 1:
        command = OS1_getStatus;
        break;
    case 2:
        command = OS2_getStatus;
        break;
    case 3:
        command = OS3_getStatus;
        break;
    case 4:
        command = OS4_getStatus;
        break;
    default:
        return error;
    }

    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] = ComRdByte(comPort);
        }
        if (data[2] == 0xA0)
        {
            *status = 0;
        }
        else if (data[2] == 0xA1)
        {
            *status = 1;
        }
    }

    return error;
}


/*
    switch the status of any OS
    you can get the command number from G_comlib.h
*/
int G_switchOS(int comPort, int OS_id, int status)
{
    int error = -1,
        command;
    if (OS_id > 4 || OS_id < 1)
    {
        return error;
    }

    // If the switch is off, turn it on
    if (status == 0)
    {
        switch (OS_id)
        {
        case 1:
            command = OS1_open;
            break;
        case 2:
            command = OS2_open;
            break;
        case 3:
            command = OS3_open;
            break;
        case 4:
            command = OS4_open;
            break;
        default:
            return error;
        }
    }
    // If the switch is on, turn it off
    else
    {
        switch (OS_id)
        {
        case 1:
            command = OS1_close;
            break;
        case 2:
            command = OS2_close;
            break;
        case 3:
            command = OS3_close;
            break;
        case 4:
            command = OS4_close;
            break;
        default:
            return error;
        }
    }
    error = G_inputCOM(comPort, command);

    return error;
}

/*
    get the status of LD1310
    LD_getStatus_1310 = 0xA90502
    status == 1 -> ON, 0 -> OFF
*/
int G_getStatusOfLD1310(int comPort, int *status)
{
    int error = -1,
        data[3] = {0},
        command = LD_getStatus_1310;
    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] = ComRdByte(comPort);
        }
        if (data[2] == 0xA0)
        {
            *status = 0;
        }
        else if (data[2] == 0xA1)
        {
            *status = 1;
        }
    }
    return error;
}

/*
    switch the status of LD1310
    LD_ON_1310 = 0xA90501
    LD_OFF_1310 = 0xA90500
*/
int G_switchLD1310(int comPort, int status)
{
    int error = -1,
        status,
        command;

    if (status == 0)
    {
        command = LD_OFF_1310;
    }
    else if (status == 1)
    {
        command = LD_ON_1310;
    }
    error = G_inputCOM(comPort, command);

    return error;
}


/*
    reset the CLD
    resetCLD = 0xA90601
*/
int G_resetCLD(int comPort)
{
    int error = -1,
        command = G_resetCLD;
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    switch the ON/OFF of MBC
    MBC_OFF = 0xA90700
    MBC_ON = 0xA90701
*/
int G_switchMBC(int comPort, int status)
{
    int error = -1,
        command;
    if (status == 0)
    {
        command = MBC_OFF;
    }
    else
    {
        command = MBC_ON;
    }
    error = G_inputCOM(comPort, command);
    return error;
}

/*
    reset MBC
    MBC_reset = 0xA90702
*/
int G_resetMBC(int comPort)
{
    int error,
        command = MBC_reset;
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    MBC points to the modulator 1550/1310
    MBC_to1550 = 0xA90703
    MBC_to1310 = 0xA90704
*/
int G_switchMBCToLD(int comPort, int status)
{
    int error = -1,
        command;
    if (status == 0)
    {
        command = MBC_to1550;
    }
    else
    {
        command = MBC_to1310;
    }
    error = G_inputCOM(comPort, command);
    return error;
}

/*
    Switch any panel indicator light switch status
*/
int G_switchLED(int comPort, int LED_id, int status)
{
    int error = -1,
        command;

    // close
    if (status == 0)
    {
        switch (LED_id)
        {
        case 1:
            command = LED_close_OuterLaser;
            break;
        case 2:
            command = LED_close_InnerLaser;
            break;
        case 3:
            command = LED_close_ModulationLaser;
            break;
        case 4:
            command = LED_close_ProbeLaser;
            break;
        default:
            return error;
        }
    }
    // open
    else
    {
        switch (LED_id)
        {
        case 1:
            command = LED_open_OuterLaser;
            break;
        case 2:
            command = LED_open_InnerLaser;
            break;
        case 3:
            command = LED_open_ModulationLaser;
            break;
        case 4:
            command = LED_open_ProbeLaser;
            break;
        default:
            return error;
        }
    }
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    switch the ON/OFF of PCB
    PCB_powerOFF = 0xA9A200
    PCB_powerON = 0xA9A201
*/
int G_switchPCB(int comPort, int status)
{
    int error = -1,
        command;
    if (status == 0)
    {
        command = PCB_powerOFF;
    }
    else
    {
        command = PCB_powerON;
    }
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    get the switch status of MBC, 0 is OFF, 1 is ON
    MBC_getStatus = 0xA9A402
*/
int G_getStatusOfMBC(int comPort, int *status)
{
    int error = -1,
        data[3] = {0},
        command = MBC_getStatus;

    error = G_inputCOM(comPort, command);
    if (error == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] = ComRdByte(comPort);
        }

        if (data[2] == 0xA0)
        {
            *status = 0;
        }
        else if (data[2] == 0xA1)
        {
            *status = 1;
        }
    }

    return error;
}


/*
    reset the LLD
    LLD_reset = 0xA9A501
*/
int G_resetLLD(int comPort)
{
    int error = -1,
        command = LLD_reset;
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    Switch working mode with light source
    C\O\L\CSL\OCSL is laser source
    M/D is modulation or direct
*/
int G_switchMode(int comPort, int mode)
{
    int error = -1,
        command;
    switch (mode)
    {
    case 0:
        command = switchMode_inner_C_M;
        break;
    case 1:
        command = switchMode_inner_C_D;
        break;
    case 2:
        command = switchMode_inner_O_M;
        break;
    case 3:
        command = switchMode_inner_O_D;
        break;
    case 4:
        command = switchMode_outer_CSL_M;
        break;
    case 5:
        command = switchMode_outer_O_M;
        break;
    case 6:
        command = switchMode_inner_L_M;
        break;
    case 7:
        command = switchMode_inner_L_D;
        break;
    case 8:
        command = switchMode_outer_OCSL_D;
        break;
    default:
        return error;
    }
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    Software reset
    resetAll = 0xAA0009
*/
int G_resetAll(int comPort)
{
    int error = -1,
        command = resetAll;
    error = G_inputCOM(comPort, command);
    return error;
}


/*
    set PC8 control a laser source or turn off it
    PC8_OFF = 0xAC0000;
    PC8_switch_O = 0xAC0001;
    PC8_switch_C = 0xAC0002;
    PC8_switch_L = 0xAC0003;
*/
int G_setPC8(int comPort, int status)
{
    int error = -1,
        command;
    switch (status)
    {
    case 0:
        command = PC8_OFF;
        break;
    case 1:
        command = PC8_switch_O;
        break;
    case 2:
        command = PC8_switch_C;
        break;
    case 3:
        command = PC8_switch_L;
        break;
    default:
        return error;
    }
    error = G_inputCOM(comPort, command);
    return error;
}