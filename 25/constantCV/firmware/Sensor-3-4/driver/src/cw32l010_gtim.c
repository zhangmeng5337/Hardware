/**
 * @file cw32l010_gtim.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
 *
 */
/*******************************************************************************
*
* 代码许可和免责信�?
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源�?
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或�?
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件�?
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失�?
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些�?
* 全部上述排除或限制可能并不适用于您�?
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "cw32l010_gtim.h"
#include "cw32l010_sysctrl.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief
 * @param GTIMx
 */
void GTIM1_DeInit(void)
{
    CW_SYSCTRL->APBRST1_f.GTIM1 = 0;
    CW_SYSCTRL->APBRST1_f.GTIM1 = 1;
}


/**
 * @brief GTIM的基础参数初始�?
 * @param GTIMx 取值范围：CW_GTIM1
 * @param GTIM_InitStruct 详见GTIM_InitTypeDef的定�?
 */
void GTIM_TimeBaseInit(GTIM_TypeDef *GTIMx, GTIM_InitTypeDef *GTIM_InitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_ARR_BUFF(GTIM_InitStruct->ARRBuffState));
    assert_param(IS_GTIM_ALIGN_MODE(GTIM_InitStruct->AlignMode));
    assert_param(IS_GTIM_DIRECTION(GTIM_InitStruct->Direction));
    assert_param(IS_GTIM_PULSE_MODE(GTIM_InitStruct->PulseMode));
    assert_param(IS_GTIM_EVENT_OPT(GTIM_InitStruct->EventOption));
    assert_param(IS_GTIM_UPDATE_OPT(GTIM_InitStruct->UpdateOption));   
    assert_param(IS_GTIM_PRESCALER(GTIM_InitStruct->Prescaler));

    REGBITS_MODIFY(GTIMx->CR1, GTIMx_CR1_ARPE_Msk | GTIMx_CR1_CMS_Msk |\
                   GTIMx_CR1_DIR_Msk | GTIMx_CR1_OPM_Msk | GTIMx_CR1_URS_Msk |\
                   GTIMx_CR1_UDIS_Msk,  GTIM_InitStruct->ARRBuffState |\
                   GTIM_InitStruct->AlignMode | GTIM_InitStruct->Direction |\
                   GTIM_InitStruct->PulseMode | GTIM_InitStruct->EventOption |\
                   GTIM_InitStruct->UpdateOption);
    
    GTIMx->PSC = GTIM_InitStruct->Prescaler;

    GTIMx->ARR = GTIM_InitStruct->ReloadValue;
    
    GTIMx->SMCR_f.SMS = 0;    // 默认禁止从模�?
}

/**
 * @brief GTIM的主模式下的TRGO输出配置
 * @param MasterTRGO
 *        @arg GTIM_MASTER_TRGO_RESET    GTIM_MASTER_TRGO_EN    GTIM_MASTER_TRGO_UPDATE
 *        @arg GTIM_MASTER_TRGO_CCH1~4
 *        @arg GTIM_MASTER_TRGO_OC1REF~GTIM_MASTER_TRGO_OC4REF
 *        @arg GTIM_MASTER_TRGO_OC1REFC~GTIM_MASTER_TRGO_OC4REFC
 */

void GTIM_MasterTRGOCfg(GTIM_TypeDef *GTIMx, uint32_t MasterTRGO)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    
    GTIMx->CR2_f.MMS = (MasterTRGO & 0x07); 
    
    GTIMx->CR2_f.MMSH = (MasterTRGO & 0x08) >> 3;    
}


