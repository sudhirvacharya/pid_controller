/*
v1.0
date:20-04-2020
close loop control
ServoX Controller Inc.
hfvliuggol;/;n;;
*/

#include "main.h"


DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim2;



float Travel, Position=0;
uint16_t uValue=0,  i=0, rotation=10,  
signed int Error=0;;



uint16_t SP_Sin_LUT[101]=
{    
0,4,16,36,64,
100,144,195,253,319,391,470,555,
646,742,844,950,1061,1176,1294,1415,
1538,1664,1791,1919,


2048,2176,2304,2431,2557,2680,2801,2919,
3034,3145,3251,3353,3449,3540,3625,3704,
3776,3842,3900,3951,3995,4031,4059,4079,
4091,4095,

4091,4079,4059,4031,3995,3951,
3900,3842,3776,3704,3625,3540,3449,3353,
3251,3145,3034,2919,2801,2680,2557,2431,
2304,2176,2048,

1919,1791,1664,1538,1415,
1294,1176,1061,950,844,742,646,555,
470,391,319,253,195,144,100,64,
36,16,4,0
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
int main(void)
{
 
HAL_Init();
SystemClock_Config();
MX_GPIO_Init();
MX_DAC_Init();
MX_TIM2_Init();
HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	while (1)
	{			
		do
		{					
			Travel = TIM2->CNT; /*Read Encoder values from 12 bit Encoder*/				
			Position = (Travel/(rotation*4095))*4095;	
			Error = SP_Sin_LUT[i] - Position;  /*Error =SP- Fbk*/ 				
			uValue=((Error*4095)/(4095*2))+2048;	
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, uValue); 					
		}				
		while(Error!=0); // WAITING FOR POSITION REACH SET POINT Waiting for postion to Reach Setpoint
		i++;
		if(i>100)
		{
			i=0;
		}
	}
} 

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig = {0};
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM2_Init(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000000000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

}
void Error_Handler(void)
{
 
}

#ifdef  USE_FULL_ASSERT
void assert_failed(char *file, uint32_t line)
{ 
}
#endif 































