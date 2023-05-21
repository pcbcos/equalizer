#ifndef _RETARGET_H__
#define _RETARGET_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f407xx.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stm32f4xx_ll_usart.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);


int _fstat(int fd, struct stat *st);

#ifdef __cplusplus
}
#endif
#endif //#ifndef _RETARGET_H__