void GTIM_SlaverModeCfg(GTIM_TypeDef *GTIMx, GTIM_SlaverCfgTypeDef *GTIM_SlaverCfgStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_SLAVER_MODE(GTIM_SlaverCfgStruct->SlaverMode));
    assert_param(IS_GTIM_TRIG_INPUT(GTIM_SlaverCfgStruct->TrigInput));
    assert_param(IS_GTIM_PRELOAD_SOURCE(GTIM_SlaverCfgStruct->PreloadSource));
    
    GTIMx->SMCR_f.SMSPS = GTIM_SlaverCfgStruct->PreloadSource;
    GTIMx->SMCR_f.SMSPE = GTIM_SlaverCfgStruct->PreloadState;
    GTIMx->SMCR_f.TSH = (GTIM_SlaverCfgStruct->TrigInput & 0x18) >> 3;
    GTIMx->SMCR_f.TS = (GTIM_SlaverCfgStruct->TrigInput & 0x07);
    GTIMx->SMCR_f.SMSH = GTIM_SlaverCfgStruct->SlaverMode >> 3;
    GTIMx->SMCR_f.SMS =  GTIM_SlaverCfgStruct->SlaverMode & 0x07;
}

void GTIM_OC1ModeCfg(GTIM_TypeDef *GTIMx, GTIM_OCModeCfgTypeDef *GTIM_OCModeCfgStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_OC_MODE(GTIM_OCModeCfgStruct->OCMode));
    assert_param(IS_GTIM_OC_POLAR(GTIM_OCModeCfgStruct->OCPolarity));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->FastMode));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->PreloadState));
    
    GTIMx->CCER_f.CC1P = GTIM_OCModeCfgStruct->OCPolarity;
    GTIMx->CCER_f.CC1NP = 0; 
    
    GTIMx->CCMR1CMP_f.CC1S = 0;
    GTIMx->CCMR1CMP_f.OC1FE = GTIM_OCModeCfgStruct->FastMode;
    GTIMx->CCMR1CMP_f.OC1PE = GTIM_OCModeCfgStruct->PreloadState;
    GTIMx->CCMR1CMP_f.OC1M = GTIM_OCModeCfgStruct->OCMode & 0x07;
    GTIMx->CCMR1CMP_f.OC1MH = GTIM_OCModeCfgStruct->OCMode >> 3 ; 
}

void GTIM_OC2ModeCfg(GTIM_TypeDef *GTIMx, GTIM_OCModeCfgTypeDef *GTIM_OCModeCfgStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_OC_MODE(GTIM_OCModeCfgStruct->OCMode));
    assert_param(IS_GTIM_OC_POLAR(GTIM_OCModeCfgStruct->OCPolarity));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->FastMode));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->PreloadState));
    
    GTIMx->CCER_f.CC2P = GTIM_OCModeCfgStruct->OCPolarity;
    GTIMx->CCER_f.CC2NP = 0; 
    
    GTIMx->CCMR1CMP_f.CC2S = 0;
    GTIMx->CCMR1CMP_f.OC2FE = GTIM_OCModeCfgStruct->FastMode;
    GTIMx->CCMR1CMP_f.OC2PE = GTIM_OCModeCfgStruct->PreloadState;
    GTIMx->CCMR1CMP_f.OC2M = GTIM_OCModeCfgStruct->OCMode & 0x07;
    GTIMx->CCMR1CMP_f.OC2MH = GTIM_OCModeCfgStruct->OCMode >> 3 ; 
}
void GTIM_OC3ModeCfg(GTIM_TypeDef *GTIMx, GTIM_OCModeCfgTypeDef *GTIM_OCModeCfgStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_OC_MODE(GTIM_OCModeCfgStruct->OCMode));
    assert_param(IS_GTIM_OC_POLAR(GTIM_OCModeCfgStruct->OCPolarity));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->FastMode));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->PreloadState));
    
    GTIMx->CCER_f.CC3P = GTIM_OCModeCfgStruct->OCPolarity;
    GTIMx->CCER_f.CC3NP = 0; 
    
    GTIMx->CCMR2CMP_f.CC3S = 0;
    GTIMx->CCMR2CMP_f.OC3FE = GTIM_OCModeCfgStruct->FastMode;
    GTIMx->CCMR2CMP_f.OC3PE = GTIM_OCModeCfgStruct->PreloadState;
    GTIMx->CCMR2CMP_f.OC3M = GTIM_OCModeCfgStruct->OCMode & 0x07;
    GTIMx->CCMR2CMP_f.OC3MH = GTIM_OCModeCfgStruct->OCMode >> 3 ; 
}
void GTIM_OC4ModeCfg(GTIM_TypeDef *GTIMx, GTIM_OCModeCfgTypeDef *GTIM_OCModeCfgStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_OC_MODE(GTIM_OCModeCfgStruct->OCMode));
    assert_param(IS_GTIM_OC_POLAR(GTIM_OCModeCfgStruct->OCPolarity));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->FastMode));
    assert_param(IS_FUNCTIONAL_STATE(GTIM_OCModeCfgStruct->PreloadState));
    
    GTIMx->CCER_f.CC4P = GTIM_OCModeCfgStruct->OCPolarity;
    GTIMx->CCER_f.CC4NP = 0; 
    
    GTIMx->CCMR2CMP_f.CC4S = 0;
    GTIMx->CCMR2CMP_f.OC4FE = GTIM_OCModeCfgStruct->FastMode;
    GTIMx->CCMR2CMP_f.OC4PE = GTIM_OCModeCfgStruct->PreloadState;
    GTIMx->CCMR2CMP_f.OC4M = GTIM_OCModeCfgStruct->OCMode & 0x07;
    GTIMx->CCMR2CMP_f.OC4MH = GTIM_OCModeCfgStruct->OCMode >> 3 ; 
}

