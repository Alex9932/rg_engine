/*
 * cl_guiwindow.h
 *
 *  Created on: Oct 1, 2021
 *      Author: alex9932
 */

#ifndef CL_GUIWINDOW_H_
#define CL_GUIWINDOW_H_

#include <rg_engine.h>

class Window {
	public:
		float x, y;
		float w, h;
		bool isGrabbed = false;

		Window();
		virtual ~Window();
		void draw(double dt);
		void eventHandler(rg_Event* event);

//		RG_INLINE void setX(float x) { this->x = x; }
//		RG_INLINE void setY(float y) { this->y = y; }
//
//		RG_INLINE float getX() { return x; }
//		RG_INLINE float getY() { return y; }
//		RG_INLINE float getW() { return w; }
//		RG_INLINE float getH() { return h; }
};

#endif /* CL_GUIWINDOW_H_ */
