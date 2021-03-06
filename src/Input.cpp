#include <Input.h>

#include <stdio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>


namespace
{

void init_keyboard();
void close_keyboard();
int kbhit();
int readch();

}

Input::Input(Watch* watch, bool isDebug)
	: watch_(watch)
	, shouldStop_(false)
	, isDebug_(isDebug)
{
	init_keyboard();
}

Input::~Input()
{
	close_keyboard();
}


void Input::displayUsage() const
{
	std::cout << "usage: a,b,c,d,e - buttons labeled A,B,C,D,E\n"
		<< "\tu - display this message\n"
		<< "\tq - quit application"
		<< std::endl;
}

void Input::frame()
{
	if(!kbhit())
		return;

	char ch = readch();
	if(isDebug_)
		std::cout << "ch pressed=" << ch << std::endl;

	if(ch == 'q' || ch == 'Q')
	{
		shouldStop_ = true;
		return;
	}

	if(ch == 'u' || ch == 'U')
	{
		displayUsage();
		return;
	}

	if(ch == 'a' || ch == 'A')
	{
		watch_->buttonPressed(Watch::A);
		return;
	}

	if(ch == 'b' || ch == 'B')
	{
		watch_->buttonPressed(Watch::B);
		return;
	}	

	if(ch == 'c' || ch == 'C')
	{
		watch_->buttonPressed(Watch::C);
		return;
	}

	if(ch == 'd' || ch == 'D')
	{
		watch_->buttonPressed(Watch::D);
		return;
	}	

	if(ch == 'e' || ch == 'E')
	{
		watch_->buttonPressed(Watch::E);
		return;
	}

	
	std::cout << "not recognized input: " << ch << "\n";
	displayUsage();
}

namespace
{

struct termios initial_settings, new_settings;
int peek_character = -1;

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit()
{
    char ch = 0;
    int nread = 0;

    if(peek_character != -1)
        return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
	while(true)
	{
		nread = read(0,&ch,1);
		if(nread != 1)
			break;

        peek_character = ch;
	}
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    return ch == 0? 0 : 1;
}

int readch()
{
    char ch;

    if(peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

}
