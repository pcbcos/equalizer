//
// Created by 29620 on 2023/5/27.
//

#ifndef CPROJECT_MENU_H
#define CPROJECT_MENU_H
#include "stdint.h"
#include "equalizer.h"
#include "ssd1306.h"
class Menu{
public:
    Menu()=default;
    ~Menu()=default;
    void up();
    void right();
    void left();
    void down();
    void change();
    void rendering();
private:
    int32_t index{};
    int32_t line{};

};

extern Menu menu;

#endif //CPROJECT_MENU_H
