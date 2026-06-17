/****************************************************************
*         |\_____/|  Orginal {,} made by Ciupaga
*         |       |  Copyright 2025-2026
*  ___    | >   < |
* /   \  _\ = , = /  https://github.com/CiupagaPL/Przecinek
* \__  \/        |   Make sure to read the License and Manual!
*    \_         /
*      \ \----\ \
*       {,{,} {,},}
****************************************************************/

// Przecinek header
#include "../przecinek.h"

// Standard C libraries
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <locale.h>

// WIN threads library
#include <process.h>

// GUID library
#include <initguid.h>

// WIN and GDI+ libraries
#include <windows.h>
#include <gdiplus.h>

// CoreAudio libraries
#include <mmdeviceapi.h>
#include <audioclient.h>

// GL and GLU libraries
#include <GL/gl.h>
#include <GL/glu.h>

/******************************************
*  ,_   _,
*  |     |
* (   ,   )  DEBUG DEFINES
*  |     |
*  `-   -`
******************************************/

/******************************************
*  ,______,  [pKey] values
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_0 0x30

#define VK_Q 0x51
#define VK_W 0x57
#define VK_E 0x45
#define VK_R 0x52
#define VK_T 0x54
#define VK_Y 0x59
#define VK_U 0x55
#define VK_I 0x49
#define VK_O 0x4F
#define VK_P 0x50
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_Z 0x5A
#define VK_X 0x58
#define VK_C 0x43
#define VK_V 0x56
#define VK_B 0x42
#define VK_N 0x4E
#define VK_M 0x4D

#define VK_LBUTTON 0x02
#define VK_RBUTTON 0x03
#define VK_MBUTTON 0x04
#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0xFD

#define VK_OEM_SEMICOLON 0xBA
#define VK_OEM_QUOTE 0xDE

/******************************************
*  ,______,  GL values
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
#ifndef GL_BGRA
	#define GL_BGRA 0x80E1
#endif

#ifndef GL_CLAMP_TO_EDGE
	#define GL_CLAMP_TO_EDGE 0x812F
#endif

/******************************************
*  ,_   _,
*  |     |
* (   ,   )  DEBUG STRUCTURES
*  |     |
*  `-   -`
******************************************/

/******************************************
*  ,______,  [pBuild] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Pixel position on the screen
	int16_t x, y;
	// Backuped pixel position on the screen
	// Used when in fullScreen
	int16_t xBac, yBac;

	// Pixel size of the window
	uint16_t width, height;
	// Backuped pixel size of the window
	// Used when in fullScreen
	uint16_t widthBac, heightBac;
	// Real pixel size of the window
	// WIN by default uses weird measurement
	uint16_t widthFix, heightFix;

	// Size limits of the window
	uint16_t widthMin, heightMin, widthMax, heightMax;

	// Window title
	wchar_t *title;

	// Window focus and full screen status
	bool focus, fullScreen;

	// Window resizable check
	bool resizable;

	// Pointer to the pixel position on the screen
	int16_t *xPoint, *yPoint;

	// Poiner to the pixel size of the window
	uint16_t *widthPoint, *heightPoint;

	// Pointer to size limits of the window
	uint16_t *widthMinPoint, *heightMinPoint, *widthMaxPoint, *heightMaxPoint;

	// Pointer to the window title
	wchar_t **titlePoint;

	// Pointer to the window focus and full screen status
	bool *focusPoint, *fullScreenPoint;

	// Window debug buffers
	HDC DHDC;
	HGLRC buffer;
	// Debug physical window
	HWND HWND;

	// Window debug actions from proc
	bool DESTROY, FOCUSIN, FOCUSOUT;
	pSize SIZE;
	pPosition MOVE;

	bool exists;
} pBuild;

pBuild build[WINDOW_MAX];

/******************************************
*  ,______,  [pWindowPoint] structure
*  |      |
*  |______|  [POINTER] [DEBUG]
* (--------)
******************************************/
typedef struct{
	uint8_t ID;

	// Physical window for the debug proc
	HWND HWND;
} pWindowPoint;

pWindowPoint *windowPoint, *newWindowPoint;

/******************************************
*  ,______,  [pFigure] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Vertice count of the object
	uint16_t verticeCount;
	// Vertice positions array
	// Equal to the vertice count
	pPosition *vertice;

	// Minimal pixel position of the object
	int32_t x, y;
	// Pixel size of the object
	uint16_t width, height;
	// Calculated center of the object
	// Used for collision purposes
	pPosition center;

	// Debug texture positions of the object
	// Values from 0.0f to 1.0f
	float *xSrc, *ySrc;

	// Debug rotation correction of the object
	uint16_t rotationFix;

	bool exists;
} pFigure;

pFigure figure[OBJECT_MAX];

/******************************************
*  ,______,  [pTexture] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Generated by GL texture ID
	GLuint source;

	bool exists;
} pTexture;

pTexture texture[IMAGE_MAX];

/******************************************
*  ,______,  [pView] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Font size in pt
	uint16_t size;

	// Debug font structures
	// Used for loading purposes
	GpFontCollection *collection;
	GpFontFamily *fontFamily;
	// Main debug font structure
	GpFont *base;

	bool exists, created[TEXT_MAX];
} pView;

pView view[FONT_MAX];

/******************************************
*  ,______,  [pCode] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Pixel position on the window
	// Depends on used font
	int32_t x[FONT_MAX], y[FONT_MAX];

	// Text max length
	// Usually decreased by `1`
	uint16_t length;
	// Text value
	wchar_t *value;

	// Debug texture buffers of the text
	GLuint *source[FONT_MAX];
	// Debug size and position buffers of the text
	pSize *sourceSize[FONT_MAX];
	pPosition *sourcePosition[FONT_MAX];

	// Text spacing and space size in pt
	int32_t letterSpacing, lineSpacing, spaceSize;

	bool exists, change;
} pCode;

pCode code[TEXT_MAX];

/******************************************
*  ,______,  [pSound] structure
*  |      |
*  |______|  [DEBUG]
* (--------)
******************************************/
typedef struct{
	// Sound main debug thread
	HANDLE thread;

	// Sound debug pause status
	long int pause;
	// Sound debug kill event
	bool kill;

	// Pointer of the volume percentage
	// Value range from `0` to the AUDIO_VOLUME_MAX
	uint16_t *volumePoint;
	// Pointer of the audio pause state
	bool *pausePoint;

	// Pointer of the audio current and max frame
	uint32_t *framePoint, *frameMaxPoint;

	bool exists;
} pSound;

pSound sound[AUDIO_MAX];

/******************************************
*  ,______,  [pAudioPoint] structure
*  |      |
*  |______|  [POINTER] [DEBUG]
* (--------)
******************************************/
typedef struct{
	uint16_t ID;

	// Debug directory of the audio
	wchar_t *directory;
} pAudioPoint;

pAudioPoint audioPoint[AUDIO_MAX];

/******************************************
*  ,_   _,
*  |     |
* (   ,   )  GLOBAL VARIABLES
*  |     |
*  `-   -`
******************************************/

// Create global [tokenGDI] and [inputGDI]
ULONG_PTR tokenGDI;
GdiplusStartupInput inputGDI;

// Create global [przecinek]
pPrzecinek przecinek={ true, 0, 0 };

// Create global [setup]
bool setup=false;

// Create global [windowMainID], [windowCount] and [windowCreateID]
uint16_t windowMainID=0, windowCount=0;
uint8_t windowCreateID=0;

// Create global [frameCount] and [lastFrameCount]
uint16_t frameCount, lastFrameCount;

// Create global [frameStart] and [lastFrame]
uint32_t frameStart;
uint16_t lastFrame=0;

// Create global [currentFrameLimit] and [frameOverhead]
uint16_t currentFrameLimit;
int16_t frameOverhead;

// Create global [DHDC], [graphics] and [stringFormat]
HDC DHDC;
GpGraphics* graphics;
GpStringFormat *stringFormat;

// Create global [input] and [keyboard]
uint16_t input[PRZECINEK_KEY_PRESS_MAX+1]={0};
uint8_t keyboard[USHRT_MAX]={0};

/******************************************
*  ,_   _,
*  |     |
* (   ,   )  DEBUG FUNCTIONS
*  |     |
*  `-   -`
******************************************/

uint16_t pDebugKeySearch(wchar_t *key);

void pDebugBuildReset(uint8_t ID);
void pDebugWindowReset(pWindow *window);
LRESULT CALLBACK pDebugBuildProc(HWND HWND, UINT uMessage, WPARAM wParameter, LPARAM lParameter);

void pDebugFigureReset(uint16_t ID);
void pDebugObjectReset(pObject *object);
void pDebugObjectSetup(pObject *object);
bool pDebugObjectCollisionTriangle(
	pPosition *v1A, pPosition *v2A, pPosition *v3A,
	pPosition *v1B, pPosition *v2B, pPosition *v3B
);

void pDebugTextureReset(uint16_t ID);

void pDebugViewReset(uint16_t ID);
void pDebugFontReset(pFont *font);

void pDebugCodeReset(uint16_t ID);
void pDebugTextReset(pText *text);
bool pDebugTextSetup(pText *text, pFont *font);

void pDebugSoundReset(uint16_t ID);
void pDebugAudioReset(pAudio *audio);
unsigned int __stdcall pDebugAudioProc(void *argument);

/******************************************
*  ,_   _,
*  |     |
* (   ,   )  FUNCTIONS
*  |     |
*  `-   -`
******************************************/

