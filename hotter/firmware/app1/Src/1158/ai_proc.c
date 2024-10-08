/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "ai_proc.h"
#include "sys.h"
#include "pt100.h"
/* NOTES:
 * - Unless specified, tests should be self-contained (results should not depend on the testing sequence)!
 * - Tests that modify register configurations should restore the device to its default state.
 * - All tests should return a boolean data type!
 * - All test functions and conditions should be documented to explain their significance.
 * - Avoid polling nDRDY without a timeout to avoid the possibility of software entering into an infinite loop.
 */

static data_ai_stru data_ai;



void ads1158_config(unsigned char adc_No)
{
    uint8_t  writeValues[7];

    // Test configuration
    writeValues[0]   = CONFIG0_DEFAULT & ~CONFIG0_MUXMOD_MASK;  // Auto-scan mode
    writeValues[1]   = CONFIG1_DEFAULT & CONFIG1_DRATE_1953SPS; // Slowest data rate
    writeValues[2]   = MUXSCH_DEFAULT;
    writeValues[3]   =
        MUXDIF_DEFAULT;                        // Differential channels off
    writeValues[4]   =
        0xff;                                      // Single-ended channels off
    writeValues[5]   = 0xff;
    writeValues[6]   = SYSRED_REF_ENABLE      |
                       SYSRED_GAIN_ENABLE   | \
                       SYSRED_TEMP_ENABLE   | \
                       SYSRED_VCC_ENABLE      | \
                       SYSRED_OFFSET_ENABLE;                      // Enable all system monitors

    // Ensure device is powered, converting, and reset to default values
    startConversions();
    //toggleRESET();

    // Write registers
    writeMultipleRegisters(adc_No, REG_ADDR_CONFIG0, 7, writeValues);

}

// bool test_read_data(void)
// Tests if reading data from the internal monitoring channels returns data within an expected range.
unsigned char adc_read_data(void)
{
    uint8_t b_pass = 1;
    uint8_t i;//, statusBytes[5];//, statusGood;
    // Read all data monitor channels
    for (i = 0; i < CHANNEL_SIZE; i++)
    {

        // Not required, but added to allow for debugger to halt code execution
        setSTART(HIGH);

        // Wait and check that /DRDY interrupt occurred
        b_pass = waitForDRDYinterrupt(1, 10);

        //  Read data in read direct mode
        if (b_pass == 1)
            //dataValues[i] = readData(&statusBytes[i], NULL, DIRECT);
            readData(1, DIRECT);
        // Not required, but added to allow for debugger to halt code execution
        setSTART(LOW);
    }

    for (i = 0; i < CHANNEL_SIZE; i++)
    {

        // Not required, but added to allow for debugger to halt code execution
        setSTART(HIGH);

        // Wait and check that /DRDY interrupt occurred
        b_pass = waitForDRDYinterrupt(2, 10);

        //  Read data in read direct mode
        if (b_pass == 2)
            //dataValues[i] = readData(&statusBytes[i], NULL, DIRECT);
            readData(2, DIRECT);
        // Not required, but added to allow for debugger to halt code execution
        setSTART(LOW);
    }
    if (get_ADC_data()->update)
    {
        data_ai.ref1 = get_ADC_data()->data_2byte[CHANNEL_SIZE - 1];
        data_ai.ref1 = data_ai.ref1 / 3072.0;

        data_ai.ref2 =  get_ADC_data()->data2_2byte[CHANNEL_SIZE - 1];
        data_ai.ref2 = data_ai.ref2 / 3072.0;
        if (data_ai.ref1 >= 2.4 && data_ai.ref1 <= 2.6)
        {
            for (i = 0; i < 16; i++) //ads1158 1
            {
                data_ai.data_ai[i] =  get_ADC_data()->data_2byte[i];
                data_ai.data_ai[i] = data_ai.data_ai[i] / 30720;
                data_ai.data_ai[i] = data_ai.data_ai[i] * data_ai.ref1;
            }
            get_ADC_data()->update = 0;
            //return 0;
        }
        else
        {
            get_ADC_data()->update = 1;
            return 4;
        }

        if (data_ai.ref2 >= 2.4 && data_ai.ref2 <= 2.6)
        {
            for (i = 0; i < 16; i++) //ads1158 2
            {
                data_ai.data2_ai[i] = get_ADC_data()->data2_2byte[i];
                data_ai.data2_ai[i] = data_ai.data2_ai[i] / 30720;
                data_ai.data2_ai[i] = data_ai.data2_ai[i] * data_ai.ref2;

            }
            get_ADC_data()->update = 0;
            return 0;
        }
        else
        {
            get_ADC_data()->update = 1;
            return 4;
        }

    }
    else
        return 4;
    //return b_pass;
}

