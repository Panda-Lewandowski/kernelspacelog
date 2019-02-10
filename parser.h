#ifndef PARSER_H
#define PARSER_H

#include <linux/keyboard.h>
#include <linux/input.h>
#include <stdio.h>
#include <time.h>

#define KEYS_NUM 127 

const char* keymap[] = { "<UKNWN>", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<BACKSPACE>", "<TAB>",
                        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "<ENTER>", "<LCTRL>", "a", "s", "d", "f",
                        "g", "h", "j", "k", "l", ";", "'", "`", "<LSHIFT>", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
                     	"/", "<RSHIFT>", "<UKNWN>", "<LALT>", "<SPACE>", "<CAPSLOCK>", "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", 
			"<F6>", "<F7>", "<F8>", "<F9>", "<F10>", "<NUMLOCK>", "<SCROLLLOCK>", "<HOME>", "<UP>", "<PGUP>", 
			"-", "<LEFT>", "5", "<RTARROW>", "+", "<END>", "<DOWN>", "<PGDN>", "<INS>", "<DEL>", "<UKNWN>", "<UKNWN>", 				"<UKNWN>", "<F11>", "<F12>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", 				"<ENTER>", "<RCTRL>", "/", "<PRTSCR>", "<RALT>", "<UKNWN>", "<HOME>", "<UP>", "<PGUP>", "<LEFT>", "<RIGHT>", 				"<END>", "<DOWN>", "<PGDN>", "<INSERT>", "<DEL>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", 				"<UKNWN>", "<UKNWN>", "<PAUSE>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", "<UKNWN>", 
			"<LCMD/LWIN>", "<RCMD/RWIN>"};
#endif
