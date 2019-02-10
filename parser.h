#ifndef PARSER_H
#define PARSER_H

#include <linux/keyboard.h>
#include <linux/input.h>
#include <stdio.h>
#include <time.h>

const char* keymap[] = { "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<BACKSPACE>", "<TAB>",
                        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "<ENTER>", "<LCTRL>", "a", "s", "d", "f",
                        "g", "h", "j", "k", "l", ";", "'", "`", "<LSHIFT>", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
                        "/", "<RSHIFT>", "\0", "<LALT>", "<SPACE>", "<CAPSLOCK>", "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>",
                        "<F8>", "<F9>", "<F10>", "<NUMLOCK>", "<SCROLLLOCK>", "<HOME>", "<UP>", "<PGUP>", "-", "<LEFT>", "5",
                        "<RTARROW>", "+", "<END>", "<DOWN>", "<PGDN>", "<INS>", "<DEL>", "\0", "\0", "\0", "<F11>", "<F12>",
                        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "<ENTER>", "<RCTRL>", "/", "<PRTSCR>", "<RALT>", "\0", "<HOME>",
                        "<UP>", "<PGUP>", "<LEFT>", "<RIGHT>", "<END>", "<DOWN>", "<PGDN>", "<INSERT>", "<DEL>", "\0", "\0",
                        "\0", "\0", "\0", "\0", "\0", "<PAUSE>", "\0", "\0", "\0", "\0", "\0", "<LCMD/LWIN>", "<RCMD/RWIN>"};
#endif
