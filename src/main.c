#include <libevdev/libevdev-uinput.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include "read_key.c"
#include "clicker.c"

static int should_stop = 0;
static int should_click = 0;
static int delay = 100;

void on_pressed(int code);

void sleepms(long int ms);

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

    sleepms(delay);

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
    case 55:
      should_click = !should_click;
      break;
    case 74:
      delay += 10;
      if(delay >= 10000) delay = 10000;
      break;
    case 78:
      delay -= 10;
      if(delay < 10) delay = 10;
      break;
    case 98:
      should_stop = 1;
      break;
    default:
      break;
  }
}

void sleepms(long ms)
{
  struct timespec ts;
  int res;

  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while(res && errno == EINTR);
}