#include <libevdev/libevdev-uinput.h>

void click(struct libevdev_uinput *dev)
{
  libevdev_uinput_write_event(dev, EV_KEY, BTN_LEFT, 1);
  libevdev_uinput_write_event(dev, EV_SYN, SYN_REPORT, 0);
  libevdev_uinput_write_event(dev, EV_KEY, BTN_LEFT, 0);
  libevdev_uinput_write_event(dev, EV_SYN, SYN_REPORT, 0);
}