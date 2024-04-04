#include <X11/X.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
  Display *display;
  Window root_window;
  XEvent event;

  display = XOpenDisplay(0);
  root_window = DefaultRootWindow(display);

  int fd, bytes;
  unsigned char data[3];

  const char *pDevice = "/dev/input/mice";

  XWindowAttributes attr;
  XGetWindowAttributes(display, root_window, &attr);

  XImage *image = XGetImage(display, root_window, 0, 0, attr.width, attr.height,
                            AllPlanes, ZPixmap);

  // Open Mouse
  fd = open(pDevice, O_RDONLY);
  if (fd == -1) {
    printf("ERROR Opening %s\n", pDevice);
    return -1;
  }

  int left;
  while (1) {
    // Read Mouse
    bytes = read(fd, data, sizeof(data));

    if (bytes > 0) {
      left = data[0] & 0x1;
      XQueryPointer(display, root_window, &event.xbutton.root,
                    &event.xbutton.subwindow, &event.xbutton.x_root,
                    &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                    &event.xbutton.state);


      if(left == 1){
        unsigned long pixel = XGetPixel(image, event.xmotion.x, event.xmotion.y);
        XColor color;
        color.pixel = pixel;
        XQueryColor(display, DefaultColormap(display, DefaultScreen(display)),
                    &color);
        printf("Color at : %d:%d is Red: %d, Green : %d, Blue : %d\n", event.xmotion.x, event.xmotion.y, color.red >> 8, color.green >> 8 , color.blue >> 8);
      }

      usleep(10000);

    }
  }
}
