#include <stdlib.h>
#include <stdio.h>
#include <libevdev/libevdev-uinput.h>

void click(struct libevdev_uinput *dev)
{
  libevdev_uinput_write_event(dev, EV_KEY, BTN_LEFT, 1);
  libevdev_uinput_write_event(dev, EV_SYN, SYN_REPORT, 0);
  libevdev_uinput_write_event(dev, EV_KEY, BTN_LEFT, 0);
  libevdev_uinput_write_event(dev, EV_SYN, SYN_REPORT, 0);
}

int main()
{
  struct libevdev *dev;
  struct libevdev_uinput *uidev;

  dev = libevdev_new();
  libevdev_set_name(dev, "Clicker");

  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);

  int err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);

  if (err != 0)
  {
    printf("Error: %i\n", err);
    return err;
  }

  while(1)
  {
    system("sleep 0.25");
    click(uidev);
  }

  libevdev_uinput_destroy(uidev);
  printf("Goodbye!\n");

  return 0;
}
