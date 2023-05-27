//
// Created by wengz on 2023/5/27.
//
#include "key.h"
#include "ssd1306.h"
#include "i2c.h"


struct pin {
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_PIN;
    uint8_t last_state;
    uint8_t state;
};

static pin key_up = {GPIOC, GPIO_PIN_0};
static pin key_down = {GPIOD, GPIO_PIN_6};
static pin key_left = {GPIOC, GPIO_PIN_1};
static pin key_right = {GPIOD, GPIO_PIN_15};
static pin key_center = {GPIOC, GPIO_PIN_7};

void key_flash() {
    auto tmp = !LL_GPIO_IsInputPinSet(key_up.GPIOx, key_up.GPIO_PIN);
    key_up.state = key_up.last_state && tmp;
    key_up.last_state = tmp;

    tmp = !LL_GPIO_IsInputPinSet(key_down.GPIOx, key_down.GPIO_PIN);
    key_down.state = key_down.last_state && tmp;
    key_down.last_state = tmp;

    tmp = !LL_GPIO_IsInputPinSet(key_left.GPIOx, key_left.GPIO_PIN);
    key_left.state = key_left.last_state && tmp;
    key_left.last_state = tmp;

    tmp = !LL_GPIO_IsInputPinSet(key_right.GPIOx, key_right.GPIO_PIN);
    key_right.state = key_right.last_state && tmp;
    key_right.last_state = tmp;

    tmp = !LL_GPIO_IsInputPinSet(key_center.GPIOx, key_center.GPIO_PIN);
    key_center.state = key_center.last_state && tmp;
    key_center.last_state = tmp;
}

void ok() {
    menu.change();
}

void up0() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("up0", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void down0() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("down0", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void left0() {
    menu.left();
}

void right0() {
    menu.right();
}

void up1() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("up1", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void down1() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("down1", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void left1() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("left1", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void right1() {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("right1", Font_11x18, White);
    ssd1306_UpdateScreen(&hi2c1);
}

void key_task() {
    static uint8_t last_center{};
    static uint8_t last_up{};
    static uint8_t last_down{};
    static uint8_t last_left{};
    static uint8_t last_right{};
    static uint8_t aux{};
    if ((!key_center.state) && last_center && (!aux)) {
        ok();
        goto copy;
    }
    if (last_center && (!key_up.state) && last_up) {
        up1();
        aux = 1;
        goto copy1;
    }
    if (last_center && (!key_down.state) && last_down) {
        down1();
        aux = 1;
        goto copy1;
    }
    if (last_center && (!key_left.state) && last_left) {
        left1();
        aux = 1;
        goto copy1;
    }
    if (last_center && (!key_right.state) && last_right) {
        right1();
        aux = 1;
        goto copy1;
    }
    if ((!last_center) && (!key_up.state) && last_up) {
        up0();
        goto copy;
    }
    if ((!last_center) && (!key_down.state) && last_down) {
        down0();
        goto copy;
    }
    if ((!last_center) && (!key_left.state) && last_left) {
        left0();
        goto copy;
    }
    if ((!last_center) && (!key_right.state) && last_right) {
        right0();
        goto copy;
    }


    copy:
    copy1:
    last_center = key_center.state;
    last_up = key_up.state;
    last_down = key_down.state;
    last_left = key_left.state;
    last_right = key_right.state;
    if (key_center.state == 0) {
        aux = 0;
    }
    return;

}

