# STM32 NUCLEO-F401RE XMAS MusicBox


Pretty useless and just made for fun XMAS MusicBox plays **Jingle Bells** on a **KY-006 passive buzzer** using a **STM32 Nucleo-F401RE** board.


Full project is not shown here. Create a project for the STM32F401RE MCU in STM32CubeIDE and copy the Inc and Src core
files given here.


## How It Works

- The buzzer is driven using **TIM3 PWM** on **PA6**.  

- **Timer setup**:
  - TIM3 is configured in **PWM mode 1**, up-counting.  
  - **Prescaler** divides the 84 MHz system clock down to 1 MHz (for easy frequency calculation).  
  - **ARR (Auto-Reload Register)** is dynamically set for each note:  
    ```c
    ARR = (timer_clk / freq) - 1;
    ```
  - **CCR (Compare Register)** is set to half the ARR for a 50% duty cycle square wave.  

- `set_buzzer_frequency(freq)` sets the PWM frequency for each note:
  - `freq = 0` → silence (pause)  
  - Otherwise, calculates ARR/CCR to generate the desired frequency.  

- The melody is stored in three arrays:
  - `notes[]` → frequencies in Hz (0 for pause)  
  - `velocity[]` → 1 = note on, 0 = pause  
  - `duration[]` → length of each note in milliseconds  

- `play_jingle_bells()` then loops through the arrays.


## Hardware

- **PA6** → buzzer positive  
- **GND** → buzzer ground  
- **VCC** → 3.3 V or 5 V  
