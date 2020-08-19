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

struct win32_sound_output
{
	int32 SamplesPerSecond;
	uint32 RunningSampleIndex;
	int WavePeriod;
	int32 BytesPerSample;
	int32 SecondaryBufferSize;
	int32 LatencySampleCount;
	real32 tSine;
};

struct game_sound_output_buffer
{
	int SampleCount;
	int16_t* Samples;
	//int WavePeriod;
	int SamplePerSecond;
};

struct game_button_state {
	int32 HalfTransitionCount;
	bool32 IsEndedDown;
};

struct game_controller_input {
	bool32 IsAnalog;

	real32 StartX;
	real32 EndX;
	real32 MinX;
	real32 MaxX;

	real32 StartY;
	real32 EndY;
	real32 MinY;
	real32 MaxY;

	union {
		game_button_state Buttons[6];
		struct {
			game_button_state Up;
			game_button_state Down;
			game_button_state Left;
			game_button_state Right;
			game_button_state LeftShoulder;
			game_button_state RightShoulder;
		};
	};
};

struct game_input {
	game_controller_input Controllers[4];
};


internal void
GameUpdateAndRender(game_input* Input, game_offscreen_buffer* Buffer,
	game_sound_output_buffer* SoundBuffer);

#endif