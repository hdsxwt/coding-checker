#ifndef FUCK
#define FUCK

#include<windows.h>
#include<string>
#include<vector>

using std::string;
using std::vector;

class vec2;

class Color;
class Button;
class Menu;
class Call_back;

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
};
const Color default_color = Color(BRIGHTWHITE, BLACK);
const Color default_highlight_color = Color(CYAN, BLACK);

class Call_back {
	vector<int> ids;
	void operator += (const Call_back b) {
		for (auto x: b.ids) {
			ids.push_back(x);
		}
	}
};

class Button {
private:
	Color last_color;
public:
	int id;
	Menu* fa;
	vec2 position;
	vec2 global_position;
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
		this -> fa = nullptr;
		this -> position = global_position = vec2(0, 0);
		this -> text = "";
		this -> visible = false;
		this -> clickable = false;
		this -> color = default_color;
		this -> highlight_color = default_highlight_color;
	}
	Call_back update() {
		
	}
};

class Menu{
private:
	Color last_color;
public:
	int id;
	Menu* fa;
	vector<Menu*> son_menu;
	vector<Button*> son_button;
	vec2 position;
	vec2 global_position;
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
		this -> position = global_position = vec2(0, 0);
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
	Call_back update() {
		
	}
};



Menu root = Menu();


void setColor(Color color) {
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
