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


COORD operator + (const COORD a, const COORD b) { return COORD{(short)(a.X + b.X), (short)(a.Y + b.Y)}; }
COORD operator - (const COORD a, const COORD b) { return COORD{(short)(a.X - b.X), (short)(a.Y - b.Y)}; }
COORD operator * (const COORD a, const short k) { return COORD{(short)(a.X * k),   (short)(a.Y * k)}; }
bool operator == (const COORD a, const COORD b) { return a.X == b.X && a.Y == b.Y; }
bool operator != (const COORD a, const COORD b) { return !(a == b); }

class Color;
class Button;
class Menu;
class Call_back;
class Fold_button;

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
const Color default_click_color = Color(GREEN, BLACK);

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


class Menu{
private:
	Fold_button* fold_button;
protected:
	bool pressed;
	Color last_color;
	Color recent_color;
	bool last_visible;
	bool auto_position;
	bool real_visible;
	short height;
	bool folded;
	COORD global_position;
	COORD last_global_position;
	short deep;
	string last_text;
public:
	int id;
	Menu* fa;
	Menu* lst;
	vector<Menu*> son;
	COORD position;
	string text;
	bool visible;
	bool clickable;
	bool foldable;
	Color color;
	Color highlight_color;
	Color click_color;
	
	void fold();
	
	void unfold();
	
	void set_auto_position(bool x) {
		auto_position = x;
	}
	
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
		this -> lst = nullptr;
		this -> fold_button = nullptr;
		this -> son.clear();
		this -> position = global_position = COORD{0, 0};
		this -> height = 2;
		this -> text = "";
		this -> folded = false;
		this -> visible = false;
		this -> real_visible = false;
		this -> clickable = false;
		this -> foldable = false;
		this -> color = default_color;
		this -> highlight_color = default_highlight_color;
		this -> click_color = default_click_color;
		this -> last_color = default_highlight_color;
		this -> recent_color = default_color;
		this -> last_visible = false;
		this -> auto_position = true;
		this -> last_text = "";
	}
	void set_position (short x, short y) {
		position = COORD{x, y};
		auto_position = false;
	}
	void add_son(Menu* menu) {
		this -> height += 2;
		if (!son.empty()) menu -> lst = son.back();
		menu -> fa = this;
		son.push_back(menu);
	}
private:
	void add_fold_button(Fold_button* fold_button);
public:
	virtual void print(bool typ = true);
	virtual Call_back update(bool is_root = true);
	virtual Menu* get_class_name() { return this; }
};

class Fold_button : public Menu {
private:
	bool last_open;
public:
	bool open;
	string close_text;
	string open_text;
	Fold_button (): Menu() {
		this -> close_text = "→";
		this -> open_text = "↓";
		this -> last_open = false;
		this -> open = true;
	}
	
	virtual Call_back update(bool is_root = true);
	virtual Fold_button* get_class_name() { return this; }
};

Menu root;

void Menu::fold() {
	folded = true;
	root.update(true);
}

void Menu::unfold(){
	folded = false;
	root.update(true);
}

void Menu::add_fold_button(Fold_button* fold_button) {
	this -> fold_button = fold_button;
	fold_button -> fa = this;
	son.push_back(fold_button);
	this -> fold_button -> set_auto_position(false);
	this -> fold_button -> set_position(((text.size() - 1)/ 20 + 1) * 20 ,0);
}

struct output_content{
	Color color;
	COORD position;
	string text;
	output_content(Color color, COORD position, string text) {
		this -> color = color;
		this -> position = position;
		this -> text = text;
	}
};

vector<output_content> output_cache;
vector<output_content> clear_cache;

void fresh_print() {
	set_color(default_color);
	for (auto output: clear_cache) {
		set_mouse_position(output.position);
		printf("%s", output.text.data());
	}
	for (auto output: output_cache) {
		set_mouse_position(output.position);
		set_color(output.color);
		printf("%s", output.text.data());
	}
	set_color(default_color);
	clear_cache.clear();
	output_cache.clear();
}

void Menu::print(bool typ) {
	if (global_position != last_global_position || last_text != text) { // clear old
		string s(last_text.size(), ' ');
		clear_cache.push_back(output_content(default_color, last_global_position + COORD{deep, 0}*2,s));
		last_global_position = global_position;
		last_text = text;
	}
	
	
	if (typ) { // output new
		if (!real_visible) return;
		output_cache.push_back(output_content(recent_color, global_position + COORD{deep, 0}*2, text));
	} else {
		string s(text.size(), ' ');
		clear_cache.push_back(output_content(default_color, last_global_position + COORD{deep, 0}*2,s));
	}
}

Call_back Menu::update(bool is_root) {
	if (auto_position) {
		if (lst != nullptr) {
			position.Y = (lst -> height + lst -> position.Y);
		} else if (fa != nullptr) {
			position.Y = 2;
		} else {
			auto_position = false;
		}
	}
	
	if (fa != nullptr) {
		global_position = position + fa -> global_position;
		real_visible = (visible) && (fa -> real_visible) && !(fa -> folded);
		this -> deep = fa -> deep + 1;
	} else {
		global_position = position;
		real_visible = visible;
	}
	
	if (id == 0) { // the root
		if (!get_mouse_event()) return Call_back();
	}
	
	if (typeid(*(this -> get_class_name())) == typeid(Menu) && foldable && fold_button == nullptr) {
		Fold_button button;
		add_fold_button(&button);
	}
	
	Call_back ret;
	
	if (recent_mouse_position.Y == global_position.Y &&
		recent_mouse_position.X >= (global_position.X + deep * 2) &&
		recent_mouse_position.X <= (global_position.X + deep * 2) + (short)text.size() - 1 && real_visible && clickable) {
		recent_color = highlight_color;
		if (recent_mouse_event.dwButtonState && recent_mouse_event.dwButtonState != MOUSE_WHEELED && !pressed) {
			recent_color = click_color;
			ret.push_back(id);
			pressed = true;
		} else if (!recent_mouse_event.dwButtonState) {
			pressed = false;
		}
	} else {
		recent_color = color;
	}
	
	
	if (recent_color != last_color || last_visible != real_visible || global_position != last_global_position) {
		print(real_visible);
		last_color = recent_color;
		last_visible = real_visible;
	}
	height = 2;
	for (auto menu: son) {
		ret += (menu -> update(false));
		if (!folded) height += menu -> height;
	}
	
	if (is_root) {
		fresh_print();
	}
	
	return ret;
}

Call_back Fold_button::update(bool is_root) {
	return Call_back();
	Call_back ret = this -> Menu::update();
	return ret;
	// TODO
}



// Console command


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

void start() {
	Set_cursor_visible(false);
	Set_console_mode(false, false, true);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	set_color(default_color);
	root.visible = true;
	root.update(1);
}

void stop() {
	Set_cursor_visible(true);
	Set_console_mode(true, true, true);
}

#endif


/*
TODO
add fold arrow
add foldable permiission
positin update
*/

//↓ →
