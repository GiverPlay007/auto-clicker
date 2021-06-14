#include <stdlib.h>
#include <stdio.h>
#include <libevdev/libevdev-uinput.h>

int main()
{
  struct libevdev *dev;
  struct libevdev_uinput *uidev;

  dev = libevdev_new();
  libevdev_set_name(dev, "Fake device");

  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);

  int err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);

  if (err != 0)
    return err;

  for(int i = 0; i < 10; i++)
  {
    libevdev_uinput_write_event(uidev, EV_KEY, BTN_LEFT, 1);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    libevdev_uinput_write_event(uidev, EV_KEY, BTN_LEFT, 0);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
  }

  libevdev_uinput_destroy(uidev);
  printf("Goodbye!\n");

  return 0;
}