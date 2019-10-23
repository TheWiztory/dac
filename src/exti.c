#include <gpio.h>
#include <stm32.h>
#include "buttons.h"
#include "exti.h"

void init_exti() {
    RCC->APB2ENR = RCC_APB2ENR_SYSCFGEN;

    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    GPIOinConfigure(JOYSTICK_LEFT_GPIO, JOYSTICK_LEFT_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Rising_Falling);
    GPIOinConfigure(JOYSTICK_FIRE_GPIO, JOYSTICK_FIRE_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Rising_Falling);
    GPIOinConfigure(JOYSTICK_RIGHT_GPIO, JOYSTICK_RIGHT_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Rising_Falling);
}

void activate_buttons() {
    activate_button(JOYSTICK_LEFT_PIN);
    activate_button(JOYSTICK_FIRE_PIN);
    activate_button(JOYSTICK_RIGHT_PIN);
}

void activate_button(uint32_t pin) {
    EXTI->IMR |= (1 << pin);

}

void deactivate_buttons() {
    EXTI->IMR &= ~(1 << JOYSTICK_LEFT_PIN);
    EXTI->IMR &= ~(1 << JOYSTICK_FIRE_PIN);
    EXTI->IMR &= ~(1 << JOYSTICK_RIGHT_PIN);

    mark_interrupt_as_handled(JOYSTICK_LEFT_PIN);
    mark_interrupt_as_handled(JOYSTICK_FIRE_PIN);
    mark_interrupt_as_handled(JOYSTICK_RIGHT_PIN);
}

uint32_t is_interrupt_from_button(uint32_t pin) {
    return EXTI->PR & (1 << pin);
}

void mark_interrupt_as_handled(uint32_t pin) {
    EXTI->PR = (1 << pin);
}
