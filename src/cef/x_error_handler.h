//
// Created by geraltigas on 24-1-15.
//

#ifndef X_ERROR_HANDLER_H
#define X_ERROR_HANDLER_H

#include <X11/Xlib.h>
int XErrorHandlerImpl(Display* display, XErrorEvent* event);
int XIOErrorHandlerImpl(Display* display);

#endif //X_ERROR_HANDLER_H
