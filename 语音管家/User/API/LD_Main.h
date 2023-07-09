#ifndef _LD_MAIN_H_
#define _LD_MAIN_H_

#include "main.h"


void  LD3320_main(void);

void LD3320_Init(void);

uint8 RunASR(void);

void recognizeLoop();
void ld3320PlayMp3(char *path);
int LD_ReloadMp3Data();
void mp3Play(char *path);

#endif