void pressure_temp_proc()
{

    float volt_tmp, volt_tmp2, tmp1, tmp2, tmp3, tmp4;
    unsigned char i;
    /**********************4-20mA convention**********************/
    for (i = ADC1_PR_INDEX; i < (ADC1_PR_INDEX+ADC1_PR_SIZE); i++) //u2/(2u1-u2)
    {
        data_ai.press[i - ADC1_PR_INDEX]  = data_ai.data_ai[i] * PRESS_RATIO +
                                            PRESS_B;//kpa
        data_ai.press[i - ADC1_PR_INDEX]  = data_ai.press[i - ADC1_PR_INDEX]  /
                                            1000; //Mpa
        if (data_ai.press[i - ADC1_PR_INDEX] < MIN_PRESS)
            data_ai.press[i - ADC1_PR_INDEX] = -1;
        if (data_ai.press[i - ADC1_PR_INDEX] > MAX_PRESS)
            data_ai.press[i - ADC1_PR_INDEX] = 2;

    }


    //**************************pt100******************************
    for (i = ADC1_PT_INDEX; i < (ADC1_PT_SIZE + ADC1_PT_INDEX); i++) //u2/(2u1-u2)
    {
        volt_tmp = 2 * data_ai.data_ai[2 * i + 1]; //2u1;
        volt_tmp2 = data_ai.data_ai[2 * i]; //u2


        volt_tmp =  volt_tmp - volt_tmp2; //(2u1-u2)
        tmp1 = volt_tmp2 * 1000  / volt_tmp; //1000u2/(2u1-u2)=75r/75+r
        //tmp2 = PT100_PR * tmp1;
       // tmp3 = PT100_PR - tmp1;
       // tmp4 = tmp2 / tmp3;
        data_ai.resis[i - ADC1_PT_INDEX] = tmp1;
        //volt_tmp = PT100_PR*data_ai.temp[i - ADC1_PT_INDEX];
        //volt_tmp2 = PT100_PR-data_ai.temp[i - ADC1_PT_INDEX];
        data_ai.temp[i - ADC1_PT_INDEX] = tmp1;
        data_ai.temp[i - ADC1_PT_INDEX] = PT100_Temp(data_ai.temp[i - ADC1_PT_INDEX]);
        data_ai.temp[i - ADC1_PT_INDEX] = data_ai.temp[i - ADC1_PT_INDEX] * PT100_RATIO;
        data_ai.temp[i - ADC1_PT_INDEX] = data_ai.temp[i - ADC1_PT_INDEX] + PT100_B;
        if (data_ai.temp[i - ADC1_PT_INDEX] < MIN_TEMP)
            data_ai.temp[i - ADC1_PT_INDEX] = -65;
        if (data_ai.temp[i - ADC1_PT_INDEX] > MAX_TEMP)
            data_ai.temp[i - ADC1_PT_INDEX] = 110;

    }

    for (i = ADC2_PT_INDEX; i < (ADC2_PT_SIZE + ADC2_PT_INDEX); i++) //u2/(2u1-u2)
    {
        volt_tmp = 2 * data_ai.data2_ai[2 * i + 1]; //2u1;
        volt_tmp2 = data_ai.data2_ai[2*i];//u2
        volt_tmp =  volt_tmp - volt_tmp2; //(2u1-u2)
        tmp1 = volt_tmp2 * 1000  / volt_tmp; //1000u2/(2u1-u2)=75r/75+r
        tmp2 = PT100_PR * tmp1;
        tmp3 = PT100_PR - tmp1;
        tmp4 = tmp2 / tmp3;
        unsigned char index;
        index = i - ADC2_PT_INDEX + ADC1_PT_SIZE;
        data_ai.temp[index] = tmp4;
        data_ai.temp[index] = PT100_Temp(data_ai.temp[index]);
        data_ai.temp[index] = data_ai.temp[index] * PT100_RATIO;
        data_ai.temp[index] = data_ai.temp[index] + PT100_B;
        if (data_ai.temp[index] < MIN_TEMP)
            data_ai.temp[index] = -65;
        if (data_ai.temp[index] > MAX_TEMP)
            data_ai.temp[index] = 110;

    }




}
data_ai_stru *get_ai_data()
{
    return &data_ai;
}

