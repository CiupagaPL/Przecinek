/*
*       |\____/| github.com/ciupagacz/przecinek
*       |      | original {,} library
*  __   | >  < |
* /  \__\ =, = / copyright (c) 2025-2026
* \__         /
*    \ \____\ \
*    {,{,} {,},}
*/

// Standard C libraries
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef PRZECINEK_H
#define PRZECINEK_H

#ifdef __cplusplus
extern "C"
{
	#endif

	// Basic size structure
	typedef struct{ uint16_t width, height; } pSize;
	// Basic point structure
	typedef struct{ int32_t x, y; } pPoint;
	// Basic color structure
	typedef struct{ uint8_t red, green, blue, alpha; } pColor;

	// Przecinek version
	#define PRZECINEK_STABLE_BRANCH L"a"
	#define PRZECINEK_STABLE_MAJOR 6
	#define PRZECINEK_STABLE_MINOR 1
	#define PRZECINEK_STABLE_PATCH L"f"
	#define PRZECINEK_UNSTABLE 31

	// Frame limits for all windows
	// Value higher than `480` is very unsafe!
	#define PRZECINEK_FRAME_DEFAULT (uint16_t)60
	#define PRZECINEK_FRAME_MIN (uint16_t)10
	#define PRZECINEK_FRAME_MAX (uint16_t)120

	// Maximal amount of pressed keys at one time
	#define PRZECINEK_KEY_PRESS_MAX (uint8_t)8

	// Default color pallet
	#define COLOR_DEFAULT_BACKGROUND (pColor){ 255, 255, 255, 255 }
	#define COLOR_DEFAULT_FOREGROUND (pColor){ 50, 50, 60, 255 }

	// Structures allocated count
	#define WINDOW_MAX (uint8_t)1
	#define OBJECT_MAX (uint16_t)256
	#define IMAGE_MAX (uint16_t)128
	#define FONT_MAX (uint16_t)4
	#define TEXT_MAX (uint16_t)64
	#define AUDIO_MAX (uint16_t)32

	// Window minimal and maximal size
	// Value higher than `SHRT_MAX/2` is very unsafe!
	#define WINDOW_WIDTH_MIN (uint16_t)128
	#define WINDOW_HEIGHT_MIN (uint16_t)128
	// Window initial minimal size
	#define WINDOW_WIDTH_MIN_DEFAULT (uint16_t)128
	#define WINDOW_HEIGHT_MIN_DEFAULT (uint16_t)128
	// Window initial maximal size
	#define WINDOW_WIDTH_MAX_DEFAULT (uint16_t)3840
	#define WINDOW_HEIGHT_MAX_DEFAULT (uint16_t)2160
	// Window initial position
	#define WINDOW_X_DEFAULT (int16_t)256
	#define WINDOW_Y_DEFAULT (int16_t)256

	// Window default title
	// String longer than `255` chars is very unsafe!
	#define WINDOW_TITLE_DEFAULT L"window"

	// Object minimal and maximal rotation value
	#define OBJECT_ROTATION_MIN (int16_t)-360
	#define OBJECT_ROTATION_MAX (int16_t)360

	// Object maximal vertex amount
	#define OBJECT_VERTEX_MAX (uint16_t)512

	// Image default pixelated status
	#define IMAGE_PIXELATED_DEFAULT false

	// Text size limits
	#define TEXT_SIZE_DEFAULT (int32_t)64
	#define TEXT_SIZE_MIN (int32_t)4
	#define TEXT_SIZE_MAX (int32_t)4096
	// Text default letter and line spacing size
	#define TEXT_LETTER_SPACING_DEFAULT (int32_t)0
	#define TEXT_LINE_SPACING_DEFAULT (int32_t)0
	// Text default space size
	#define TEXT_SPACE_SIZE_DEFAULT (int32_t)32

	// Maximal audio volume in percent
	#define AUDIO_VOLUME_MAX (uint16_t)200

	// Audio refresh rate
	#define AUDIO_REFRESH_RATE (uint32_t)200000
	// Audio thread delays used for diffrent purposes
	// Experiment with it for quicker/slower audio changes
	// Too radical values can mess up debug audio thread
	#define AUDIO_SLEEP_RATE (uint8_t)40
	#define AUDIO_PROCESS_RATE (uint8_t)10

	// Debug structure
	typedef struct
	{
		// Global frame limit and frame count
		uint16_t frameLimit, frameCount;

		// Main screen size
		pSize display;
		// Cursor position on the screen
		pPoint cursor;
	} pDebug;

	// Declare [przecinek] variable
	extern pDebug przecinek;

	/*
	*  ,____, [pWindow] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint8_t ID;
		bool active;

		// Position on the screen
		int16_t x, y;
		// Size of the window
		uint16_t width, height;
		// Size limits of the window
		// Used only if window is resizable
		uint16_t widthMin, heightMin, widthMax, heightMax;

		// Window title
		wchar_t *title;

		// Window full screen check
		bool fullScreen;
		// Window focus status
		// Modified by the library automatically
		bool focus;
	} pWindow;

	/*
	*  ,____, [pObject] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint16_t ID;

		// Vertex count
		uint16_t vertexCount;
		// Vertex positions array
		pPoint *vertex;
	} pObject;

	/*
	*  ,____, [pImage] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint16_t ID;

		// Texture filtering mode
		// Set `true` if you want to upscale pixel art
		bool pixelated;
	} pImage;

	/*
	*  ,____, [pText] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint16_t ID;

		// Position on the window
		int32_t x, y;

		// Text allocated maximal length
		// Is decreased by `1` automatically for `NULL` pointer
		uint16_t length;
		// Text value
		wchar_t *value;
		// Text/Font size
		uint16_t size;

		// Text letter and line spacing
		int32_t letterSpacing, lineSpacing;
		// Width of the ' ' character
		int32_t spaceSize;
	} pText;

	/*
	*  ,____, [pFont] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint16_t ID;
	} pFont;

	/*
	*  ,____, [pAudio] structure
	*  |    |
	*  |____|
	* (------)
	*/
	typedef struct
	{
		uint16_t ID;

		// Volume percentage
		// Value range from `0` to the AUDIO_VOLUME_MAX
		uint16_t volume;
		// Audio pause state
		bool pause;

		// Current and maximal frame of the audio
		uint32_t frame, frameMax;
	} pAudio;

	/*
	* |\____/| pSetup()
	* |      |
	* | o  o | In: bool [debug]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [debug] - error/warning messages status.
	*
	* > Description:
	* This function initializes Przecinek library. It setups locale and
	* libraries used later. It creates several global debug objects, setups
	* all needed values and variables, opens debug console and downloads
	* initial values of [przecinek] variable.
	*/
	int8_t pSetup(bool debug);

	/*
	* |\____/| pUpdate()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Description:
	* This function updates all windows and global variable values.
	* Firstly it updates main debug [window] [ID], then it refreshes
	* input and downloads several [przecinek] values. After that it
	* loads signals from debug threads, handles all [window] actions
	* and modifies several debug values. In the end it stops main loop
	* for a short amount of time and updates all frame related stuff.
	*/
	int8_t pUpdate();

	/*
	* |\____/| pKeyPress()
	* |      |
	* | o  o | In: wchar_t* [key]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [key] - which [key] status should be checked.
	*
	* > Description:
	* This function checks if [key] is being pressed. Returned value will
	* be equal to `true` for one frame only. Before the [key] is being
	* checked function searches for a debug [key] code. The code depends
	* on the operating system and used libraries.
	*/
	int8_t pKeyPress(const wchar_t *key);

	/*
	* |\____/| pKeyHold()
	* |      |
	* | o  o | In: wchar_t* [key]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [key] - which [key] status should be checked.
	*
	* > Description:
	* This function checks if [key] is being hold. Returned value will
	* be equal to `true` only after the first frame. Before the [key] is
	* being checked function searches for a debug [key] code. The code
	* depends on the operating system and used libraries.
	*/
	int8_t pKeyHold(const wchar_t *key);

	/*
	* |\____/| pKeyCaps()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Caps Lock key status.
	*/
	int8_t pKeyCaps();

	/*
	* |\____/| pKeyNum()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Num Lock key status.
	*/
	int8_t pKeyNum();

	/*
	* |\____/| pKeyScroll()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Scroll Lock key status.
	*/
	int8_t pKeyScroll();

	/*
	* |\____/| pEndup()
	* |      |
	* | o  o | In:
	* \ = .= / Out:
	*
	* > Description:
	* This function safely destroys structures and cleans memory before
	* the end of the program. It firstly uninitializes all debug libraries.
	* Then it searches for all undestroyed structures and removes them.
	* In the end it resets [przecinek] and several debug variables.
	*/
	void pEndup();

	/*
	* |\____/| pWindowCreate()
	* |      | In: pWindow *window, uint16_t [width], [height],
	* | o  o | bool [resizable]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] structure should be initialized.
	* If given [window] is already created, then it will be overwritten.
	* [width], [height] - initial size of a newly created [window].
	* [resizable] - should the [window] be resizable or constant.
	*
	* > Description:
	* This function choses [ID] for given [window], then it creates debug
	* process, setups all needed values and variables, setups debug pointers
	* to the [window] structure, then it creates physical [window] and
	* creates second GL buffer. It also creates debug [window] thread which
	* loads and responses to all signals. In the end it fills background
	* using `COLOR_BACKGROUND_DEFAULT` fill.
	*/
	int8_t pWindowCreate(
		pWindow *window,
		uint16_t width,
		uint16_t height,
		bool resizable
	);

	/*
	* |\____/| pWindowDrawObject()
	* |      | In: pWindow* [window], pObject* [object], pImage* [image],
	* | o  o | pColor* [color], uint16_t [colorCount]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] should be used for rendering.
	* [object] - which [object] should be rendered on [window].
	* [image] - in which [image] fill should [object] be rendered in.
	* Can be `NULL`. Can be mixed with [color].
	* [color] - in which [color] fill should [object] be rendered in.
	* Can be `NULL`. Can be multiple. Can be mixed with [image].
	* [colorCount] - how many [color] values should be used.
	* If [vertexCount] is bigger than the [colorCount], then [color]
	* will be used interchangeably. If value is equal to `0`,
	* then `COLOR_FOREGROUND_DEFAULT` will be used for rendering instead.
	*
	* > Description:
	* This function uses GL to render [object] on the given [window].
	* Before any action is taken, it checks and refreshes [object] values
	* if needed. If any [vertex] was modified, then debug values are
	* being recalculated. Then when everything is prepared [color]
	* and [image] are being applied to the GL and [object] is being
	* rendered. If [image] source is `NULL` then default texture is
	* being used instead. If [object] is fully outside the rendering
	* area then it is being skipped.
	*/
	int8_t pWindowDrawObject(
		pWindow *window,
		pObject *object,
		pImage *image,
		pColor *color,
		uint16_t colorCount
	);

	/*
	* |\____/| pWindowDrawText()
	* |      | In: pWindow* [window], pText* [text], pFont* [font],
	* | o  o | pColor* [color], uint16_t [colorCount]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] should be used for rendering.
	* [text] - which [text] should be drawn.
	* [font] - which [font] should be used to render [text].
	* [color] - in which [color] fill should [text] be rendered in.
	* Can be `NULL`. Can be multiple.
	* [colorCount] - how many [color] values should be used. If value
	* is smaller than `4`, then [color] values will be used interchangeably.
	* If value is equal to `0`, then `COLOR_FOREGROUND_DEFAULT` will be
	* used for rendering instead.
	*
	* > Description:
	* This function uses GL to render [text] using [font] on the
	* given [window]. Before any action is taken, it checks and
	* refreshes [text] and [font] values if needed. Depending on what
	* changes were made either position values of all [text] buffers are
	* being recalculated or [text] debug values are being reseted and
	* regenerated. Then when everything is prepared [color] is being
	* applied to the GL and [text] buffers are being rendered. If
	* any [text] buffer is fully outside the rendering area then it is
	* being skipped.
	*/
	int8_t pWindowDrawText(
		pWindow *window,
		pText *text,
		pFont *font,
		pColor *color,
		uint16_t colorCount
	);

	/*
	* |\____/| pWindowDestroy()
	* |      |
	* | o  o | In: pWindow* [window]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] structure should receive kill signal.
	* Removed debug values will depend on [window] [ID].
	*/
	int8_t pWindowDestroy(pWindow* window);

	/*
	* |\____/| pObjectCreate()
	* |      |
	* | o  o | In: pObject* [object], uint16_t [vertexCount]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be initialized.
	* If [object] was already created, then it will be overwritten.
	* [vertexCount] - how many [vertex] variables should be allocated.
	*
	* > Description:
	* This function choses [ID] for given [object], then it setups all
	* needed values and variables and allocates [vertexCount] in the memory.
	*/
	int8_t pObjectCreate(
		pObject *object,
		uint16_t vertexCount
	);

	/*
	* |\____/| pObjectGenerateShape()
	* |      | In: pObject* [object], uint16_t [width], [height]
	* | o  o | int16_t [rotation]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be generated.
	* [width], [height] - how big should the [object] be.
	* [rotation] - how rotated should the [object] be.
	*
	* > Description:
	* This function generates [object] shape based on given parameters.
	* It automatically calculates [vertex] positions and creates geometric
	* figure. It also prepares debug variables used later for rendering.
	*/
	int8_t pObjectGenerateShape(
		pObject *object,
		uint16_t width,
		uint16_t height,
		int16_t rotation
	);

	/*
	* |\____/| pObjectMove()
	* |      |
	* | o  o | In: pObject* [object], int32_t [x], [y]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be moved.
	* [x], [y] - what should be the [object] start position.
	*
	* > Description:
	* This function moves [object] structure based on given parameters.
	* Initial position of the [object] is allways equal to the [object]
	* minimal [vertex] position.
	*/
	int8_t pObjectMove(
		pObject *object,
		int32_t x,
		int32_t y
	);

	/*
	* |\____/| pObjectCollisionSimple()
	* |      |
	* | o  o | In: pObject* [objectA], [objectB]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [objectA], [objectB] - which structures should be checked.
	*
	* > Description:
	* This function checks for collision between two objects using their
	* debug size values. This method is much faster but less accurate
	* and works only well with rectangles.
	*/
	int8_t pObjectCollisionSimple(
		pObject *objectA,
		pObject *objectB
	);

	/*
	* |\____/| pObjectCollisionComplex()
	* |      |
	* | o  o | In: pObject* [objectA], [objectB]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [objectA], [objectB] - which structures should be checked.
	*
	* > Description:
	* This function checks for collision between two objects using
	* their triangular mini-collisions. Firstly it checks if simple
	* collision is being found. Then it divides both objects into
	* triangles using ear clipping method. In the end it checks if
	* any triangles collides with each other. This method is much slower
	* but very accurate and works well with pretty much everything.
	*/
	int8_t pObjectCollisionComplex(
		pObject *objectA,
		pObject *objectB
	);

	/*
	* |\____/| pObjectDestroy()
	* |      |
	* | o  o | In: pObject* [object]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be destroyed.
	* Removed debug values will depend on [object] [ID].
	*/
	int8_t pObjectDestroy(pObject *object);

	/*
	* |\____/| pImageCreate()
	* |      |
	* | o  o | In: pImage* [image], wchar_t* [directory]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [image] - which [image] structure should be initialized.
	* If [image] was already created, then it will be overwritten.
	* [directory] - from where the [image] should be loaded.
	*
	* > Description:
	* This function choses [ID] for given [image], then it setups all
	* needed values and variables, loads [image] from given [directory]
	* and creates proper [image] debug GL bitmap for future rendering.
	*/
	int8_t pImageCreate(
		pImage *image,
		const wchar_t *directory
	);

	/*
	* |\____/| pImageDestroy()
	* |      |
	* | o  o | In: pImage* [image]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [image] - which [image] structure should be destroyed.
	* Removed debug values will depend on [image] [ID].
	*/
	int8_t pImageDestroy(pImage *image);

	/*
	* |\____/| pTextCreate()
	* |      |
	* | o  o | In: pText* [text], uint16_t [length]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [text] - which [text] structure should be initialized.
	* If [text] was already created, then it will be overwritten.
	* [length] - maximal length of the [text] [value].
	* Last position should allways be L'\0' for safety.
	*
	* > Description:
	* This function choses [ID] for given [text], then it allocates memory,
	* setups all needed values and variables and you are ready to go.
	*/
	int8_t pTextCreate(
		pText *text,
		uint16_t length
	);

	/*
	* |\____/| pTextDestroy()
	* |      |
	* | o  o | In: pText* [text]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [text] - which [text] structure should be destroyed.
	* Removed debug values will depend on [text] [ID].
	*/
	int8_t pTextDestroy(pText *text);

	/*
	* |\____/| pFontCreate()
	* |      |
	* | o  o | In: pFont* [font], wchar_t* [directory]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [font] - which [font] structure should be initialized.
	* If [font] was already created, then it will be overwritten.
	* [directory] - from where the [font] should be loaded.
	*
	* > Description:
	* This function choses [ID] for given [font], then it loads [font]
	* from given [directory] and setups all needed values and variables.
	*/
	int8_t pFontCreate(
		pFont *font,
		const wchar_t *directory
	);

	/*
	* |\____/| pFontDestroy()
	* |      |
	* | o  o | In: pFont* [font]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [font] - which [font] structure should be destroyed.
	* Removed debug values will depend on [font] [ID].
	*/
	int8_t pFontDestroy(pFont *font);

	/*
	* |\____/| pAudioCreate()
	* |      |
	* | o  o | In: pAudio* [audio], wchar_t* [directory]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [audio] - which [audio] structure should be initialized.
	* If [audio] was already created, then it will be overwritten.
	* [directory] - from where the [audio] should be loaded.
	*
	* > Description:
	* This function choses [ID] for given [audio], then it setups all
	* needed values and variables, loads sound file from given [directory]
	* and creates [audio] debug thread. Debug thread works non stop even
	* if [audio] is paused. It properly decodes [audio] file on creation.
	* Then it is used to properly play, pause and manage [audio].
	*/
	int8_t pAudioCreate(
		pAudio *audio,
		const wchar_t *directory
	);

	/*
	* |\____/| pAudioDestroy()
	* |      |
	* | o  o | In: pAudio* [audio]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [audio] - which [audio] structure should be destroyed.
	* Removed debug values will depend on [audio] [ID].
	*/
	uint8_t pAudioDestroy(pAudio *audio);

	#ifdef __cplusplus
}

#endif
#endif