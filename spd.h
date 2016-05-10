/*
 * MemTest86+ V5.00 Specific code (GPL V2.0)
 * By Samuel DEMEULEMEESTER, sdemeule@memtest.org
 * http://www.canardpc.com - http://www.memtest.org
 */

#ifndef _SPD_H_
#define _SPD_H_

//
// For platforms that do not support SPD information
// spd related functionality should be disabled
// using SPD_DISABLED macro
// TODO: maybe this should be moved to config

#define SPD_DISABLED

void get_spd_spec(void);

void show_spd(void);

#endif