/****************************************************************
* |\_____/| pSetup()
* | .     |
* |     . | In: bool [debug], uint16_t [frameLimit]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [debug] - console debug messages status.
* [frameLimit] - what should be the frame limit of all windows.
*
* Additional Description:
* This function initializes Przecinek library. It setups locale
* and libraries used later. It also creates several global
* debug objects, setups all needed values and variables,
* setups [przecinek], creates debug console and downloads
* initial values of the [display] and the [cursor].
****************************************************************/
uint8_t pSetup(bool debug, uint16_t frameLimit){
	if(setup==true){
		if(przecinek.debug==true){
			printf("[pSetup() Error]\n");
			printf("Przecinek was already initialized!\n");
			printf("Make sure to use pEndup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	// Initialize locale
	setlocale(LC_ALL, "");

	// Initialize GDI+ library
	inputGDI.GdiplusVersion=1;
	inputGDI.DebugEventCallback=NULL;
	inputGDI.SuppressBackgroundThread=FALSE;
	inputGDI.SuppressExternalCodecs=FALSE;
	GdiplusStartup(&tokenGDI, &inputGDI, NULL);

	// Create and setup local [power]
    PROCESS_POWER_THROTTLING_STATE power;
    RtlZeroMemory(&power, sizeof(power));

    power.Version=PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    power.ControlMask=PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
    power.StateMask=0;

	// Disable slowdown using local [power]
    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &power, sizeof(power));

	// Set [przecinek] [debug] value
	przecinek.debug=debug;

	// Check [frameLimit] value
	if(frameLimit<PRZECINEK_FRAME_MIN){
		if(przecinek.debug==true){
			printf("[pSetup() Warning]\n");
			printf("Value of the przecinek.frameLimit is too small!\n");
			printf(
				"Value of the przecinek.frameLimit was changed from %i to %i,\n",
				frameLimit, PRZECINEK_FRAME_MIN
			);
			fflush(stdout);
		}

		// Update [frameLimit] value
		frameLimit=PRZECINEK_FRAME_MIN;
	}
	else if(frameLimit>PRZECINEK_FRAME_MAX){
		if(przecinek.debug==true){
			printf("[pSetup() Warning]\n");
			printf("Value of the przecinek.frameLimit is too large!\n");
			printf(
				"Value of the przecinek.frameLimit was changed from %i to %i,\n",
				frameLimit, PRZECINEK_FRAME_MAX
			);
			fflush(stdout);
		}

		// Update [frameLimit] value
		frameLimit=PRZECINEK_FRAME_MAX;
	}

	// Set [przecinek] [frameLimit] value
	przecinek.frameLimit=frameLimit;

	// Set [przecinek] [display] values
	przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
	przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

	// Create local [point]
	POINT point;

	// Load local [point] position
	if(GetCursorPos(&point)){
		// Set [przecinek] [cursor] values
		przecinek.cursor.x=point.x;
		przecinek.cursor.y=point.y;
	}
	else{
		// Set [przecinek] [cursor] values
		przecinek.cursor.x=0;
		przecinek.cursor.y=0;
	}

	// Initialize [DHDC]
	DHDC=GetDC(NULL);

	// Initialize [graphics]
	GdipCreateFromHDC(DHDC, &graphics);

	// Initialize and setup [stringFormat]
	GdipCreateStringFormat(0, 0, &stringFormat);
	GdipStringFormatGetGenericTypographic(&stringFormat);

	// Allocate global WIN console
	if(debug==true){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}

	// Fill all structures [exists] values with `false`
	for(uint8_t ID=0; ID<WINDOW_MAX; ID+=1){ build[ID].exists=false; }
	for(uint16_t ID=0; ID<OBJECT_MAX; ID+=1){ figure[ID].exists=false; }
	for(uint16_t ID=0; ID<FONT_MAX; ID+=1){ view[ID].exists=false; }
	for(uint16_t ID=0; ID<TEXT_MAX; ID+=1){ code[ID].exists=false; }
	for(uint16_t ID=0; ID<IMAGE_MAX; ID+=1){ texture[ID].exists=false; }
	for(uint16_t ID=0; ID<AUDIO_MAX; ID+=1){ sound[ID].exists=false; }

	// Set [lastFrame] and [currentFrameLimit] values
	lastFrame=frameLimit;
	currentFrameLimit=frameLimit;

	// Set [frameStart] initial value
	timeBeginPeriod(1);
	frameStart=GetTickCount();

	// Mark [setup] as `true`
	setup=true;

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pUpdate()
* | .     |
* |     . | In:
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Additional Description:
* This function updates all [window] structures and [przecinek]
* global variable values. Firstly it updates debug main
* [window] [ID], then it refreshes input and updates all
* [przecinek] stuff. After that it loads signals from main
* debug window thread and modifies several debug values.
* In the end it stops main loop for a short amount of time and
* updates all frame related stuff.
****************************************************************/
uint8_t pUpdate(){
	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pUpdate() Error]\n");
		printf("Could not handle update!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Return `1`, finished with error
		return 1;
	}

	// Check if [windowMainID] is real
	if(build[windowMainID].exists==false && windowCount!=0){
		for(uint8_t current=0; current<WINDOW_MAX; current+=1){
			if(build[windowMainID].exists==true){
				// Update [windowMainID] value
				windowMainID=current;

				break;
			}
		}
	}

	for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
		// Update [keyboard] values from `1` to `2`
		if(keyboard[input[button]]==1){ keyboard[input[button]]=2; }
	}

	// Check [przecinek] [frameLimit] value
	if(przecinek.frameLimit<PRZECINEK_FRAME_MIN){
		if(przecinek.debug==true){
			printf("[pUpdate() Warning]\n");
			printf("Value of the przecinek.frameLimit is too small!\n");
			printf(
				"Value of the przecinek.frameLimit was changed from %i to %i,\n",
				przecinek.frameLimit, PRZECINEK_FRAME_MIN
			);
			fflush(stdout);
		}

		// Update [przecinek] [frameLimit] value
		przecinek.frameLimit=PRZECINEK_FRAME_MIN;
	}
	else if(przecinek.frameLimit>PRZECINEK_FRAME_MAX){
		if(przecinek.debug==true){
			printf("[pUpdate() Warning]\n");
			printf("Value of the przecinek.frameLimit is too large!\n");
			printf(
				"Value of the przecinek.frameLimit was changed from %i to %i,\n",
				przecinek.frameLimit, PRZECINEK_FRAME_MAX
			);
			fflush(stdout);
		}

		// Update [przecinek] [frameLimit] value
		przecinek.frameLimit=PRZECINEK_FRAME_MAX;
	}

	if(lastFrame!=przecinek.frameLimit){
		// Update [lastFrame] value
		lastFrame=przecinek.frameLimit;

		// Set [currentFrameLimit] value to [przecinek] [frameLimit]
		currentFrameLimit=przecinek.frameLimit;
	}

	// Update [przecinek] [display] values
	przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
	przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

	// Create local [point]
	POINT point;

	// Load local [point] position
	if(GetCursorPos(&point)){
		// Update [przecinek] [cursor] values
		przecinek.cursor.x=point.x;
		przecinek.cursor.y=point.y;
	}
	else{
		// Update [przecinek] [cursor] values
		przecinek.cursor.x=0;
		przecinek.cursor.y=0;
	}

	for(uint16_t ID=0; ID<WINDOW_MAX; ID+=1){
		if(build[ID].exists==true){
			// Create and load local [message]
			MSG message;
			while(PeekMessageW(&message, build[ID].HWND, 0, 0, PM_REMOVE)!=0){
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			// Manage [build] [DESTROY] event
			if(build[ID].DESTROY==true){
				// Reset [build]
				pDebugBuildReset(ID);

				continue;
			}

			// Create local [windowRectangle] and [fixRectangle]
			RECT windowRectangle, fixRectangle;

			// Calculate [build] real offset using local [windowRectangle] and [fixRectangle]
			GetWindowRect(build[ID].HWND, &windowRectangle);
			GetClientRect(build[ID].HWND, &fixRectangle);

			if(build[ID].fullScreen==false){
				// Calculate [build] [widthFix] and [heightFix] values
				build[ID].widthFix=
					(windowRectangle.right-windowRectangle.left)-(fixRectangle.right-fixRectangle.left);
				build[ID].heightFix=
					(windowRectangle.bottom-windowRectangle.top)-(fixRectangle.bottom-fixRectangle.top);
			}

			if(*build[ID].fullScreenPoint==true && build[ID].fullScreen==false){
				// Backup [build] parameters
				build[ID].xBac=*build[ID].xPoint;
				build[ID].yBac=*build[ID].yPoint;
				build[ID].widthBac=build[ID].width;
				build[ID].heightBac=build[ID].height;

				// Update [build] [widthPoint] and [heightPoint] values
				*build[ID].widthPoint=przecinek.display.width;
				*build[ID].heightPoint=przecinek.display.height;

				// Update [build] [width] and [height] values
				build[ID].width=przecinek.display.width;
				build[ID].height=przecinek.display.height;

				// Reset [build] [xPoint] and [yPoint]
				*build[ID].xPoint=0;
				*build[ID].yPoint=0;

				// Reset [build] [x] and [y]
				build[ID].x=0;
				build[ID].y=0;

				// Update [build] [HWND] mode
				SetWindowLong(build[ID].HWND, GWL_STYLE, WS_POPUP | WS_VISIBLE);

				// Resize and move [build] [HWND]
				SetWindowPos(
					build[ID].HWND, HWND_TOP, 0, 0,
					przecinek.display.width, przecinek.display.height,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED
				);

				// Update [build] [buffer]
				glViewport(0, 0, przecinek.display.width, przecinek.display.height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0, przecinek.display.width, przecinek.display.height, 0, (-1), 1);

				// Mark [build] [fullScreen] as `true`
				build[ID].fullScreen=true;
			}
			else if(*build[ID].fullScreenPoint==false && build[ID].fullScreen==true){
				// Update [build] [widthPoint] and [heightPoint] values to the backuped ones
				*build[ID].widthPoint=build[ID].widthBac+build[ID].widthFix;
				*build[ID].heightPoint=build[ID].heightBac+build[ID].heightFix;

				// Update [build] [width] and [height] values to the backuped ones
				build[ID].width=build[ID].widthBac+build[ID].widthFix;
				build[ID].height=build[ID].heightBac+build[ID].heightFix;

				// Update [build] [xPoint] and [yPoint] values to the backuped ones
				*build[ID].xPoint=build[ID].xBac-(build[ID].widthFix/2);
				*build[ID].yPoint=build[ID].yBac-build[ID].heightFix+(build[ID].widthFix/2);

				// Update [build] [x] and [y] values to the backuped ones
				build[ID].x=build[ID].xBac-(build[ID].widthFix/2);
				build[ID].y=build[ID].yBac-build[ID].heightFix+(build[ID].widthFix/2);

				// Correct [build] [x] and [y] values
				if(*build[ID].xPoint<0){ build[ID].x+=USHRT_MAX; }
				if(*build[ID].yPoint<0){ build[ID].y+=USHRT_MAX; }

				// Create local [style]
				uint32_t style;

				// Set local [style] value
				if(build[ID].resizable==true){ style=WS_OVERLAPPEDWINDOW | WS_VISIBLE; }
				else{
					style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
				}

				// Update [build] [HWND] mode to the local [style]
				SetWindowLong(build[ID].HWND, GWL_STYLE, style);
				ShowWindow(build[ID].HWND, SW_RESTORE);

				// Resize and move [build] [HWND]
				SetWindowPos(
					build[ID].HWND, NULL, *build[ID].xPoint, *build[ID].yPoint,
					*build[ID].widthPoint, *build[ID].heightPoint,
					SWP_NOZORDER | SWP_FRAMECHANGED
				);

				// Update [build] [x] and [y] values
				build[ID].x=*build[ID].xPoint;
				build[ID].y=*build[ID].yPoint;

				// Update [build] [buffer]
				glViewport(0, 0, *build[ID].widthPoint, *build[ID].heightPoint);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0, *build[ID].widthPoint, *build[ID].heightPoint, 0, (-1), 1);

				// Mark [build] [fullScreen] as `false`
				build[ID].fullScreen=false;
			}

			if(build[ID].fullScreen==false){
				if(*build[ID].xPoint!=build[ID].x || *build[ID].yPoint!=build[ID].y){
					// Update [build] [x] and [y] values
					build[ID].x=*build[ID].xPoint;
					build[ID].y=*build[ID].yPoint;

					// Move [build] [HWND]
					SetWindowPos(
						build[ID].HWND, NULL, *build[ID].xPoint, *build[ID].yPoint,
						0, 0, SWP_NOZORDER | SWP_NOSIZE
					);
				}
				else if(build[ID].MOVE.x!=0 || build[ID].MOVE.y!=0){
					// Update [build] [xPoint] and [yPoint] to [MOVE] event values
					*build[ID].xPoint=build[ID].MOVE.x;
					*build[ID].yPoint=build[ID].MOVE.y;

					// Update [build] [x] and [y] to [MOVE] event values
					build[ID].x=build[ID].MOVE.x;
					build[ID].y=build[ID].MOVE.y;
				}

				// Reset [build] [MOVE] values
				build[ID].MOVE.x=0;
				build[ID].MOVE.y=0;

				if(*build[ID].widthPoint!=build[ID].width || *build[ID].heightPoint!=build[ID].height){
					// Check [build] [widthPoint] value
					if(*build[ID].widthPoint<WINDOW_WIDTH_MIN){
						if(przecinek.debug==true){
							printf("[pUpdate() Warning]\n");
							printf("Value of the window.width is too small!\n");
							printf(
								"Value of the window.width was changed from %i to %i,\n",
								*build[ID].widthPoint, WINDOW_WIDTH_MIN
							);
							fflush(stdout);
						}

						// Change [build] [widthPoint] value
						*build[ID].widthPoint=WINDOW_WIDTH_MIN;
					}
					else if(*build[ID].widthPoint>WINDOW_WIDTH_MAX){
						if(przecinek.debug==true){
							printf("[pUpdate() Warning]\n");
							printf("Value of the window.width is too large!\n");
							printf(
								"Value of the window.width was changed from %i to %i,\n",
								*build[ID].widthPoint, WINDOW_WIDTH_MAX
							);
							fflush(stdout);
						}

						// Change [build] [widthPoint] value
						*build[ID].widthPoint=WINDOW_WIDTH_MAX;
					}

					// Check [build] [heightPoint] value
					if(*build[ID].heightPoint<WINDOW_HEIGHT_MIN){
						if(przecinek.debug==true){
							printf("[pUpdate() Warning]\n");
							printf("Value of the window.height is too small!\n");
							printf(
								"Value of the window.height was changed from %i to %i,\n",
								*build[ID].heightPoint, WINDOW_HEIGHT_MIN
							);
							fflush(stdout);
						}

						// Change [build] [heightPoint] value
						*build[ID].heightPoint=WINDOW_HEIGHT_MIN;
					}
					else if(*build[ID].heightPoint>WINDOW_HEIGHT_MAX){
						if(przecinek.debug==true){
							printf("[pUpdate() Warning]\n");
							printf("Value of the window.height is too large!\n");
							printf(
								"Value of the window.height was changed from %i to %i,\n",
								*build[ID].heightPoint, WINDOW_HEIGHT_MAX
							);
							fflush(stdout);
						}

						// Change [build] [heightPoint] value
						*build[ID].heightPoint=WINDOW_HEIGHT_MAX;
					}

					// Update [build] [width] and [height] values
					build[ID].width=*build[ID].widthPoint;
					build[ID].height=*build[ID].heightPoint;

					// Resize [build] [HWND]
					SetWindowPos(
						build[ID].HWND, NULL, 0, 0,
						*build[ID].widthPoint, *build[ID].heightPoint,
						SWP_NOZORDER | SWP_NOMOVE
					);

					// Update [build] [buffer]
					glViewport(0, 0, *build[ID].widthPoint, *build[ID].heightPoint);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0, *build[ID].widthPoint, *build[ID].heightPoint, 0, (-1), 1);
				}
				else if(build[ID].SIZE.width!=0 || build[ID].SIZE.height!=0){
					// Update [build] [widthPoint] and [heightPoint] to [SIZE] event values
					*build[ID].widthPoint=build[ID].SIZE.width;
					*build[ID].heightPoint=build[ID].SIZE.height;

					// Update [build] [width] and [height] to [SIZE] event values
					build[ID].width=build[ID].SIZE.width;
					build[ID].height=build[ID].SIZE.height;

					// Update [build] [buffer]
					glViewport(0, 0, *build[ID].widthPoint, *build[ID].heightPoint);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0, *build[ID].widthPoint, *build[ID].heightPoint, 0, (-1), 1);
				}

				// Reset [build] [SIZE] values
				build[ID].SIZE.width=0;
				build[ID].SIZE.height=0;
			}

			if(build[ID].resizable==true){
				// Update [build] [widthMin] and [heightMin] values
				build[ID].widthMin=*build[ID].widthMinPoint;
				build[ID].heightMin=*build[ID].heightMinPoint;

				// Update [build] [widthMax] and [heightMax] values
				build[ID].widthMax=*build[ID].widthMaxPoint;
				build[ID].heightMax=*build[ID].heightMaxPoint;

				// Check [build] [widthMin] value
				if(build[ID].widthMin>build[ID].widthMax){
					if(przecinek.debug==true){
						printf("[pUpdate() Warning]\n");
						printf("Value of the window.widthMin is larger than window.widthMax!\n");
						printf(
							"Value of the window.widthMin was changed from %i to %i,\n",
							build[ID].widthMin, build[ID].widthMax
						);
						fflush(stdout);
					}

					// Change [build] [widthMinPoint] and [widthMin] values
					*build[ID].widthMinPoint=*build[ID].widthMaxPoint;
					build[ID].widthMin=build[ID].widthMax;
				}

				// Check [build] [heightMin] value
				if(build[ID].heightMin>build[ID].heightMax){
					if(przecinek.debug==true){
						printf("[pUpdate() Warning]\n");
						printf("Value of the window.heightMin is larger than window.heightMax!\n");
						printf(
							"Value of the window.heightMin was changed from %i to %i,\n",
							build[ID].heightMin, build[ID].heightMax
						);
						fflush(stdout);
					}

					// Change [build] [heightMinPoint] and [heightMin] values
					*build[ID].heightMinPoint=*build[ID].heightMaxPoint;
					build[ID].heightMin=build[ID].heightMax;
				}

				if(build[ID].widthMin>build[ID].width || build[ID].widthMax<build[ID].width ||
						build[ID].heightMin>build[ID].height || build[ID].heightMax<build[ID].height){

					if(build[ID].widthMin>build[ID].width){
						// Correct [build] [width] and [widthPoint] values
						build[ID].width=build[ID].widthMin;
						*build[ID].widthPoint=build[ID].widthMin;
					}
					else if(build[ID].widthMax<build[ID].width){
						// Correct [build] [width] and [widthPoint] values
						build[ID].width=build[ID].widthMax;
						*build[ID].widthPoint=build[ID].widthMax;
					}

					if(build[ID].heightMin>build[ID].height){
						// Correct [build] [height] and [heightPoint] values
						build[ID].height=build[ID].heightMin;
						*build[ID].heightPoint=build[ID].heightMin;
					}
					else if(build[ID].heightMax<build[ID].height){
						// Correct [build] [height] and [heightPoint] values
						build[ID].height=build[ID].heightMax;
						*build[ID].heightPoint=build[ID].heightMax;
					}

					// Resize [build] [HWND]
					SetWindowPos(
						build[ID].HWND, NULL, 0, 0,
						build[ID].width, build[ID].height, SWP_NOZORDER | SWP_NOMOVE
					);

					// Update [build] [buffer]
					glViewport(0, 0, build[ID].width, build[ID].height);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0, build[ID].width, build[ID].height, 0, (-1), 1);

					// Reset [build] [SIZE] values
					build[ID].SIZE.width=0;
					build[ID].SIZE.height=0;
				}
			}

			// Manage [build] [FOCUSIN] event
			if(build[ID].FOCUSIN==true){
				// Mark [build] [focus] as `true`
				build[ID].focus=true;

				// Mark [build] [FOCUSIN] as `false`
				build[ID].FOCUSIN=false;
			}

			// Manage [build] [FOCUSOUT] event
			if(build[ID].FOCUSOUT==true){
				// Mark [build] [focus] as `false`
				build[ID].focus=false;

				// Mark [build] [FOCUSOUT] as `false`
				build[ID].FOCUSOUT=false;
			}

			// Set current [build] [buffer] for [DHDC]
			wglMakeCurrent(build[ID].DHDC, build[ID].buffer);

			// Switch [build] buffers using [DHDC]
			SwapBuffers(build[ID].DHDC);

			if(wcscmp(*build[ID].titlePoint, build[ID].title)!=0){
				if(wcslen(*build[ID].titlePoint)<WINDOW_TITLE_LENGTH_MAX-1){
					// Safely copy [build] [titlePoint] to [title]
					wmemcpy(build[ID].title, *build[ID].titlePoint, wcslen(*build[ID].titlePoint));
					build[ID].title[wcslen(*build[ID].titlePoint)]=L'\0';
				}
				else{
					if(przecinek.debug==true){
						printf("[pUpdate() Warning]\n");
						printf("Value of the window.title is too long!\n");
						printf(
							"Value of the window.title was shorten from %i to %i chars,\n",
							wcslen(*build[ID].titlePoint), WINDOW_TITLE_LENGTH_MAX-1
						);
						fflush(stdout);
					}

					// Safely copy [build] [titlePoint] to [title]
					wmemcpy(*build[ID].titlePoint, *build[ID].titlePoint, WINDOW_TITLE_LENGTH_MAX-1);
					*build[ID].titlePoint[WINDOW_TITLE_LENGTH_MAX-1]=L'\0';

					// Update [build] [titlePoint]
					wcscpy(*build[ID].titlePoint, build[ID].title);
				}

				// Set [build] [titlePoint] for [HWND]
				SetWindowTextW(build[ID].HWND, *build[ID].titlePoint);
			}

			// Update [build] [focusPoint]
			*build[ID].focusPoint=build[ID].focus;

			// Refresh [build] [HWND]
			InvalidateRect(build[ID].HWND, NULL, TRUE);
		}
	}

	// Update [frameCount] by `1`
	frameCount+=1;
	// Sleep for short amount of time
	Sleep(1000/currentFrameLimit);

	// Calculate [frameOverhead] value
	if(frameCount>=przecinek.frameLimit+3 && GetTickCount()-frameStart<1000){ frameOverhead+=1; }
	else if(GetTickCount()-frameStart>=1000 && frameCount<przecinek.frameLimit-5){
		frameOverhead=frameCount-przecinek.frameLimit;
	}

	if(GetTickCount()-frameStart>=1000){
		// Update [lastFrameCount] value
		lastFrameCount=frameCount;

		// Reset [frameCount] and update [frameStart]
		frameCount=0;
		frameStart=GetTickCount();

		// Correct [currentFrameLimit] value
		currentFrameLimit-=frameOverhead;
		if(currentFrameLimit>=przecinek.frameLimit*3){ currentFrameLimit=przecinek.frameLimit*3; }

		// Reset [frameOverhead] value
		frameOverhead=0;
	}

	// Update [przecinek] [frameCount] value
	przecinek.frameCount=lastFrameCount;

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pClear()
* | .     |
* |     . | In:
* \ = , = / Out:
*
* Additional Description:
* This function clears and refreshes the debug console. It uses
* `cls` or `clear` depending on the operating system.
****************************************************************/
void pClear(){
	// Clear console
	system("cls");
	fflush(stdout);

	return;
}

/****************************************************************
* |\_____/| pDebugKeySearch()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
uint16_t pDebugKeySearch(wchar_t *key){
	if(wcscmp(key, L"LMOUSE")==0 || wcscmp(key, L"LMouse")==0 || wcscmp(key, L"lmouse")==0){ return VK_LBUTTON; }
	if(wcscmp(key, L"MMOUSE")==0 || wcscmp(key, L"MMouse")==0 || wcscmp(key, L"mmouse")==0){ return VK_MBUTTON; }
	if(wcscmp(key, L"RMOUSE")==0 || wcscmp(key, L"RMouse")==0 || wcscmp(key, L"rmouse")==0){ return VK_RBUTTON; }
	if(wcscmp(key, L"BACK")==0 || wcscmp(key, L"Back")==0 || wcscmp(key, L"back")==0){ return VK_XBUTTON1; }
	if(wcscmp(key, L"FORWARD")==0 || wcscmp(key, L"Forward")==0 || wcscmp(key, L"forward")==0){ return VK_XBUTTON2; }

	if(wcscmp(key, L"ESC")==0 || wcscmp(key, L"esc")==0 || wcscmp(key, L"Esc")==0){
		return MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"TAB")==0 || wcscmp(key, L"tab")==0 || wcscmp(key, L"Tab")==0){
		return MapVirtualKey(VK_TAB, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"LSHIFT")==0 || wcscmp(key, L"lshift")==0 || wcscmp(key, L"LShift")==0){
		return MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"LCTRL")==0 || wcscmp(key, L"lctrl")==0 || wcscmp(key, L"LCtrl")==0){
		return MapVirtualKey(VK_LCONTROL, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"LMOD")==0 || wcscmp(key, L"lmod")==0 || wcscmp(key, L"LMod")==0){
		return MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"LALT")==0 || wcscmp(key, L"lalt")==0 || wcscmp(key, L"LAlt")==0){
		return MapVirtualKey(VK_LMENU, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"SPACE")==0 || wcscmp(key, L"space")==0 || wcscmp(key, L"Space")==0){
		return MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"F1")==0 || wcscmp(key, L"f1")==0){ return MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F2")==0 || wcscmp(key, L"f2")==0){ return MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F3")==0 || wcscmp(key, L"f3")==0){ return MapVirtualKey(VK_F3, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F4")==0 || wcscmp(key, L"f4")==0){ return MapVirtualKey(VK_F4, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F5")==0 || wcscmp(key, L"f5")==0){ return MapVirtualKey(VK_F5, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F6")==0 || wcscmp(key, L"f6")==0){ return MapVirtualKey(VK_F6, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F7")==0 || wcscmp(key, L"f7")==0){ return MapVirtualKey(VK_F7, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F8")==0 || wcscmp(key, L"f8")==0){ return MapVirtualKey(VK_F8, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F9")==0 || wcscmp(key, L"f9")==0){ return MapVirtualKey(VK_F9, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F10")==0 || wcscmp(key, L"f10")==0){ return MapVirtualKey(VK_F10, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F11")==0 || wcscmp(key, L"f11")==0){ return MapVirtualKey(VK_F11, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F12")==0 || wcscmp(key, L"f12")==0){ return MapVirtualKey(VK_F12, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"RALT")==0 || wcscmp(key, L"ralt")==0 || wcscmp(key, L"RAlt")==0){
		return MapVirtualKey(VK_RMENU, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"RWIN")==0 || wcscmp(key, L"rwin")==0 || wcscmp(key, L"RWin")==0){
		return MapVirtualKey(VK_RWIN, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"MENU")==0 || wcscmp(key, L"menu")==0 || wcscmp(key, L"Menu")==0){
		return MapVirtualKey(VK_APPS, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"RCTRL")==0 || wcscmp(key, L"rctrl")==0 || wcscmp(key, L"RCtrl")==0){
		return MapVirtualKey(VK_RCONTROL, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"RSHIFT")==0 || wcscmp(key, L"rshift")==0 || wcscmp(key, L"RShift")==0){
		return MapVirtualKey(VK_RSHIFT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"ENTER")==0 || wcscmp(key, L"enter")==0 || wcscmp(key, L"Enter")==0){
		return MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"BACKSPACE")==0 || wcscmp(key, L"backspace")==0 || wcscmp(key, L"Backspace")==0){
		return MapVirtualKey(VK_BACK, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"LARROW")==0 || wcscmp(key, L"larrow")==0 || wcscmp(key, L"LArrow")==0){
		return MapVirtualKey(VK_LEFT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"DARROW")==0 || wcscmp(key, L"darrow")==0 || wcscmp(key, L"DArrow")==0){
		return MapVirtualKey(VK_DOWN, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"RARROW")==0 || wcscmp(key, L"rarrow")==0 || wcscmp(key, L"RArrow")==0){
		return MapVirtualKey(VK_RIGHT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"UARROW")==0 || wcscmp(key, L"uarrow")==0 || wcscmp(key, L"UArrow")==0){
		return MapVirtualKey(VK_UP, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"PRINTSCRN")==0 || wcscmp(key, L"printscrn")==0 || wcscmp(key, L"PrintScrn")==0){
		return MapVirtualKey(VK_SNAPSHOT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"PAUSEBREAK")==0 || wcscmp(key, L"pausebreak")==0 || wcscmp(key, L"PauseBreak")==0){
		return MapVirtualKey(VK_PAUSE, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"INS")==0 || wcscmp(key, L"ins")==0 || wcscmp(key, L"Ins")==0){
		return MapVirtualKey(VK_INSERT, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"HOME")==0 || wcscmp(key, L"home")==0 || wcscmp(key, L"Home")==0){
		return MapVirtualKey(VK_HOME, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"PAGEU")==0 || wcscmp(key, L"pageu")==0 || wcscmp(key, L"PageU")==0){
		return MapVirtualKey(VK_PRIOR, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"DEL")==0 || wcscmp(key, L"del")==0 || wcscmp(key, L"Del")==0){
		return MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"END")==0 || wcscmp(key, L"end")==0 || wcscmp(key, L"End")==0){
		return MapVirtualKey(VK_END, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"PAGED")==0 || wcscmp(key, L"paged")==0 || wcscmp(key, L"PageD")==0){
		return MapVirtualKey(VK_NEXT, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"Q")==0 || wcscmp(key, L"q")==0){ return MapVirtualKey(VK_Q, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"W")==0 || wcscmp(key, L"w")==0){ return MapVirtualKey(VK_W, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"E")==0 || wcscmp(key, L"e")==0){ return MapVirtualKey(VK_E, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"R")==0 || wcscmp(key, L"r")==0){ return MapVirtualKey(VK_R, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"T")==0 || wcscmp(key, L"t")==0){ return MapVirtualKey(VK_T, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"Y")==0 || wcscmp(key, L"y")==0){ return MapVirtualKey(VK_Y, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"U")==0 || wcscmp(key, L"u")==0){ return MapVirtualKey(VK_U, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"I")==0 || wcscmp(key, L"i")==0){ return MapVirtualKey(VK_I, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"O")==0 || wcscmp(key, L"o")==0){ return MapVirtualKey(VK_O, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"P")==0 || wcscmp(key, L"p")==0){ return MapVirtualKey(VK_P, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"A")==0 || wcscmp(key, L"a")==0){ return MapVirtualKey(VK_A, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"S")==0 || wcscmp(key, L"s")==0){ return MapVirtualKey(VK_S, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"D")==0 || wcscmp(key, L"d")==0){ return MapVirtualKey(VK_D, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"F")==0 || wcscmp(key, L"f")==0){ return MapVirtualKey(VK_F, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"G")==0 || wcscmp(key, L"g")==0){ return MapVirtualKey(VK_G, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"H")==0 || wcscmp(key, L"h")==0){ return MapVirtualKey(VK_H, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"J")==0 || wcscmp(key, L"j")==0){ return MapVirtualKey(VK_J, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"K")==0 || wcscmp(key, L"k")==0){ return MapVirtualKey(VK_K, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"L")==0 || wcscmp(key, L"l")==0){ return MapVirtualKey(VK_L, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"Z")==0 || wcscmp(key, L"z")==0){ return MapVirtualKey(VK_Z, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"X")==0 || wcscmp(key, L"x")==0){ return MapVirtualKey(VK_X, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"C")==0 || wcscmp(key, L"c")==0){ return MapVirtualKey(VK_C, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"V")==0 || wcscmp(key, L"v")==0){ return MapVirtualKey(VK_V, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"B")==0 || wcscmp(key, L"b")==0){ return MapVirtualKey(VK_B, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"N")==0 || wcscmp(key, L"n")==0){ return MapVirtualKey(VK_N, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"M")==0 || wcscmp(key, L"m")==0){ return MapVirtualKey(VK_M, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"1")==0){ return MapVirtualKey(VK_1, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"2")==0){ return MapVirtualKey(VK_2, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"3")==0){ return MapVirtualKey(VK_3, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"4")==0){ return MapVirtualKey(VK_4, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"5")==0){ return MapVirtualKey(VK_5, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"6")==0){ return MapVirtualKey(VK_6, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"7")==0){ return MapVirtualKey(VK_7, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"8")==0){ return MapVirtualKey(VK_8, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"9")==0){ return MapVirtualKey(VK_9, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"0")==0){ return MapVirtualKey(VK_0, MAPVK_VK_TO_VSC); }

	else if(wcscmp(key, L"`")==0){ return MapVirtualKey(VK_OEM_3, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L",")==0){ return MapVirtualKey(VK_OEM_COMMA, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L".")==0){ return MapVirtualKey(VK_OEM_PERIOD, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"/")==0){ return MapVirtualKey(VK_OEM_2, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L";")==0){ return MapVirtualKey(VK_OEM_SEMICOLON, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"'")==0){ return MapVirtualKey(VK_OEM_QUOTE, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"\\")==0){ return MapVirtualKey(VK_OEM_5, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"[")==0){ return MapVirtualKey(VK_OEM_4, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"]")==0){ return MapVirtualKey(VK_OEM_6, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"-")==0){ return MapVirtualKey(VK_OEM_MINUS, MAPVK_VK_TO_VSC); }
	else if(wcscmp(key, L"=")==0){ return MapVirtualKey(VK_OEM_PLUS, MAPVK_VK_TO_VSC); }

	// Return `0`, key code was not found
	return 0;
}

/****************************************************************
* |\_____/| pKeyPress()
* | .     |
* |     . | In: wchar_t* [key]
* \ = , = / Out: bool (`true` -> pressed)
*
* Parameters:
* [key] - which [key] status should be checked.
*
* Additional Description:
* This function checks if [key] is being pressed. Returned
* value will be `true` only for one frame. Before [key] is
* being checked this function searches for debug [key] code.
* Debug code depends on the operating system.
****************************************************************/
bool pKeyPress(wchar_t *key){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pKeyPress() Error]\n");
			printf("Could not check key status!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	// Create local [convert] and search for [key]
	uint16_t convert=pDebugKeySearch(key);

	if(convert==0){
		if(przecinek.debug==true){
			printf("[pKeyPress() Error]\n");
			printf("Could not find key value!\n");
			printf("Check if the key value is correct and if it exists,\n");
			fflush(stdout);
		}

		// Return `false`, key not found
		return false;
	}

	// Return [keyboard] equal to `1` status
	return keyboard[convert]==1;
}

