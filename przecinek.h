/****************************************************
*       |\____/|
*       |      | github.com/CiupagaPL/Przecinek
*  __   | >  < | original {,} library
* /  \__\ =, = / copyright 2025-2026
* \__         /
*    \ \____\ \
*    {,{,} {,},}
*****************************************************/

// Standard C libraries
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

#ifndef PRZECINEK_H
#define PRZECINEK_H

#ifdef __cplusplus
extern "C"{
#endif
	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  GLOBAL STRUCTURES
	*  |     |
	*  `-   -`
	*************************************/

	typedef struct{ uint16_t width, height; } pSize;
	typedef struct{ int32_t x, y; } pPosition;
	typedef struct{ uint8_t red, green, blue, alpha; } pColor;

	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  GLOBAL DEFINES
	*  |     |
	*  `-   -`
	*************************************/

	// Przecinek version
	#define PRZECINEK_STABLE_BRANCH L"a"
	#define PRZECINEK_STABLE_MAJOR 6
	#define PRZECINEK_STABLE_MINOR 1
	#define PRZECINEK_STABLE_PATCH L"c"
	#define PRZECINEK_UNSTABLE 26

	// Frame limit for all windows
	// Value higher than `480` is very unsafe!
	#define PRZECINEK_FRAME_MIN (uint16_t)10
	#define PRZECINEK_FRAME_MAX (uint16_t)120

	// Maximal amount of pressed keys at one time
	#define PRZECINEK_KEY_PRESS_MAX (uint8_t)8

	// Default color pallet
	#define COLOR_DEFAULT_BACKGROUND (pColor){ 255, 255, 255, 255 }
	#define COLOR_DEFAULT_FOREGROUND (pColor){ 50, 50, 60, 255 }

	// Structures count
	#define WINDOW_MAX (uint8_t)1
	#define OBJECT_MAX (uint16_t)512
	#define IMAGE_MAX (uint16_t)128
	#define FONT_MAX (uint16_t)4
	#define TEXT_MAX (uint16_t)64
	#define AUDIO_MAX (uint16_t)32

	// Window minimal and maximal size
	// Value higher than `SHRT_MAX/2` is very unsafe!
	#define WINDOW_WIDTH_MIN (uint16_t)128
	#define WINDOW_HEIGHT_MIN (uint16_t)128
	#define WINDOW_WIDTH_MAX (uint16_t)SHRT_MAX/2
	#define WINDOW_HEIGHT_MAX (uint16_t)SHRT_MAX/2
	// Window default position
	#define WINDOW_X_DEF (int16_t)256
	#define WINDOW_Y_DEF (int16_t)256

	// Window default title
	// String longer than `254` chars is very unsafe!
	#define WINDOW_TITLE_DEF L"window"
	// Window title maximal length
	// Is decreased by `1` for NULL terminator
	#define WINDOW_TITLE_LENGTH_MAX (uint8_t)255

	// Object minimal and maximal rotation value
	#define OBJECT_ROTATION_MIN (int16_t)(-360)
	#define OBJECT_ROTATION_MAX (int16_t)360

	// Object minimal and maximal vertice amount
	#define OBJECT_VERTICE_MIN (uint16_t)3
	#define OBJECT_VERTICE_MAX (uint16_t)1024

	// Text minimal length
	#define TEXT_LENGTH_MIN (uint16_t)2

	// Text default space width in pt
	#define TEXT_DEFAULT_SPACE (int32_t)32

	// Font minimal and maximal size in pt
	#define FONT_SIZE_MIN (uint16_t)4
	#define FONT_SIZE_MAX (uint16_t)1024

	// Maximal audio volume in percent
	#define AUDIO_VOLUME_MAX (uint16_t)200

	// Audio default refresh rate
	#define AUDIO_DEFAULT_REFRESH_RATE (uint32_t)200000
	// Audio thread delays used for diffrent purposes
	// Experiment with it for quicker/slower audio changes
	// Too radical values can mess up debug audio thread
	#define AUDIO_SLEEP_RATE (uint8_t)40
	#define AUDIO_PROCESS_RATE (uint8_t)10

	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  PRZECINEK VARIABLE
	*  |     |
	*  `-   -`
	*************************************/

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

	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  STRUCTURES
	*  |     |
	*  `-   -`
	*************************************/

	/************************************
	*  ,____,  [pWindow] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
	typedef struct{
		uint8_t ID;
		bool exists;

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

	/************************************
	*  ,____,  [pObject] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
	typedef struct{
		uint16_t ID;

		// Vertice count of the object
		uint16_t verticeCount;
		// Vertice positions array
		// Equal to the vertice count
		pPosition *vertice;
	} pObject;

	/************************************
	*  ,____,  [pImage] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
	typedef struct{
		uint16_t ID;
	} pImage;

	/************************************
	*  ,____,  [pFont] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
	typedef struct{
		uint16_t ID;

		// Font size in pt
		uint16_t size;
	} pFont;

	/************************************
	*  ,____,  [pText] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
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

	/************************************
	*  ,____,  [pAudio] structure
	*  |    |
	*  |____|
	* (------)
	*************************************/
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

	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  DEFAULT FUNCTIONS
	*  |     |
	*  `-   -`
	*************************************/

	/****************************************************************
	* |\____/| pSetup()
	* |      |
	* | o  o | In: bool [debug], uint16_t [frameLimit]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [debug] - console debug messages status.
	* [frameLimit] - what should be the frame limit of all windows.
	*
	* > Description:
	* This function initializes Przecinek library. It setups locale
	* and libraries used later. It also creates several global
	* debug objects, setups all needed values and variables,
	* setups [przecinek], creates debug console and downloads
	* initial values of the [display] and the [cursor].
	****************************************************************/
	int8_t pSetup(bool debug, uint16_t frameLimit);

	/****************************************************************
	* |\____/| pUpdate()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Description:
	* This function updates all [window] structures and [przecinek]
	* global variable values. Firstly it updates debug main
	* [window] [ID], then it refreshes input and updates all
	* [przecinek] stuff. After that it loads signals from main
	* debug window thread and modifies several debug values.
	* In the end it stops main loop for a short amount of time and
	* updates all frame related stuff.
	****************************************************************/
	int8_t pUpdate();

	/****************************************************************
	* |\____/| pClear()
	* |      |
	* | o  o | In:
	* \ = .= / Out:
	*
	* > Description:
	* This function clears and refreshes the debug console. It uses
	* `cls` or `clear` depending on the operating system.
	****************************************************************/
	void pClear();

	/****************************************************************
	* |\____/| pKeyPress()
	* |      |
	* | o  o | In: wchar_t* [key]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [key] - which [key] status should be checked.
	*
	* > Description:
	* This function checks if [key] is being pressed. Returned
	* value will be `true` only for one frame. Before [key] is
	* being checked this function searches for debug [key] code.
	* Debug code depends on the operating system.
	****************************************************************/
	int8_t pKeyPress(const wchar_t *key);

	/****************************************************************
	* |\____/| pKeyHold()
	* |      |
	* | o  o | In: wchar_t* [key]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [key] - which [key] status should be checked.
	*
	* > Description:
	* This function checks if [key] is being hold. Returned
	* value will be `true` after the first frame. Before [key] is
	* being checked this function searches for debug [key] code.
	* Debug code depends on the operating system.
	****************************************************************/
	int8_t pKeyHold(const wchar_t *key);

	/****************************************************************
	* |\____/| pKeyCaps()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Caps Lock key status.
	****************************************************************/
	int8_t pKeyCaps();

	/****************************************************************
	* |\____/| pKeyNum()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Num Lock key status.
	****************************************************************/
	int8_t pKeyNum();

	/****************************************************************
	* |\____/| pKeyScroll()
	* |      |
	* | o  o | In:
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Description:
	* This function returns Scroll Lock key status.
	****************************************************************/
	int8_t pKeyScroll();

	/****************************************************************
	* |\____/| pEndup()
	* |      |
	* | o  o | In:
	* \ = .= / Out:
	*
	* > Description:
	* This function safely destroys structures and cleans memory
	* before the end of the program. It firstly uninitializes all
	* debug libraries. Then it searches for all undestroyed
	* structures and removes them. In the end it resets [przecinek]
	* and several debug variables.
	****************************************************************/
	void pEndup();

	/************************************
	*  ,_   _,
	*  |     |
	* (   ,   )  STRUCTURE FUNCTIONS
	*  |     |
	*  `-   -`
	*************************************/

	/****************************************************************
	* |\____/| pWindowCreate()
	* |      | In: pWindow *window, uint16_t [width], [height],
	* | o  o | bool [resizable]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] structure should be initialized. If
	* given [window] is already created, then it will be overwritten.
	* [width], [height] - initial size of a newly created [window].
	* [resizable] - should the [window] be resizable or constant.
	*
	* > Description:
	* This function choses [ID] for given [window], then it
	* creates debug process, setups all needed values and variables,
	* setups debug pointers to the [window] structure, then it
	* creates physical [window] and creates second GL buffer.
	* It also creates debug [window] thread which loads and
	* responses to all signals. In the end it fills background
	* with `COLOR_DEFAULT_BACKGROUND` fill.
	****************************************************************/
	int8_t pWindowCreate(pWindow *window, uint16_t width, uint16_t height, bool resizable);

	/****************************************************************\
	* |\____/| pWindowDrawObject()
	* |      | In: pWindow* [window], pObject* [object],
	* | o  o | pColor* [color], pImage* [image]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] should be used for rendering.
	* [object] - which [object] should be rendered on [window].
	* [color] - in which [color] fill should [object] be
	* rendered in. Can be `NULL`.
	* [image] - in which [image] fill should [object] be
	* rendered in. Can be `NULL`.
	*
	* > Description:
	* This function uses GL to render [object] on given [window].
	* Before any action is taken, it checks and refreshes
	* [object] values if needed. If any [vertice] was modified,
	* then [texture] positions, debug [width], [height] and
	* center [x] and [y] position are being recalculated. Then
	* when everything is prepared [color] and [image] are
	* being applied to GL and [object] is being rendered. If
	* [color] is `NULL` then `COLOR_DEFAULT_FOREGROUND` is being
	* used. If [texture] source is `NULL` then default texture
	* with `COLOR_DEFAULT_BACKGROUND` and `COLOR_DEFAULT_FOREGROUND`
	* is being used instead. If [object] is fully outside the
	* rendering area then it is skipped.
	****************************************************************/
	int8_t pWindowDrawObject(pWindow *window, pObject *object, pColor *color, pImage *image);

	/****************************************************************
	* |\____/| pWindowDrawText()
	* |      | In: pWindow* [window], pFont* [font],
	* | o  o | pText* [text], pColor* [color]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] should be used for rendering.
	* [font] - which [font] should be to render [text].
	* [text] - which [text] should be drawn.
	* [color] - in which [color] fill should [text] be
	* rendered in. Can be `NULL`.
	*
	* > Description:
	* This function uses GL to render [text] using [font]
	* on given [window]. Before any action is taken, it checks
	* and refreshes [text] and [font] values if needed.
	* Depending on what changes were made either position values
	* of all [text] buffers are being recalculated or [text]
	* debug values are being reseted and regenerated. Then
	* when everything is prepared [color] is being applied to
	* GL and [text] buffers are being rendered. If [color]
	* is `NULL` then `COLOR_DEFAULT_FOREGROUND` is being used. If
	* any [text] buffer is fully outside the rendering area then
	* it is skipped.
	****************************************************************/
	int8_t pWindowDrawText(pWindow *window, pFont *font, pText *text, pColor *color);

	/****************************************************************
	* |\____/| pWindowDestroy()
	* |      |
	* | o  o | In: pWindow* [window]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [window] - which [window] structure should receive kill signal.
	* Removed debug values will depend on [window] [ID].
	****************************************************************/
	int8_t pWindowDestroy(pWindow* window);

	/****************************************************************
	* |\____/| pObjectCreate()
	* |      |
	* | o  o | In: pObject* [object], uint16_t [verticeCount]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be initialized.
	* If [object] was already created, then it will be overwritten.
	* [verticeCount] - how many [vertice] should be allocated.
	*
	* > Description:
	* This function choses [ID] for given [object], then it setups
	* all needed values and variables and allocates [vertice]
	* amount in the memory.
	****************************************************************/
	int8_t pObjectCreate(pObject *object, uint16_t verticeCount);

	/****************************************************************
	* |\____/| pObjectGenerate()
	* |      | In: pObject* [object], int32_t [x], [y],
	* | o  o | uint16_t [width], [height], int16_t [rotation]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be generated.
	* [x], [y] - from where [object] vertices should start from.
	* [width], [height] - how big should the [object] be.
	* [rotation] - how rotated should the [object] be.
	*
	* > Description:
	* This function generates [object] structure based on given
	* parameters. It automatically calculates [vertice] positions
	* and creates geometric figure. It also prepares [texture]
	* positions and calculates debug [width], [height] and
	* figure center [x] and [y] position.
	****************************************************************/
	int8_t pObjectGenerate(
		pObject *object, int32_t x, int32_t y,
		uint16_t width, uint16_t height, int16_t rotation
	);

	/****************************************************************
	* |\____/| pObjectMove()
	* |      |
	* | o  o | In: pObject* [object], int32_t [x], [y]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be moved.
	* [x], [y] - from where [object] vertices should start from.
	*
	* > Description:
	* This function moves [object] structure based on given
	* parameters. It also recalculates figure debug center [x]
	* and [y] position.
	****************************************************************/
	int8_t pObjectMove(pObject *object, int32_t x, int32_t y);

	/****************************************************************
	* |\____/| pObjectCollisionSquare()
	* |      |
	* | o  o | In: pObject* [object1], [object2]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [object1], [object2] - which structures should be checked.
	*
	* > Description:
	* This function checks for collision between two objects
	* using their width and height. Their size parameters are
	* calculated as debug values. This method is much faster but
	* less accurate.
	****************************************************************/
	int8_t pObjectCollisionSquare(pObject *object1, pObject *object2);

	/****************************************************************
	* |\____/| pObjectCollisionComplex()
	* |      |
	* | o  o | In: pObject* [object1], [object2]
	* \ = .= / Out: int8_t (`0`: `false`; `1`: `true`)
	*
	* > Parameters:
	* [object1], [object2] - which structures should be checked.
	*
	* > Description:
	* This function checks for collision between two objects
	* using their vertices. Firstly it searches five closest
	* vertices of [object1] to the center of [object2]. Then
	* it checks five closest vertices of [object2] to the center
	* of [object1]. Finally using big chunk of code it checks
	* triangular collision between simulated mini objects. This
	* method is much slower but nearly perfectly accurate.
	****************************************************************/
	int8_t pObjectCollisionComplex(pObject *object1, pObject *object2);

	/****************************************************************
	* |\____/| pObjectDestroy()
	* |      |
	* | o  o | In: pObject* [object]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [object] - which [object] structure should be destroyed.
	* Removed debug values will depend on [object] [ID].
	****************************************************************/
	int8_t pObjectDestroy(pObject *object);

	/****************************************************************
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
	* This function choses [ID] for given [image], then it setups
	* all needed values and variables, loads [image] from given
	* [directory] and creates proper [image] debug GL bitmap
	* for future rendering.
	****************************************************************/
	int8_t pImageCreate(pImage *image, const wchar_t *directory);

	/****************************************************************
	* |\____/| pImageDestroy()
	* |      |
	* | o  o | In: pImage* [image]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [image] - which [image] structure should be destroyed.
	* Removed debug values will depend on [image] [ID].
	****************************************************************/
	int8_t pImageDestroy(pImage *image);

	/****************************************************************
	* |\____/| pTextCreate()
	* |      |
	* | o  o | In: pText* [text], uint16_t [length], wchar_t* [value]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [text] - which [text] structure should be initialized.
	* If [text] was already created, then it will be overwritten.
	* [length] - maximal length of [text] [value].
	* Last position should allways be L'\0' for safety.
	* [value] - initial [text] [value].
	*
	* > Description:
	* This function choses [ID] for given [text], then
	* it allocates memory, setups all needed values and variables
	* and you are ready to go.
	****************************************************************/
	int8_t pTextCreate(pText *text, uint16_t length, const wchar_t *value);

	/****************************************************************
	* |\____/| pTextDestroy()
	* |      |
	* | o  o | In: pText* [text]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [text] - which [text] structure should be destroyed.
	* Removed debug values will depend on [text] [ID].
	****************************************************************/
	int8_t pTextDestroy(pText *text);

	/****************************************************************
	* |\____/| pFontCreate()
	* |      | In: pFont* [font], wchar_t* [directory],
	* | o  o | uint16_t [size]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [font] - which [font] structure should be initialized.
	* If [font] was already created, then it will be overwritten.
	* [directory] - from where the [font] should be loaded.
	* [size] - initial size of newly created [font].
	*
	* > Description:
	* This function choses [ID] for given [font], then it loads
	* [font] from given [directory] and setups all needed
	* values and variables.
	****************************************************************/
	int8_t pFontCreate(pFont *font, const wchar_t *directory, uint16_t size);

	/****************************************************************
	* |\____/| pFontDestroy()
	* |      |
	* | o  o | In: pFont* [font]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [font] - which [font] structure should be destroyed.
	* Removed debug values will depend on [font] [ID].
	****************************************************************/
	int8_t pFontDestroy(pFont *font);

	/****************************************************************
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
	* This function choses [ID] for given [audio], then
	* it setups all needed values and variables, loads file from
	* given [directory] and creates [audio] debug thread.
	* Debug thread works non stop even if [audio] is paused. It
	* properly decodes [audio] file on creation. Then it is used
	* to properly play/pause [audio].
	****************************************************************/
	int8_t pAudioCreate(pAudio *audio, const wchar_t *directory);

	/****************************************************************
	* |\____/| pAudioDestroy()
	* |      |
	* | o  o | In: pAudio* [audio]
	* \ = .= / Out: int8_t (`0`: success)
	*
	* > Parameters:
	* [audio] - which [audio] structure should be destroyed.
	* Removed debug values will depend on [audio] [ID].
	****************************************************************/
	uint8_t pAudioDestroy(pAudio *audio);
#ifdef __cplusplus
}
#endif
#endif