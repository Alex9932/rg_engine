/*
 * cl_democam.h
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#ifndef CL_DEMOCAM_H_
#define CL_DEMOCAM_H_

#include "cl_camera.h"
#include <rg_demo.h>

void cl_democam_init(Camera* cam);
void cl_democam_update();
void cl_democam_start();
void cl_democam_end();


#endif /* CL_DEMOCAM_H_ */