/****************************************************************
* |\_____/| pKeyHold()
* | .     |
* |     . | In: wchar_t* [key]
* \ = , = / Out: bool (`true` -> hold)
*
* Parameters:
* [key] - which [key] status should be checked.
*
* Additional Description:
* This function checks if [key] is being hold. Returned
* value will be `true` after the first frame. Before [key] is
* being checked this function searches for debug [key] code.
* Debug code depends on the operating system.
****************************************************************/
bool pKeyHold(wchar_t *key){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pKeyHold() Error]\n");
			printf("Could not check key status!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	// Create local [convert] and search for [key]
	uint16_t convert=pDebugKeySearch(key);

	if(convert==0){
		if(przecinek.debug==true){
			printf("[pKeyHold() Error]\n");
			printf("Could not find key value!\n");
			printf("Check if the key value is correct and if it exists,\n");
			fflush(stdout);
		}

		// Return `false`, key not found
		return false;
	}

	// Return [keyboard] equal to `2` status
	return keyboard[convert]==2;
}

/****************************************************************
* |\_____/| pKeyCaps()
* | .     |
* |     . | In:
* \ = , = / Out: bool (`true` -> activate)
*
* Additional Description:
* This function returns Caps Lock key status.
****************************************************************/
bool pKeyCaps(){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pKeyCaps() Error]\n");
			printf("Could not check key status!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	// Return caps lock status
	return (GetKeyState(VK_CAPITAL)&0x0001)!=0;
}

/****************************************************************
* |\_____/| pKeyNum()
* | .     |
* |     . | In:
* \ = , = / Out: bool (`true` -> activate)
*
* Additional Description:
* This function returns Num Lock key status.
****************************************************************/
bool pKeyNum(){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pKeyNum() Error]\n");
			printf("Could not check key status!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	// Return num lock status
	return (GetKeyState(VK_NUMLOCK)&0x0001)!=0;
}

/****************************************************************
* |\_____/| pKeyScroll()
* | .     |
* |     . | In:
* \ = , = / Out: bool (`true` -> activate)
*
* Additional Description:
* This function returns Scroll Lock key status.
****************************************************************/
bool pKeyScroll(){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pKeyScroll() Error]\n");
			printf("Could not check key status!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	// Return scroll lock status
	return (GetKeyState(VK_SCROLL)&0x0001)!=0;
}

/****************************************************************
* |\_____/| pEndup()
* | .     |
* |     . | In:
* \ = , = / Out:
*
* Additional Description:
* This function safely destroys structures and cleans memory
* before the end of the program. It firstly uninitializes all
* debug libraries. Then it searches for all undestroyed
* structures and removes them. At the end it resets [przecinek]
* and several debug variables.
****************************************************************/
void pEndup(){
	// End GDI+ session
	GdiplusShutdown(tokenGDI);

	// Clean up [graphics], [DHDC] and [stringFormat]
	DeleteObject(graphics);
	ReleaseDC(NULL, DHDC);
	GdipDeleteStringFormat(stringFormat);

	// Reset all existing structures
	for(uint8_t ID=0; ID<WINDOW_MAX; ID+=1){
		if(build[ID].exists==true){ pDebugBuildReset(ID); }
	}
	for(uint16_t ID=0; ID<OBJECT_MAX; ID+=1){
		if(figure[ID].exists==true){ pDebugFigureReset(ID); }
	}
	for(uint16_t ID=0; ID<FONT_MAX; ID+=1){
		if(view[ID].exists==true){ pDebugViewReset(ID); }
	}
	for(uint16_t ID=0; ID<TEXT_MAX; ID+=1){
		if(code[ID].exists==true){ pDebugCodeReset(ID); }
	}
	for(uint16_t ID=0; ID<IMAGE_MAX; ID+=1){
		if(texture[ID].exists==true){ pDebugTextureReset(ID); }
	}
	for(uint16_t ID=0; ID<AUDIO_MAX; ID+=1){
		if(sound[ID].exists==true){ pDebugSoundReset(ID); }
	}

	// Reset [przecinek] [frameLimit] and [frameCount] values
	przecinek.frameLimit=0;
	przecinek.frameCount=0;

	// Reset [przecinek] [display] values
	przecinek.display.width=0;
	przecinek.display.height=0;

	// Reset [przecinek] [cursor] values
	przecinek.cursor.x=0;
	przecinek.cursor.x=0;

	// Fill [input] and [keyboard] with `0`
	memset(input, 0, sizeof(input));
	memset(keyboard, 0, sizeof(keyboard));

	// Mark [setup] as `false`
	setup=false;

	return;
}