//检测到传感器有问题，不清零，重启设备后清零
void ai_health_dec()
{
    unsigned char i;
    uint32_t failure_flag;

    for (i = 0; i < (ADC1_PT_SIZE+ADC2_PT_SIZE); i++)
    {
        failure_flag = 1;
        if (data_ai.temp[i] >= MAX_TEMP || data_ai.temp[i] <= MIN_TEMP)
        {
            failure_flag = failure_flag << i;

            data_ai.channel_status = data_ai.channel_status | failure_flag;
        }
        else
        {
           /* if (data_ai.temp[i] == data_ai.last_gather[i])
            {
                data_ai.failure_count[i] = data_ai.failure_count[i] + 1;
            }
            else
            {
                if (data_ai.failure_count[i] > 0)
                    data_ai.failure_count[i] = data_ai.failure_count[i] - 1;
                data_ai.last_gather[i] = data_ai.temp[i];

            }
            if (data_ai.failure_count[i] >= MAX_FAILUE)
            {
                failure_flag = failure_flag << i;
                data_ai.channel_status = data_ai.channel_status | failure_flag;
                data_ai.failure_count[i] = 0;
                //data_ai.temp[i] = 110;
            }
			else*/
			{
			failure_flag = 1;
			failure_flag = failure_flag << (i);
			failure_flag = ~failure_flag ;
			data_ai.channel_status = data_ai.channel_status & failure_flag;
			
			}

        }
    }
    for (i = 0; i < ADC1_PR_SIZE; i++)
    {
        failure_flag = 1;
        if (data_ai.press[i] >= MAX_PRESS || data_ai.press[i] < MIN_PRESS)
        {
            failure_flag = failure_flag << (i + 12);
            data_ai.channel_status = data_ai.channel_status | failure_flag;
        }
        else
        {
            /*if (data_ai.press[i] == data_ai.last_gather[i])
            {
                data_ai.failure_count[i + 12] = data_ai.failure_count[i] + 1;
            }
            else
            {
                if (data_ai.failure_count[i + 12] > 0)
                    data_ai.failure_count[i + 12] = data_ai.failure_count[i + 12] - 1;
                data_ai.last_gather[i] = data_ai.press[i];

            }
            if (data_ai.failure_count[i + 12] >= MAX_FAILUE)
            {
                failure_flag = 1;
                failure_flag = failure_flag << (i + 12);
                data_ai.channel_status = data_ai.channel_status | failure_flag;
                data_ai.failure_count[i + 12] = 0;
                //data_ai.press[i] = 2;
            }
			else*/
			{
			failure_flag = 1;
			failure_flag = failure_flag << (i + 12);
			
			failure_flag = ~failure_flag ;
			data_ai.channel_status = data_ai.channel_status & failure_flag;

			}
        }
    }
}

void ai_proc()
{
    registerTick(ADC_SAMP_TICK_NO, 2000);
    //if(GetTickResult(ADC_SAMP_TICK_NO)==1)//2s
    {
        reset_registerTick(ADC_SAMP_TICK_NO);
        //adc_read_data();//adc sample
        if (adc_read_data() == 0)
            pressure_temp_proc();//temp pressure convert
        ai_health_dec();//channels health detected
    }

}
