#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


int main() {

    XEvent event;
    uint64_t black, white;

    // Open connection to X server
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    // Get the default screen
    int screen = DefaultScreen(display);

    // Get black and white pixel values
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

    // Create the window
    Window* window;
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 800, 600, 1, black, white);

    // Select kind of events we are interested in
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Map (show) the window
    XMapWindow(display, window);

    // Create a "Graphics Context"
    GC gc = XCreateGC(display, window, 0, NULL);

    int width = 800;
    int height = 600;
    int depth = DefaultDepth(display, screen);
    uint32_t* front_buffer = malloc(width * height * 4);
    uint32_t* back_buffer = malloc(width * height * 4);

    // Event loop
    while (1) {
        XNextEvent(display, &event);

        // Draw or handle events as needed
        if (event.type == Expose) {
            // Draw a simple pixel buffer

            XImage *image = XCreateImage(display, DefaultVisual(display, screen), depth, ZPixmap, 0, front_buffer, width, height, 32, 0);
            
            int i = 0;

            // Fill the pixel buffer with some color pattern
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    long pixel = x* y; // Simple pattern
                    XPutPixel(image, x, y, (pixel << 16) | (pixel << 8) | pixel);
                }
            }

            // Put the image onto the window
            XPutImage(display, window, gc, image, 0, 0, 0, 0, width, height);

            // Cleanup
            XDestroyImage(image);
        }

        // Exit on key press
        if (event.type == KeyPress)
            break;
    }

    // Close the display
    XCloseDisplay(display);
    return 0;
}
