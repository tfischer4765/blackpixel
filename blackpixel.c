#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

// Global variables
bool running = true;
Display *display = NULL;

// Signal handler for SIGTERM
void handle_sigterm(int signum) {
    if (display != NULL) {
        // Close display
        XCloseDisplay(display);
    }
    exit(0);
}

int main() {
    Window window;
    XEvent event;
    int screen;

    // Set up signal handler for SIGTERM
    signal(SIGTERM, handle_sigterm);

    // Connect to X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    // Get default screen
    screen = DefaultScreen(display);

    // Create window
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 10, 10, 0,
                                  BlackPixel(display, screen), WhitePixel(display, screen));

    // Select events to be notified of
    XSelectInput(display, window, ExposureMask | StructureNotifyMask);

    // Set close atom
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    // Map window to screen
    XMapWindow(display, window);

    // Event loop
    while (running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case Expose:
                // Draw black rectangle
                XSetForeground(display, DefaultGC(display, screen), BlackPixel(display, screen));
                XFillRectangle(display, window, DefaultGC(display, screen), 0, 0, 10, 10);
                break;
            case ClientMessage:
                if (event.xclient.data.l[0] == wm_delete_window)
                    running = false;
                break;
        }
    }

    // Close display
    XCloseDisplay(display);
    return 0;
}
