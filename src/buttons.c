#include <stm32.h>
#include "buttons.h"

uint32_t is_released(GPIO_TypeDef * const gpio, uint32_t pin) {
    return gpio->IDR & (1 << pin);
}

uint32_t is_pressed(GPIO_TypeDef * const gpio, uint32_t pin) {
    return !is_released(gpio, pin);
}
