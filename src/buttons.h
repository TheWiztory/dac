#ifndef BUTTONS_H
#define BUTTONS_H

#define JOYSTICK_LEFT_GPIO GPIOB
#define JOYSTICK_LEFT_PIN 3

#define JOYSTICK_RIGHT_GPIO GPIOB
#define JOYSTICK_RIGHT_PIN 4

#define JOYSTICK_FIRE_GPIO GPIOB
#define JOYSTICK_FIRE_PIN 10

typedef enum buttons_state {
    NONE_PRESSED,
    JOYSTICK_LEFT_PRESSED,
    JOYSTICK_FIRE_PRESSED,
    JOYSTICK_RIGHT_PRESSED
} buttons_state;

uint32_t is_released(GPIO_TypeDef * const gpio, uint32_t pin);

uint32_t is_pressed(GPIO_TypeDef * const gpio, uint32_t pin);

#endif
