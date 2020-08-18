#ifndef HANDMADE_H
#define HANDMADE_H

#include "handmade_platform.h"

struct game_offscreen_buffer 
{
	void* Memory;
	int Pitch;
	int Width;
	int Height;
};

struct game_sound_output_buffer
{
	int SampleCount;
	int16_t* Samples;
	//int WavePeriod;
	int SamplePerSecond;
};

internal void
GameUpdateAndRender(game_offscreen_buffer* Buffer, int XOffset, int YOffset, 
	game_sound_output_buffer* SoundBuffer, int ToneHz);

#endif