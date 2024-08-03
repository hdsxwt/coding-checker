#ifndef SCREEMELEMENTLIB
#define SCREEMELEMENTLIB

#include<windows.h>
#include<string>
#include<vector>
#include<iostream> // test

using std::string;
using std::vector;
using std::cout;
using std::endl;


COORD operator + (const COORD a, const COORD b) {
	return COORD{(short)(a.X + b.X), (short)(a.Y + b.Y)};
}

COORD operator - (const COORD a, const COORD b) {
	return COORD{(short)(a.X - b.X), (short)(a.Y - b.Y)};
}

class Color;
class Button;
class Menu;
class Call_back;

class Color {
	#define BLACK 0 // hei
	#define BLUE 1 // lan
	#define GREEN 2 // lv
	#define CYAN 3 // qing
	#define RED 4 // hong
	#define PURPLE 5 // zi
	#define YELLOW 6 // huang
	#define WHITE 7 // bai
	#define BRIGHTBLACK 8
	#define BRIGHTBLUE 9
	#define BRIGHTGREEN 10
	#define BRIGHTCYAN 11
	#define BRIGHTRED 12
	#define BRIGHTPURPLE 13
	#define BRIGHTYELLOW 14
	#define BRIGHTWHITE 15
public:
	WORD front_color;
	WORD back_color;
	Color(): front_color(WHITE), back_color(BLACK) { }
	Color(WORD front, WORD back): front_color(front), back_color(back) { }
	WORD mix() {return (back_color << 4) | front_color;}
	bool operator == (Color b) {
		return front_color == b.front_color && back_color == b.back_color;
	}
	bool operator != (Color b) {
		return !(*this == b);
	}
};
const Color default_color = Color(BRIGHTWHITE, BLACK);
const Color default_highlight_color = Color(BRIGHTCYAN, BLACK);

void set_color(Color color);
void set_mouse_position(const COORD cursorPosition);

MOUSE_EVENT_RECORD recent_mouse_event;
COORD recent_mouse_position;

bool get_mouse_event() {
	INPUT_RECORD record;
	DWORD temp;
	
	
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &temp);
	if (record.EventType == MOUSE_EVENT) {
		recent_mouse_event = record.Event.MouseEvent;
		recent_mouse_position = recent_mouse_event.dwMousePosition;
		return true;
	}
	return false;
}

class Call_back {
public:
	vector<int> ids;
	void operator += (const Call_back b) {
		for (auto x: b.ids) {
			ids.push_back(x);
		}
	}
	Call_back() {
		ids.clear();
	}
	void push_back(int x) {
		ids.push_back(x);
	}
	int size() {
		return ids.size();
	}
};

class Button {
private:
	Color last_color;
	Color recent_color;
	bool last_visible;
public:
	int id;
	Menu* fa;
	COORD position;
	COORD global_position;
	string text;
	bool visible;
	bool clickable;
	Color color;
	Color highlight_color;
	void set_visible(bool visible) {
		this -> visible = visible;
	}
	void set_clickable(bool clickable) {
		this -> clickable = clickable;
	}
	Button () {
		this -> id = 0;
		this -> last_color = default_highlight_color;
		this -> recent_color = default_color;
		this -> fa = nullptr;
		this -> position = global_position = COORD{0, 0};
		this -> text = "";
		this -> visible = false;
		this -> clickable = false;
		this -> color = default_color;
		this -> highlight_color = default_highlight_color;
	}
	
	void print();
	Call_back update();
};

class Menu{
private:
	Color recent_color;
	Color last_color;
public:
	int id;
	Menu* fa;
	vector<Menu*> son_menu;
	vector<Button*> son_button;
	COORD position;
	COORD global_position;
	string text;
	bool folded;
	bool visible;
	bool clickable;
	bool foldable;
	Color color;
	Color highlight_color;
	void set_visible (bool visible) {
		this -> visible = visible;
	}
	void set_clickable (bool clickable) {
		this -> clickable = clickable;
	}
	void set_foldable (bool foldable) {
		this -> foldable = foldable;
	}
	Menu () {
		this -> fa = nullptr;
		this -> son_menu.clear();
		this -> son_button.clear();
		this -> position = global_position = COORD{0, 0};
		this -> text = "";
		this -> folded = false;
		this -> visible = false;
		this -> clickable = false;
		this -> foldable = false;
		this -> color = default_color;
		this -> highlight_color = default_highlight_color;
		this -> last_color = default_highlight_color;
	}
	void add_son(Menu menu) {
		menu.fa = this;
		son_menu.push_back(&menu);
	}
	void add_son(Button button) {
		button.fa = this;
		son_button.push_back(&button);
	}
	
	void print();
	Call_back update();
};

void Button::print() {
	set_mouse_position(global_position);
	set_color(recent_color);
	printf("%s", text.data());
}

void Menu::print() {
	if (!visible) return;
	set_mouse_position(global_position);
	set_color(recent_color);
	printf("%s", text.data());
	set_color(default_color);
}

Call_back Button::update() {
	
	
	if (fa != nullptr) global_position = position + fa -> position;
	else global_position = position;
	
	Call_back ret;
	if (recent_mouse_position.Y == global_position.Y &&
			recent_mouse_position.X >= global_position.X &&
			recent_mouse_position.X <= global_position.X + (short)text.size() - 1) {
		recent_color = highlight_color;
	} else {
		recent_color = color;
	}
	
	if (recent_color != last_color) {
		print();
		last_color = recent_color;
		ret.push_back(id);
	}
	return ret;
}

Call_back Menu::update() {
	
	if (fa != nullptr) global_position = position + fa -> position;
	else global_position = position;
	
	if (id == 0) { // the root
		if (!get_mouse_event()) return Call_back();
	}
	
	Call_back ret;
	
	if (recent_mouse_position.Y == global_position.Y &&
		recent_mouse_position.X >= global_position.X &&
		recent_mouse_position.X <= global_position.X + (short)text.size() - 1) {
		recent_color = highlight_color;
	} else {
		recent_color = color;
	}
	
	
	if (recent_color != last_color) {
		print();
		last_color = recent_color;
		ret.push_back(id);
	}
	
	
	
	for (auto button: son_button) {
		ret += button -> update();
	}
	for (auto menu: son_menu) {
		ret += menu-> update();
	}
	return ret;
}



Menu root;


void set_color(Color color) {
	const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color.mix());
}

void set_mouse_position(const COORD cursorPosition) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
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
	Set_cursor_visible(false);
	Set_console_mode(false, false, true);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	set_color(default_color);
}

void stop() { // just a note
	Set_cursor_visible(true);
	Set_console_mode(true, true, true);
}

#endif


/*
TODO
fold
positin update
click
*/
