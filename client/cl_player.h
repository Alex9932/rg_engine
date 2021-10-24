/*
 * cl_player.h
 *
 *  Created on: Sep 14, 2021
 *      Author: alex9932
 */

#ifndef CL_PLAYER_H_
#define CL_PLAYER_H_

#include "cl_camera.h"
#include <rg_vecmath.h>

void cl_player_init(cl_camera* cam);
void cl_player_update(double dt);


#endif /* CL_PLAYER_H_ */