/****************************************************************
* |\_____/| pDebugBuildReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugBuildReset(uint8_t ID){
	// Clean up [build] [title] value
	if(build[ID].title!=NULL){
		free(build[ID].title);
		build[ID].title=NULL;
	}

	// Clean up [build] [DHDC] and [buffer] structures
	if(build[ID].DHDC!=NULL){
		wglMakeCurrent(build[ID].DHDC, build[ID].buffer);
		wglDeleteContext(build[ID].buffer);
		build[ID].buffer=NULL;
		DeleteDC(build[ID].DHDC);
		build[ID].DHDC=NULL;
	}
	// Clean up [build] [HWND] structure
	if(build[ID].HWND!=NULL){
		DestroyWindow(build[ID].HWND);
		build[ID].HWND=NULL;
	}

	// Mark [build] [exists] as `false`
	build[ID].exists=false;

	// Change [windowCount] value by `-1`
	windowCount-=1;

	return;
}

/****************************************************************
* |\_____/| pDebugWindowReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugWindowReset(pWindow *window){
	// Reset [window] [x] and [y] values
	window->x=0;
	window->y=0;

	// Reset [window] [width] and [height] values
	window->width=0;
	window->height=0;

	// Reset [window] [widthMin] and [heightMin] values
	window->widthMin=0;
	window->heightMin=0;
	// Reset [window] [widthMax] and [heightMax] values
	window->widthMax=0;
	window->heightMax=0;

	// Clean up [window] [title] value
	if(window->title!=NULL){
		free(window->title);
		window->title=NULL;
	}

	// Mark [window] [focus] and [fullScreen] as `false`
	window->focus=false;
	window->fullScreen=false;

	return;
}

/****************************************************************
* |\_____/| pDebugBuildProc()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
LRESULT CALLBACK pDebugBuildProc(HWND HWND, UINT uMessage, WPARAM wParameter, LPARAM lParameter){
	// Create and load local [windowPoint]
	windowPoint=(pWindowPoint*)GetWindowLongPtr(HWND, GWLP_USERDATA);

	// Manage `WM_CREATE` signal
	if(uMessage==WM_CREATE){
		// Create and setup local [newWindowPoint]
		newWindowPoint=(pWindowPoint*)malloc(sizeof(pWindowPoint));
		newWindowPoint->ID=windowCreateID;
		newWindowPoint->HWND=HWND;

		// Apply [newWindowPoint]
		SetWindowLongPtr(HWND, GWLP_USERDATA, (LONG_PTR)newWindowPoint);
	}

	// Manage `WM_DESTROY` signal
	if(uMessage==WM_DESTROY){
		// Send [build] [DESTROY] signal
		build[windowPoint->ID].DESTROY=true;

		// Set [HWND] value to `NULL`
		HWND=NULL;

		// Return `0`, finished with close event
		return 0;
	}

	// Manage `WM_WINDOWPOSCHANGING` signal
	if(uMessage==WM_WINDOWPOSCHANGING){
		// Create local [limit] and load value from [lParameter]
		WINDOWPOS *limit=(WINDOWPOS*)lParameter;

		if(build[windowPoint->ID].resizable==true){
			// Check [build] [widthMin] and [heightMin] value
			if(limit->cx<build[windowPoint->ID].widthMin+build[windowPoint->ID].widthFix){
				limit->cx=build[windowPoint->ID].widthMin+build[windowPoint->ID].widthFix;
			}
			if(limit->cy<build[windowPoint->ID].heightMin+build[windowPoint->ID].heightFix){
				limit->cy=build[windowPoint->ID].heightMin+build[windowPoint->ID].heightFix;
			}

			// Check [build] [widthMax] and [heightMax] value
			if(limit->cx>build[windowPoint->ID].widthMax+build[windowPoint->ID].widthFix){
				limit->cx=build[windowPoint->ID].widthMax+build[windowPoint->ID].widthFix;
			}
			if(limit->cy>build[windowPoint->ID].heightMax+build[windowPoint->ID].heightFix){
				limit->cy=build[windowPoint->ID].heightMax+build[windowPoint->ID].heightFix;
			}
		}
	}

	// Manage `WM_ACTIVATE` signal
	if(uMessage==WM_ACTIVATE){
		if(wParameter==WA_ACTIVE || wParameter==WA_CLICKACTIVE){
			// Send [build] [FOCUSIN] signal
			build[windowPoint->ID].FOCUSIN=true;
		}
		else{
			// Send [build] [FOCUSOUT] signal
			build[windowPoint->ID].FOCUSOUT=true;
		}
	}

	// Manage [window] position change signal
	if(uMessage==WM_MOVE){
		// Send [build] [MOVE] signal
		build[windowPoint->ID].MOVE.x=LOWORD(lParameter);
		build[windowPoint->ID].MOVE.y=HIWORD(lParameter);

		// Correct [build] [MOVE] signal values
		while(build[windowPoint->ID].MOVE.x>SHRT_MAX+1){ build[windowPoint->ID].MOVE.x-=(USHRT_MAX+1); }
		while(build[windowPoint->ID].MOVE.y>SHRT_MAX+1){ build[windowPoint->ID].MOVE.y-=(USHRT_MAX+1); }
	}

	// Manage `WM_SIZE` signal
	if(uMessage==WM_SIZE){
		// Refresh [HWND]
		InvalidateRect(HWND, NULL, TRUE);
		UpdateWindow(HWND);

		// Send [build] [SIZE] signal
		build[windowPoint->ID].SIZE.width=LOWORD(lParameter);
		build[windowPoint->ID].SIZE.height=HIWORD(lParameter);
	}

	// Manage `WM_DPICHANGED` signal
	if(uMessage==WM_DPICHANGED){
		// Create local [dpi] and load value from [lParameter]
		RECT *dpi=(RECT*)lParameter;

		// Send [build] [SIZE] signal
		build[windowPoint->ID].SIZE.width=dpi->right-dpi->left;
		build[windowPoint->ID].SIZE.height=dpi->bottom-dpi->top;
	}

	// Manage `WM_KEYDOWN` signal
	if(uMessage==WM_KEYDOWN){
		if(windowPoint->ID==windowMainID && keyboard[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]==0){
			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==0 || input[button]==MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)){
					// Set [input] value to the debug key code
					input[button]=MapVirtualKey(wParameter, MAPVK_VK_TO_VSC);

					// Set [keyboard] value to `1`
					keyboard[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=1;

					break;
				}
			}
		}
	}
	// Manage `WM_KEYUP` signal
	if(uMessage==WM_KEYUP){
		if(windowPoint->ID==windowMainID){
			// Set [keyboard] value to `0`
			keyboard[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=0;

			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)){
					while(button!=PRZECINEK_KEY_PRESS_MAX){
						// Move [input] value by `1` position
						input[button]=input[button+1];

						// Modify [button] value by `1`
						button+=1;
					}

					// Set [input] value to `0`
					input[PRZECINEK_KEY_PRESS_MAX]=0;
				}
			}
		}
	}

	// Manage `WM_LBUTTONDOWN` signal
	if(uMessage==WM_LBUTTONDOWN){
		if(windowPoint->ID==windowMainID && keyboard[VK_LBUTTON]==0){
			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==0 || input[button]==VK_LBUTTON){
					// Set [input] value to the debug key code
					input[button]=VK_LBUTTON;

					// Set [keyboard] value to `1`
					keyboard[VK_LBUTTON]=1;

					break;
				}
			}
		}
	}
	// Manage `WM_LBUTTONUP` signal
	if(uMessage==WM_LBUTTONUP){
		if(windowPoint->ID==windowMainID){
			// Set [keyboard] value to `0`
			keyboard[VK_LBUTTON]=0;

			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==VK_LBUTTON){
					while(button!=PRZECINEK_KEY_PRESS_MAX){
						// Move [input] value by `1` position
						input[button]=input[button+1];

						// Modify [button] value by `1`
						button+=1;
					}

					// Set [input] value to `0`
					input[PRZECINEK_KEY_PRESS_MAX]=0;
				}
			}
		}
	}

	// Manage `WM_RBUTTONDOWN` signal
	if(uMessage==WM_RBUTTONDOWN){
		if(windowPoint->ID==windowMainID && keyboard[VK_RBUTTON]==0){
			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==0 || input[button]==VK_RBUTTON){
					// Set [input] value to the debug key code
					input[button]=VK_RBUTTON;

					// Set [keyboard] value to `1`
					keyboard[VK_RBUTTON]=1;

					break;
				}
			}
		}
	}
	// Manage `WM_RBUTTONUP` signal
	if(uMessage==WM_RBUTTONUP){
		if(windowPoint->ID==windowMainID){
			// Set [keyboard] value to `0`
			keyboard[VK_RBUTTON]=0;

			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==VK_RBUTTON){
					while(button!=PRZECINEK_KEY_PRESS_MAX){
						// Move [input] value by `1` position
						input[button]=input[button+1];

						// Modify [button] value by `1`
						button+=1;
					}

					// Set [input] value to `0`
					input[PRZECINEK_KEY_PRESS_MAX]=0;
				}
			}
		}
	}

	// Manage `WM_MBUTTONDOWN` signal
	if(uMessage==WM_MBUTTONDOWN){
		if(windowPoint->ID==windowMainID && keyboard[VK_MBUTTON]==0){
			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==0 || input[button]==VK_MBUTTON){
					// Set [input] value to the debug key code
					input[button]=VK_MBUTTON;

					// Set [keyboard] value to `1`
					keyboard[VK_MBUTTON]=1;

					break;
				}
			}
		}
	}
	// Manage `WM_MBUTTONUP` signal
	if(uMessage==WM_MBUTTONUP){
		if(windowPoint->ID==windowMainID){
			// Set [keyboard] value to `0`
			keyboard[VK_MBUTTON]=0;

			for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
				if(input[button]==VK_MBUTTON){
					while(button!=PRZECINEK_KEY_PRESS_MAX){
						// Move [input] value by `1` position
						input[button]=input[button+1];

						// Modify [button] value by `1`
						button+=1;
					}

					// Set [input] value to `0`
					input[PRZECINEK_KEY_PRESS_MAX]=0;
				}
			}
		}
	}

	// Manage `WM_XBUTTONDOWN` signal
	if(uMessage==WM_XBUTTONDOWN){
		if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){
			if(windowPoint->ID==windowMainID && keyboard[VK_XBUTTON1]==0){
				for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
					if(input[button]==0 || input[button]==VK_XBUTTON1){
						// Set [input] value to the debug key code
						input[button]=VK_XBUTTON1;

						// Set [keyboard] value to `1`
						keyboard[VK_XBUTTON1]=1;

						break;
					}
				}
			}
		}
		else if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){
			if(windowPoint->ID==windowMainID && keyboard[VK_XBUTTON2]==0){
				for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
					if(input[button]==0 || input[button]==VK_XBUTTON2){
						// Set [input] value to the debug key code
						input[button]=VK_XBUTTON2;

						// Set [keyboard] value to `1`
						keyboard[VK_XBUTTON2]=1;

						break;
					}
				}
			}
		}
	}
	// Manage `WM_XBUTTONUP` signal
	if(uMessage==WM_XBUTTONUP){
		if(windowPoint->ID==windowMainID){
			if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){
				// Set [keyboard] value to `0`
				keyboard[VK_XBUTTON1]=0;

				for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
					if(input[button]==VK_XBUTTON1){
						while(button!=PRZECINEK_KEY_PRESS_MAX){
							// Move [input] value by `1` position
							input[button]=input[button+1];

							// Modify [button] value by `1`
							button+=1;
						}

						// Set [input] value to `0`
						input[PRZECINEK_KEY_PRESS_MAX]=0;
					}
				}
			}
			else if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){
				// Set [keyboard] value to `0`
				keyboard[VK_XBUTTON2]=0;

				for(uint8_t button=0; button<PRZECINEK_KEY_PRESS_MAX; button+=1){
					if(input[button]==VK_XBUTTON2){
						while(button!=PRZECINEK_KEY_PRESS_MAX){
							// Move [input] value by `1` position
							input[button]=input[button+1];

							// Modify [button] value by `1`
							button+=1;
						}

						// Set [input] value to `0`
						input[PRZECINEK_KEY_PRESS_MAX]=0;
					}
				}
			}
		}
	}

	// Return [build] proc
	return DefWindowProcW(HWND, uMessage, wParameter, lParameter);
}

/****************************************************************
* |\_____/| pWindowCreate()
* | .     | In: pWindow *window, uint16_t [width], [height],
* |     . | bool [resizable]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [window] - which [window] structure should be initialized. If
* given [window] is already created, then it will be overwritten.
* [width], [height] - initial size of a newly created [window].
* [resizable] - should the [window] be resizable or constant.
*
* Additional Description:
* This function choses [ID] for given [window], then it
* creates debug process, setups all needed values and variables,
* setups debug pointers to the [window] structure, then it
* creates physical [window] and creates second GL buffer.
* It also creates debug [window] thread which loads and
* responses to all signals.
****************************************************************/
uint8_t pWindowCreate(pWindow *window, uint16_t width, uint16_t height, bool resizable){
	if(window==NULL){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Error]\n");
			printf("Given window is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	// Change [windowCount] value
	windowCount+=1;

	for(uint8_t current=0; current<WINDOW_MAX; current+=1){
		if(build[current].exists==false){
			// Set some values to `NULL`
			window->title=NULL;
			build[current].title=NULL;

			// Set [window] [ID] and reset it
			window->ID=current;
			pDebugWindowReset(window);
			pDebugBuildReset(window->ID);

			// Change [windowCount] value
			windowCount+=1;

			break;
		}
		else if(current==WINDOW_MAX-1){
			if(przecinek.debug==true){
				printf("[pWindowCreate() Error]\n");
				printf("Too many windows were created!\n");
				printf("Current window limit is equal to %i.\n", WINDOW_MAX);
				printf("Try to destroy unused windows or change Przecinek window limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pWindowCreate() Error]\n");
		printf("Could not create window!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [window] and [build]
		pDebugWindowReset(window);
		pDebugBuildReset(window->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Check [width] value
	if(width<WINDOW_WIDTH_MIN){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Warning]\n");
			printf("Value of the window.width is too small!\n");
			printf(
				"Value of the window.width was changed from %i to %i,\n",
				width, WINDOW_WIDTH_MIN
			);
			fflush(stdout);
		}

		// Change [width] value
		width=WINDOW_WIDTH_MIN;
	}
	else if(width>WINDOW_WIDTH_MAX){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Warning]\n");
			printf("Value of the window.width is too large!\n");
			printf(
				"Value of the window.width was changed from %i to %i,\n",
				width, WINDOW_WIDTH_MAX
			);
			fflush(stdout);
		}

		// Change [width] value
		width=WINDOW_WIDTH_MAX;
	}

	// Check [height] value
	if(height<WINDOW_HEIGHT_MIN){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Warning]\n");
			printf("Value of the window.height is too small!\n");
			printf(
				"Value of the window.height was changed from %i to %i,\n",
				height, WINDOW_HEIGHT_MIN
			);
			fflush(stdout);
		}

		// Change [height] value
		height=WINDOW_HEIGHT_MIN;
	}
	else if(height>WINDOW_HEIGHT_MAX){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Warning]\n");
			printf("Value of the window.height is too large!\n");
			printf(
				"Value of the window.height was changed from %i to %i,\n",
				height, WINDOW_HEIGHT_MAX
			);
			fflush(stdout);
		}

		// Change [height] value
		height=WINDOW_HEIGHT_MAX;
	}

	// Set [window] values
	window->exists=true;

	window->x=WINDOW_X_DEF;
	window->y=WINDOW_Y_DEF;

	window->width=width;
	window->height=height;

	if(resizable==true){
		window->widthMin=WINDOW_WIDTH_MIN;
		window->heightMin=WINDOW_HEIGHT_MIN;
		window->widthMax=WINDOW_WIDTH_MAX;
		window->heightMax=WINDOW_HEIGHT_MAX;
	}
	else{
		window->widthMin=width;
		window->heightMin=height;
		window->widthMax=width;
		window->heightMax=height;
	}

	// Set [windowCreateID] value
	windowCreateID=window->ID;

	// Set [build] values
	build[window->ID].exists=true;

	build[window->ID].x=window->x;
	build[window->ID].y=window->y;
	build[window->ID].width=width;
	build[window->ID].height=height;

	build[window->ID].widthMin=window->widthMin;
	build[window->ID].heightMin=window->heightMin;
	build[window->ID].widthMax=window->widthMax;
	build[window->ID].heightMax=window->heightMax;

	build[window->ID].resizable=resizable;

	// Allocate [build] pointers
	build[window->ID].xPoint=&window->x;
	build[window->ID].yPoint=&window->y;
	build[window->ID].widthPoint=&window->width;
	build[window->ID].heightPoint=&window->height;
	build[window->ID].widthMinPoint=&window->widthMin;
	build[window->ID].heightMinPoint=&window->heightMin;
	build[window->ID].widthMaxPoint=&window->widthMax;
	build[window->ID].heightMaxPoint=&window->heightMax;

	build[window->ID].titlePoint=&window->title;

	build[window->ID].focusPoint=&window->focus;
	build[window->ID].fullScreenPoint=&window->fullScreen;

	// Create and allocate local [class]
	wchar_t *class=malloc(sizeof(wchar_t)*14);
	if(class==NULL){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [window] and [build]
		pDebugWindowReset(window);
		pDebugBuildReset(window->ID);

		// Return `4`, finished with error
		return 4;
	}

	// Set local [class] value
	swprintf(class, 14, L"pClass%hhu\0", window->ID);

	// Create and setup local [windowClass]
	WNDCLASSW windowClass={};
	windowClass.lpfnWndProc=pDebugBuildProc;
	windowClass.hInstance=GetModuleHandle(NULL);

	// Create local [found]
	uint8_t found=0;

	while(GetClassInfoW(GetModuleHandle(NULL), class, &windowClass)==true){
		// Modify [found] value
		found+=1;

		// Modify [build] [class] if it already exists
		swprintf(class, 14, L"pClass%hu\0", (uint16_t)(WINDOW_MAX*found)+window->ID);
	}

	// Finish setting and register local [windowClass]
	windowClass.lpszClassName=class;
	windowClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	if(RegisterClassW(&windowClass)==0){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Error]\n");
			printf("WinAPI could not register window class!\n");
			printf("Try to close other Przecinek instances or recompile Przecinek,\n");
			fflush(stdout);
		}

		// Reset [window] and [build]
		pDebugWindowReset(window);
		pDebugBuildReset(window->ID);

		// Return `5`, finished with error
		return 5;
	}

	// Create local [style]
	uint32_t style=0;

	// Set local [style] value
	if(resizable==true){ style=WS_OVERLAPPEDWINDOW; }
	else{ style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; }

	// Enable [build] DPI awareness
	SetProcessDPIAware();
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Create and setup local [windowRectangle]
	RECT windowRectangle=(RECT){ 0, 0, width, height };
	AdjustWindowRectEx(&windowRectangle, style, FALSE, 0);

	// Set [window] and [build] [title] values
	window->title=WINDOW_WIDE_TITLE_DEF;
	build[window->ID].title=WINDOW_WIDE_TITLE_DEF;

	// Create [HWND] for [build]
	build[window->ID].HWND=CreateWindowExW(
		0, class, window->title, style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRectangle.right-windowRectangle.left,
		windowRectangle.bottom-windowRectangle.top,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);
	if(build[window->ID].HWND==NULL){
		if(przecinek.debug==true){
			printf("[pWindowCreate() Error]\n");
			printf("WinAPI library HWND doesn't work!\n");
			printf("Try to reinstall WIN depencies or recompile Przecinek,\n");
			fflush(stdout);
		}

		// Reset [window] and [build]
		pDebugWindowReset(window);
		pDebugBuildReset(window->ID);

		// Return `6`, finished with error
		return 6;
	}

	// Create local [fixRectangle]
	RECT fixRectangle;

	// Calculate [build] real offset using local [windowRectangle] and [fixRectangle]
	GetWindowRect(build[window->ID].HWND, &windowRectangle);
	GetClientRect(build[window->ID].HWND, &fixRectangle);

	// Set [build] [widthFix] and [heightFix] values
	build[window->ID].widthFix=
		(windowRectangle.right-windowRectangle.left)-(fixRectangle.right-fixRectangle.left);
	build[window->ID].heightFix=
		(windowRectangle.bottom-windowRectangle.top)-(fixRectangle.bottom-fixRectangle.top);

	// Refresh and show [build] [HWND]
	InvalidateRect(build[window->ID].HWND, NULL, TRUE);
	ShowWindow(build[window->ID].HWND, SW_SHOW);

	// Move and resize [build] [HWND]
	SetWindowPos(
		build[window->ID].HWND, NULL, window->x, window->y,
		0, 0, SWP_NOZORDER | SWP_NOSIZE
	);
	SetForegroundWindow(build[window->ID].HWND);

	// Setup [build] [DHDC]
	build[window->ID].DHDC=GetDC(build[window->ID].HWND);

	// Create and setup local [pixel] for future rendering
	PIXELFORMATDESCRIPTOR pixel=(PIXELFORMATDESCRIPTOR){
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	SetPixelFormat(
		build[window->ID].DHDC, ChoosePixelFormat(build[window->ID].DHDC, &pixel), &pixel
	);

	// Create [build] [buffer]
	build[window->ID].buffer=wglCreateContext(build[window->ID].DHDC);
	wglMakeCurrent(build[window->ID].DHDC, build[window->ID].buffer);

	// Setup [build] [buffer]
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, (-1), 1);
	glMatrixMode(GL_MODELVIEW);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pWindowDrawObject()
