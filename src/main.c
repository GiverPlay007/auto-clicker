#include <libevdev/libevdev-uinput.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "read_key.c"
#include "clicker.c"

static int should_stop = 0;
static int should_click = 0;

void on_pressed(int code);

int main()
{
  struct libevdev_uinput *uidev;
  struct libevdev *dev;

  dev = libevdev_new();
  libevdev_set_name(dev, "Clicker");

  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);

  int err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);

  if (err != 0)
  {
    printf("Error: %i\n", err);
    return 0;
  }

  pthread_t event_thread;
  pthread_create(&event_thread, NULL, read_key_loop, (void *) on_pressed);

  while(!should_stop)
  {
    if(should_stop)
      break;

    system("sleep 0.25");

    if(should_click)
      click(uidev);
  }

  libevdev_uinput_destroy(uidev);
  printf("Goodbye!\n");

  return 0;
}

void on_pressed(int code)
{
  switch(code)
  {
    case 74:
      should_stop = 1;
      break;
    case 78:
      should_click = !should_click;
      break;
    default:
      break;
  }
}