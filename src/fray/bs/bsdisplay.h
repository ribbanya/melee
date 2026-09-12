#ifndef FRAY_BS_BSDISPLAY_H
#define FRAY_BS_BSDISPLAY_H

#include <fray/bs/bisimulation.h>

void BsDisplay_Init(void);

/// Redraw the current page from the snapshot. Call once per frame,
/// or only when the page index changes.
void BsDisplay_Draw(const Bisim_GlobalSnapshot* g);

/// Advance to the next page and redraw on the next call.
void BsDisplay_NextPage(void);

/// Show or hide the panel.
void BsDisplay_Show(void);
void BsDisplay_Hide(void);

#endif
