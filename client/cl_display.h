/*
 * cl_display.h
 *
 *  Created on: 3 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_DISPLAY_H_
#define CL_DISPLAY_H_


void cl_display_init();
void cl_display_open();
void cl_display_close();
void cl_display_update();
void cl_display_toggleFullscreen();
void cl_display_setSwapInterval(float interval);

void cl_display_setMouseGrabbed(bool g);

float cl_display_getAspect();
float cl_display_getWidth();
float cl_display_getHeight();

#endif /* CL_DISPLAY_H_ */