void GTIM_OC1Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC1E = NewState;
}

void GTIM_OC2Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC2E = NewState;
}
void GTIM_OC3Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC3E = NewState;
}
void GTIM_OC4Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC4E = NewState;
}


void GTIM_IC1ModeCfg(GTIM_TypeDef* GTIMx, GTIM_ICModeCfgTypeDef *GTIM_ICInitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IC_MAP(GTIM_ICInitStruct->ICMap));
    assert_param(IS_GTIM_IC_PRESCALER(GTIM_ICInitStruct->ICPrescaler));
    assert_param(IS_GTIM_IC_FILTER(GTIM_ICInitStruct->ICFilter));
    assert_param(IS_GTIM_ICPolarity(GTIM_ICInitStruct->ICPolarity));
    
    GTIMx->CCMR1CAP_f.CC1S = GTIM_ICInitStruct->ICMap;
    GTIMx->CCMR1CAP_f.IC1PSC = GTIM_ICInitStruct->ICPrescaler;
    GTIMx->CCMR1CAP_f.IC1F = GTIM_ICInitStruct->ICFilter;
    
    switch(GTIM_ICInitStruct->ICPolarity)
    {
        default:
        case 1:
            GTIMx->CCER_f.CC1P = 0;
            GTIMx->CCER_f.CC1NP = 0;
            break;
        case 2:
            GTIMx->CCER_f.CC1P = 1;
            GTIMx->CCER_f.CC1NP = 0;
            break;
        case 3:
            GTIMx->CCER_f.CC1P = 1;
            GTIMx->CCER_f.CC1NP = 1;
            break;
    }    
}


void GTIM_IC2ModeCfg(GTIM_TypeDef* GTIMx, GTIM_ICModeCfgTypeDef *GTIM_ICInitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IC_MAP(GTIM_ICInitStruct->ICMap));
    assert_param(IS_GTIM_IC_PRESCALER(GTIM_ICInitStruct->ICPrescaler));
    assert_param(IS_GTIM_IC_FILTER(GTIM_ICInitStruct->ICFilter));
    assert_param(IS_GTIM_ICPolarity(GTIM_ICInitStruct->ICPolarity));
    
    GTIMx->CCMR1CAP_f.CC2S = GTIM_ICInitStruct->ICMap;
    GTIMx->CCMR1CAP_f.IC2PSC = GTIM_ICInitStruct->ICPrescaler;
    GTIMx->CCMR1CAP_f.IC2F = GTIM_ICInitStruct->ICFilter;
    
    switch(GTIM_ICInitStruct->ICPolarity)
    {
        default:
        case 1:
            GTIMx->CCER_f.CC2P = 0;
            GTIMx->CCER_f.CC2NP = 0;
            break;
        case 2:
            GTIMx->CCER_f.CC2P = 1;
            GTIMx->CCER_f.CC2NP = 0;
            break;
        case 3:
            GTIMx->CCER_f.CC2P = 1;
            GTIMx->CCER_f.CC2NP = 1;
            break;
    }    
}


