#ifndef FUCK
#define FUCK

#include<windows.h>

class vec2;

class Color;
class Position;
class Button;
class Menu;

class vec2 {
public:
	int x, y;
	vec2(): x(0), y(0) {}
	vec2(int x, int y): x(x), y(y) {}
	vec2 operator + (const vec2 b) { return vec2(x + b.x, y + b.y);}
	vec2 operator - (const vec2 b) { return vec2(x - b.x, y - b.y);}
	vec2 operator * (const int k) { return vec2(x * k, y * k);}
};

class Color {
	#define Black 0 // hei
	#define Blue 1 // lan
	#define Green 2 // lv
	#define Cyan 3 // qing
	#define Red 4 // hong
	#define Purple 5 // zi
	#define Yellow 6 // huang
	#define White 7 // bai
	#define bright_black 8
	#define bright_blue 9
	#define bright_green 10
	#define bright_cyan 11
	#define bright_red 12
	#define bright_purple 13
	#define bright_yellow 14
	#define bright_white 15
public:
	WORD front_color;
	WORD back_color;
	WORD mix() {return (back_color << 4) | front_color;}
};

class Position {
	Menu* father;
	vec2 position;
	vec2 real_position;
};


void setColor(Color color) { // just a note
	const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color.mix());
}

void Set_cursor_visible(bool visible) {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}

void Set_console_mode(bool QuickEditMode, bool InsertMode, bool MouseInput) {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	if (QuickEditMode) {
		mode |= ENABLE_QUICK_EDIT_MODE;
	} else {
		mode &= ~ENABLE_QUICK_EDIT_MODE;
	}
	if (InsertMode) {
		mode |= ENABLE_INSERT_MODE;
	} else {
		mode &= ~ENABLE_INSERT_MODE;
	}
	if (MouseInput) {
		mode |= ENABLE_MOUSE_INPUT;
	} else {
		mode &= ~ENABLE_MOUSE_INPUT;
	}
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

void start() { // just a note
	Set_cursor_visible(true);
	Set_console_mode(true, true, true);
}

void stop() { // just a note
	Set_cursor_visible(false);
	Set_console_mode(false, false, true);
	
}



#endif