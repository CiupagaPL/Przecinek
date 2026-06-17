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

// Standard C libraries
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef PRZECINEK_H
#define PRZECINEK_H

#ifdef __cplusplus
extern "C"{
#endif

	/******************************************
	*  ,______,  Base structures
	*  |      |  [pSize], [pPosition], [pColor]
	*  |______|
	* (--------)
	******************************************/
	typedef struct{ uint16_t width, height; } pSize;
	typedef struct{ int32_t x, y; } pPosition;
	typedef struct{ uint8_t red, green, blue, alpha; } pColor;

	/******************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  DEFINES
	*  |     |
	*  `-   -`
	******************************************/

	/******************************************
	*  ,______,  Default values and
	*  |      |  Global limits
	*  |______|
	* (--------)
	******************************************/
	// Przecinek branch (alpha / beta / release)
	#define PRZECINEK_VERSION L"a"
	// Last major release
	#define PRZECINEK_VERSION_MAJOR 5
	#define PRZECINEK_VERSION_MINOR 0
	#define PRZECINEK_VERSION_PATCH L"a"
	// Last unstable version
	#define PRZECINEK_UNSTABLE 21

	// Frame limits for all windows
	#define PRZECINEK_FRAME_MIN 10 // `<=480` safe
	#define PRZECINEK_FRAME_MAX 120 // `<=480` safe

	// Maximal amount of pressed keys at one time
	#define PRZECINEK_KEY_PRESS_MAX 8 // `uint8_t`

	// Default color pallet
	#define COLOR_DEFAULT_BACKGROUND (pColor){ 255, 255, 255, 255 }
	#define COLOR_DEFAULT_FOREGROUND (pColor){ 50, 50, 60, 255 }

	// Structures amount limits
	#define WINDOW_MAX 1 // `uint8_t`
	#define OBJECT_MAX 512 // `uint16_t`
	#define IMAGE_MAX 128 // `uint16_t`
	#define FONT_MAX 4 // `uint16_t`
	#define TEXT_MAX 64 // `uint16_t`
	#define AUDIO_MAX 32 // `uint16_t`

	// Window size limits
	#define WINDOW_WIDTH_MIN 128 // `uint16_t`
	#define WINDOW_HEIGHT_MIN 128 // `uint16_t`
	#define WINDOW_WIDTH_MAX (SHRT_MAX+1)/2 // DEBUG
	#define WINDOW_HEIGHT_MAX (SHRT_MAX+1)/2 // DEBUG
	// Window default position
	#define WINDOW_X_DEF 256 // `int16_t`
	#define WINDOW_Y_DEF 256 // `int16_t`
	// Window default title
	#define WINDOW_CHAR_TITLE_DEF "{,} Window"
	#define WINDOW_WIDE_TITLE_DEF L"{,} Window"
	// Window title max length
	// Is decreased by `1`
	#define WINDOW_TITLE_LENGTH_MAX 256 // DEBUG

	// Object minimal and maximal rotation value
	#define OBJECT_ROTATION_MIN (-360) // `int16_t`
	#define OBJECT_ROTATION_MAX 360 // `int16_t`

	// Object vertice amount limits
	#define OBJECT_VERTICE_MIN 3 // `uint16_t`
	#define OBJECT_VERTICE_MAX 1024 // `uint16_t`

	// Text minimal length
	#define TEXT_LENGTH_MIN 2 // `uint16_t`

	// Text default space width in pt
	#define TEXT_DEFAULT_SPACE 32 // `int32_t`

	// Font size limits in pt
	#define FONT_SIZE_MIN 4 // `uint16_t`
	#define FONT_SIZE_MAX 1024 // `uint16_t`

	// Maximal audio volume
	#define AUDIO_VOLUME_MAX 200 // uint16_t
	// Audio refresh rate
	#define AUDIO_REFRESH_RATE 200000 // DEBUG
	// Audio thread delay
	#define AUDIO_SLEEP_RATE 40 // DEBUG
	#define AUDIO_PROCESS_RATE 10 // DEBUG

	/******************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  STRUCTURES
	*  |     |
	*  `-   -`
	******************************************/

	/******************************************
	*  ,______,  [pPrzecinek] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		// Console mode
		// `true` for enabled console
		bool debug;
		// Frame limit and current count for all windows
		uint16_t frameLimit, frameCount;

		// Screen size in pixels
		pSize display;
		// Current cursor position in pixels
		pPosition cursor;
	} pPrzecinek;

	extern pPrzecinek przecinek;

	/******************************************
	*  ,______,  [pWindow] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint8_t ID, exists;

		// Pixel position on the screen
		int16_t x, y;
		// Pixel size of the window
		uint16_t width, height;
		// Size limits of the window
		uint16_t widthMin, heightMin, widthMax, heightMax;

		// Window title
		wchar_t *title;

		// Window focus and fullScreen status
		bool focus, fullScreen;
	} pWindow;

	/******************************************
	*  ,______,  [pObject] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint16_t ID;

		// Vertice count of the object
		uint16_t verticeCount;
		// Vertice positions array
		// Equal to the vertice count
		pPosition *vertice;
	} pObject;

	/******************************************
	*  ,______,  [pImage] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint16_t ID;
	} pImage;

	/******************************************
	*  ,______,  [pFont] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint16_t ID;

		// Font size in pt
		uint16_t size;
	} pFont;

	/******************************************
	*  ,______,  [pText] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint16_t ID;

		// Pixel position on the window
		int32_t x, y;

		// Text max length
		// Should be decreased by `1`
		uint16_t length;
		// Text value
		wchar_t *value;

		// Text spacing and space size in pt
		int32_t letterSpacing, lineSpacing, spaceSize;
	} pText;

	/******************************************
	*  ,______,  [pAudio] structure
	*  |      |
	*  |______|
	* (--------)
	******************************************/
	typedef struct{
		uint16_t ID;

		// Volume percentage
		// Value range from `0` to the AUDIO_VOLUME_MAX
		uint16_t volume;
		// Audio pause state
		bool pause;

		// Audio current and max frame
		uint32_t frame, frameMax;
	} pAudio;

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
	uint8_t pSetup(bool debug, uint16_t frameLimit);

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
	uint8_t pUpdate();

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
	void pClear();

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
	bool pKeyPress(wchar_t *key);

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
	bool pKeyHold(wchar_t *key);

	/****************************************************************
	* |\_____/| pKeyCaps()
	* | .     |
	* |     . | In:
	* \ = , = / Out: bool (`true` -> activate)
	*
	* Additional Description:
	* This function returns Caps Lock key status.
	****************************************************************/
	bool pKeyCaps();

	/****************************************************************
	* |\_____/| pKeyNum()
	* | .     |
	* |     . | In:
	* \ = , = / Out: bool (`true` -> activate)
	*
	* Additional Description:
	* This function returns Num Lock key status.
	****************************************************************/
	bool pKeyNum();

	/****************************************************************
	* |\_____/| pKeyScroll()
	* | .     |
	* |     . | In:
	* \ = , = / Out: bool (`true` -> activate)
	*
	* Additional Description:
	* This function returns Scroll Lock key status.
	****************************************************************/
	bool pKeyScroll();

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
	void pEndup();

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
	uint8_t pWindowCreate(pWindow *window, uint16_t width, uint16_t height, bool resizable);

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
	uint8_t pWindowDrawObject(pWindow *window, pObject *object, pColor *color, pImage *image);

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
	uint8_t pWindowDrawText(pWindow *window, pFont *font, pText *text, pColor *color);

	/****************************************************************
	* |\_____/| pWindowDestroy()
	* | .     |
	* |     . | In: pWindow* [window]
	* \ = , = / Out: uint16_t (debug key code, `0` -> not found)
	*
	* Parameters:
	* [window] - which [window] structure should receive kill signal.
	* Removed debug values will depend on [window] [ID].
	****************************************************************/
	uint8_t pWindowDestroy(pWindow* window);

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
	uint8_t pObjectCreate(pObject *object, uint16_t verticeCount);

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
	);

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
	uint8_t pObjectMove(pObject *object, uint16_t verticeCount, int32_t x, int32_t y);

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
	bool pObjectCollisionSquare(pObject *object1, pObject *object2);

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
	bool pObjectCollisionComplex(pObject *object1, pObject *object2);

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
	uint8_t pObjectDestroy(pObject *object);

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
	uint8_t pImageCreate(pImage *image, wchar_t *directory);

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
	uint8_t pImageDestroy(pImage *image);

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
	uint8_t pFontCreate(pFont *font, wchar_t *directory, uint16_t size);

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
	uint8_t pFontDestroy(pFont *font);

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
	uint8_t pTextCreate(pText *text, uint16_t length, wchar_t *value);

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
	uint8_t pTextDestroy(pText *text);

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
	uint8_t pAudioCreate(pAudio *audio, wchar_t *directory);

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
	uint8_t pAudioDestroy(pAudio *audio);

#ifdef __cplusplus
}
#endif
#endif