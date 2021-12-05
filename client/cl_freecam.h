/*
 * cl_freecam.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_FREECAM_H_
#define CL_FREECAM_H_

#include "cl_camera.h"
#include <rg_demo.h>

void cl_freecam_init(Camera* cam);
void cl_freecam_update(double dt);

// Demo
void cl_freecam_recordDemo();
void cl_freecam_stopDemo();
void cl_freecam_addFrame();
rg_demo* cl_freecam_getDemo();


#endif /* CL_FREECAM_H_ */
