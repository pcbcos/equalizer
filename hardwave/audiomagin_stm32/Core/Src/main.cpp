/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "i2s.h"
#include "dma.h"
#include "dac.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include <cstdio>
#include "retarget.h"
#include "usart.h"
#include "random"
#include "equalizer.h"
#include "i2c.h"


int16_t i2s_dma_tmp[samples_per_packet * 4];//接受ADC数据
int16_t dma_decoded[samples_per_packet];//ADC数据解码后
uint16_t dac_data[samples_per_packet];//给DAC的数据
int8_t flag = 0;

void PeriphCommonClock_Config(void);

void SystemClock_Config(void);

void gen_random(float *data_buff, uint32_t size) {
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_real_distribution<float> dist(std::numeric_limits<int16_t>::min(),
                                               std::numeric_limits<int16_t>::max());
    for (int i = 0; i < size; ++i) {
        data_buff[i] = dist(engine);
    }
}

equalizer eq1;

int main() {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

    HAL_Init();
    SystemClock_Config();
    PeriphCommonClock_Config();

    MX_USART1_UART_Init();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_DMA_Init();
    MX_I2S2_Init();
    MX_DAC_Init();
    MX_TIM6_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();
    LL_USART_EnableIT_RXNE(USART2);
    RetargetInit(&huart1);


    for (int i = 0; i < 10; i++) {
        eq1.set_gain(i, 0);
    }
    eq1.peak_filter_design();


//    LL_TIM_EnableIT_UPDATE(TIM2);  //使能更新中断
//    LL_TIM_EnableCounter(TIM2);  //使能计数
//    for (std::size_t i = 0; i < 44 * 2; i++) {
//        eq1.EQ_process(data, data_q15, 1024);
//    }
//
//    LL_TIM_DisableCounter(TIM2);
//    extern uint32_t ms_10;
//    //HAL_I2S_DMAStop(&hi2s2);
//    printf("%f ms\n", ms_10 / 10.0f);
    //printf("hello,world");
    HAL_TIM_Base_Start(&htim6);
    HAL_I2S_Receive_DMA(&hi2s2, (uint16_t *) i2s_dma_tmp, samples_per_packet * 2);//开启DMA接收,接收samples_per_packet个数据
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *) dac_data, samples_per_packet, DAC_ALIGN_12B_R);

    while (1) {
//        if (flag) {
//            flag = 0;
//            for (int i = 0; i < samples_per_packet; i += 8) {
//                printf("%d\n", dac_data[i]);
//            }
//            continue;
//        }
    }
}

void SystemClock_Config(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    while (LL_PWR_IsActiveFlag_VOS() == 0) {
    }
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1) {

    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {

    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {

    }
    LL_SetSystemCoreClock(168000000);

    /* Update the time base */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void PeriphCommonClock_Config(void) {
    LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_4, 192, LL_RCC_PLLI2SR_DIV_2);
    LL_RCC_PLLI2S_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLLI2S_IsReady() != 1) {

    }
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
    if (hi2s == &hi2s2) {
        //接受完了前一半数据
        for (std::size_t i = 0; i < samples_per_packet / 2; i++) {
            int32_t tmp1 = int32_t(i2s_dma_tmp[i * 4 + 2]) << 8;
            int32_t tmp2 = (int32_t(i2s_dma_tmp[i * 4 + 3]) >> 8) & 0xff;
            dma_decoded[i] = (tmp1 | tmp2) >> 8;
            //dac_data[i] = (dma_decoded[i] >> 4) + 0x800;
        }
        eq1.EQ_process(dma_decoded, (q15_t *) dac_data, samples_per_packet / 2);
        for (std::size_t i = 0; i < samples_per_packet / 2; i++) {
            dac_data[i] += 0x800;
        }
        flag = 1;
    }
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
    if (hi2s == &hi2s2) {
        //HAL_I2S_DMAPause(&hi2s2);
        for (std::size_t i = samples_per_packet / 2; i < samples_per_packet; i++) {
            int32_t tmp1 = int32_t(i2s_dma_tmp[i * 4 + 2]) << 8;
            int32_t tmp2 = (int32_t(i2s_dma_tmp[i * 4 + 3]) >> 8) & 0xff;
            dma_decoded[i] = (tmp1 | tmp2) >> 8;//24bit to 16bit
        }
        eq1.EQ_process(dma_decoded + samples_per_packet / 2, (q15_t *) (dac_data + samples_per_packet / 2),
                       samples_per_packet / 2);
        for (std::size_t i = samples_per_packet / 2; i < samples_per_packet; i++) {
            dac_data[i] += 0x800;
        }
    }
}


void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
