#include "shift_driver.h"

extern TIM_HandleTypeDef htim2;
static void MX_TIM2_Init(void);
enum state_t {RELEASE, WRITE} state;
static int16_t step;
static uint8_t value;

static int16_t SRCLK   = 0b0101010101010101000000;
static int16_t RCLK    = 0b0000101010101010101010;
static int16_t SRCLR   = 0b0011111111111111111111;
static int16_t OE      = 0b1111111111111111000000;


void shift_init()
{
  MX_TIM2_Init();

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_NVIC_SetPriority(TIM2_IRQn,0,0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  state = RELEASE;

}

void shift_write(uint8_t value_param)
{
  state = WRITE;
  step = 0;
  value = value_param;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

  if (state == WRITE)
  {
    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); SRCLK
    if (SRCLK && ( 1<< (21-step)))
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

    uint16_t pin_value = GPIO_PIN_RESET;
    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0); SER
    if (step > 1 && step < 18)
    {
      uint8_t position = step / 2 - 1;
      uint8_t compare = 1 << position;
      if (compare & value)
        pin_value = GPIO_PIN_SET;
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, pin_value);

    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); RCLK
    if (RCLK && ( 1<< (21-step)))
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4); SRCLR
    if (SRCLR && ( 1<< (21-step)))
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1); OE
    if (OE && ( 1<< (21-step)))
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);


    step++;
    if (step == 22)
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
  htim2.Init.Prescaler = 63;
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
