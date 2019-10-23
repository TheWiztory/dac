#include <gpio.h>
#include <stm32.h>
#include "clock.h"

#define M 25
#define N 400
#define P 4
#define Q 8

void set_max_clock_frequency() {
    RCC->CR &= ~(RCC_CR_PLLI2SON | RCC_CR_PLLON | RCC_CR_HSEBYP | RCC_CR_HSEON);
    RCC->CR |= RCC_CR_HSEON;

    while (!(RCC->CR & RCC_CR_HSERDY));

    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE | M | N << 6 | ((P >> 1) - 1) << 16 | Q << 24;

    RCC->CR |= RCC_CR_PLLON;

    while (!(RCC->CR & RCC_CR_PLLRDY));

    uint32_t latency = 100 / 30;

    FLASH->ACR = FLASH_ACR_DCEN | /* data cache */
                 FLASH_ACR_ICEN | /* instr. cache */
                 FLASH_ACR_PRFTEN | /* prefetch */
                 latency;

    uint32_t reg;

    reg = RCC->CFGR;
    reg &= ~RCC_CFGR_HPRE;
    reg |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR = reg;

    reg = RCC->CFGR;
    reg &= ~RCC_CFGR_PPRE1;
    reg |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR = reg;

    reg = RCC->CFGR;
    reg &= ~RCC_CFGR_PPRE2;
    reg |= RCC_CFGR_PPRE2_DIV1;
    RCC->CFGR = reg;

    reg = RCC->CFGR;
    reg &= ~RCC_CFGR_SW;
    reg |= RCC_CFGR_SW_PLL;
    RCC->CFGR = reg;

    while (!((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL));
}

void init_clock() {
    set_max_clock_frequency();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
}
