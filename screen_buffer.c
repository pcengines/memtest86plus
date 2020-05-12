/* screen_buffer.c - MemTest-86  Version 3.3
 *
 * Released under version 2 of the Gnu Public License.
 * By Jani Averbach, Jaa@iki.fi, 2001
 */

#include "test.h"
#include "screen_buffer.h"

#define SCREEN_X 80
#define SCREEN_Y 25
#define Y_SIZE SCREEN_Y
/*
 * X-size should by one of by screen size,
 * so that there is room for ending '\0'
 */
#define X_SIZE SCREEN_X+1

static char screen_buf[Y_SIZE][X_SIZE];
static char debug_screen_buf[Y_SIZE][X_SIZE];


#ifdef SCRN_DEBUG

char *padding = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";

#define CHECK_BOUNDS(y,x) do {if (y < 0 || Y_SIZE <= y || x < 0 || X_SIZE <= x) print_error("out of index");}while(0)

#else /* ! SCRN_DEBUG */

#define CHECK_BOUNDS(y,x)

#endif /* SCRN_DEBUG */

char
get_scrn_buf(const int y,
             const int x)
{
    CHECK_BOUNDS(y,x);
    return screen_buf[y][x];
}


void
set_scrn_buf(const int y,
             const int x,
             const char val)
{
    CHECK_BOUNDS(y,x);
    screen_buf[y][x] = val;
}

void clear_screen_buf(short tty)
{
    int y, x;

    char (*buf)[X_SIZE] = 
        (tty == DEBUG_SERIAL_TTY) ? debug_screen_buf : screen_buf;

    for (y=0; y < SCREEN_Y; ++y){
        for (x=0; x < SCREEN_X; ++x){
            CHECK_BOUNDS(y,x);
            buf[y][x] = ' ';
        }
        CHECK_BOUNDS(y,SCREEN_X);
        buf[y][SCREEN_X] = '\0';
    }
}

void tty_print_region(const int pi_top,
                      const int pi_left,
                      const int pi_bottom,
                      const int pi_right)
{
    int y;
    char tmp;

    for (y=pi_top; y < pi_bottom; ++y){
        CHECK_BOUNDS(y, pi_right);

        tmp = screen_buf[y][pi_right];
        screen_buf[y][pi_right] = '\0';

        CHECK_BOUNDS(y, pi_left);
        ttyprint(y, pi_left, &(screen_buf[y][pi_left]));

        screen_buf[y][pi_right] = tmp;
    }
}

void tty_print_line(
	int y, int x, short tty, const char *text)
{
	char (*buf)[X_SIZE] = 
        (tty == DEBUG_SERIAL_TTY) ? debug_screen_buf : screen_buf;

	for(; *text && (x < SCREEN_X); x++, text++) {
		if (*text != buf[y][x]) {
			break;
		}
	}
	/* If there is nothing to do return */
	if (*text == '\0') {
		return;
	}
	ttyprint_tty(y, x, tty, text);
	for(; *text && (x < SCREEN_X); x++, text++) {
		buf[y][x] = *text;
	}
}


void tty_print_screen(void)
{
#ifdef SCRN_DEBUG
    int i;

    for (i=0; i < SCREEN_Y; ++i)
        ttyprint(i,0, padding);
#endif /* SCRN_DEBUG */

    tty_print_region(0, 0, SCREEN_Y, SCREEN_X);
}

void print_error(char *pstr)
{

#ifdef SCRN_DEBUG
    ttyprint(0,0, padding);
#endif /* SCRN_DEBUG */

    ttyprint(0,35, pstr);

    while(1);
}
