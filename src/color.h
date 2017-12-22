#ifndef COLOR_H
#define COLOR_H


// Based on https://misc.flogisoft.com/bash/tip_colors_and_formatting


#define C_RESET      "\x1b[0m"
#define C_RESET_FG   "\x1b[39m"
#define C_RESET_BG   "\x1b[49m"


#define C_BOLD       "\x1b[1m"
#define C_DIM        "\x1b[2m"
#define C_UNDERLINE  "\x1b[4m"
#define C_INVERSE    "\x1b[7m"
#define C_HIDDEN     "\x1b[8m"

#define C_RESET_BOLD       "\x1b[21m"
#define C_RESET_DIM        "\x1b[22m"
#define C_RESET_UNDERLINE  "\x1b[24m"
#define C_RESET_INVERSE    "\x1b[27m"
#define C_RESET_HIDDEN     "\x1b[28m"


#define C_RED      "\x1b[31m"
#define C_GREEN    "\x1b[32m"
#define C_YELLOW   "\x1b[33m"
#define C_BLUE     "\x1b[34m"
#define C_MAGENTA  "\x1b[35m"
#define C_CYAN     "\x1b[36m"

#define C_LIGHT_RED      "\x1b[91m"
#define C_LIGHT_GREEN    "\x1b[92m"
#define C_LIGHT_YELLOW   "\x1b[93m"
#define C_LIGHT_BLUE     "\x1b[94m"
#define C_LIGHT_MAGENTA  "\x1b[95m"
#define C_LIGHT_CYAN     "\x1b[96m"

#define C_WHITE       "\x1b[97m"
#define C_LIGHT_GRAY  "\x1b[37m"
#define C_DARK_GRAY   "\x1b[90m"
#define C_BLACK       "\x1b[30m"


#define C_BG_RED      "\x1b[41m"
#define C_BG_GREEN    "\x1b[42m"
#define C_BG_YELLOW   "\x1b[43m"
#define C_BG_BLUE     "\x1b[44m"
#define C_BG_MAGENTA  "\x1b[45m"
#define C_BG_CYAN     "\x1b[46m"

#define C_BG_LIGHT_RED      "\x1b[101m"
#define C_BG_LIGHT_GREEN    "\x1b[102m"
#define C_BG_LIGHT_YELLOW   "\x1b[103m"
#define C_BG_LIGHT_BLUE     "\x1b[104m"
#define C_BG_LIGHT_MAGENTA  "\x1b[105m"
#define C_BG_LIGHT_CYAN     "\x1b[106m"

#define C_BG_WHITE       "\x1b[107m"
#define C_BG_LIGHT_GRAY  "\x1b[47m"
#define C_BG_DARK_GRAY   "\x1b[100m"
#define C_BG_BLACK       "\x1b[40m"


#endif
