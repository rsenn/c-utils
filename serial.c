/*
 * Copyright 2012 - 2014 Thomas Buck
 * <xythobuz@xythobuz.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <poll.h>

#include "serial.h"
#include "lib/uint64.h"
#include "lib/io.h"

#ifndef XON
#define XON 0x11
#endif

#ifndef XOFF
#define XOFF 0x13
#endif

#ifndef TIMEOUT
#define TIMEOUT 2
#endif

int
serial_open(const char* port, unsigned int baud) {
  struct termios options;

  int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1) {
    fprintf(stderr,
            "Couldn't open port "
            "\"%s\": %s\n",
            port,
            strerror(errno));
    return -1;
  }

  tcgetattr(fd, &options);

  options.c_lflag = 0;
  options.c_oflag = 0;
  options.c_iflag = 0;

  // Set Baudrate
  switch(baud) {
    case 0:
      cfsetispeed(&options, B0);
      cfsetospeed(&options, B0);
      break;
    case 50:
      cfsetispeed(&options, B50);
      cfsetospeed(&options, B50);
      break;
    case 75:
      cfsetispeed(&options, B75);
      cfsetospeed(&options, B75);
      break;
    case 110:
      cfsetispeed(&options, B110);
      cfsetospeed(&options, B110);
      break;
    case 134:
      cfsetispeed(&options, B134);
      cfsetospeed(&options, B134);
      break;
    case 150:
      cfsetispeed(&options, B150);
      cfsetospeed(&options, B150);
      break;
    case 200:
      cfsetispeed(&options, B200);
      cfsetospeed(&options, B200);
      break;
    case 300:
      cfsetispeed(&options, B300);
      cfsetospeed(&options, B300);
      break;
    case 600:
      cfsetispeed(&options, B600);
      cfsetospeed(&options, B600);
      break;
    case 1200:
      cfsetispeed(&options, B1200);
      cfsetospeed(&options, B1200);
      break;
    case 1800:
      cfsetispeed(&options, B1800);
      cfsetospeed(&options, B1800);
      break;
    case 2400:
      cfsetispeed(&options, B2400);
      cfsetospeed(&options, B2400);
      break;
    case 4800:
      cfsetispeed(&options, B4800);
      cfsetospeed(&options, B4800);
      break;
    case 9600:
      cfsetispeed(&options, B9600);
      cfsetospeed(&options, B9600);
      break;
    case 19200:
      cfsetispeed(&options, B19200);
      cfsetospeed(&options, B19200);
      break;
    case 38400:
      cfsetispeed(&options, B38400);
      cfsetospeed(&options, B38400);
      break;
    case 57600:
      cfsetispeed(&options, B57600);
      cfsetospeed(&options, B57600);
      break;
    case 115200:
      cfsetispeed(&options, B115200);
      cfsetospeed(&options, B115200);
      break;
    case 230400:
      cfsetispeed(&options, B230400);
      cfsetospeed(&options, B230400);
      break;
    case 460800:
      cfsetispeed(&options, B460800);
      cfsetospeed(&options, B460800);
      break;
    case 500000:
      cfsetispeed(&options, B500000);
      cfsetospeed(&options, B500000);
      break;
    case 576000:
      cfsetispeed(&options, B576000);
      cfsetospeed(&options, B576000);
      break;
    case 921600:
      cfsetispeed(&options, B921600);
      cfsetospeed(&options, B921600);
      break;
    case 1000000:
      cfsetispeed(&options, B1000000);
      cfsetospeed(&options, B1000000);
      break;
    case 1152000:
      cfsetispeed(&options, B1152000);
      cfsetospeed(&options, B1152000);
      break;
    case 1500000:
      cfsetispeed(&options, B1500000);
      cfsetospeed(&options, B1500000);
      break;
    case 2000000:
      cfsetispeed(&options, B2000000);
      cfsetospeed(&options, B2000000);
      break;
    case 2500000:
      cfsetispeed(&options, B2500000);
      cfsetospeed(&options, B2500000);
      break;
    case 3000000:
      cfsetispeed(&options, B3000000);
      cfsetospeed(&options, B3000000);
      break;
    case 3500000:
      cfsetispeed(&options, B3500000);
      cfsetospeed(&options, B3500000);
      break;
    case 4000000:
      cfsetispeed(&options, B4000000);
      cfsetospeed(&options, B4000000);
      break;

    default:
      fprintf(stderr,
              "Warning: Baudrate not "
              "supported!\n");
      serial_close(fd);
      return -1;
  }

  // Input Modes
  options.c_iflag &= ~IGNCR; // Ignore CR
#ifdef XONXOFF
  options.c_iflag |= IXON; // XON-XOFF Flow Control
#endif

  // Output Modes
  options.c_oflag |= OPOST; // Post-process output

  // Control Modes
  options.c_cflag |= CS8;    // 8 data bits
  options.c_cflag |= CREAD;  // Enable Receiver
  options.c_cflag |= CLOCAL; // Ignore modem status
                             // lines

  // Local Modes
  options.c_lflag |= IEXTEN; // Extended input
                             // character processing

  // Special characters
  options.c_cc[VMIN] = 0;  // Always return...
  options.c_cc[VTIME] = 0; // ..immediately from read()
#ifdef XONXOFF
  options.c_cc[VSTOP] = XOFF;
  options.c_cc[VSTART] = XON;
#endif

  tcsetattr(fd, TCSANOW, &options);

  tcflush(fd, TCIOFLUSH);

  return fd;
}

void
serial_close(int fd) {
  tcflush(fd, TCIOFLUSH);
  close(fd);
}

int
serial_has_char_timeout(int fd, int64 msecs) {
  struct pollfd fds;
  fds.fd = fd;
  fds.events = (POLLIN | POLLPRI); // Data may be read
  if(poll(&fds, 1, msecs) > 0) {
    return 1;
  } else {
    return 0;
  }
}

int
serial_has_char(int fd) {
  return serial_has_char_timeout(fd, 0);
}
void
serial_wait_until_sent(int fd) {
  while(tcdrain(fd) == -1) {
    fprintf(stderr, "Could not drain data: %s\n", strerror(errno));
  }
}

unsigned int
serial_write_raw(int fd, const char* d, unsigned int len) {
  unsigned int processed = 0;
  time_t start = time(NULL);

  while((processed < len) && (time(NULL) - start < TIMEOUT)) {
    int t = write(fd, (d + processed), (len - processed));
    if(t == -1) {
      fprintf(stderr, "Error while writing: %s\n", strerror(errno));
      return processed;
    } else {
      processed += t;
    }
  }

  return processed;
}

unsigned int
serial_read_raw(int fd, char* d, unsigned int len) {
  unsigned int processed = 0;
  time_t start = time(NULL);

  while((processed < len) && (time(NULL) - start < TIMEOUT)) {
    int t = read(fd, (d + processed), (len - processed));
    if(t == -1) {
      fprintf(stderr, "Error while reading: %s\n", strerror(errno));
      return processed;
    } else {
      processed += t;
    }
  }

  return processed;
}

void
serial_write_char(int fd, char c) {
  while(serial_write_raw(fd, &c, 1) != 1)
    ;
}

void
serial_read_char(int fd, char* c) {
  while(serial_read_raw(fd, c, 1) != 1)
    ;
#ifdef XONXOFF
  if(*c == XON) {
    if(tcflow(fd, TCOON) == -1) {
      fprintf(stderr,
              "Could not restart flow: "
              "%s\n",
              strerror(errno));
    }
    serial_read_char(fd, c);
  } else if(*c == XOFF) {
    if(tcflow(fd, TCOOFF) == -1) {
      fprintf(stderr, "Could not stop flow: %s\n", strerror(errno));
    }
    serial_read_char(fd, c);
  }
#endif
}

void
serial_write_string(int fd, const char* s) {
  while(*s) serial_write_char(fd, *(s++));
}

char**
get_serial_ports(void) {
  DIR* dir;
  struct dirent* ent;
  char** files;
  int size = 0;
  dir = opendir("/dev/");
  while((ent = readdir(dir)) != NULL) {
#ifdef SEARCH
    if(strstr(ent->d_name, SEARCH) != NULL)
#endif
      size++;
  }
  closedir(dir);

  files = (char**)malloc((size + 1) * sizeof(char*));

  int i = 0;
  dir = opendir("/dev/");
  while(((ent = readdir(dir)) != NULL) && (i < size)) {

#ifdef SEARCH
    if(strstr(ent->d_name, SEARCH) != NULL) {
#endif

      int tmp = strlen(ent->d_name) + 6;
      files[i] = (char*)malloc(tmp * sizeof(char));
      strcpy(files[i], "/dev/");
      strcpy(files[i] + 5, ent->d_name);
      files[i][tmp - 1] = '\0';

#ifdef TRY_TO_OPEN_PORTS
      int fdtmp = serial_open(files[i], 9600);
      if(fdtmp != -1) {
        serial_close(fdtmp);
#endif

        i++;

#ifdef TRY_TO_OPEN_PORTS
      } else {
        free(files[i]);
      }
#endif

#ifdef SEARCH
    }
#endif
  }
  closedir(dir);
  files[i] = NULL;
  return files;
}
