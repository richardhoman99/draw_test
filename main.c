/*
 *	main.c
 *	ETCH AND SKETCH ON PS2!!!
 */

#include <tamtypes.h>
#include "libpad.h"
#include <loadfile.h>
#include <sifrpc.h>
#include <kernel.h>
#include <stdio.h>
#include "gif.h"
#include "dma.h"
#include "gs.h"

#define PORT 0
#define SLOT 0

static char padBuff[256] __attribute__((aligned(64)));

DECLARE_GS_PACKET(pixel, 4);
DECLARE_GS_PACKET(rect, 5);

void put_pixel(u16 x, u16 y);
void put_rect(u16 x0, u16 y0, u16 x1, u16 y1);

typedef struct
{
	u8 r, g, b;
} color_t;

enum colors
{
	kRED = 0,
	kGREEN,
	kBLUE,
	colorsLength
};

color_t colorPreset[] = 
{
	{255, 0, 0}, // red
	{0, 255, 0}, // green
	{0, 0, 255} // blue
};

static color_t *currentColor;

int main(int argc, const char *argv[])
{
	int colorIndex = kGREEN;
	int err;
	int padState;
	struct padButtonStatus buttonInfo;
	register int drawX = 0;
	register int drawY = 0;
	u32 pButtons = 0x00000000;

	dma_reset();
	SifInitRpc(0);
	err = SifLoadModule("rom0:SIO2MAN", 0, NULL);
	if (err < 0)
	{
		printf("Failed to load module SIO2MAN: code %d\n", err);
		SleepThread();
	}
	err = SifLoadModule("rom0:PADMAN", 0, NULL);
	if (err < 0)
	{
		printf("Failed to load module SIO2MAN: code %d\n", err);
		SleepThread();
	}

	padInit(0);
	err = padPortOpen(PORT, SLOT, padBuff);
	if (err == 0) printf("Could not open controller connection - code %d", err);

	GS_RESET();

	__asm__ (" \
		li		$a0,0x0000FF00; \
		.set noat; \
		lui 	$at,0x1200; \
		ori 	$at,$at,0x1000; \
		ld		$v0,0x0($at); \
		.set at; \
		dsrl	$v0,16; \
		andi	$v0,0xFF; \
		li 		$v0,0x20; \
		li		$v1,0x71; \
		syscall;"); // set imr

	__asm__ (" \
		li 		$a0,0; # non-interlaced \
		li 		$a1,2; # ntsc \
		li 		$a2,1; # frame \
		li		$v1,0x02; \
		dli 	$v0,0xFF65; \
		syscall"); // set gs crtc

	GS_SET_PMODE(
		1,		// ReadCircuit1 OFF 
		0,		// ReadCircuit2 ON
		1,		// Use ALP register for Alpha Blending
		1,		// Alpha Value of ReadCircuit2 for output selection
		0,		// Blend Alpha with the output of ReadCircuit2
		0xFF	// Alpha Value = 1.0
	);

	GS_SET_DISPFB1(
		0,				// Frame Buffer base pointer = 0 (Address/2048)
		256/64,	// Buffer Width (Address/64)
		0,			// Pixel Storage Format
		0,				// Upper Left X in Buffer = 0
		0				// Upper Left Y in Buffer = 0
	);

	GS_SET_DISPLAY1(
		656,		// X position in the display area (in VCK units)
		36,			// Y position in the display area (in Raster units)
		1,			// Horizontal Magnification - 1
		0,			// Vertical Magnification = 1x
		255,		// Display area width  - 1 (in VCK units) (Width*HMag-1)
		256			// Display area height - 1 (in pixels)	  (Height-1)
	);

	GS_SET_BGCOLOR(
		0,	// RED
		0,	// GREEN
		0	// BLUE
	);

	DECLARE_GS_PACKET(gs_setup_buf, 4);

	BEGIN_GS_PACKET(gs_setup_buf);

	GIF_TAG_AD(gs_setup_buf, 3, 1, 0, 0, 0);

	GIF_DATA_AD(gs_setup_buf, frame_1,
		GS_FRAME(
			0,					// FrameBuffer base pointer = 0 (Address/2048)
			512/64,		// Frame buffer width (Pixels/64)
			0,				// Pixel Storage Format
			0));

	// No displacement between Primitive and Window coordinate systems.
	GIF_DATA_AD(gs_setup_buf, xyoffset_1,
		GS_XYOFFSET(
			0x0,
			0x0));

	// Clip to frame buffer.
	GIF_DATA_AD(gs_setup_buf, scissor_1,
		GS_SCISSOR(
			0,
			512,
			0,
			256));

	SEND_GS_PACKET(gs_setup_buf);

	currentColor = &(colorPreset[kGREEN]);

	while (1)
	{
		register int wait;
		for (wait = 0; wait < 0xFFFFF; wait++) { __asm__("nop;");}

		padState = padGetState(PORT, SLOT);
		if (padState != PAD_STATE_STABLE)
		{
			printf("Controller state not stable");
			continue;
		}

		err = padRead(PORT, SLOT, &buttonInfo);
		if (err == 0) printf("Error reading controller snapshot - code: %d\n", err);

		u32 buttons = 0xFFFF ^ buttonInfo.btns;
		register int draw = 0;

		if (buttons & PAD_UP)
		{
			drawY--;
			draw = 1;
		}
		if (buttons & PAD_RIGHT)
		{
			drawX++;
			draw = 1;
		}
		if (buttons & PAD_DOWN)
		{
			drawY++;
			draw = 1;
		}
		if (buttons & PAD_LEFT)
		{
			drawX--;
			draw = 1;
		}
		if (pButtons & PAD_SELECT && !(buttons & PAD_SELECT)) // select released
		{
			colorIndex++;
			if (colorIndex >= colorsLength) colorIndex = 0;
			currentColor = &(colorPreset[colorIndex]);
		}

		if (draw)
		{
			put_pixel(drawX, drawY);
		}
		pButtons = buttons;

		put_rect((256/2)-10, 0, (256/2), 10);
	}

	SleepThread();

	return 0;
}

void put_pixel(u16 x, u16 y)
{
	BEGIN_GS_PACKET(pixel);

	GIF_TAG_AD(pixel, 3, 0, 0, 0, 0);

	GIF_DATA_AD(pixel, prim, GS_PRIM(PRIM_POINT, 0, 0, 0, 0, 0, 0, 0, 0));
	
	GIF_DATA_AD(pixel, rgbaq, GS_RGBAQ(currentColor->r, currentColor->g, currentColor->b, 0, 0));

	GIF_DATA_AD(pixel, xyz2, GS_XYZ2(x<<4, y<<4, 0));

	SEND_GS_PACKET(pixel);
}

void put_rect(u16 x0, u16 y0, u16 x1, u16 y1)
{
	BEGIN_GS_PACKET(rect);

	GIF_TAG_AD(rect, 4, 1, 0, 0, 0);

	GIF_DATA_AD(rect, prim, GS_PRIM(PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0));
	
	GIF_DATA_AD(rect, rgbaq, GS_RGBAQ(currentColor->r, currentColor->g, currentColor->b, 0, 0));

	GIF_DATA_AD(rect, xyz2, GS_XYZ2(x0<<4, y0<<4, 0));

	GIF_DATA_AD(rect, xyz2, GS_XYZ2((x1)<<4, (y1)<<4, 0));

	SEND_GS_PACKET(rect);
}
