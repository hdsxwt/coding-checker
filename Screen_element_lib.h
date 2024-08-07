#ifndef SCREENELEMENTLIB
#define SCREENELEMENTLIB


#include<string>
#include<vector>
#include<typeinfo>
#include<windows.h>

using std::string;
using std::vector;


COORD operator + (const COORD a, const COORD b) { return COORD{(short)(a.X + b.X), (short)(a.Y + b.Y)}; }
COORD operator - (const COORD a, const COORD b) { return COORD{(short)(a.X - b.X), (short)(a.Y - b.Y)}; }
COORD operator * (const COORD a, const short k) { return COORD{(short)(a.X * k),   (short)(a.Y * k)}; }
bool operator == (const COORD a, const COORD b) { return a.X == b.X && a.Y == b.Y; }
bool operator != (const COORD a, const COORD b) { return !(a == b); }

class Color;
class Button;
class Button;
class Call_back;
class Fold_button;
class Screen_element_controller;

class Screen_element_controller {
public:
	bool get_mouse_event();
	void set_color(Color color);
	void set_mouse_position (const short x, const short y);
	void set_mouse_position (const COORD cursorPosition);
	void Set_cursor_visible(bool visible);
	void Set_console_mode(bool QuickEditMode, bool InsertMode, bool MouseInput);
	void start();
	void stop();
}screen_element_controller;

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

MOUSE_EVENT_RECORD recent_mouse_event;
COORD recent_mouse_position;


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
	bool empty() {
		return ids.empty();
	}
};

struct output_content{ // print --------------------------------------------------------------------------
	Color color;
	COORD position;
	string text;
	bool typ;
	bool indent_line;
	short deep;
	output_content(Color color, COORD position, short deep, bool typ, bool indent_line, string text) {
		this -> color = color;
		this -> position = position;
		this -> deep = deep;
		this -> typ = typ;
		this -> indent_line = indent_line;
		this -> text = text;
	}
};

vector<output_content> output_cache;
vector<output_content> clear_cache;

void print(output_content output) {
	screen_element_controller.set_mouse_position(output.position);
	if (output.deep > 0 && output.indent_line) {
		screen_element_controller.set_color(default_color);
		for (int i = 0; i < output.deep - 2; i++) putchar(" -"[output.typ]);
		putchar(" >"[output.typ]);
		putchar(' ');
	} else {
		screen_element_controller.set_mouse_position(output.position + COORD{output.deep, 0});
	}
	screen_element_controller.set_color(output.color);
	short cnt = 0;
	for (size_t i = 0; i < output.text.size(); i++) {
		if (output.text[i] != '\n') {
			putchar(output.text[i]);
		} else {
			screen_element_controller.set_mouse_position(output.position + COORD{output.deep, ++cnt});
		}
	}
}

void fresh_print() {
	screen_element_controller.set_color(default_color);
	for (auto output: clear_cache) {
		print(output);
	}
	for (auto output: output_cache) {
		print(output);
	}
	screen_element_controller.set_color(default_color);
	clear_cache.clear();
	output_cache.clear();
}


class Button { // Button -------------------------------------------------------------------------------------
private:
	bool pressed;
	
	// relation
	Fold_button* fold_button;
	Button* lst;
	vector<Button*> son;
	
	// Color
	Color last_color;
	Color recent_color;
	Color normal_color;
	Color highlight_color;
	Color click_color;
	
	// position
	COORD last_global_position;
	COORD global_position;
	COORD position;
	bool auto_position;
	short deep;
	bool indent_line;
	
	// visible
	bool last_visible;
	bool visible;
	bool real_visible;
	string last_text;
	
	// fold
	bool folded;
	bool foldable;
	