void GTIM_IC3ModeCfg(GTIM_TypeDef* GTIMx, GTIM_ICModeCfgTypeDef *GTIM_ICInitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IC_MAP(GTIM_ICInitStruct->ICMap));
    assert_param(IS_GTIM_IC_PRESCALER(GTIM_ICInitStruct->ICPrescaler));
    assert_param(IS_GTIM_IC_FILTER(GTIM_ICInitStruct->ICFilter));
    assert_param(IS_GTIM_ICPolarity(GTIM_ICInitStruct->ICPolarity));
    
    GTIMx->CCMR2CAP_f.CC3S = GTIM_ICInitStruct->ICMap;
    GTIMx->CCMR2CAP_f.IC3PSC = GTIM_ICInitStruct->ICPrescaler;
    GTIMx->CCMR2CAP_f.IC3F = GTIM_ICInitStruct->ICFilter;
    
    switch(GTIM_ICInitStruct->ICPolarity)
    {
        default:
        case 1:
            GTIMx->CCER_f.CC3P = 0;
            GTIMx->CCER_f.CC3NP = 0;
            break;
        case 2:
            GTIMx->CCER_f.CC3P = 1;
            GTIMx->CCER_f.CC3NP = 0;
            break;
        case 3:
            GTIMx->CCER_f.CC3P = 1;
            GTIMx->CCER_f.CC3NP = 1;
            break;
    }    
}


void GTIM_IC4ModeCfg(GTIM_TypeDef* GTIMx, GTIM_ICModeCfgTypeDef *GTIM_ICInitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IC_MAP(GTIM_ICInitStruct->ICMap));
    assert_param(IS_GTIM_IC_PRESCALER(GTIM_ICInitStruct->ICPrescaler));
    assert_param(IS_GTIM_IC_FILTER(GTIM_ICInitStruct->ICFilter));
    assert_param(IS_GTIM_ICPolarity(GTIM_ICInitStruct->ICPolarity));
    
    GTIMx->CCMR2CAP_f.CC4S = GTIM_ICInitStruct->ICMap;
    GTIMx->CCMR2CAP_f.IC4PSC = GTIM_ICInitStruct->ICPrescaler;
    GTIMx->CCMR2CAP_f.IC4F = GTIM_ICInitStruct->ICFilter;
    
    switch(GTIM_ICInitStruct->ICPolarity)
    {
        default:
        case 1:
            GTIMx->CCER_f.CC4P = 0;
            GTIMx->CCER_f.CC4NP = 0;
            break;
        case 2:
            GTIMx->CCER_f.CC4P = 1;
            GTIMx->CCER_f.CC4NP = 0;
            break;
        case 3:
            GTIMx->CCER_f.CC4P = 1;
            GTIMx->CCER_f.CC4NP = 1;
            break;
    }    
}

void GTIM_IC1Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC1E = NewState;
}

void GTIM_IC2Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC2E = NewState;
}
void GTIM_IC3Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC3E = NewState;
}
void GTIM_IC4Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CCER_f.CC4E = NewState;
}




#if 0 

/**
 * @brief 编码模式初始化�?
 * @param GTIMx 取值范围：CW_GTIM1,CW_GTIM2,
 * @param GTIM_EncodeInitStruct
 */
