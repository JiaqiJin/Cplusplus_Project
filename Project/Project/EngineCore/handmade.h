#ifndef HANDMADE_H
#define HANDMADE_H

#include "handmade_platform.h"

struct game_offscreen_buffer {
	void* Memory;
	int BytesPerPixel;
	int Width;
	int Height;
};
internal void
GameUpdateAndRender(game_offscreen_buffer* Buffer, int XOffset, int YOffset);

#endif