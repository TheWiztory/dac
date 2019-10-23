#include <stm32.h>
#include "clock.h"
#include "buttons.h"
#include "exti.h"
#include "music.h"
#include "timers.h"
#include "dma.h"
#include "usart.h"
#include "clock.h"

static uint32_t is_playing = 1;

static buttons_state current_buttons_state = NONE_PRESSED;

// rewind
void EXTI3_IRQHandler(void) {
    if (is_pressed(JOYSTICK_LEFT_GPIO, JOYSTICK_LEFT_PIN)) {
        position_delta = REWIND_POSITION_DELTA;
        current_buttons_state = JOYSTICK_LEFT_PRESSED;
        if (!is_playing)
            run_dac_timer();
        debug_message("REWINDING PRESSED\n\r", 19);
    } else {
        position_delta = DEFAULT_POSITION_DELTA;
        current_buttons_state = NONE_PRESSED;
        if (!is_playing)
            stop_dac_timer();
        debug_message("REWINDING RELEASED\n\r", 20);
    }
    deactivate_buttons();
    run_buttons_timer();
}

// fast forward
void EXTI4_IRQHandler(void) {
    if (is_pressed(JOYSTICK_RIGHT_GPIO, JOYSTICK_RIGHT_PIN)) {
        position_delta = FAST_FORWARD_POSITION_DELTA;
        current_buttons_state = JOYSTICK_RIGHT_PRESSED;
        if (!is_playing)
            run_dac_timer();
        debug_message("FAST FORWARDING PRESSED\n\r", 25);
    } else {
        position_delta = DEFAULT_POSITION_DELTA;
        current_buttons_state = NONE_PRESSED;
        if (!is_playing)
            stop_dac_timer();
        debug_message("FAST FORWARDING RELEASED\n\r", 26);
    }
    deactivate_buttons();
    run_buttons_timer();
}

// play/pause
void EXTI15_10_IRQHandler(void) {
    if (is_interrupt_from_button(JOYSTICK_FIRE_PIN)) {
        if (is_pressed(JOYSTICK_FIRE_GPIO, JOYSTICK_FIRE_PIN)) {
            current_buttons_state = JOYSTICK_FIRE_PRESSED;
            if (is_playing)
                stop_dac_timer();
            else
                run_dac_timer();
            is_playing = !is_playing;
            debug_message("PLAY/PAUSE PRESSED\n\r", 20);
        } else {
            current_buttons_state = NONE_PRESSED;
            debug_message("PLAY/PAUSE RELEASED\n\r", 21);
        }
        deactivate_buttons();
        run_buttons_timer();
    }
}

void reactivate_buttons() {
    stop_buttons_timer();
    activate_buttons();
//    debug_message("BUTTONS REACTIVATED\n\r", 21);
}

void TIM2_IRQHandler(void) {
    if (is_buttons_timer_interrupt()) {
        stop_buttons_timer();
        switch (current_buttons_state) {
            case JOYSTICK_LEFT_PRESSED:
                if (is_pressed(JOYSTICK_LEFT_GPIO, JOYSTICK_LEFT_PIN)) {
                    activate_button(JOYSTICK_LEFT_PIN);
                } else {
                    current_buttons_state = NONE_PRESSED;
                    reactivate_buttons();
                    position_delta = DEFAULT_POSITION_DELTA;
                    if (!is_playing)
                        stop_dac_timer();
                }
                break;
            case JOYSTICK_FIRE_PRESSED:
                if (is_pressed(JOYSTICK_FIRE_GPIO, JOYSTICK_FIRE_PIN)) {
                    activate_button(JOYSTICK_FIRE_PIN);
                } else {
                    current_buttons_state = NONE_PRESSED;
                    reactivate_buttons();
                }
                break;
            case JOYSTICK_RIGHT_PRESSED:
                if (is_pressed(JOYSTICK_RIGHT_GPIO, JOYSTICK_RIGHT_PIN)) {
                    activate_button(JOYSTICK_RIGHT_PIN);
                } else {
                    current_buttons_state = NONE_PRESSED;
                    reactivate_buttons();
                    position_delta = DEFAULT_POSITION_DELTA;
                    if (!is_playing)
                        stop_dac_timer();
                }
                break;
            case NONE_PRESSED:
                reactivate_buttons();
                break;
        }
        mark_buttons_timer_as_handled();
    }
}

void move_samples_position() {
    position = (samples_size + position + position_delta) % samples_size;
}

uint32_t prescale_sample() {
//    return DAC_TIMER_MAX_VALUE * samples[position] / 256;
    return samples[position];
}

//void TIM1_UP_TIM10_IRQHandler(void) {
void TIM3_IRQHandler(void) {
    if (is_refresh_sample_interrupt()) {
        move_samples_position();
        set_new_sample(prescale_sample());
        mark_refresh_sample_interrupt_as_handled();
    }
}

int main() {
    init_clock();
    init_exti();
    init_timers();
    init_usart();
    init_dma();
    enable_usart();

    run_dac_timer();
    debug_message("PLAYING\n\r", 9);

    for (;;) {}
}