void GTIM_EncodeInit(GTIM_TypeDef *GTIMx, GTIM_EncodeInitTypeDef *GTIM_EncodeInitStruct)
{
    uint32_t tmp;
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_ENCODE_MODE(GTIM_EncodeInitStruct->EncodeMode));
    assert_param(IS_GTIM_ENCODE_RESET(GTIM_EncodeInitStruct->EncodeResetMode));
    assert_param(IS_GTIM_ENCODE_RELOAD(GTIM_EncodeInitStruct->EncodeReloadMode));
    assert_param(IS_GTIM_CHx_FILTER(GTIM_EncodeInitStruct->CH1Filter));
    assert_param(IS_GTIM_CHx_FILTER(GTIM_EncodeInitStruct->CH2Filter));
    assert_param(IS_GTIM_CHx_FILTER(GTIM_EncodeInitStruct->CH3Filter));
    assert_param(IS_GTIM_CHx_INVERT(GTIM_EncodeInitStruct->CH1Invert));
    assert_param(IS_GTIM_CHx_INVERT(GTIM_EncodeInitStruct->CH2Invert));
    assert_param(IS_GTIM_CHx_INVERT(GTIM_EncodeInitStruct->CH3Invert));

    REGBITS_MODIFY(GTIMx->CR0,
                   GTIMx_CR0_ENCRELOAD_Msk | GTIMx_CR0_ENCRESET_Msk | GTIMx_CR0_ENCMODE_Msk,
                   GTIM_EncodeInitStruct->EncodeMode | GTIM_EncodeInitStruct->EncodeResetMode |
                   GTIM_EncodeInitStruct->EncodeReloadMode);

    tmp = GTIM_EncodeInitStruct->CH1Filter | GTIM_EncodeInitStruct->CH1Invert |
          (GTIM_EncodeInitStruct->CH2Filter << 4) | (GTIM_EncodeInitStruct->CH2Invert << 7) |
          (GTIM_EncodeInitStruct->CH3Filter << 8) | (GTIM_EncodeInitStruct->CH3Invert << 11);
    REGBITS_MODIFY(GTIMx->CR1, ~(GTIMx_CR1_CH4FLT_Msk | GTIMx_CR1_CH4POL_Msk), tmp);
}

/**
 * @brief 比较输出功能初始�?
 * @param GTIMx 取值范围：CW_GTIM1,CW_GTIM2
 * @param CHx 输出通道：CH1~CH4
 * @param OCMode
 */
void GTIM_OCInit(GTIM_TypeDef *GTIMx, uint32_t CHx, uint32_t OCMode)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_CHANNEL(CHx));
    assert_param(IS_GTIM_OC_OUTPUT(OCMode));

    REGBITS_MODIFY(GTIMx->CMMR, (GTIMx_CMMR_CC1M_Msk << CHx), (OCMode << CHx));
}

void GTIM_ICInit(GTIM_TypeDef *GTIMx, GTIM_ICInitTypeDef *GTIM_ICInitStruct)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_CHANNEL(GTIM_ICInitStruct->CHx));
    assert_param(IS_GTIM_ICPolarity(GTIM_ICInitStruct->ICPolarity));
    assert_param(IS_GTIM_CHx_FILTER(GTIM_ICInitStruct->ICFilter));
    assert_param(IS_GTIM_CHx_INVERT(GTIM_ICInitStruct->ICInvert));

    REGBITS_MODIFY(GTIMx->CMMR, (GTIMx_CMMR_CC1M_Msk << GTIM_ICInitStruct->CHx),
                   (GTIM_ICInitStruct->ICPolarity << GTIM_ICInitStruct->CHx));

    REGBITS_MODIFY(GTIMx->CR1, (0xFUL << GTIM_ICInitStruct->CHx),
                   (GTIM_ICInitStruct->ICFilter | (GTIM_ICInitStruct->ICInvert << 3)) << GTIM_ICInitStruct->CHx);
}


/**
 * @brief 配置ETR信号的极性和滤波�?
 * @param GTIMx
 * @param ETRPolarity 极�?
 * @param ETRFilter 滤波参数
 */
void GTIM_ETRConfig(GTIM_TypeDef *GTIMx, uint32_t ETRPolarity, uint32_t ETRFilter)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_ETR_POL(ETRPolarity));
    assert_param(IS_GTIM_ETR_FILTER(ETRFilter));

    REGBITS_MODIFY(GTIMx->CR0, GTIMx_CR0_TRS_Msk | GTIMx_CR0_POL_Msk, ETRPolarity);

    REGBITS_MODIFY(GTIMx->ETR, GTIMx_ETR_ETRFLT_Msk, ETRFilter);
}



/**
 * @brief 配置输入通道的参�?
 * @param GTIMx
 * @param CHx
 * @param CHxFilter
 * @param CHxInvert
 */
