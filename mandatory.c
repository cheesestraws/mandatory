#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "backend.h"

/* This contains the name of the executable.  It must be populated from argv[0]
   by main().  It is for error message printing purposes. */
static char* executable_name;



/* 
   print_error prints out an error message to standard input. Exciting, eh? 
*/
void print_error(char* error) {
	fprintf(stderr, "%s: %s\n", executable_name, error);
}



/* 
   Lets the user select a window on the screen by clicking on it. 
*/
Window userWindowSelection(Display* d) {
	Window selectedWindow = 0;
	XEvent event;

	/* TODO: Change cursor here like xprop does */

	XGrabPointer(d, DefaultRootWindow(d), 0, ButtonPressMask, GrabModeSync, GrabModeAsync, DefaultRootWindow(d), None, CurrentTime);
	
	while (!selectedWindow) {
		XAllowEvents(d, SyncPointer, CurrentTime);
		XWindowEvent(d, DefaultRootWindow(d), ButtonPressMask, &event);
		selectedWindow = event.xbutton.subwindow;
	}

	XUngrabPointer(d, CurrentTime);
	return selectedWindow;
}



/*
   Does an XSelectInput on the provided window and its children.
*/
void SelectInputAndChildren(Display* disp, Window win, long event_mask) {
	Window root, parent;
	Window* children;
	unsigned int childCount;
	int i;


	XSelectInput(disp, win, event_mask);

	if (!XQueryTree(disp, win, &root, &parent, &children, &childCount)) {
		return;
	}

	/* Base case: the window has no children. */
	if (childCount == 0) {
		return;
	}

	/* Recursive case: the window has children. */
	for (i = 0; i < childCount; i++) {
		SelectInputAndChildren(disp, children[i], event_mask);
	}

	XFree(children);
}

int main(int argc, char** argv) {
	BackendState state;
	Display* disp;
	Window inputWindow;
	char* display_name;
	XEvent evt;
	char keysym_buffer[256];
	KeySym ks;

	/* Save the name we were invoked as for error message purposes later */
	executable_name = argv[0];

	backend_init(&state);

	/* Find the user's display. */
	display_name = getenv("DISPLAY");
	if (!display_name) {
		print_error("No display set in environment.");
		return 1;
	}

	disp = XOpenDisplay(display_name);
	if (!disp) {
		print_error("Could not open display.");
		return 1;
	}

	printf("Please select a window.\n");
	inputWindow = userWindowSelection(disp);
	printf("Thankyou!\n");
	SelectInputAndChildren(disp, inputWindow, KeyPressMask);

	while(1) {
		keysym_buffer[0] = '\0';
		XNextEvent(disp, &evt);
		
		if(evt.type == KeyPress) {
			if (XLookupString(&evt, keysym_buffer, 256, 
			    &ks, NULL)) {
				if (ks == XK_BackSpace) {
					backend_handleBackspace(&state);
				} else {
					if (keysym_buffer[0] >= ' ') {
						backend_handleChar(&state,
						    keysym_buffer[0]);
					}
				}
			}
		}
	}

	backend_terminate(&state);
	XCloseDisplay(disp);
	
}
