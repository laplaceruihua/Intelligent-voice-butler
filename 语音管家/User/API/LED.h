#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

#define RED_TOGGLE()  GPIOC->ODR ^= (1<<8)
#define GREEN_TOGGLE()  GPIOC->ODR ^= (1<<9)
#define BLUE_TOGGLE() GPIOC->ODR ^= (1<<8)

#define RED_CTL(x)   x?(GPIOC->ODR &=~ (1<<8)):(GPIOC->ODR |= (1<<8))
#define GREEN_CTL(x) x?(GPIOC->ODR &=~ (1<<9)):(GPIOC->ODR |= (1<<9))
#define BLUE_CTL(x)  x?(GPIOA->ODR &=~ (1<<8)):(GPIOA->ODR |= (1<<8))
#define BEEP(x)  x?(GPIOC->ODR |= (1<<6)):(GPIOA->ODR &=~ (1<<6))




void LED_gpio_cfg(void);
void Glide_LED(void);
void Flicker_LED(void);

void Off_LED(void);
void On_LED(void);

void Beep_Config(void);
void KEY_Config(void);
void Lantern_Config(void);
void Flashlight_Config(void);
 void PWM_LED(int16_t led);


#endif
