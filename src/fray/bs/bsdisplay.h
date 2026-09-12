#ifndef FRAY_BS_BSDISPLAY_H
#define FRAY_BS_BSDISPLAY_H

#include <fray/bs/bisimulation.h>

void BsDisplay_Init(void);
void BsDisplay_Show(void);
void BsDisplay_Hide(void);
void BsDisplay_NextPage(void);
void BsDisplay_Draw(const Bisim_GlobalSnapshot* g);

#endif
