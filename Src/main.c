/**************************************************************************
--                 NUCLEO-F401RE XMAS Musicbox
--
--           Copyright (C) 2025 By Ulrik Hørlyk Hjort
--
--  This Program is Free Software; You Can Redistribute It and/or
--  Modify It Under The Terms of The GNU General Public License
--  As Published By The Free Software Foundation; Either Version 2
--  of The License, or (at Your Option) Any Later Version.
--
--  This Program is Distributed in The Hope That It Will Be Useful,
--  But WITHOUT ANY WARRANTY; Without Even The Implied Warranty of
--  MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See The
--  GNU General Public License for More Details.
--
-- You Should Have Received A Copy of The GNU General Public License
-- Along with This Program; if not, See <Http://Www.Gnu.Org/Licenses/>.
***************************************************************************/

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void set_buzzer_frequency(uint32_t freq)
{
    uint32_t timer_clk = 1000000; // after prescaler
    uint32_t arr = (timer_clk / freq) - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim3, arr);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, arr / 2); // 50% duty cycle
}


void play_jingle_bells() {
	uint16_t notes[] = {
	    659,0, 659,0, 659,0,    // E E E
	    659,0, 659,0, 659,0,    // E E E
	    659,0, 784,0, 523,0, 587,0, 659,0, // E G C D E
	    698,0, 698,0, 698,0, 698,0, 698,0, // F F F F F
	    659,0, 659,0, 659,0, 659,0,  // E E E E
	    587,0, 587,0, 659,0, 587,0, 784,0  // D D E D G
	};


	uint8_t velocity[] = {
	    1,0, 1,0, 1,0,
	    1,0, 1,0, 1,0,
	    1,0, 1,0, 1,0, 1,0, 1,0,
	    1,0, 1,0, 1,0, 1,0, 1,0,
	    1,0, 1,0, 1,0, 1,0,
	    1,0, 1,0, 1,0, 1,0, 1,0
	};

	// Durations (ms) – includes short pauses
	float duration[] = {
	    180,40, 180,40, 180,40,
	    180,40, 180,40, 180,40,
	    180,40, 180,40, 180,40, 180,40, 400,40,
	    200,40, 200,40, 200,40, 200,40, 200,40,
	    200,40, 200,40, 200,40, 200,40,
	    200,40, 200,40, 200,40, 200,40, 400,40
	};


	for (size_t note = 0; note < sizeof(notes)/sizeof(uint16_t); note++) {
		set_buzzer_frequency(notes[note]*velocity[note]);
		HAL_Delay(duration[note]);
	}
	set_buzzer_frequency(0);
}



void SystemClock_Config(void);

int main(void) {

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM3_Init();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);


  while (1){
	  play_jingle_bells();
  }

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(void)
{
  __disable_irq();
  while (1);

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line){}
#endif
