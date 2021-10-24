/*
 * clguiscreen.h
 *
 *  Created on: Oct 1, 2021
 *      Author: alex9932
 */

#ifndef CL_GUISCREEN_H_
#define CL_GUISCREEN_H_

#include "cl_guiwindow.h"
#include <vector>
#include <rg_engine.h>

class GuiScreen {
	private:
		std::vector<Window*> windows;

	public:
		GuiScreen();
		virtual ~GuiScreen();

		bool eventHandler(rg_Event* event);
		void drawScreen(double dt);

		RG_INLINE void addWindow(Window* wnd) { windows.push_back(wnd); }
};

#endif /* CL_GUISCREEN_H_ */