void GTIM_ICChannelConfig(GTIM_TypeDef *GTIMx, uint32_t CHx, uint32_t CHxFilter, uint32_t CHxInvert)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_CHANNEL(CHx));
    assert_param(IS_GTIM_CHx_FILTER(CHxFilter));
    assert_param(IS_GTIM_CHx_INVERT(CHxInvert));

    REGBITS_MODIFY(GTIMx->CR1, (0xFUL << CHx), (CHxFilter | (CHxInvert << 3)) << CHx);
}

#endif
/**
 * @brief GTIM使能
 * @param GTIMx
 */
void GTIM_Cmd(GTIM_TypeDef *GTIMx, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    GTIMx->CR1_f.CEN = NewState;
}

/**
 * @brief GTIM 重映射更新中断标志位到CNT
 * @param NewState
 *        @arg Enable or Disable
 */

void GTIM_RemapUpdateFlag(GTIM_TypeDef *GTIMx,FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    GTIMx->CR1_f.UIFREMAP = NewState;
}


/**
 * @brief 设置预分频系�?
 * @param GTIMx
 * @param GTIMPrescaler
 */
void GTIM_SetPrescaler(GTIM_TypeDef *GTIMx, uint16_t GTIMPrescaler)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_PRESCALER(GTIMPrescaler));

    GTIMx->PSC = GTIMPrescaler;
}

/**
 * @brief 设置重载�?
 * @param GTIMx
 * @param ReloadValue 取值范围：0x0000~0xFFFF
 */
void GTIM_SetReloadValue(GTIM_TypeDef *GTIMx, uint16_t ReloadValue)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->ARR = ReloadValue;
}

/**
 * @brief
 * @param GTIMx
 * @param Value
 */
void GTIM_SetCounterValue(GTIM_TypeDef *GTIMx, uint16_t Value)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->CNT = Value;
}

void GTIM_SetCompare1(GTIM_TypeDef *GTIMx, uint16_t Value)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->CCR1 = Value;
}

void GTIM_SetCompare2(GTIM_TypeDef *GTIMx, uint16_t Value)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->CCR2 = Value;
}

void GTIM_SetCompare3(GTIM_TypeDef *GTIMx, uint16_t Value)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->CCR3 = Value;
}

void GTIM_SetCompare4(GTIM_TypeDef *GTIMx, uint16_t Value)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    GTIMx->CCR4 = Value;
}

/**
 * @brief
 * @param GTIMx
 * @return uint16_t
 */
uint16_t GTIM_GetCapture1(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return (uint16_t)GTIMx->CCR1;
}

/**
 * @brief
 * @param GTIMx
 * @return uint16_t
 */
uint16_t GTIM_GetCapture2(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return (uint16_t)GTIMx->CCR2;
}

/**
 * @brief
 * @param GTIMx
 * @return uint16_t
 */
uint16_t GTIM_GetCapture3(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return (uint16_t)GTIMx->CCR3;
}

/**
 * @brief
 * @param GTIMx
 * @return uint16_t
 */
uint16_t GTIM_GetCapture4(GTIM_TypeDef *GTIMx)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));

    return (uint16_t)GTIMx->CCR4;
}

/**
 * @brief
 * @param GTIMx
 * @param GTIM_IT
 * @param NewState
 */
void GTIM_ITConfig(GTIM_TypeDef *GTIMx, uint32_t GTIM_IT, FunctionalState NewState)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IT(GTIM_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        GTIMx->IER |= GTIM_IT;
    }
    else
    {
        GTIMx->IER &= ~GTIM_IT;
    }
}

/**
 * @brief
 * @param GTIMx
 * @param GTIM_IT
 * @return ITStatus
 */
ITStatus GTIM_GetITStatus(GTIM_TypeDef *GTIMx, uint32_t GTIM_IT)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_GET_IT(GTIM_IT));

    return (GTIMx->ISR & GTIM_IT) ? SET : RESET;
}

/**
 * @brief
 * @param GTIMx
 * @param GTIM_IT
 */
void GTIM_ClearITPendingBit(GTIM_TypeDef *GTIMx, uint32_t GTIM_IT)
{
    assert_param(IS_GTIM_DEFINE(GTIMx));
    assert_param(IS_GTIM_IT(GTIM_IT));

    GTIMx->ICR = ~GTIM_IT;
}


