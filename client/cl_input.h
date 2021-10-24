/*
 * cl_input.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_INPUT_H_
#define CL_INPUT_H_

#include <SDL2/SDL.h>

void cl_inputInit();
void cl_inputUpdate();
bool cl_isKeyPressed(Uint16 key);
bool cl_isButtonPressed(Uint8 button);
double cl_getMouseDX();
double cl_getMouseDY();
double cl_getMouseSensitivity();
void cl_setMouseSensitivity(double sens);

#endif /* CL_INPUT_H_ */