	// others
	bool clickable;
protected:
	string text;
	short height;
	int id;
	Button* fa;
public:
	Button () {
		this -> pressed = false;
		
		// realations
		this -> fold_button = nullptr;
		this -> lst = nullptr;
		this -> son.clear();
		this -> fa = nullptr;
		
		// color
		this -> last_color = default_highlight_color;
		this -> recent_color = default_color;
		this -> normal_color = default_color;
		this -> highlight_color = default_highlight_color;
		this -> click_color = default_click_color;
		
		// position
		this -> last_global_position = COORD{0, 0};
		this -> global_position = COORD{0, 0};
		this -> position = COORD{0, 0};
		this -> auto_position = true;
		this -> deep = 0;
		this -> height = -1;
		this -> indent_line = true;
		
		// text
		this -> last_text = "";
		this -> text = "";
		
		// visible
		this -> last_visible = false;
		this -> visible = false;
		this -> real_visible = false;
		
		// fold
		this -> folded = false;
		this -> foldable = false;
		this -> clickable = false;
	}
	// relation
	void add_son(Button* Button, bool typ = true);
	// color
	void set_normal_color    (Color normal_color) { this -> normal_color = normal_color; }
	void set_highlight_color (Color normal_color) { this -> highlight_color = highlight_color; }
	void set_click_color     (Color click_color)  { this -> click_color = click_color; }
	Color get_normal_color    () { return this -> normal_color; }
	Color get_highlight_color () { return this -> highlight_color; }
	Color get_click_color     () { return this -> click_color; }
	// position
	void set_auto_position (bool auto_position) { this -> auto_position = auto_position; }
	void set_position      (short x, short y)   { this -> position = COORD{x, y}; }
	void set_position      (COORD position)     { this -> position = position; }
	void set_height        (short height)       { this -> height = height; }
	void set_deep          (short deep)         { this -> deep = deep; }
	void set_indent_line   (bool indent_line)   { this -> indent_line = indent_line; }
	COORD get_position        () { return position; }
	COORD get_global_position () { return global_position; }
	short get_height          () { return height; }
	short get_deep            () { return deep; }
	bool get_indent_line      () { return indent_line; }
	// visible
	void set_visible (bool visible) { this -> visible = visible; }
	bool get_visible      () { return visible; }
	bool get_real_visible () { return real_visible; }
	// fold
	void fold   () { folded = true; }
	void unfold () { folded = false; }
	void set_foldable  (bool foldable)  { this -> foldable = foldable; }
	void set_clickable (bool clickable) { this -> clickable = clickable; }
	bool get_foldable  () { return foldable; }
	bool get_folded    () { return folded; }
	bool get_clickable () { return clickable; }
	// text
	void set_text (string text);
	void add_text (string text) { set_text(this -> text + text); }
	string get_text () { return text; }
	// others
	void set_id (int id) { this -> id = id; }
	int  get_id () { return id; }
	// process
	virtual Call_back update(bool is_root = true);
	
private:
	void add_fold_button(Fold_button* fold_button);
	virtual void print(bool typ = true);
	virtual Button* get_class_name() { return this; }
	void cal_height();
	bool mouse_on_button();
};

int psz = 9999;

class Fold_button : public Button { // Fold_BUtton -----------------------------------------------------------------
private:
	bool last_open;
	bool open;
	string close_text;
	string open_text;
public:
	Fold_button (): Button() {
		this -> close_text = "+";
		this -> open_text = "-";
		this -> text = close_text;
		this -> last_open = false;
		this -> open = true;
		this -> height = 0;
		this -> id = ++psz;
	}
	// text
	void set_open_text  (string open_text)  { this -> open_text = open_text; }
	void set_close_text (string close_text) { this -> close_text = close_text[0]; }
	string get_open_text  () { return open_text; }
	string get_close_text () { return close_text; }
	// others
	virtual Call_back update(bool is_root);
	virtual Fold_button* get_class_name() { return this; }
};

Button root;

void Button::set_text(string text) {
	for (size_t i = 0; i < text.size(); i++) if (text[i] == '\r')
		text.erase(text.begin() + i, text.begin() + i), i--;
	this -> text = text;
	if (auto_position) cal_height();
}

void Button::cal_height() {
	height = 1;
	for (size_t i = 0; i < text.size(); i++) if (text[i] == '\n')
		height++;
}

void Button::add_son(Button* Button, bool typ) {
	if (!son.empty()) Button -> lst = son.back();
	Button -> fa = this;
	son.push_back(Button);
}

void Button::add_fold_button(Fold_button* fold_button) {
	this -> fold_button = fold_button;
	this -> fold_button -> fa = this;
	this -> fold_button -> clickable = true;
	this -> fold_button -> visible = true;
	this -> fold_button -> set_auto_position(false);
	size_t i; for (i = 0; i < text.size(); i++) if (text[i] == '\n') break;
	this -> fold_button -> set_position(i + deep*2 + 5, 0);
	son.push_back(fold_button);
}