* | .     | In: pWindow* [window], pObject* [object],
* |     . | pColor* [color], pImage* [image]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [window] - which [window] should be used for rendering.
* [object] - which [object] should be rendered on [window].
* [color] - in which [color] fill should [object] be
* rendered in. Can be `NULL`.
* [image] - in which [image] fill should [object] be
* rendered in. Can be `NULL`.
*
* Additional Description:
* This function uses GL to render [object] on given [window].
* Before any action is taken, it checks and refreshes
* [object] values if needed. If any [vertice] was modified,
* then [texture] positions, debug [width], [height] and
* center [x] and [y] position are being recalculated. Then
* when everything is prepared [color] and [image] are
* being applied to GL and [object] is being rendered.
****************************************************************/
uint8_t pWindowDrawObject(pWindow *window, pObject *object, pColor *color, pImage *image){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Could not draw object!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(window==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given window is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(window->ID>WINDOW_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given window was not created properly!\n");
			printf("Value of the window.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(build[window->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given window was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}

		// Return `4`, finished with error
		return 4;
	}

	if(object==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `5`, finished with error
		return 5;
	}
	else if(object->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `6`, finished with error
		return 6;
	}
	else if(figure[object->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawObject() Error]\n");
			printf("Given object was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}

		// Return `7`, finished with error
		return 7;
	}

	if(image!=NULL){
		if(image->ID>IMAGE_MAX){
			if(przecinek.debug==true){
				printf("[pWindowDrawObject() Error]\n");
				printf("Given image was not created properly!\n");
				printf("Value of the image.ID is out of range.\n");
				printf("Try to not use corrupted variables next time,\n");
				fflush(stdout);
			}

			// Return `8`, finished with error
			return 8;
		}
		else if(texture[image->ID].exists==false){
			if(przecinek.debug==true){
				printf("[pWindowDrawObject() Error]\n");
				printf("Given image was already destroyed!\n");
				printf("Not sure what you were trying todo :-PP,\n");
				fflush(stdout);
			}

			// Return `9`, finished with error
			return 9;
		}
	}

	// Refresh [object]
	pDebugObjectSetup(object);

	// Check for square collision between [window] and [object]
	if(
			(0<figure[object->ID].x+figure[object->ID].width) &&
			(build[window->ID].width>figure[object->ID].x) &&
			(0<figure[object->ID].y+figure[object->ID].height) &&
			(build[window->ID].height>figure[object->ID].y)==false){

		// Return `0`, nothing to draw
		return 0;
	}

	// Set current [build] [buffer]
	wglMakeCurrent(build[window->ID].DHDC, build[window->ID].buffer);

	// Setup [build] [buffer]
	if(color==NULL){
		// Use default [color] for rendering
		if(image==NULL){
			glColor4f(
				(float)COLOR_DEFAULT_FOREGROUND.red/255, (float)COLOR_DEFAULT_FOREGROUND.green/255,
				(float)COLOR_DEFAULT_FOREGROUND.blue/255, (float)COLOR_DEFAULT_FOREGROUND.alpha/255
			);
		}
		else{ glColor4f(1.f, 1.f, 1.f, 1.f); }
	}
	else{
		// Set rendering [color]
		glColor4f(
			(float)color->red/255, (float)color->green/255,
			(float)color->blue/255, (float)color->alpha/255
		);
	}

	if(image!=NULL){
		// Bind [texture] based on [image] [ID]
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[image->ID].source);
	}

	// Draw on [build] [buffer]
	glBegin(GL_POLYGON);

	for(uint32_t current=0; current<figure[object->ID].verticeCount; current+=1){
		glTexCoord2f(
			figure[object->ID].xSrc[current], figure[object->ID].ySrc[current]
		);
		glVertex2i(
			figure[object->ID].vertice[current].x, figure[object->ID].vertice[current].y
		);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pWindowDrawText()
* | .     | In: pWindow* [window], pFont* [font], pText* [text],
* |     . | pColor* [color]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [window] - which [window] should be used for rendering.
* [font] - which [font] should be to render [text].
* [text] - which [text] should be drawn.
* [color] - in which [color] fill should [text] be
* rendered in. Can be `NULL`.
*
* Additional Description:
* This function uses GL to render [text] using [font]
* on given [window]. Before any action is taken, it checks
* and refreshes [text] and [font] values if needed.
* Depending on what changes were made either position values
* of all [text] buffers are being recalculated or [text]
* debug values are being reseted and regenerated. Then
* when everything is prepared [color] is being applied to
* GL and [text] is being rendered.
****************************************************************/
uint8_t pWindowDrawText(pWindow *window, pFont *font, pText *text, pColor *color){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Could not draw text!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(window==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given window is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(window->ID>WINDOW_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given window was not created properly!\n");
			printf("Value of the window.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(build[window->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given window was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}

		// Return `4`, finished with error
		return 4;
	}

	if(font==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given font is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `5`, finished with error
		return 5;
	}
	else if(font->ID>FONT_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given font was not created properly!\n");
			printf("Value of the font.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `6`, finished with error
		return 6;
	}
	else if(view[font->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given font was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}

		// Return `7`, finished with error
		return 7;
	}

	if(text==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given text is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `8`, finished with error
		return 8;
	}
	else if(text->ID>TEXT_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given text was not created properly!\n");
			printf("Value of the text.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `9`, finished with error
		return 9;
	}
	else if(code[text->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Error]\n");
			printf("Given text was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}

		// Return `10`, finished with error
		return 10;
	}

	// Refresh [text] and [font]
	pDebugTextSetup(text, font);

	// Set current [build] [buffer]
	wglMakeCurrent(build[window->ID].DHDC, build[window->ID].buffer);

	// Setup [build] [buffer]
	glEnable(GL_TEXTURE_2D);

	if(color!=NULL){
		glColor4f(
			(float)color->red/255,
			(float)color->green/255,
			(float)color->blue/255,
			(float)color->alpha/255
		);
	}
	else{
		glColor4f(
			(float)COLOR_DEFAULT_FOREGROUND.red/255,
			(float)COLOR_DEFAULT_FOREGROUND.green/255,
			(float)COLOR_DEFAULT_FOREGROUND.blue/255,
			(float)COLOR_DEFAULT_FOREGROUND.alpha/255
		);
	}

	for(uint16_t current=0; current<wcslen(code[text->ID].value); current+=1){
		// Check for square collision between [window] and [text] char
		if(
				(0<code[text->ID].sourcePosition[font->ID][current].x+
				code[text->ID].sourceSize[font->ID][current].width) &&
				(build[window->ID].width>code[text->ID].sourcePosition[font->ID][current].x) &&
				(0<code[text->ID].sourcePosition[font->ID][current].y+
				code[text->ID].sourceSize[font->ID][current].height) &&
				(build[window->ID].height>code[text->ID].sourcePosition[font->ID][current].y)==false){

			continue;
		}

		// Skip special chars
		if(code[text->ID].value[current]==L'\n' || code[text->ID].value[current]==L' '){ continue; }

		// Bind [code] [source]
		glBindTexture(GL_TEXTURE_2D, code[text->ID].source[font->ID][current]);

		// Draw on [build] [buffer]
		glBegin(GL_QUADS);

		glTexCoord2f(0.f, 0.f);
		glVertex2i(
			code[text->ID].sourcePosition[font->ID][current].x,
			code[text->ID].sourcePosition[font->ID][current].y
		);

		glTexCoord2f(0.f, 1.f);
		glVertex2i(
			code[text->ID].sourcePosition[font->ID][current].x,
			(code[text->ID].sourcePosition[font->ID][current].y+
			code[text->ID].sourceSize[font->ID][current].height)
		);

		glTexCoord2f(1.f, 1.f);
		glVertex2i(
			(code[text->ID].sourcePosition[font->ID][current].x+
			code[text->ID].sourceSize[font->ID][current].width),
			(code[text->ID].sourcePosition[font->ID][current].y+
			code[text->ID].sourceSize[font->ID][current].height)
		);

		glTexCoord2f(1.f, 0.f);
		glVertex2i(
			(code[text->ID].sourcePosition[font->ID][current].x+
			code[text->ID].sourceSize[font->ID][current].width),
			code[text->ID].sourcePosition[font->ID][current].y
		);

		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pWindowDestroy()
* | .     |
* |     . | In: pWindow* [window]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [window] - which [window] structure should receive kill signal.
* Removed debug values will depend on [window] [ID].
****************************************************************/
uint8_t pWindowDestroy(pWindow* window){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pWindowDestroy() Error]\n");
			printf("Could not destroy window!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(window==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDestroy() Error]\n");
			printf("Given window is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(window->ID>WINDOW_MAX){
		if(przecinek.debug==true){
			printf("[pWindowDestroy() Error]\n");
			printf("Given window was not created properly!\n");
			printf("Value of the window.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(build[window->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pWindowDestroy() Warning]\n");
			printf("Given window was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(build[window->ID].exists==true){
		// Reset [window]
		pDebugWindowReset(window);

		// Send [build] `WM_CLOSE` signal
		PostMessage(build[window->ID].HWND, WM_CLOSE, 0, 0);
	}

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pDebugFigureReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugFigureReset(uint16_t ID){
	// Clean up [figure] [vertice] values
	if(figure[ID].vertice!=NULL){
		free(figure[ID].vertice);
		figure[ID].vertice=NULL;
	}

	// Clean up [figure] [xSrc] values
	if(figure[ID].xSrc!=NULL){
		free(figure[ID].xSrc);
		figure[ID].xSrc=NULL;
	}
	// Clean up [figure] [ySrc] values
	if(figure[ID].ySrc!=NULL){
		free(figure[ID].ySrc);
		figure[ID].ySrc=NULL;
	}

	// Mark [figure] [exists] as `false`
	figure[ID].exists=false;

	return;
}

/****************************************************************
* |\_____/| pDebugObjectReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugObjectReset(pObject *object){
	// Reset [object] [verticeCount] value
	object->verticeCount=0;

	// Clean up [object] [vertice] values
	if(object->vertice!=NULL){
		free(object->vertice);
		object->vertice=NULL;
	}

	return;
}

/****************************************************************
* |\_____/| pDebugObjectSetup()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugObjectSetup(pObject *object){
	// Update [object] [verticeCount]
	object->verticeCount=figure[object->ID].verticeCount;

	// Check for any changes
	if(memcmp(figure[object->ID].vertice, object->vertice,
			sizeof(pPosition)*figure[object->ID].verticeCount)!=0){

		// Refresh [figure] [vertice] values
		memcpy(
			figure[object->ID].vertice, object->vertice,
			sizeof(pPosition)*figure[object->ID].verticeCount
		);

		// Create local [objectMin] and [objectMax] for [object]
		pPosition objectMin={ INT_MAX, INT_MAX }, objectMax={ INT_MIN, INT_MIN };

		for(uint16_t current=0; current<figure[object->ID].verticeCount; current+=1){
			// Calculate local [objectMax] values
			if(figure[object->ID].vertice[current].x>objectMax.x){
				objectMax.x=figure[object->ID].vertice[current].x;
			}
			if(figure[object->ID].vertice[current].y>objectMax.y){
				objectMax.y=figure[object->ID].vertice[current].y;
			}

			// Calculate local [objectMin] values
			if(figure[object->ID].vertice[current].x<objectMin.x){
				objectMin.x=figure[object->ID].vertice[current].x;
			}
			if(figure[object->ID].vertice[current].y<objectMin.y){
				objectMin.y=figure[object->ID].vertice[current].y;
			}
		}

		for(uint16_t current=0; current<figure[object->ID].verticeCount; current+=1){
			// Calculate [figure] [xSrc] and [ySrc] values
			figure[object->ID].xSrc[current]=
				(float)(objectMax.x-figure[object->ID].vertice[current].x)/
				(float)(objectMax.x-objectMin.x);
			figure[object->ID].ySrc[current]=
				(float)(objectMax.y-figure[object->ID].vertice[current].y)/
				(float)(objectMax.y-objectMin.y);

			figure[object->ID].xSrc[current]=1.f-figure[object->ID].xSrc[current];
			figure[object->ID].ySrc[current]=1.f-figure[object->ID].ySrc[current];
		}

		// Update [figure] size and position parameters
		figure[object->ID].x=objectMin.x;
		figure[object->ID].y=objectMin.y;

		figure[object->ID].width=objectMax.x-objectMin.x;
		figure[object->ID].height=objectMax.y-objectMin.y;

		figure[object->ID].center.x=figure[object->ID].x+(figure[object->ID].width/2);
		figure[object->ID].center.y=figure[object->ID].y+(figure[object->ID].height/2);
	}

	return;
}

/****************************************************************
* |\_____/| pDebugObjectCollisionTriangle()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
bool pDebugObjectCollisionTriangle(
	pPosition *v1A, pPosition *v2A, pPosition *v3A,
	pPosition *v1B, pPosition *v2B, pPosition *v3B
){

	// Create local [triangleA] and [triangleB]
	pPosition triangleA[3]={ *v1A, *v2A, *v3A };
	pPosition triangleB[3]={ *v1B, *v2B, *v3B };

	for(uint8_t current=0; current<6; current+=1){
		// Create local [edge]
		pPosition edge;

		// Calculate local [edge] values
		if(current<3){
			edge.x=triangleA[(current+1)%3].x-triangleA[current].x;
			edge.y=triangleA[(current+1)%3].y-triangleA[current].y;
		}
		else{
			edge.x=triangleB[(current-2)%3].x-triangleB[current-3].x;
			edge.y=triangleB[(current-2)%3].y-triangleB[current-3].y;
		}

		// Create local [distanceMin] and [distanceMax]
		pPosition distanceMin={ INT_MAX, INT_MAX }, distanceMax={ INT_MIN, INT_MIN };

		for(uint8_t loop=0; loop<3; loop+=1){
			// Create local [projection]
			pPosition projection;

			// Caululate local [projection] values
			projection.x=(triangleA[loop].x*(-edge.y))+(triangleA[loop].y*edge.x);
			projection.y=(triangleB[loop].x*(-edge.y))+(triangleB[loop].y*edge.x);

			// Check for collision
			if(projection.x<distanceMin.x){ distanceMin.x=projection.x; }
			if(projection.x>distanceMax.x){ distanceMax.x=projection.x; }
			if(projection.y<distanceMin.y){ distanceMin.y=projection.y; }
			if(projection.y>distanceMax.y){ distanceMax.y=projection.y; }
		}

		if(distanceMax.x<distanceMin.y || distanceMax.y<distanceMin.x){
			// Return `false`, collision undetected
			return false;
		}
	}

	// Return `true`, collision detected
	return true;
}

/****************************************************************
* |\_____/| pObjectCreate()
* | .     |
* |     . | In: pObject* [object], uint16_t [verticeCount]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [object] - which [object] structure should be initialized.
* If [object] was already created, then it will be overwritten.
* [verticeCount] - how many [vertice] should be allocated.
*
* Additional Description:
* This function choses [ID] for given [object], then it setups
* all needed values and variables and allocates [vertice]
* amount in the memory.
****************************************************************/
uint8_t pObjectCreate(pObject *object, uint16_t verticeCount){
	if(object==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	for(uint16_t current=0; current<OBJECT_MAX; current+=1){
		if(figure[current].exists==false){
			// Set some values to `NULL`
			object->vertice=NULL;

			figure[current].vertice=NULL;
			figure[current].xSrc=NULL;
			figure[current].ySrc=NULL;

			// Set [object] [ID] and reset it
			object->ID=current;
			pDebugObjectReset(object);
			pDebugFigureReset(object->ID);

			break;
		}
		else if(current==OBJECT_MAX-1){
			if(przecinek.debug==true){
				printf("[pObjectCreate() Error]\n");
				printf("Too many objects were created!\n");
				printf("Current object limit is equal to %i.\n", OBJECT_MAX);
				printf("Try to destroy unused objects or change Przecinek object limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pObjectCreate() Error]\n");
		printf("Could not create object!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Check [verticeCount] value
	if(verticeCount<OBJECT_VERTICE_MIN){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Warning]\n");
			printf("Value of the object.verticeCount is too small!\n");
			printf(
				"Value of the object.verticeCount was changed from %i to %i,\n",
				verticeCount, OBJECT_VERTICE_MIN
			);
			fflush(stdout);
		}

		// Change [vertice] value
		verticeCount=OBJECT_VERTICE_MIN;
	}
	else if(verticeCount>OBJECT_VERTICE_MAX){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Warning]\n");
			printf("Value of the object.verticeCount is too large!\n");
			printf(
				"Value of the object.verticeCount was changed from %i to %i,\n",
				verticeCount, OBJECT_VERTICE_MAX
			);
			fflush(stdout);
		}

		// Change [vertice] value
		verticeCount=OBJECT_VERTICE_MAX;
	}

	// Allocate [object] [vertice]
	object->vertice=malloc(sizeof(pPosition)*verticeCount);
	if(object->vertice==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);

		// Return `4`, finished with error
		return 4;
	}

	// Set [object] values
	object->verticeCount=verticeCount;
	memset(object->vertice, 0, sizeof(pPosition)*verticeCount);

	// Allocate [figure] [vertice]
	figure[object->ID].vertice=malloc(sizeof(pPosition)*verticeCount);
	if(figure[object->ID].vertice==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);

		// Return `5`, finished with error
		return 5;
	}

	// Allocate [figure] [xSrc]
	figure[object->ID].xSrc=malloc(sizeof(float)*verticeCount);
	if(figure[object->ID].xSrc==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);

		// Return `6`, finished with error
		return 6;
	}

	// Allocate [figure] [ySrc]
	figure[object->ID].ySrc=malloc(sizeof(float)*verticeCount);
	if(figure[object->ID].ySrc==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);

		// Return `7`, finished with error
		return 7;
	}

	// Set [figure] values
	figure[object->ID].exists=true;

	figure[object->ID].verticeCount=verticeCount;
	memset(figure[object->ID].vertice, 0, sizeof(pPosition)*verticeCount);

	// Set [figure] [rotationFix] value
	if(verticeCount==4){ figure[object->ID].rotationFix=45; }
	else if(verticeCount%2!=0){ figure[object->ID].rotationFix=270; }
	else{ figure[object->ID].rotationFix=0; }

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pObjectGenerate()
* | .     | In: pObject* [object], uint16_t [verticeCount], int32_t
* |     . | [x], [y], uint16_t [width], [height], int16_t [rotation]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [object] - which [object] structure should be generated.
* [verticeCount] - how many [object] [vertice] should be used.
* Generated figure will depend on [verticeCount].
* [x], [y] - from where [object] vertices should start from.
* [width], [height] - how big should the [object] be.
* [rotation] - how rotated should the [object] be.
*
* Additional Description:
* This function generates [object] structure based on given
* parameters. It automatically calculates [vertice] positions
* and creates geometric figure. It also prepares [texture]
* positions and calculates debug [width], [height] and
* figure center [x] and [y] position.
****************************************************************/
uint8_t pObjectGenerate(
	pObject *object, uint16_t verticeCount, int32_t x, int32_t y,
	uint16_t width, uint16_t height, int16_t rotation
){

	if(setup==false){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Error]\n");
			printf("Could not generate object!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(object==NULL){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(object->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}

	// Check [verticeCount]
	if(verticeCount>figure[object->ID].verticeCount){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Warning]\n");
			printf("Value of verticeCount is bigger than object.verticeCount!\n");
			printf(
				"Value of verticeCount was changed from %i to %i,\n",
				verticeCount, figure[object->ID].verticeCount
			);
			fflush(stdout);
		}

		// Update [verticeCount] value
		verticeCount=figure[object->ID].verticeCount;
	}

	// Check [rotation] value
	if(rotation<OBJECT_ROTATION_MIN){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Warning]\n");
			printf("Value of the object.rotation is too small!\n");
			printf(
				"Value of the object.rotation was changed from %i to %i,\n",
				rotation, OBJECT_ROTATION_MIN
			);
			fflush(stdout);
		}

		// Update [rotation] value
		rotation=OBJECT_ROTATION_MIN;
	}
	if(rotation>OBJECT_ROTATION_MAX){
		if(przecinek.debug==true){
			printf("[pObjectGenerate() Warning]\n");
			printf("Value of the object.rotation is too large!\n");
			printf(
				"Value of the object.rotation was changed from %i to %i,\n",
				rotation, OBJECT_ROTATION_MAX
			);
			fflush(stdout);
		}

		// Update [rotation] value
		rotation=OBJECT_ROTATION_MAX;
	}

	// Create local [distanceMin] and [distanceMax] for [object]
	pPosition distanceMin={ INT_MAX, INT_MAX };
	pPosition distanceMax={ INT_MIN, INT_MIN };

	for(uint16_t current=0; current<verticeCount; current+=1){
		// Create and calculate local [ratio]
		float ratio=(float)((current*360/verticeCount)+
			rotation+figure[object->ID].rotationFix)*(M_PI/180);

		// Calculate [figure] [vertice] [x]
		figure[object->ID].vertice[current].x=50+(sqrt(pow(100, 2)*2)/2)*cos(ratio);

		// Calculate [figure] [vertice] [y]
		figure[object->ID].vertice[current].y=50+(sqrt(pow(100, 2)*2)/2)*sin(ratio);

		// Check for local [distanceMin] values
		if(figure[object->ID].vertice[current].x<distanceMin.x){
			distanceMin.x=figure[object->ID].vertice[current].x;
		}
		if(figure[object->ID].vertice[current].y<distanceMin.y){
			distanceMin.y=figure[object->ID].vertice[current].y;
		}

		// Check for local [distanceMax] values
		if(figure[object->ID].vertice[current].x>distanceMax.x){
			distanceMax.x=figure[object->ID].vertice[current].x;
		}
		if(figure[object->ID].vertice[current].y>distanceMax.y){
			distanceMax.y=figure[object->ID].vertice[current].y;
		}
	}

	// Create and calculate local [widthScale] and [heightScale]
	float widthScale=(float)(distanceMax.x-distanceMin.x)/100;
	float heightScale=(float)(distanceMax.y-distanceMin.y)/100;

	// Reset local [distanceMin] and [distanceMax]
	distanceMin=(pPosition){ INT_MAX, INT_MAX };
	distanceMax=(pPosition){ INT_MIN, INT_MIN };

	for(uint16_t current=0; current<verticeCount; current+=1){
		// Create and calculate local [ratio]
		float ratio=(float)((current*360/verticeCount)+
			rotation+figure[object->ID].rotationFix)*(M_PI/180);

		// Calculate [figure] [vertice] [x]
		figure[object->ID].vertice[current].x=
			(width/2)+(sqrt(pow((float)width/widthScale, 2)*2)/2)*cos(ratio);

		// Calculate [figure] [vertice] [y]
		figure[object->ID].vertice[current].y=
			(height/2)+(sqrt(pow((float)height/heightScale, 2)*2)/2)*sin(ratio);

		// Check for local [distanceMin] values
		if(figure[object->ID].vertice[current].x<distanceMin.x){
			distanceMin.x=figure[object->ID].vertice[current].x;
		}
		if(figure[object->ID].vertice[current].y<distanceMin.y){
			distanceMin.y=figure[object->ID].vertice[current].y;
		}
	}

	for(uint16_t current=0; current<verticeCount; current+=1){
		// Correct [figure] [vertice] positions
		figure[object->ID].vertice[current].x-=distanceMin.x-x;
		figure[object->ID].vertice[current].y-=distanceMin.y-y;

		// Correct local [distanceMax]
		if(figure[object->ID].vertice[current].x>distanceMax.x){
			distanceMax.x=figure[object->ID].vertice[current].x;
		}
		if(figure[object->ID].vertice[current].y>distanceMax.y){
			distanceMax.y=figure[object->ID].vertice[current].y;
		}
	}

	for(uint16_t current=0; current<verticeCount; current+=1){
		// Calculate [figure] [xSrc] and [ySrc] values
		figure[object->ID].xSrc[current]=
			(float)(distanceMax.x-figure[object->ID].vertice[current].x)/
			(float)(distanceMax.x-x);
		figure[object->ID].ySrc[current]=
			(float)(distanceMax.y-figure[object->ID].vertice[current].y)/
			(float)(distanceMax.y-y);

		figure[object->ID].xSrc[current]=1.f-figure[object->ID].xSrc[current];
		figure[object->ID].ySrc[current]=1.f-figure[object->ID].ySrc[current];
	}

	// Copy [figure] [vertice] to the [object]
	memcpy(object->vertice, figure[object->ID].vertice, sizeof(pPosition)*verticeCount);

	// Update [figure] size and position parameters
	figure[object->ID].x=x;
	figure[object->ID].y=y;

	figure[object->ID].width=width;
	figure[object->ID].height=height;

	figure[object->ID].center.x=x+(width/2);
	figure[object->ID].center.y=y+(height/2);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pObjectMove()
* | .     | In: pObject* [object], uint16_t [verticeCount],
* |     . | int32_t [x], [y]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [object] - which [object] structure should be moved.
* [verticeCount] - how many [object] [vertice] should be moved.
* [x], [y] - from where [object] vertices should start from.
*
* Additional Description:
* This function moves [object] structure based on given
* parameters. It also recalculates figure debug center [x]
* and [y] position.
****************************************************************/
uint8_t pObjectMove(pObject *object, uint16_t verticeCount, int32_t x, int32_t y){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pObjectMove() Error]\n");
			printf("Could not generate object!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(object==NULL){
		if(przecinek.debug==true){
			printf("[pObjectMove() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(object->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pObjectMove() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}

	// Check [verticeCount]
	if(verticeCount>figure[object->ID].verticeCount){
		if(przecinek.debug==true){
			printf("[pObjectMove() Warning]\n");
			printf("Value of verticeCount is bigger than object.verticeCount!\n");
			printf(
				"Value of verticeCount was changed from %i to %i,\n",
				verticeCount, figure[object->ID].verticeCount
			);
			fflush(stdout);
		}

		// Update [verticeCount] value
		verticeCount=figure[object->ID].verticeCount;
	}

	// Create local [diffrence]
	pPosition diffrence=(pPosition){ (x-figure[object->ID].x), (y-figure[object->ID].y) };

	for(uint16_t current=0; current<verticeCount; current+=1){
		// Change [figure] [vertice] positions by local [diffrence] values
		figure[object->ID].vertice[current].x+=diffrence.x;
		figure[object->ID].vertice[current].y+=diffrence.y;
	}

	// Copy [figure] [vertice] to the [object]
	memcpy(object->vertice, figure[object->ID].vertice, sizeof(pPosition)*verticeCount);

	// Update [figure] position parameters
	figure[object->ID].x=x;
	figure[object->ID].y=y;

	figure[object->ID].center.x=x+(figure[object->ID].width/2);
	figure[object->ID].center.y=y+(figure[object->ID].height/2);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pObjectCollisionSquare()
* | .     |
* |     . | In: pObject* [object1], [object2]
* \ = , = / Out: bool (`true` -> found collision)
*
* Parameters:
* [object1], [object2] - which structures should be checked.
*
* Additional Description:
* This function checks for collision between two objects
* using their width and height. Their size parameters are
* calculated as debug values. This method is much faster but
* less accurate.
****************************************************************/
bool pObjectCollisionSquare(pObject *object1, pObject *object2){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pObjectCollisionSquare() Error]\n");
			printf("Could not check objects collision!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	if(object1==NULL || object2==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCollisionSquare() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Could not check collsion, return `false`
		return false;
	}
	else if(object1->ID>OBJECT_MAX || object2->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pObjectCollisionSquare() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Could not check collsion, return `false`
		return false;
	}
	else if(object1->ID==object2->ID){
		if(przecinek.debug==true){
			printf("[pObjectCollisionSquare() Warning]\n");
			printf("Given objects are exactly the same!\n");
			printf("And even if they are not you still use the same object->ID.\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Same objects, return `true`
		return true;
	}

	// Refresh [object1] and [object2]
	pDebugObjectSetup(object1);
	pDebugObjectSetup(object2);

	// Check for square collision between [object1] and [object2]
	if(
			(figure[object1->ID].x<figure[object2->ID].x+figure[object2->ID].width) &&
			(figure[object1->ID].x+figure[object1->ID].width>figure[object2->ID].x) &&
			(figure[object1->ID].y<figure[object2->ID].y+figure[object2->ID].height) &&
			(figure[object1->ID].y+figure[object1->ID].height>figure[object2->ID].y)==true){

		// Return `true`, collision detected
		return true;
	}
}

/****************************************************************
* |\_____/| pObjectCollisionComplex()
* | .     |
* |     . | In: pObject* [object1], [object2]
* \ = , = / Out: bool (`true` -> found collision)
*
* Parameters:
* [object1], [object2] - which structures should be checked.
*
* Additional Description:
* This function checks for collision between two objects
* using their vertices. Firstly it searches five closest
* vertices of [object1] to the center of [object2]. Then
* it checks five closest vertices of [object2] to the center
* of [object1]. Finally using big chunk of code it checks
* triangular collision between simulated mini objects. This
* method is much slower but nearly perfectly accurate.
****************************************************************/
bool pObjectCollisionComplex(pObject *object1, pObject *object2){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pObjectCollisionComplex() Error]\n");
			printf("Could not check objects collision!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `false`, finished with error
		return false;
	}

	if(object1==NULL || object2==NULL){
		if(przecinek.debug==true){
			printf("[pObjectCollisionComplex() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Could not check collsion, return `false`
		return false;
	}
	else if(object1->ID>OBJECT_MAX || object2->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pObjectCollisionComplex() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Could not check collsion, return `false`
		return false;
	}
	else if(object1->ID==object2->ID){
		if(przecinek.debug==true){
			printf("[pObjectCollisionComplex() Warning]\n");
			printf("Given objects are exactly the same!\n");
			printf("And even if they are not you still use the same object->ID.\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Same objects, return `true`
		return true;
	}

	// Check for square collision before any further calculations
	if(pObjectCollisionSquare(object1, object2)==true){
		// Create local [distance] variable
		uint32_t distance=UINT_MAX;

		// Create local [shape1] and [shape2]
		int32_t shape1[5], shape2[5];

		for(uint16_t current=0; current<figure[object1->ID].verticeCount; current+=1){
			// Create and calculate local [ratio] value
			uint32_t ratio=(double)sqrt(
				pow((figure[object2->ID].center.x-figure[object1->ID].vertice[current].x), 2)+
				pow((figure[object2->ID].center.y-figure[object1->ID].vertice[current].y), 2));

			// Update local [distance] value
			if(ratio<distance){
				distance=ratio;
				shape1[0]=current;
			}
		}

		// Create and calculate local [shapeSpacing]
		uint16_t shapeSpacing=((float)figure[object1->ID].verticeCount/15)+1;

		// Calculate local [shape1] positon `1` -[shapeSpacing]
		if(shape1[0]-shapeSpacing<0){
			shape1[1]=(shape1[0]-shapeSpacing)+figure[object1->ID].verticeCount;
		}
		else{ shape1[1]=shape1[0]-shapeSpacing; }

		// Calculate local [shape1] positon `2` +[shapeSpacing]
		if(shape1[0]+shapeSpacing>figure[object1->ID].verticeCount){
			shape1[2]=(shape1[0]+shapeSpacing)-figure[object1->ID].verticeCount;
		}
		else{ shape1[2]=shape1[0]+shapeSpacing; }

		if(figure[object1->ID].verticeCount>=8){
			// Calculate local [shape1] positon `3` -[shapeSpacing]*2
			if(shape1[1]-shapeSpacing<0){
				shape1[3]=(shape1[1]-shapeSpacing)+figure[object1->ID].verticeCount;
			}
			else{ shape1[3]=shape1[1]-shapeSpacing; }

			// Calculate local [shape1] positon `4` +[shapeSpacing]*2
			if(shape1[2]+shapeSpacing>figure[object1->ID].verticeCount){
				shape1[4]=(shape1[2]+shapeSpacing)-figure[object1->ID].verticeCount;
			}
			else{ shape1[4]=shape1[2]+shapeSpacing; }
		}

		// Reset local [distance] value
		distance=UINT_MAX;

		for(uint16_t current=0; current<figure[object2->ID].verticeCount; current+=1){
			// Create and calculate local [ratio] value
			uint32_t ratio=(double)sqrt(
				pow((figure[object1->ID].center.x-figure[object2->ID].vertice[current].x), 2)+
				pow((figure[object1->ID].center.y-figure[object2->ID].vertice[current].y), 2));

			// Update local [distance] value
			if(ratio<distance){
				distance=ratio;
				shape2[0]=current;
			}
		}

		// Update local [shapeSpacing]
		shapeSpacing=((float)figure[object2->ID].verticeCount/15)+1;

		// Calculate local [shape2] positon `1` -[shapeSpacing]
		if(shape2[0]-shapeSpacing<0){
			shape2[1]=(shape2[0]-shapeSpacing)+figure[object2->ID].verticeCount;
		}
		else{ shape2[1]=shape2[0]-shapeSpacing; }

		// Calculate local [shape2] positon `2` +[shapeSpacing]
		if(shape2[0]+shapeSpacing>figure[object2->ID].verticeCount){
			shape2[2]=(shape2[0]+shapeSpacing)-figure[object2->ID].verticeCount;
		}
		else{ shape2[2]=shape2[0]+shapeSpacing; }

		if(figure[object2->ID].verticeCount>=8){
			// Calculate local [shape2] positon `3` -[shapeSpacing]*2
			if(shape2[1]-shapeSpacing<0){
				shape2[3]=(shape2[1]-shapeSpacing)+figure[object2->ID].verticeCount;
			}
			else{ shape2[3]=shape2[1]-shapeSpacing; }

			// Calculate local [shape2] positon `4` +[shapeSpacing]*2
			if(shape2[2]+shapeSpacing>figure[object2->ID].verticeCount){
				shape2[4]=(shape2[2]+shapeSpacing)-figure[object2->ID].verticeCount;
			}
			else{ shape2[4]=shape2[2]+shapeSpacing; }
		}

		// Try to detect collison with this type shit mess X.X
		if(
pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&figure[object2->ID].center)==false &&

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&figure[object2->ID].center)==false &&

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&figure[object2->ID].center)==false &&

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&figure[object2->ID].center)==false){

			if(figure[object2->ID].verticeCount>=8){
				if(
pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[3]].x, figure[object2->ID].vertice[shape2[3]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[4]].x, figure[object2->ID].vertice[shape2[4]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[3]].x, figure[object2->ID].vertice[shape2[3]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[0]].x, figure[object1->ID].vertice[shape1[0]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[4]].x, figure[object2->ID].vertice[shape2[4]].y },
&figure[object2->ID].center)==true){

					// Collision found, return `true`
					return true;
				}
			}
			if(figure[object1->ID].verticeCount>=8){
				if(
pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[3]].x, figure[object1->ID].vertice[shape1[3]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[3]].x, figure[object1->ID].vertice[shape1[3]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[4]].x, figure[object1->ID].vertice[shape1[4]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[4]].x, figure[object1->ID].vertice[shape1[4]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[0]].x, figure[object2->ID].vertice[shape2[0]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&figure[object2->ID].center)==true){

					// Collision found, return `true`
					return true;
				}
			}
			if(figure[object1->ID].verticeCount>=8 && figure[object2->ID].verticeCount>=8){
				if(
pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[3]].x, figure[object1->ID].vertice[shape1[3]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[3]].x, figure[object2->ID].vertice[shape2[3]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[1]].x, figure[object1->ID].vertice[shape1[1]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[3]].x, figure[object1->ID].vertice[shape1[3]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[4]].x, figure[object2->ID].vertice[shape2[4]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[4]].x, figure[object1->ID].vertice[shape1[4]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[1]].x, figure[object2->ID].vertice[shape2[1]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[3]].x, figure[object2->ID].vertice[shape2[3]].y },
&figure[object2->ID].center)==true ||

pDebugObjectCollisionTriangle(
&(pPosition){ figure[object1->ID].vertice[shape1[2]].x, figure[object1->ID].vertice[shape1[2]].y },
&(pPosition){ figure[object1->ID].vertice[shape1[4]].x, figure[object1->ID].vertice[shape1[4]].y },
&figure[object1->ID].center,
&(pPosition){ figure[object2->ID].vertice[shape2[2]].x, figure[object2->ID].vertice[shape2[2]].y },
&(pPosition){ figure[object2->ID].vertice[shape2[4]].x, figure[object2->ID].vertice[shape2[4]].y },
&figure[object2->ID].center)==true){

					// Collision found, return `true`
					return true;
				}
			}
		}
		else{
			// Collision found, return `true`
			return true;
		}
	}

	// There is no collision, return `false`
	return false;
}

/****************************************************************
* |\_____/| pObjectDestroy()
* | .     |
* |     . | In: pObject* [object]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [object] - which [object] structure should be destroyed.
* Removed debug values will depend on [object] [ID].
****************************************************************/
uint8_t pObjectDestroy(pObject *object){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pObjectDestroy() Error]\n");
			printf("Could not destroy object!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(object==NULL){
		if(przecinek.debug==true){
			printf("[pObjectDestroy() Error]\n");
			printf("Given object is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(object->ID>OBJECT_MAX){
		if(przecinek.debug==true){
			printf("[pObjectDestroy() Error]\n");
			printf("Given object was not created properly!\n");
			printf("Value of the object.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(figure[object->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pObjectDestroy() Warning]\n");
			printf("Given object was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(figure[object->ID].exists==true){
		// Reset [object] and [figure]
		pDebugObjectReset(object);
		pDebugFigureReset(object->ID);
	}

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pDebugTextureReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugTextureReset(uint16_t ID){
	// Clean up [texture] [source] structure
	glDeleteTextures(1, &texture[ID].source);

	// Mark [texture] [exists] as `false`
	texture[ID].exists=false;

	return;
}

/****************************************************************
* |\_____/| pImageCreate()
* | .     |
* |     . | In: pImage* [image], wchar_t* [directory]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [image] - which [image] structure should be initialized.
* If [image] was already created, then it will be overwritten.
* [directory] - from where the [image] should be loaded.
*
* Additional Description:
* This function choses [ID] for given [image], then it setups
* all needed values and variables, loads [image] from given
* [directory] and creates proper [image] debug GL bitmap
* for future rendering.
****************************************************************/
uint8_t pImageCreate(pImage *image, wchar_t *directory){
	if(image==NULL){
		if(przecinek.debug==true){
			printf("[pImageCreate() Error]\n");
			printf("Given image is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	for(uint16_t current=0; current<IMAGE_MAX; current+=1){
		if(texture[current].exists==false){
			// Set [image] [ID] and reset it
			image->ID=current;
			pDebugTextureReset(image->ID);

			break;
		}
		else if(current==IMAGE_MAX-1){
			if(przecinek.debug==true){
				printf("[pImageCreate() Error]\n");
				printf("Too many images were created!\n");
				printf("Current image limit is equal to %i.\n", IMAGE_MAX);
				printf("Try to destroy unused images or change Przecinek image limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pImageCreate() Error]\n");
		printf("Could not create image!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [texture]
		pDebugTextureReset(image->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Set [texture] values
	texture[image->ID].exists=true;

	// Create local [buffer]
	GpBitmap *buffer;

	// Check [directory] value
	if((wcslen(directory)>=4 && (wcscmp(directory+wcslen(directory)-4, L".png")==0 ||
			wcscmp(directory+wcslen(directory)-4, L".jpg")==0)) ||
			(wcslen(directory)>=5 && wcscmp(directory+wcslen(directory)-5, L".jpeg")==0)){

		// Check if [texture] [directory] exists
		if(GdipCreateBitmapFromFile(directory, &buffer)!=Ok){
			if(przecinek.debug==true){
				printf("[pImageCreate() Error]\n");
				printf("GDI+ library could not load texture!\n");
				printf("Check if image.directory value is correct and not corrupted,\n");				
				fflush(stdout);
			}

			// Reset [texture]
			pDebugTextureReset(image->ID);

			// Return `4`, finished with error
			return 4;
		}
	}
	else{
		if(przecinek.debug==true){
			printf("[pImageCreate() Error]\n");
			printf("Value of the image.directory doesn't include .png/.jpg/.jpeg extenstion!\n");
			printf("Check if you gave the correct value,\n");
			fflush(stdout);
		}

		// Reset [texture]
		pDebugTextureReset(image->ID);

		// Return `5`, finished with error
		return 5;
	}

	// Create local [width] and [height]
	uint32_t width, height;

	// Get local [width] and [height] values
	GdipGetImageWidth(buffer, &width);
	GdipGetImageHeight(buffer, &height);

	// Create local [picture]
	GpRect picture=(GpRect){ 0, 0, width, height };
	
	// Create and setup local [bufferData]
	BitmapData bufferData;
	GdipBitmapLockBits(buffer, &picture, ImageLockModeRead, PixelFormat32bppARGB, &bufferData);

	// Generate [texture] [source]
	glGenTextures(1, &texture[image->ID].source);
	glBindTexture(GL_TEXTURE_2D, texture[image->ID].source);

	// Fill [texture] [source]
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, bufferData.Scan0
	);

	// Setup [texture] [source]
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Clean up
	GdipBitmapUnlockBits(buffer, &bufferData);
	GdipDisposeImage(buffer);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pImageDestroy()
* | .     |
* |     . | In: pImage* [image]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [image] - which [image] structure should be destroyed.
* Removed debug values will depend on [image] [ID].
****************************************************************/
uint8_t pImageDestroy(pImage *image){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pImageDestroy() Error]\n");
			printf("Could not destroy image!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(image==NULL){
		if(przecinek.debug==true){
			printf("[pImageDestroy() Error]\n");
			printf("Given image is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(image->ID>IMAGE_MAX){
		if(przecinek.debug==true){
			printf("[pImageDestroy() Error]\n");
			printf("Given image was not created properly!\n");
			printf("Value of the image.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(texture[image->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pImageDestroy() Warning]\n");
			printf("Given image was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(texture[image->ID].exists==true){
		// Reset [texture]
		pDebugTextureReset(image->ID);
	}

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pDebugViewReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugViewReset(uint16_t ID){
	// Clean up [view] [collection] structure
	if(view[ID].collection!=NULL){
		GdipDeletePrivateFontCollection(&view[ID].collection);
		view[ID].collection=NULL;
	}
	// Clean up [view] [fontFamily] structure
	if(view[ID].fontFamily!=NULL){
		GdipDeleteFontFamily(view[ID].fontFamily);
		view[ID].fontFamily=NULL;
	}
	// Clean up [view] [base] structure
	if(view[ID].base!=NULL){
		GdipDeleteFont(view[ID].base);
		view[ID].base=NULL;
	}

	// Mark [view] [exists] as `false`
	view[ID].exists=false;
	// Fill [view] [created] with `0`
	memset(view[ID].created, false, sizeof(view[ID].created));

	return;
}

/****************************************************************
* |\_____/| pDebugFontReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugFontReset(pFont *font){
	// Reset [font] [size] value
	font->size=0;

	return;
}

/****************************************************************
* |\_____/| pFontCreate()
* | .     | In: pFont* [font], wchar_t* [directory],
* |     . | uint16_t [size]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [font] - which [font] structure should be initialized.
* If [font] was already created, then it will be overwritten.
* [directory] - from where the [font] should be loaded.
* [size] - initial size of newly created [font].
*
* Additional Description:
* This function choses [ID] for given [font], then it loads
* [font] from given [directory] and setups all needed
* values and variables.
****************************************************************/
uint8_t pFontCreate(pFont *font, wchar_t *directory, uint16_t size){
	if(font==NULL){
		if(przecinek.debug==true){
			printf("[pFontCreate() Error]\n");
			printf("Given font is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	for(uint16_t current=0; current<FONT_MAX; current+=1){
		if(view[current].exists==false){
			// Set some values to `NULL`
			view[current].collection=NULL;
			view[current].fontFamily=NULL;
			view[current].base=NULL;

			// Set [font] [ID] and reset it
			font->ID=current;
			pDebugFontReset(font);
			pDebugViewReset(font->ID);

			break;
		}
		else if(current==FONT_MAX-1){
			if(przecinek.debug==true){
				printf("[pFontCreate() Error]\n");
				printf("Too many fonts were created!\n");
				printf("Current font limit is equal to %i.\n", FONT_MAX);
				printf("Try to destroy unused fonts or change Przecinek font limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pFontCreate() Error]\n");
		printf("Could not create font!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [font] and [view]
		pDebugFontReset(font);
		pDebugViewReset(font->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Check [size] value
	if(size<FONT_SIZE_MIN){
		if(przecinek.debug==true){
			printf("[pFontCreate() Warning]\n");
			printf("Value of the font.size is too small!\n");
			printf("Value of the font.size was changed from %i to %i,\n", size, FONT_SIZE_MIN);
			fflush(stdout);
		}

		// Change [size] value
		size=FONT_SIZE_MIN;
	}
	else if(size>FONT_SIZE_MAX){
		if(przecinek.debug==true){
			printf("[pFontCreate() Warning]\n");
			printf("Value of the font.size is too large!\n");
			printf("Value of the font.size was changed from %i to %i,\n", size, FONT_SIZE_MAX);
			fflush(stdout);
		}

		// Change [size] value
		size=FONT_SIZE_MAX;
	}

	// Set [font] values
	font->size=size;

	// Set [view] values
	view[font->ID].exists=true;

	view[font->ID].size=size;

	// Check [directory] value
	if(wcslen(directory)>=4 && (wcscmp(directory+wcslen(directory)-4, L".ttf")==0 ||
			wcscmp(directory+wcslen(directory)-4, L".otf")==0)){

		// Setup [view] [collection] and check if [directory] exists
		GdipNewPrivateFontCollection(&view[font->ID].collection);
		if(GdipPrivateAddFontFile(view[font->ID].collection, directory)!=Ok){
			if(przecinek.debug==true){
				printf("[pFontCreate() Error]\n");
				printf("Font located in font.directory doesn't exist!\n");
				printf("Check if you gave the correct value,\n");
				fflush(stdout);
			}

			// Reset [font] and [view]
			pDebugFontReset(font);
			pDebugViewReset(font->ID);

			// Return `4`, finished with error
			return 4;
		}
	}
	else{
		if(przecinek.debug==true){
			printf("[pFontCreate() Error]\n");
			printf("Value of the font.directory doesn't include .ttf/.otf extenstion!\n");
			printf("Check if you gave the correct value,\n");
			fflush(stdout);
		}

		// Reset [font] and [view]
		pDebugFontReset(font);
		pDebugViewReset(font->ID);

		// Return `5`, finished with error
		return 5;
	}

	// Create local [family] and [familyCount]
	GpFontFamily **family;
	uint32_t familyCount;

	// Get local [familyCount] from [view] [collection]
	GdipGetFontCollectionFamilyCount(view[font->ID].collection, &familyCount);

	// Allocate local [family]
	family=(GpFontFamily**)malloc(familyCount*sizeof(GpFontFamily*));
	if(family==NULL){
		if(przecinek.debug==true){
			printf("[pFontCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [font] and [view]
		pDebugFontReset(font);
		pDebugViewReset(font->ID);

		// Return `6`, finished with error
		return 6;
	}

	// Load all local [family] names
	GdipGetFontCollectionFamilyList(view[font->ID].collection, familyCount, family, &familyCount);

	// Check for [view] [collection] name
	for(uint32_t current=0; current<familyCount; current+=1){
		// Create local [familyName]
		wchar_t familyName[LF_FACESIZE];

		// Load and check local [familyName]
		GdipGetFamilyName(family[current], familyName, 0);
		if(GdipCreateFontFamilyFromName(
				familyName, view[font->ID].collection, &view[font->ID].fontFamily)==Ok){

			break;
		}
		else if(current==familyCount-1){
			if(przecinek.debug==true){
				printf("[pFontCreate() Error]\n");
				printf("GDI+ library could not load font!\n");
				printf("File from font.directory might be corrupted,\n");
				fflush(stdout);
			}

			// Reset [font] and [view]
			pDebugFontReset(font);
			pDebugViewReset(font->ID);

			// Clean up local [family]
			free(family);

			// Return `7`, finished with error
			return 7;
		}
	}
	// Clean up local [family]
	free(family);

	// Initialize [view] [base]
	GdipCreateFont(
		view[font->ID].fontFamily, size*1.33, FontStyleRegular, UnitPixel, &view[font->ID].base
	);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pFontDestroy()
* | .     |
* |     . | In: pFont* [font]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [font] - which [font] structure should be destroyed.
* Removed debug values will depend on [font] [ID].
****************************************************************/
uint8_t pFontDestroy(pFont *font){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pFontDestroy() Error]\n");
			printf("Could not destroy font!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(font==NULL){
		if(przecinek.debug==true){
			printf("[pFontDestroy() Error]\n");
			printf("Given font is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(font->ID>FONT_MAX){
		if(przecinek.debug==true){
			printf("[pFontDestroy() Error]\n");
			printf("Given font was not created properly!\n");
			printf("Value of the font.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(view[font->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pFontDestroy() Warning]\n");
			printf("Given font was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(view[font->ID].exists==true){
		// Reset [font] and [view]
		pDebugFontReset(font);
		pDebugViewReset(font->ID);
	}

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pDebugCodeReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugCodeReset(uint16_t ID){
	// Clean up [code] [value]
	if(code[ID].value!=NULL){
		free(code[ID].value);
		code[ID].value=NULL;
	}

	for(uint16_t font=0; font<FONT_MAX; font+=1){
		for(uint16_t current=0; current<code[ID].length; current+=1){
			glDeleteTextures(1, &code[ID].source[font][current]);
		}

		// Clean up [code] [source], [sourceSize] and [sourcePosition] structures
		free(code[ID].source[font]);
		free(code[ID].sourceSize[font]);
		free(code[ID].sourcePosition[font]);
	}

	// Mark [code] [exists] and [change] as `false`
	code[ID].exists=false;
	code[ID].change=false;

	return;
}

/****************************************************************
* |\_____/| pDebugTextReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugTextReset(pText *text){
	// Reset [text] [x] and [y] values
	text->x=0;
	text->y=0;

	// Reset [text] [length] value
	text->length=0;
	// Clean up [text] [value]
	if(text->value!=NULL){
		free(text->value);
		text->value=NULL;
	}

	// Reset [text] [letterSpacing], [lineSpacing] and [spaceSize] values
	text->letterSpacing=0;
	text->lineSpacing=0;
	text->spaceSize=0;

	return;
}

/****************************************************************
* |\_____/| pDebugTextSetup()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
bool pDebugTextSetup(pText *text, pFont *font){
	if(font->size!=view[font->ID].size){
		// Update [view] [size] value
		view[font->ID].size=font->size;

		// Mark [code] [change] as `true`
		code[text->ID].change=true;
	}

	// Update [text] values
	text->length=code[text->ID].length;

	// Check [text] [value]
	if(text->value==NULL){
		if(przecinek.debug==true){
			printf("[pWindowDrawText() Warning]\n");
			printf("Value of the text.value is `NULL`!\n");
			printf(
				"Value of the text.value was changed from `NULL` to %ls,\n",
				code[text->ID].value
			);
			fflush(stdout);
		}

		// Update [text] [value]
		text->value=code[text->ID].value;
	}
	else if(wcscmp(text->value, code[text->ID].value)!=0){
		if(wcslen(text->value)<code[text->ID].length-1){
			// Safely update [code] [value]
			wmemcpy(code[text->ID].value, text->value, wcslen(text->value));
			code[text->ID].value[wcslen(text->value)]=L'\0';

			// Update [text] [value]
			text->value[wcslen(text->value)]=L'\0';
		}
		else{
			if(przecinek.debug==true){
				printf("[pWindowDrawText() Warning]\n");
				printf("Value of the text.value is too long!\n");
				printf(
					"Value of the text.value was shorten from %i to %i chars,\n",
					wcslen(text->value), code[text->ID].length-1
				);
				fflush(stdout);
			}

			// Safely pdate [code] [value]
			wmemcpy(code[text->ID].value, text->value, code[text->ID].length-1);
			code[text->ID].value[code[text->ID].length-1]=L'\0';

			// Update [text] [value]
			wcscpy(text->value, code[text->ID].value);
		}

		// Mark [code] [change] as `true`
		code[text->ID].change=true;
	}

	if(code[text->ID].change==false){
		if(text->x!=code[text->ID].x[font->ID] || text->y!=code[text->ID].y[font->ID]){
			// Create local [diffrence]
			pPosition diffrence=
				(pPosition){ (text->x-code[0].x[font->ID]), (text->y-code[0].y[font->ID]) };

			for(uint16_t current=0; current<wcslen(text->value); current+=1){
				// Update [code] [sourcePosition] using local [diffrence] values
				code[text->ID].sourcePosition[font->ID][current].x+=diffrence.x;
				code[text->ID].sourcePosition[font->ID][current].y+=diffrence.y;
			}

			// Update [code] [x] and [y]
			code[text->ID].x[font->ID]=text->x;
			code[text->ID].y[font->ID]=text->y;
		}

		if(text->letterSpacing!=code[text->ID].letterSpacing){
			// Create local [diffrence]
			int32_t diffrence=text->letterSpacing-code[text->ID].letterSpacing;

			for(uint16_t current=0; current<wcslen(text->value); current+=1){
				// Update [code] [sourcePosition] [x] using local [diffrence]
				code[text->ID].sourcePosition[font->ID][current].x+=diffrence;
			}

			// Update [code] [letterSpacing] value
			code[text->ID].letterSpacing=text->letterSpacing;
		}

		if(text->lineSpacing!=code[text->ID].lineSpacing){
			// Create local [diffrence]
			int32_t diffrence=text->lineSpacing-code[text->ID].lineSpacing;

			for(uint16_t current=0; current<wcslen(text->value); current+=1){
				// Update [code] [sourcePosition] [y] using local [diffrence]
				code[text->ID].sourcePosition[font->ID][current].y+=diffrence;
			}

			// Update [code] [lineSpacing] value
			code[text->ID].lineSpacing=text->lineSpacing;
		}

		if(text->spaceSize!=code[text->ID].spaceSize){
			// Create local [spaceCount] and [yCur]
			uint16_t spaceCount=0;
			int32_t yCur=0;

			// Create local [diffrence]
			int32_t diffrence=text->spaceSize-code[text->ID].spaceSize;

			for(uint16_t current=0; current<wcslen(text->value); current+=1){
				if(yCur!=code[text->ID].sourcePosition[font->ID][current].y){
					// Update local [yCur] and reset local [spaceCount]
					yCur=code[text->ID].sourcePosition[font->ID][current].y;
					spaceCount=0;
				}

				// Update [code] [sourcePosition] [x] using local values
				code[text->ID].sourcePosition[font->ID][current].x+=spaceCount*diffrence;
			}

			// Update [code] [spaceSize] value
			code[text->ID].spaceSize=text->spaceSize;
		}
	}
	else{
		// Update [code] [x] and [y]
		code[text->ID].x[font->ID]=text->x;
		code[text->ID].y[font->ID]=text->y;

		// Update [code] spacing values
		code[text->ID].letterSpacing=text->letterSpacing;
		code[text->ID].lineSpacing=text->lineSpacing;
		code[text->ID].spaceSize=text->spaceSize;
	}

	if(view[font->ID].created[text->ID]==false || code[text->ID].change==true){
		// Create local [line] and [xFix]
		uint16_t line=0;
		int32_t xFix=0;

		if(view[font->ID].created[text->ID]==true){
			for(uint16_t current=0; current<code[text->ID].length; current+=1){
				// Reset [code] [source] for current [font]
				glDeleteTextures(1, &code[text->ID].source[font->ID][current]);
			}
		}

		for(uint16_t current=0; current<code[text->ID].length; current+=1){
			// Prepare [code] [source] for current [font]
			glGenTextures(1, &code[text->ID].source[font->ID][current]);
		}

		for(uint16_t current=0; current<wcslen(code[text->ID].value); current+=1){
			if(code[text->ID].value[current]==L'\n'){
				// Reset local [xFix] and increase local [line] value
				xFix=0;
				line+=1;

				continue;
			}
			if(code[text->ID].value[current]==L' '){
				// Modify local [xFix] value
				xFix-=code[text->ID].spaceSize*1.33;

				continue;
			}

			// Create local [width] and [height]
			uint32_t width, height;

			// Create local [widthBox] and [realHeight]
			GpRectF widthBox;
			float realHeight;

			// Get local [widthBox] and [width] value
			GdipMeasureString(
				graphics, &code[text->ID].value[current], 1, view[font->ID].base,
				&(GpRectF){ 0, 0 }, stringFormat, &widthBox, NULL, NULL
			);
			width=widthBox.Width+1;

			// Get local [realHeight] and [height] value
			GdipGetFontHeight(view[font->ID].base, graphics, &realHeight);
			height=realHeight+1;

			// Create and allocate local [charData]
			uint8_t *charData=(uint8_t*)malloc(sizeof(uint8_t)*width*height*4);
			if(charData==NULL){
				if(przecinek.debug==true){
					printf("[pDebugTextSetup() Error]\n");
					printf("Could not allocate enough memory!\n");
					printf("Try to close some background applications,\n");
					fflush(stdout);
				}

				// Return `true`, finished with error
				return true;
			}

			// Create local [buffer] and [graphics]
			GpBitmap *buffer;
			GpGraphics *graphics;

			// Fill [charData] with `0`
			memset(charData, 0, width*height*4);

			// Create [buffer] and [graphics] based on [charData]
			GdipCreateBitmapFromScan0(
				width, height, width*4, PixelFormat32bppARGB,
				charData, &buffer
			);
			GdipGetImageGraphicsContext(buffer, &graphics);

			// Create and setup local [argb]
			ARGB argb=((ARGB)(255<<24) | (255<<16) | (255<<8) | 255);

			// Create local [fill] based on local [argb]
			GpSolidFill *fill;
			GdipCreateSolidFill(argb, &fill);

			// Draw current char on [graphics]
			GdipDrawString(
				graphics, &code[text->ID].value[current], 1, view[font->ID].base,
				&(GpRectF){ 0, 0, width, height }, stringFormat, fill
			);

			// Create local [bufferData]
			BitmapData bufferData;

			// Save [buffer] to [bufferData]
			GdipBitmapLockBits(
				buffer, &(GpRect){ 0, 0, width, height },
				ImageLockModeRead, PixelFormat32bppARGB, &bufferData
			);

			// Setup [code] [source]
			glBindTexture(GL_TEXTURE_2D, code[text->ID].source[font->ID][current]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Initialize [code] [source]
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, bufferData.Scan0
			);

			// Set [code] [sourceSize] values
			code[text->ID].sourceSize[font->ID][current].width=width;
			code[text->ID].sourceSize[font->ID][current].height=height;

			// Set [code] [sourcePosition] values
			code[text->ID].sourcePosition[font->ID][current].x=code[text->ID].x[font->ID]-xFix;
			code[text->ID].sourcePosition[font->ID][current].y=
				code[text->ID].y[font->ID]+(height*line)+(code[text->ID].lineSpacing*1.33*line);

			// Change local [xFix] value
			xFix-=width+(code[text->ID].letterSpacing*1.33);

			// Clean up
			GdipBitmapUnlockBits(buffer, &bufferData);
			GdipDisposeImage(buffer);
			GdipDeleteGraphics(graphics);
			free(charData);
			GdipDeleteBrush(fill);
		}

		// Mark [view] [created] as `true`
		view[font->ID].created[text->ID]=true;

		// Mark [code] [change] as `false`
		code[text->ID].change=false;
	}

	// Return `false`, finished succesfully
	return false;
}

/****************************************************************
* |\_____/| pTextCreate()
* | .     | In: pText* [text], uint16_t [length]
* |     . | wchar_t* [value]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [text] - which [text] structure should be initialized.
* If [text] was already created, then it will be overwritten.
* [length] - maximal length of [text] [value].
* Last position should allways have L'\0' value for safety.
* [value] - initial [text] [value].
*
* Additional Description:
* This function choses [ID] for given [text], then
* it allocates memory, setups all needed values and variables
* and you are ready to go.
****************************************************************/
uint8_t pTextCreate(pText *text, uint16_t length, wchar_t *value){
	if(text==NULL){
		if(przecinek.debug==true){
			printf("[pTextCreate() Error]\n");
			printf("Given text is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	for(uint16_t current=0; current<TEXT_MAX; current+=1){
		if(code[current].exists==false){
			// Set some values to `NULL`
			text->value=NULL;
			code[current].value=NULL;

			// Set [text] [ID] and reset it
			text->ID=current;
			pDebugTextReset(text);
			pDebugCodeReset(text->ID);

			break;
		}
		else if(current==TEXT_MAX-1){
			if(przecinek.debug==true){
				printf("[pTextCreate() Error]\n");
				printf("Too many texts were created!\n");
				printf("Current text limit is equal to %i.\n", TEXT_MAX);
				printf("Try to destroy unused texts or change Przecinek text limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pTextCreate() Error]\n");
		printf("Could not create text!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [text] and [code]
		pDebugTextReset(text);
		pDebugCodeReset(text->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Check [length] value
	if(length<TEXT_LENGTH_MIN){
		if(przecinek.debug==true){
			printf("[pTextCreate() Warning]\n");
			printf("Value of the text.length is too small!\n");
			printf("Value of the text.length was changed from %i to %i,\n", length, TEXT_LENGTH_MIN);
			fflush(stdout);
		}

		length=TEXT_LENGTH_MIN;
	}

	// Check [value]
	if(value==NULL){
		printf("[pTextCreate() Error]\n");
		printf("Could not create text!\n");
		printf("Value of the text.value is `NULL`.\n");
		printf("Make sure to use your brain once in a while,\n");
		fflush(stdout);

		// Reset [text] and [code]
		pDebugTextReset(text);
		pDebugCodeReset(text->ID);

		// Return `4`, finished with error
		return 4;
	}

	// Allocate [text] [value]
	text->value=malloc(sizeof(wchar_t)*length);
	if(text->value==NULL){
		if(przecinek.debug==true){
			printf("[pTextCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [text] and [code]
		pDebugTextReset(text);
		pDebugCodeReset(text->ID);

		// Return `5`, finished with error
		return 5;
	}

	// Set [text] values
	text->length=length;

	if(wcslen(value)<length-1){
		// Safely copy [value] to [text] [value]
		wmemcpy(text->value, value, wcslen(value));
		text->value[wcslen(value)]=L'\0';
	}
	else{
		if(przecinek.debug==true){
			printf("[pTextCreate() Warning]\n");
			printf("Value of the text.value is too long!\n");
			printf(
				"Value of the text.value was shorten from %i to %i chars,\n",
				wcslen(value), length-1
			);
			fflush(stdout);
		}

		// Safely copy [value] to [text] [value]
		wmemcpy(text->value, value, length-1);
		text->value[length-1]=L'\0';
	}

	text->letterSpacing=0;
	text->lineSpacing=0;
	text->spaceSize=TEXT_DEFAULT_SPACE;

	// Allocate [code] [value]
	code[text->ID].value=malloc(sizeof(wchar_t)*length);
	if(code[text->ID].value==NULL){
		if(przecinek.debug==true){
			printf("[pTextCreate() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Reset [text] and [code]
		pDebugTextReset(text);
		pDebugCodeReset(text->ID);

		// Return `6`, finished with error
		return 6;
	}

	for(uint16_t current=0; current<FONT_MAX; current+=1){
		// Allocate [code] [source]
		code[text->ID].source[current]=malloc(sizeof(GLuint)*length);
		if(code[text->ID].source[current]==NULL){
			if(przecinek.debug==true){
				printf("[pTextCreate() Error]\n");
				printf("Could not allocate enough memory!\n");
				printf("Try to close some background applications,\n");
				fflush(stdout);
			}

			// Return `7`, finished with error
			return 7;
		}

		// Allocate [code] [sourceSize]
		code[text->ID].sourceSize[current]=malloc(sizeof(pSize)*length);
		if(code[text->ID].sourceSize[current]==NULL){
			if(przecinek.debug==true){
				printf("[pTextCreate() Error]\n");
				printf("Could not allocate enough memory!\n");
				printf("Try to close some background applications,\n");
				fflush(stdout);
			}

			// Return `8`, finished with error
			return 8;
		}

		// Allocate [code] [sourcePosition]
		code[text->ID].sourcePosition[current]=malloc(sizeof(pPosition)*length);
		if(code[text->ID].sourcePosition[current]==NULL){
			if(przecinek.debug==true){
				printf("[pTextCreate() Error]\n");
				printf("Could not allocate enough memory!\n");
				printf("Try to close some background applications,\n");
				fflush(stdout);
			}

			// Return `9`, finished with error
			return 9;
		}
	}

	// Set [code] values
	code[text->ID].exists=true;

	code[text->ID].length=length;

	// Safely copy [text] [value] to [code] [value]
	wmemcpy(code[text->ID].value, text->value, wcslen(text->value));
	code[text->ID].value[wcslen(text->value)]=L'\0';

	code[text->ID].letterSpacing=0;
	code[text->ID].lineSpacing=0;
	code[text->ID].spaceSize=TEXT_DEFAULT_SPACE;

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pTextDestroy()
* | .     |
* |     . | In: pText* [text]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [text] - which [text] structure should be destroyed.
* Removed debug values will depend on [text] [ID].
****************************************************************/
uint8_t pTextDestroy(pText *text){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pTextDestroy() Error]\n");
			printf("Could not destroy text!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(text==NULL){
		if(przecinek.debug==true){
			printf("[pTextDestroy() Error]\n");
			printf("Given text is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(text->ID>TEXT_MAX){
		if(przecinek.debug==true){
			printf("[pTextDestroy() Error]\n");
			printf("Given text was not created properly!\n");
			printf("Value of the text.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(code[text->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pTextDestroy() Warning]\n");
			printf("Given text was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(code[text->ID].exists==true){
		// Reset [text] and [code]
		pDebugTextReset(text);
		pDebugCodeReset(text->ID);
	}

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pDebugSoundReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugSoundReset(uint16_t ID){
	if(sound[ID].thread!=NULL){
		// Mark [sound] [kill] as `true`
		sound[ID].kill=true;

		// Clean up [sound] [thread] structure
		WaitForSingleObject(sound[ID].thread, INFINITE);
		CloseHandle(sound[ID].thread);
		sound[ID].thread=NULL;
	}

	// Mark [sound] [exists] as `false`
	sound[ID].exists=false;

	return;
}

/****************************************************************
* |\_____/| pDebugAudioReset()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
void pDebugAudioReset(pAudio *audio){
	// Set [sound] [pause] status to `1`
	InterlockedExchange(&sound[audioPoint->ID].pause, 1);

	// Reset [audio] [volume]
	audio->volume=0;
	// Mark [audio] [pause] as `false`
	audio->pause=false;

	// Reset [audio] [frame] and [frameMax]
	audio->frame=0;
	audio->frameMax=0;

	return;
}

/****************************************************************
* |\_____/| pDebugAudioProc()
* | .     |
* |     . | [DEBUG]
* \ = , = /
****************************************************************/
unsigned int __stdcall pDebugAudioProc(void *argument){
	// Load local [audioPoint] from [argument]
	pAudioPoint *audioPoint=(pAudioPoint*)argument;

	// Try to initialize multithread
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not be initialized!\n");
			printf("Try to update your Windows machine or recompile Przecinek,\n");
			fflush(stdout);
		}

		// Clean up
		free(audioPoint->directory);

		// Return `1`, finished with error
		return 1;
	}

	// Create local [fileHandle] and load [audioPoint] [directory]
	HANDLE fileHandle=CreateFileW(
		audioPoint->directory, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);
	if(fileHandle==INVALID_HANDLE_VALUE){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if audio.directory value is correct and not corrupted\n");
			fflush(stdout);
		}

		// Clean up
		free(audioPoint->directory);

		// Return `2`, finished with error
		return 2;
	}

	// Create local [fileSize]
	size_t fileSize=GetFileSize(fileHandle, NULL);
	if(fileSize==INVALID_FILE_SIZE){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't too big or corruped,\n");
			fflush(stdout);
		}

		// Clean up
		CloseHandle(fileHandle);
		free(audioPoint->directory);

		// Return `3`, finished with error
		return 3;
	}

	// Create local [fileBuffer] and setup it
	uint8_t *fileBuffer=(uint8_t*)malloc(fileSize);
	if(fileBuffer==NULL){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("Could not allocate enough memory!\n");
			printf("Try to close some background applications,\n");
			fflush(stdout);
		}

		// Clean up
		CloseHandle(fileHandle);
		free(audioPoint->directory);

		// Return `4`, finished with error
		return 4;
	}

	// Create local [fileRead]
	DWORD fileRead=0;
	if(ReadFile(fileHandle, fileBuffer, fileSize, &fileRead, NULL)==false || fileRead!=fileSize){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		CloseHandle(fileHandle);
		free(fileBuffer);
		free(audioPoint->directory);

		// Return `5`, finished with error
		return 5;
	}

	// Safely close local [fileHandle]
	CloseHandle(fileHandle);

	// Create local [fileEnd] and set its value
	uint8_t *fileEnd=fileBuffer+fileSize;
	if(fileEnd-fileBuffer<12 || memcmp(fileBuffer, "RIFF", 4) || memcmp(fileBuffer+8, "WAVE", 4)){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		free(fileBuffer);
		free(audioPoint->directory);

		// Return `6`, finished with error
		return 6;
	}

	// Create local [fileFormat] and [dataPoint]
	WAVEFORMATEX *fileFormat=NULL;
	uint8_t *dataPoint=NULL;

	// Create local [fileOffset], [dataSize] and [chunkSize]
	uint32_t fileOffset=12, dataSize, chunkSize;

	while(fileBuffer+fileOffset+8<=fileEnd){
		// Create and setup local [wavID]
		char wavID[5]={0};
		memcpy(wavID, fileBuffer+fileOffset, 4);

		// Update local [fileOffset] value
		fileOffset+=8;

		// Update local [chunkSize]
		chunkSize=*(uint32_t*)(fileBuffer+fileOffset-4);

		if(fileBuffer+fileOffset+chunkSize>fileEnd){ break; }

		if(memcmp(wavID, "fmt ", 4)==false){
			if(chunkSize>=16){
				// Create and allocate local [wavFormat]
				WAVEFORMATEX *wavFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX)+300);
				if(wavFormat==NULL){
					if(przecinek.debug==true){
						printf("[pDebugAudioProc() Error]\n");
						printf("Could not allocate enough memory!\n");
						printf("Try to close some background applications,\n");
						fflush(stdout);
					}

					// Clean up
					free(fileBuffer);
					free(audioPoint->directory);

					// Return `7`, finished with error
					return 7;
				}

				// Fill local [wavFormat] with `0`
				memset(wavFormat, 0, sizeof(WAVEFORMATEX)+300);

				// Setup local [wavFormat] with this binary shit
				wavFormat->wFormatTag=*(uint16_t*)(fileBuffer+fileOffset);
				wavFormat->nChannels=*(uint16_t*)(fileBuffer+fileOffset+2);
				wavFormat->nSamplesPerSec=*(uint32_t*)(fileBuffer+fileOffset+4);
				wavFormat->nAvgBytesPerSec=*(uint32_t*)(fileBuffer+fileOffset+8);
				wavFormat->nBlockAlign=*(uint16_t*)(fileBuffer+fileOffset+12);
				wavFormat->wBitsPerSample=*(uint16_t*)(fileBuffer+fileOffset+14);
				if(chunkSize>16){
					wavFormat->cbSize=(uint16_t)(chunkSize-16);
					if(wavFormat->cbSize>0){
						memcpy(
							((uint8_t*)wavFormat)+sizeof(WAVEFORMATEX),
							fileBuffer+fileOffset+18, wavFormat->cbSize
						);
					}
				}

				// Set local [fileFormat] value to local [wavFormat]
				fileFormat=wavFormat;
			}
		}
		else if(memcmp(wavID, "data", 4)==false){
			// Copy some local typeshit stuff
			dataSize=chunkSize;
			dataPoint=(uint8_t*)malloc(dataSize);
			if(dataPoint!=NULL){ memcpy(dataPoint, fileBuffer+fileOffset, dataSize); }
		}

		// Update [fileOffset] value
		fileOffset+=chunkSize+(chunkSize&1);
	}

	// Check for any errors in the meantime
	if(fileFormat==NULL || dataPoint==NULL){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		// Return `8`, finished with error
		return 8;
	}

	// Create local [enumerator]
	IMMDeviceEnumerator *enumerator=NULL;

	// Initialize [enumerator]
	if(FAILED(CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
			&IID_IMMDeviceEnumerator, (void**)&enumerator))){

		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `9`, finished with error
		return 9;
	}

	// Create local [device]
	IMMDevice *device=NULL;

	if(FAILED(enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator, eRender, eConsole, &device))){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		enumerator->lpVtbl->Release(enumerator);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `10`, finished with error
		return 10;
	}

	// Safely release local [enumerator]
	enumerator->lpVtbl->Release(enumerator);

	// Create local [audioClient]
	IAudioClient *audioClient=NULL;

	if(FAILED(device->lpVtbl->Activate(
			device, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient))){

		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		device->lpVtbl->Release(device);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `11`, finished with error
		return 11;
	}

	if(FAILED(audioClient->lpVtbl->Initialize(
			audioClient, AUDCLNT_SHAREMODE_SHARED,
			AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
			(REFERENCE_TIME)AUDIO_REFRESH_RATE, 0, fileFormat, NULL))){

		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		audioClient->lpVtbl->Release(audioClient);
		device->lpVtbl->Release(device);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `12`, finished with error
		return 12;
	}

	// Create local [renderClient]
	IAudioRenderClient *renderClient=NULL;

	if(FAILED(audioClient->lpVtbl->GetService(
			audioClient, &IID_IAudioRenderClient, (void**)&renderClient))){

		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		audioClient->lpVtbl->Release(audioClient);
		device->lpVtbl->Release(device);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `13`, finished with error
		return 13;
	}

	// Create local [volumeClient]
	ISimpleAudioVolume *volumeClient;

	if(FAILED(audioClient->lpVtbl->GetService(
		audioClient, &IID_ISimpleAudioVolume, (void**)&volumeClient))){

		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		renderClient->lpVtbl->Release(renderClient);
		audioClient->lpVtbl->Release(audioClient);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `14`, finished with error
		return 14;
	}

	// Safely release local [device]
	device->lpVtbl->Release(device);

	// Create local [frameCount]
	uint32_t frameCount;

	// Load local [frameCount]
	if(FAILED(audioClient->lpVtbl->GetBufferSize(audioClient, &frameCount))){
		if(przecinek.debug==true){
			printf("[pDebugAudioProc() Error]\n");
			printf("CoreAudio library could not load audio!\n");
			printf("Check if your file isn't corruped,\n");
			fflush(stdout);
		}

		// Clean up
		renderClient->lpVtbl->Release(renderClient);
		audioClient->lpVtbl->Release(audioClient);

		if(fileFormat!=NULL){ free(fileFormat); }
		if(dataPoint!=NULL){ free(dataPoint); }

		free(fileBuffer);
		free(audioPoint->directory);

		CoUninitialize();

		// Return `15`, finished with error
		return 15;
	}

	// Create local [frameByte] and [frameTotal]
	uint32_t frameByte, frameTotal;

	// Calculate local [frameByte] and [frameTotal]
	frameByte=fileFormat->nBlockAlign;
	frameTotal=dataSize/frameByte;

	// Create local [frameWrite]
	uint32_t frameWrite=0;

	// Set [sound] [pause] status to `1`
	InterlockedExchange(&sound[audioPoint->ID].pause, 1);

	while(sound[audioPoint->ID].kill==false){
		// Update [sound] [frameMaxPoint] value
		*sound[audioPoint->ID].frameMaxPoint=frameTotal;

		if(*sound[audioPoint->ID].framePoint!=frameWrite){
			// Check [sound] [framePoint] value
			if(*sound[audioPoint->ID].framePoint>frameTotal){
				if(przecinek.debug==true){
					printf("[pDebugAudioProc() Warning]\n");
					printf("Value of the audio.frame is larger than audio.frameMax!\n");
					printf(
						"Value of the audio.frame was changed from %i to %i,\n",
						*sound[audioPoint->ID].framePoint, frameTotal
					);
					fflush(stdout);
				}

				// Change [sound] [framePoint] value
				*sound[audioPoint->ID].framePoint=frameTotal;
			}

			// Update local [frameWrite] value
			frameWrite=*sound[audioPoint->ID].framePoint;
		}
		if(frameWrite>=frameTotal){
			// Reset local [frameWrite] value
			frameWrite=0;
		}

		if(sound[audioPoint->ID].pause==true){
			if(*sound[audioPoint->ID].pausePoint==false){
				// Set [sound] [pause] status to `0`
				InterlockedExchange(&sound[audioPoint->ID].pause, 0);

				// Unpause local [audioClient]
				audioClient->lpVtbl->Start(audioClient);

				continue;
			}

			// Sleep for `AUDIO_SLEEP_RATE`
			Sleep(AUDIO_SLEEP_RATE);

			continue;
		}
		else if(*sound[audioPoint->ID].pausePoint==true){
			// Set [sound] [pause] status to `1`
			InterlockedExchange(&sound[audioPoint->ID].pause, 1);

			continue;
		}

		// Check [sound] [volumePoint] value
		if(*sound[audioPoint->ID].volumePoint>AUDIO_VOLUME_MAX){
			if(przecinek.debug==true){
				printf("[pDebugAudioProc() Warning]\n");
				printf("Value of the audio.volume is too large!\n");
				printf(
					"Value of the audio.volume was changed from %i to %i,\n",
					*sound[audioPoint->ID].volumePoint, AUDIO_VOLUME_MAX
				);
				fflush(stdout);
			}

			// Change [sound] [volumePoint] value
			*sound[audioPoint->ID].volumePoint=AUDIO_VOLUME_MAX;
		}

		// Create and calculate local [percent] variable
		float percent=(float)*sound[audioPoint->ID].volumePoint/100;

		// Set [percent] for local [volumeClient]
		volumeClient->lpVtbl->SetMasterVolume(volumeClient, percent, NULL);

		// Create and check local [padding]
		uint32_t padding;
		if(FAILED(audioClient->lpVtbl->GetCurrentPadding(audioClient, &padding))){ break; }

		// Create and calculate local [frameAvail]
		uint16_t frameAvail=frameCount-padding;

		// Create local [framePause]
		bool framePause=false;

		// Check local [frameAvail] value
		if(frameAvail==0){
			// Sleep for `AUDIO_PROCESS_RATE`
			Sleep(AUDIO_PROCESS_RATE);

			continue;
		}
		else if(frameWrite+frameAvail>=frameTotal){
			// Update local [frameAvail] value
			frameAvail=frameTotal-frameWrite;

			// Mark [framePause] as `true`
			framePause=true;
		}

		// Create and check local [checkData]
		uint8_t *checkData=NULL;
		if(FAILED(renderClient->lpVtbl->GetBuffer(renderClient, frameAvail, &checkData))){ break; }

		// Create and calculate local [data]
		uint8_t *data=dataPoint+(frameWrite*frameByte);

		if(fileFormat->wBitsPerSample==8){
			for(uint32_t current=0; current<frameAvail*frameByte; current+=1){
				// Create and calculate local [calculation]
				int32_t calculation=data[current]-128;
				calculation=calculation*percent;

				if(calculation+128<0){ calculation=0; }
				else if(calculation+128>255){ calculation=255; }

				// Set local [checkData] value
				checkData[current]=(uint8_t)calculation;
			}
		}
		else if(fileFormat->wBitsPerSample==16){
			for(uint32_t current=0; current<frameAvail*(frameByte/2); current+=1){
				// Create and calculate local [calculation]
				float calculation=(float)((int16_t*)data)[current]*percent;
				if(calculation>SHRT_MAX){ calculation=SHRT_MAX; }
				if(calculation<SHRT_MIN){ calculation=SHRT_MIN; }

				// Set local [checkData] value
				((int16_t*)checkData)[current]=(int16_t)calculation;
			}
		}
		else if(fileFormat->wBitsPerSample==24){
			for(uint32_t frame=0; frame<frameAvail; frame+=1){
				for(uint32_t current=0; current<fileFormat->nChannels; current+=1){
					// Change local [fileOffset] value
					fileOffset=(frame*frameByte)+(current*3);

					// Create and calculate local [sample]
					int32_t sample=(int32_t)(
						data[fileOffset] | (data[fileOffset+1]<<8) | (data[fileOffset+2]<<16)
					);
					if(sample&0x800000){ sample|=~0xFFFFFF; }

					// Create and calculate local [calculation]
					float calculation=sample*percent;

					if(calculation>8388607.f){ calculation=8388607.f; }
					if(calculation<(-8388608.f)){ calculation=(-8388608.f); }

					// Set local [checkData] values
					checkData[fileOffset]=(uint8_t)((int32_t)calculation&0xFF);
					checkData[fileOffset+1]=(uint8_t)(((int32_t)calculation>>8)&0xFF);
					checkData[fileOffset+2]=(uint8_t)(((int32_t)calculation>>16)&0xFF);
				}
			}
		}
		else{
			// Copy local [data] to local [checkData]
			memcpy(checkData, data, frameAvail*frameByte);
		}

		// Check for any errors
		if(FAILED(renderClient->lpVtbl->ReleaseBuffer(renderClient, frameAvail, 0))){ break; }

		// Modify [frameWrite] value
		frameWrite+=frameAvail;

		if(framePause==true){
			// Mark [sound] [pausePoint] as `true`
			*sound[audioPoint->ID].pausePoint=true;

			// Set [sound] [pause] status to `1`
			InterlockedExchange(&sound[audioPoint->ID].pause, 1);
		}

		// Create and calculate local [calculation]
		float calculation=ceil(frameWrite);

		// Update [sound] [framePoint] value
		*sound[audioPoint->ID].framePoint=(uint32_t)calculation;
	}

	// Clean up
	free(audioClient);
	free(renderClient);
	free(volumeClient);

	if(fileFormat!=NULL){ free(fileFormat); }
	if(dataPoint!=NULL){ free(dataPoint); }
	free(fileBuffer);
	free(audioPoint->directory);

	CoUninitialize();

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pAudioCreate()
* | .     |
* |     . | In: pAudio* [audio], wchar_t* [directory]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [audio] - which [audio] structure should be initialized.
* If [audio] was already created, then it will be overwritten.
* [directory] - from where the [audio] should be loaded.
*
* Additional Description:
* This function choses [ID] for given [audio], then
* it setups all needed values and variables, loads file from
* given [directory] and creates [audio] debug thread.
* Debug thread works non stop even if [audio] is paused. It
* properly decodes [audio] file on creation. Then it is used
* to properly play/pause [audio].
****************************************************************/
uint8_t pAudioCreate(pAudio *audio, wchar_t *directory){
	if(audio==NULL){
		if(przecinek.debug==true){
			printf("[pAudioCreate() Error]\n");
			printf("Given audio is `NULL`!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	for(uint16_t current=0; current<AUDIO_MAX; current+=1){
		if(sound[current].exists==false){
			// Set some values to `NULL`
			sound[current].thread=NULL;

			// Set [audio] [ID] and reset it
			audio->ID=current;
			pDebugAudioReset(audio);
			pDebugSoundReset(audio->ID);

			break;
		}
		else if(current==AUDIO_MAX-1){
			if(przecinek.debug==true){
				printf("[pAudioCreate() Error]\n");
				printf("Too many audios were created!\n");
				printf("Current audio limit is equal to %i.\n", AUDIO_MAX);
				printf("Try to destroy unused sounds or change Przecinek audio limit,\n");
				fflush(stdout);
			}

			// Return `2`, finished with error
			return 2;
		}
	}

	// Check if Przecinek is initialized
	if(setup==false){
		printf("[pAudioCreate() Error]\n");
		printf("Could not create audio!\n");
		printf("Przecinek is not initialized.\n");
		printf("Try to run pSetup() first,\n");
		fflush(stdout);

		// Reset [audio] and [sound]
		pDebugAudioReset(audio);
		pDebugSoundReset(audio->ID);

		// Return `3`, finished with error
		return 3;
	}

	// Set [audio] values
	audio->pause=true;
	audio->volume=100;

	audio->frame=0;
	audio->frameMax=0;

	// Set [sound] values
	sound[audio->ID].exists=true;

	sound[audio->ID].kill=false;

	// Allocate [sound] pointers
	sound[audio->ID].volumePoint=&audio->volume;
	sound[audio->ID].pausePoint=&audio->pause;

	sound[audio->ID].framePoint=&audio->frame;
	sound[audio->ID].frameMaxPoint=&audio->frameMax;

	// Check [directory] value
	if(wcslen(directory)<4 || wcscmp(directory+wcslen(directory)-4, L".wav")!=0){
		if(przecinek.debug==true){
			printf("[pAudioCreate() Error]\n");
			printf("Value of the audio.directory doesn't include .wav extenstion!\n");
			printf("Check if you gave the correct value,\n");
			fflush(stdout);
		}

		// Reset [audio] and [sound]
		pDebugAudioReset(audio);
		pDebugSoundReset(audio->ID);

		// Return `4`, finished with error
		return 4;
	}

	// Setup [audioPoint] for [audio]
	audioPoint[audio->ID].ID=audio->ID;
	audioPoint[audio->ID].directory=_wcsdup(directory);

	// Create local [threadID]
	unsigned int threadID;

	// Initialize [sound] [thread]
	sound[audio->ID].thread=(HANDLE)_beginthreadex(
		NULL, 0, pDebugAudioProc, &audioPoint[audio->ID], 0, &threadID
	);

	// Return `0`, finished succesfully
	return 0;
}

/****************************************************************
* |\_____/| pAudioDestroy()
* | .     |
* |     . | In: pAudio* [audio]
* \ = , = / Out: uint8_t (`0` -> finished succesfully)
*
* Parameters:
* [audio] - which [audio] structure should be destroyed.
* Removed debug values will depend on [audio] [ID].
****************************************************************/
uint8_t pAudioDestroy(pAudio *audio){
	if(setup==false){
		if(przecinek.debug==true){
			printf("[pAudioDestroy() Error]\n");
			printf("Could not destroy audio!\n");
			printf("Przecinek is not initialized.\n");
			printf("Try to run pSetup() first,\n");
			fflush(stdout);
		}

		// Return `1`, finished with error
		return 1;
	}

	if(audio==NULL){
		if(przecinek.debug==true){
			printf("[pAudioDestroy() Error]\n");
			printf("Given audio `is `NULL``!\n");
			printf("Make sure to use your brain once in a while,\n");
			fflush(stdout);
		}

		// Return `2`, finished with error
		return 2;
	}
	else if(audio->ID>AUDIO_MAX){
		if(przecinek.debug==true){
			printf("[pAudioDestroy() Error]\n");
			printf("Given audio was not created properly!\n");
			printf("Value of the audio.ID is out of range.\n");
			printf("Try to not use corrupted variables next time,\n");
			fflush(stdout);
		}

		// Return `3`, finished with error
		return 3;
	}
	else if(sound[audio->ID].exists==false){
		if(przecinek.debug==true){
			printf("[pAudioDestroy() Warning]\n");
			printf("Given audio was already destroyed!\n");
			printf("Not sure what you were trying todo :-PP,\n");
			fflush(stdout);
		}
	}

	if(sound[audio->ID].exists==true){
		// Reset [audio] and [sound]
		pDebugAudioReset(audio);
		pDebugSoundReset(audio->ID);
	}

	// Return `0`, finished succesfully
	return 0;
}