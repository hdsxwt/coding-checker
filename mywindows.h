

// windows.h start

#ifndef _WINDOWS_

// winbase.h start

#include <_mingw_unicode.h>

#include <processenv.h>
#include <wow64apiset.h>

#define STD_INPUT_HANDLE ((DWORD)-10)
#define STD_OUTPUT_HANDLE ((DWORD)-11)

// winbase.h



// wincon.h start

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct _COORD {
		SHORT X;
		SHORT Y;
	} COORD,*PCOORD;
	typedef struct _KEY_EVENT_RECORD {
		WINBOOL bKeyDown;
		WORD wRepeatCount;
		WORD wVirtualKeyCode;
		WORD wVirtualScanCode;
		union {
			WCHAR UnicodeChar;
			CHAR AsciiChar;
		} uChar;
		DWORD dwControlKeyState;
	} KEY_EVENT_RECORD,*PKEY_EVENT_RECORD;
	typedef struct _MOUSE_EVENT_RECORD {
		COORD dwMousePosition;
		DWORD dwButtonState;
		DWORD dwControlKeyState;
		DWORD dwEventFlags;
	} MOUSE_EVENT_RECORD,*PMOUSE_EVENT_RECORD;
#define MOUSE_WHEELED 0x4
	typedef struct _WINDOW_BUFFER_SIZE_RECORD {
		COORD dwSize;
	} WINDOW_BUFFER_SIZE_RECORD,*PWINDOW_BUFFER_SIZE_RECORD;
	
	typedef struct _MENU_EVENT_RECORD {
		UINT dwCommandId;
	} MENU_EVENT_RECORD,*PMENU_EVENT_RECORD;
	
	typedef struct _FOCUS_EVENT_RECORD {
		WINBOOL bSetFocus;
	} FOCUS_EVENT_RECORD,*PFOCUS_EVENT_RECORD;
	
	typedef struct _INPUT_RECORD {
		WORD EventType;
		union {
			KEY_EVENT_RECORD KeyEvent;
			MOUSE_EVENT_RECORD MouseEvent;
			WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
			MENU_EVENT_RECORD MenuEvent;
			FOCUS_EVENT_RECORD FocusEvent;
		} Event;
	} INPUT_RECORD,*PINPUT_RECORD;
#define MOUSE_EVENT 0x2
	typedef struct _CONSOLE_CURSOR_INFO {
		DWORD dwSize;
		WINBOOL bVisible;
	} CONSOLE_CURSOR_INFO,*PCONSOLE_CURSOR_INFO;
#define ENABLE_MOUSE_INPUT 0x10
#define ENABLE_INSERT_MODE 0x20
#define ENABLE_QUICK_EDIT_MODE 0x40
#define ReadConsoleInput __MINGW_NAME_AW(ReadConsoleInput)
	WINBASEAPI WINBOOL WINAPI ReadConsoleInputA(HANDLE hConsoleInput,PINPUT_RECORD lpBuffer,DWORD nLength,LPDWORD lpNumberOfEventsRead);
	WINBASEAPI WINBOOL WINAPI GetConsoleMode(HANDLE hConsoleHandle,LPDWORD lpMode);
	WINBASEAPI WINBOOL WINAPI GetConsoleCursorInfo(HANDLE hConsoleOutput,PCONSOLE_CURSOR_INFO lpConsoleCursorInfo);
	WINBASEAPI WINBOOL WINAPI SetConsoleMode(HANDLE hConsoleHandle,DWORD dwMode);
	WINBASEAPI WINBOOL WINAPI FlushConsoleInputBuffer(HANDLE hConsoleInput);
	WINBASEAPI WINBOOL WINAPI SetConsoleCursorPosition(HANDLE hConsoleOutput,COORD dwCursorPosition);
	WINBASEAPI WINBOOL WINAPI SetConsoleCursorInfo(HANDLE hConsoleOutput,CONST CONSOLE_CURSOR_INFO *lpConsoleCursorInfo);
	WINBASEAPI WINBOOL WINAPI SetConsoleTextAttribute(HANDLE hConsoleOutput,WORD wAttributes);
	
#ifdef __cplusplus
}
#endif
// wincon.h end

#endif


// windows.h end