void Button::print(bool typ) { // print --------------------------------------------------------------------------
	// clear old
	if (global_position != last_global_position || last_text != text) { 
		string s(last_text.size(), ' ');
		for (size_t i = 0; i < text.size(); i++) if (text[i] == '\n') s[i] = '\n';
		clear_cache.push_back(output_content(default_color, last_global_position , deep*2, false, indent_line, s));
		last_global_position = global_position;
		last_text = text;
	}
	
	// output new
	if (typ) {
		if (!real_visible) return;
		output_cache.push_back(output_content(recent_color, global_position, deep*2, true, indent_line, text));
	} else {
		string s(text.size(), ' ');
		for (size_t i = 0; i < text.size(); i++) if (text[i] == '\n') s[i] = '\n';
		clear_cache.push_back(output_content(default_color, last_global_position, deep*2, false, indent_line, s));
	}
}

Call_back Button::update(bool is_root) { // update -------------------------------------------------------------------
	if (auto_position) {
		if (lst != nullptr) {
			position.Y = (lst -> height + lst -> position.Y + 1);
		} else if (fa != nullptr) {
			position.Y = 2;
		} else {
			auto_position = false;
		}
	}
	
	if (fa != nullptr) {
		global_position = position + fa -> global_position;
		real_visible = (visible) && (fa -> real_visible) &&
				(!(fa -> folded) || typeid(*(this -> get_class_name())) == typeid(Fold_button));
		if (typeid(*(this -> get_class_name())) == typeid(Button))
			this -> deep = fa -> deep + 1;
		else
			this -> deep = 0;
	} else {
		global_position = position;
		real_visible = visible;
	}
	
	if (id == 0) { // the root
		if (!screen_element_controller.get_mouse_event()) return Call_back();
	}
	
	if (typeid(*(this -> get_class_name())) == typeid(Button) && foldable && fold_button == nullptr) {
		Fold_button *button = new Fold_button();
		add_fold_button(button);
	}
	
	Call_back ret;
	
	if ( mouse_on_button() && real_visible && clickable) {
		recent_color = highlight_color;
		if (recent_mouse_event.dwButtonState && recent_mouse_event.dwButtonState != MOUSE_WHEELED && !pressed) {
			recent_color = click_color;
			pressed = true;
			ret.push_back(id);
		} else if (!recent_mouse_event.dwButtonState) {
			pressed = false;
		}
	} else {
		recent_color = normal_color;
	}
	
	if (recent_color != last_color || last_visible != real_visible ||
				global_position != last_global_position || last_text != text) {
		print(real_visible);
		last_color = recent_color;
		last_visible = real_visible;
		last_text = text;
	}
	
	if (typeid(*(this -> get_class_name())) == typeid(Fold_button)) {
		height = -1;
	} else if (auto_position) {
		cal_height();
	}
	
	for (Button* Button: son) {
		ret += (Button -> update(false));
		if (!folded) height += Button -> height + 1;
	}
	
	if (is_root) {
		fresh_print();
	}
	
	return ret;
}


bool Button::mouse_on_button() {
	int h = 1;
	for (size_t i = 0; i < text.size(); i++) if (text[i] == '\n') h++;
	return recent_mouse_position.Y >= global_position.Y &&
			recent_mouse_position.Y <= global_position.Y + h - 1 &&
			recent_mouse_position.X >= (global_position.X + deep * 2) &&
			recent_mouse_position.X <= (global_position.X + deep * 2) + (short)text.size() - 1;
}

Call_back Fold_button::update(bool is_root) { // update ----------------------------------------------------
	Call_back ret = this -> Button::update(false);
	
	if (!ret.empty()) {
		open = !open;
	}
	
	if (open != last_open) {
		last_open = open;
		if (open == true) {
			text = open_text;
			this -> fa -> unfold();
		} else {
			text = close_text;
			fa -> fold();
		}
	}
	
	return Call_back();
}

bool Screen_element_controller::get_mouse_event() {
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

void Screen_element_controller::set_color(Color color) {
	const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color.mix());
}

void Screen_element_controller::set_mouse_position (const short x, const short y) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{x, y});
}
void Screen_element_controller::set_mouse_position (const COORD cursorPosition) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Screen_element_controller::Set_cursor_visible(bool visible) {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}

void Screen_element_controller::Set_console_mode(bool QuickEditMode, bool InsertMode, bool MouseInput) {
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

void Screen_element_controller::start() {
	Set_cursor_visible(false);
	Set_console_mode(false, false, true);
	FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	set_color(default_color);
	root.set_visible(true);
	root.update(1);
	root.set_deep(0);
}

void Screen_element_controller::stop() {
	Set_cursor_visible(true);
	Set_console_mode(true, true, true);
}

// Console command

#endif

