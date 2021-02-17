#include "shift_driver.h"

extern TIM_HandleTypeDef htim2;
static void MX_TIM2_Init(void);
enum state_t {RELEASE, WRITE} state;
static int16_t step;

// TODO, change to bit pattern
static int16_t SRCLK[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
static int16_t SER[]   = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int16_t RCLK[]  = {0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
static int16_t SRCLR[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
static int16_t OE[]    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void shift_init()
{
  MX_TIM2_Init();

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_NVIC_SetPriority(TIM2_IRQn,0,0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  state = RELEASE;
}

void shift_write()
{
  state = WRITE;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

  if (state == WRITE)
  {
    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); SRCLK
    if (SRCLK[step])
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0); SER
    if (SER[step])
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); RCLK
    if (RCLK[step])
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4); SRCLR
    if (SRCLR[step])
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1); OE
    if (OE[step])
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);


    step++;
    if (step == 20)
    {
      step = 0;
      state = RELEASE;
    }
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 63999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 53;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END TIM2_Init 2 */

}
