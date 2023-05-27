/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
#include "usart.h"
#include "equalizer.h"
#include "key.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern DMA_HandleTypeDef hdma_dac1;
extern DMA_HandleTypeDef hdma_spi2_rx;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern DMA_HandleTypeDef hdma_i2s2_ext_rx;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */

    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
uint32_t ms_10{};

void TIM2_IRQHandler(void) {
    /* USER CODE BEGIN TIM2_IRQn 0 */
    LL_TIM_ClearFlag_UPDATE(TIM2);
    /* USER CODE END TIM2_IRQn 0 */
    /* USER CODE BEGIN TIM2_IRQn 1 */
    ms_10++;
    /* USER CODE END TIM2_IRQn 1 */
}

void TIM3_IRQHandler(void) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    static uint8_t t{};
    t++;
    if(t==30){
        t=0;
        key_flash();
        key_task();
    }
}

/* USER CODE BEGIN 1 */
void DMA1_Stream3_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

    /* USER CODE END DMA1_Stream3_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi2_rx);
    /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

    /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/* USER CODE END 1 */
void DMA1_Stream5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

    /* USER CODE END DMA1_Stream5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_dac1);
    /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

    /* USER CODE END DMA1_Stream5_IRQn 1 */
}

extern equalizer eq1;
char rx_buff[128]{};
uint8_t rx_len{};
uint8_t ana_state{};
std::vector<uint8_t> selected_list;

void USART2_IRQHandler(void) {
    if (LL_USART_IsActiveFlag_RXNE(USART2)) {
        LL_USART_ClearFlag_RXNE(USART2);
        rx_buff[rx_len] = LL_USART_ReceiveData8(USART2);
        LL_USART_TransmitData8(USART2, rx_buff[rx_len]);
        rx_len++;
        if (rx_len == 128) {
            rx_len = 0;
        }
        if (rx_buff[rx_len - 1] == ' ') {
            //接收到空格
            if (ana_state == 0) {
                if (strncmp(rx_buff, "SELECT", 6) == 0) {
                    ana_state = 1;
                    rx_len = 0;
                } else if (strncmp(rx_buff, "END", 3) == 0) {
                    ana_state = 0;
                    rx_len = 0;
                    eq1.peak_filter_design();
                } else if (strncmp(rx_buff, "RESET", 5) == 0) {
                    ana_state = 0;
                    rx_len = 0;
                    for (int i = 0; i < 10; i++) {
                        eq1.set_gain(i, 0);
                        eq1.set_q(i, 1.414f);
                    }
                    eq1.set_freq(0, 31.25);
                    eq1.set_freq(1, 62.5);
                    eq1.set_freq(2, 125);
                    eq1.set_freq(3, 250);
                    eq1.set_freq(4, 500);
                    eq1.set_freq(5, 1000);
                    eq1.set_freq(6, 2000);
                    eq1.set_freq(7, 4000);
                    eq1.set_freq(8, 8000);
                    eq1.set_freq(9, 16000);
                }
            } else if (ana_state == 1) {
                if (strncmp(rx_buff, "SET", 3) != 0) {
                    selected_list.push_back(atoi(rx_buff));
                    rx_len = 0;
                } else {
                    if (strncmp(rx_buff, "SETA", 4) == 0) {
                        ana_state = 2;
                        rx_len = 0;
                    } else if (strncmp(rx_buff, "SETF", 4) == 0) {
                        ana_state = 3;
                        rx_len = 0;
                    } else if (strncmp(rx_buff, "SETQ", 4) == 0) {
                        ana_state = 4;
                        rx_len = 0;
                    } else if (strncmp(rx_buff, "SETT", 4) == 0) {
                        ana_state = 5;
                        rx_len = 0;
                    }
                }
            } else if (ana_state == 2) {
                for (auto i: selected_list) {
                    float gain = atof(rx_buff);
                    gain = std::min(gain, 10.0f);
                    gain = std::max(gain, -10.0f);
                    eq1.set_gain(i, gain);
                }
                selected_list.clear();
                ana_state = 0;
                rx_len = 0;
            } else if (ana_state == 3) {
                float f = atof(rx_buff);
                f = std::min(f, 18000.0f);
                f = std::max(f, 20.0f);
                eq1.set_freq(selected_list[0], f);
                selected_list.clear();
                ana_state = 0;
                rx_len = 0;
            } else if (ana_state == 4) {
                float q = atof(rx_buff);
                q = std::min(q, 2.0f);
                q = std::max(q, 1.0f);
                for (auto i: selected_list) {
                    eq1.set_q(i, q);
                }
                selected_list.clear();
                ana_state = 0;
                rx_len = 0;
            } else if (ana_state == 5) {
                selected_list.clear();
                ana_state = 0;
                rx_len = 0;
            }
        }
    }
}