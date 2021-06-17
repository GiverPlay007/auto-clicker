#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>

void *read_key_loop(void *arguments)
{
  void (*on_pressed)() = arguments;

  struct input_event ev;
  const char *keyboard = "/dev/input/event10";
  
  int fd = open(keyboard, O_RDONLY);

  if (fd == -1)
  {
    fprintf(stderr, "Cannot open %s: %s.\n", keyboard, strerror(errno));
    return 0;
  }

  ssize_t n;
  
  while (1)
  {
    n = read(fd, &ev, sizeof ev);

    if (n == (ssize_t)-1)
    {
      if (errno == EINTR)
        continue;
      else
        break;
    }
    else if (n != sizeof ev)
    {
      errno = EIO;
      break;
    }

    if (ev.type == EV_KEY && ev.value == 1)
    {
      on_pressed(ev.code);
    }
  }
}