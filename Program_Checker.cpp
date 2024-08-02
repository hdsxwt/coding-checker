#include<iostream>
#include<string>
#include<windows.h>


#define KEY(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

using std::string;
using std::endl;

void set_position(short x, short y) {
	COORD position = { x, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, position);
}

string toString(int i) {
	string s = "";
	while (i) {
		s = char('0'+i%10) + s;
		i /= 10;
	}
	return s;
}

int toInt(const char *s) {
	int ret = 0;
	for (int i = 0; i < (int)strlen(s); i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			ret = ret * 10 + s[i] - '0';
		} else {
			throw (1);
		}
	}
	return ret;
}

void color_printf(const char* s, int front_color,int back_color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | back_color*16 | FOREGROUND_INTENSITY | front_color);
	printf(s);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}

bool vis[3];
const char s[3][20] = {"Data_Generator", "Answer_Generator", "Tested_Program"};

void init() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | 7);
	vis[0] = vis[1] = vis[2] = false;
}

void welcome() {
	system("cls");
	printf("welcome to use Program checker 1.0.1\n\n");
	for (int i = 0; i < 3; i++) {
		printf("%s: ", s[i]);
		if (vis[i]) {
			color_printf("updated\n", 2, 15);
		} else {
			color_printf("not updated\n", 4, 15);
		}
	}
	printf("\n");
	for (int i = 0; i < 3; i++) {
		printf("enter %d to update file: %s\n", i+1, s[i]);
	}
	printf("enter 4 to start testing the program.\n");
	printf("enter 5 to delete all the sample before.\n");
	printf("enter 6 to open the sample files.\n");
	printf("enter 0 to exit.");
	printf("\n");
}

void exit() {
	system("del .\\task\\in.txt");
	system("del .\\task\\out.txt");
	system("del .\\task\\ans.txt");
	system("cls");
}

int sum = 0;
int cnt = 0;

void testing() {
	system("md task");
	int n;
	while (1) {
		system("cls");
		printf("please enter the number of sample you want (enter 0 to exit): ");
		char c[20];
		scanf("%s", c);
	
		try {
			n = toInt(c);
			if (n < 0) throw(2);
			
			if (n == 0) return;
			else break;
		} catch (int x) {
			if (x == 1) {
				printf("please enter a number.\n");
			} else {
				printf("please enter a number larger than 0.\n");
			}
		}
	}
	printf("press ");
	color_printf("Q", 4, 15);
	printf(" to stop in the proccess.\n");
	Sleep(700);
	
	cnt = 0;
	int i;
	system("cls");
	for (i = 0; i < n; i++) {
		if (KEY('Q')) {
			while (KEY('Q'));
			system("cls");
			break;
		}
		set_position(0, 0);
		
		printf("press ");
		color_printf("Q", 4, 15);
		printf(" to stop in the proccess.\n");
		
		putchar('[');
		int first = true;
		for (int j = 0; j < 100; j++) {
			if (j*n < i*100) {
				putchar('=');
			} else {
				putchar(" >"[first]);
				first = false;
			}
		}
		putchar(']');
		putchar(' ');
		printf("%d\t/%d\n", i, n);
		
		printf("Data Generating...\n");
		system("Data_Generator.exe > .\\task\\in.txt");
		printf("Answer Generator...\n");
		system("Answer_Generator.exe < .\\task\\in.txt > .\\task\\ans.txt");
		printf("Test Running...\n");
		system("Tested_Program.exe < .\\task\\in.txt > .\\task\\out.txt");
		printf("check ...");
		if (system("fc .\\task\\ans.txt .\\task\\out.txt")) {
			string s_in = "copy .\\task\\in.txt .\\task\\in" + toString(++sum) + ".txt";
			string s_out = "copy .\\task\\out.txt .\\task\\out" + toString(sum) + ".txt";
			string s_ans = "copy .\\task\\ans.txt .\\task\\ans" + toString(sum) + ".txt";
			system(s_in.data());
			system(s_out.data());
			system(s_ans.data());
			++cnt;
		}
	}
	char message[40];
	sprintf(message, "Done!\nSample: %d\nError: %d\nAC\%: %.2f\%", i, cnt, (double)(i - cnt)/i * 100);
	MessageBox(NULL, message, "Program Checker", MB_OK); // TODO
	printf("\n\n");
}

void del_all() {
	system("del task /Q");
	sum = 0;
}

void open_files() {
	ShellExecute(NULL, NULL, ".\\task\\", NULL, NULL, SW_MAXIMIZE);
}

int main(int argc, char* argv[]) {
	init();
	
	while (true) {
		welcome();
		int op;
		while (1) {
			char c[20];
			scanf("%s", c);
			
			try {
				op = toInt(c);
				if ((op != 0) && (op < 1 || op > 6)) throw (2);
				break;
			} catch (int x) {
				if (x == 1) {
					printf("please enter a number.\n");
				} else {
					printf("please enter a number between 1 and 6 or 0\n");
				}
			}
		}
		
		
		if (op == 1 || op == 2 || op == 3) {
			string opt1 = "notepad "; opt1 += s[op-1]; opt1 += ".cpp";
			system(opt1.data());
			
			string opt2 = "clang++ "; opt2 += s[op-1]; opt2 += ".cpp -o "; opt2 += s[op-1]; opt2 += ".exe -std=c++14 -O2";
			int res = system(opt2.data());
			if (res == 0) {
				vis[op-1] = true;
				color_printf("done!\n", 2, 15);
			} else {
				color_printf("error!\n", 4, 15);
				system("pause");
			}
			Sleep(700);
		} else if (op == 4) {
			testing();
			color_printf("done!\n", 2, 15);
			Sleep(700);
		} else if (op == 5) {
			del_all();
			color_printf("done!\n", 2, 15);
			Sleep(700);
		} else if (op == 6) {
			open_files();
			color_printf("done!\n", 2, 15);
			Sleep(700);
		} else if (op == 0) {
			exit();
			color_printf("done!\n", 2, 15);
			Sleep(700);
			return 0;
		}
	}
}