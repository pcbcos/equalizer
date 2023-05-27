//
// Created by 29620 on 2023/5/27.
//
#include <cstdio>
#include "menu.h"
#include "i2c.h"

Menu menu;




void Menu::right() {
    index = (index + 1) % 10;
    rendering();
}

void Menu::left() {
    index = index - 1;
    if (index == -1) {
        index = 9;
    }
}

void Menu::up() {

    rendering();
}

void Menu::down() {

    rendering();
}

void Menu::change() {
    line = (line + 1) % 3;
    rendering();
}

extern equalizer eq1;
void Menu::rendering() {
    ssd1306_Fill(Black);
    auto gain = eq1.get_gain(index);
    auto freq = eq1.get_freq(index);
    auto Q = eq1.get_q(index);
    char buff[11];
    sprintf(buff, "Freq=%5d", uint16_t(freq));
    ssd1306_SetCursor(0, 0);
    if (line == 0) {
        ssd1306_InvertColors();
        ssd1306_WriteString(buff, Font_7x10, White);
        ssd1306_InvertColors();
    } else {
        ssd1306_WriteString(buff, Font_7x10, White);
    }


    for (auto &c: buff) {
        c = 0;
    }
    sprintf(buff, "Gain=%2d", uint16_t(gain));
    ssd1306_SetCursor(0, 10);
    if (line == 1) {
        ssd1306_InvertColors();
        ssd1306_WriteString(buff, Font_7x10, White);
        ssd1306_InvertColors();
    } else {
        ssd1306_WriteString(buff, Font_7x10, White);
    }

    for (auto &c: buff) {
        c = 0;
    }
    sprintf(buff, "Q=%2.1f", Q);
    ssd1306_SetCursor(0, 20);
    if (line == 2) {
        ssd1306_InvertColors();
        ssd1306_WriteString(buff, Font_7x10, White);
        ssd1306_InvertColors();
    } else {
        ssd1306_WriteString(buff, Font_7x10, White);
    }

    ssd1306_UpdateScreen(&hi2c1);
}
