/*
 * Copyright 2012 - 2014 Thomas Buck
 * <xythobuz@xythobuz.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "lib/windoze.h"
#include "lib/uint64.h"
#include "lib/io.h"
#include "lib/dir.h"

#include "serial.h"

#if !WINDOWS_NATIVE
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#ifndef XON
#define XON 0x11
#endif

#ifndef XOFF
#define XOFF 0x13
#endif

#ifndef TIMEOUT
#define TIMEOUT 2
#endif

#if WINDOWS_NATIVE

static char*
last_error(void) {
  unsigned long err = 0;
  unsigned long ret = 0;
  static char errbuf[MAX_PATH + 1] = {0};
  static char retbuf[MAX_PATH + 1] = {0};

  err = GetLastError();
  ret = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, errbuf, MAX_PATH, NULL);
  if(ret != 0) {
    /* CRLF fun */
    errbuf[ret - 2] = 0;
    snprintf(retbuf, MAX_PATH, "LastError: %s (%d)", errbuf, ret);
  } else
    snprintf(retbuf, MAX_PATH, "LastError: %d (FormatMessageA failed)", ret);

  return retbuf;
}

#define GET_PORT_TIMEOUTS(fd, t)                                                                                       \
  memset(t, 0, sizeof(COMMTIMEOUTS));                                                                                  \
  if(!GetCommTimeouts(fd, t)) {                                                                                        \
    fprintf(stderr, "GetCommTimeouts() %s\n", last_error());                                                           \
    return 1;                                                                                                          \
  }

#define SET_PORT_TIMEOUTS(fd, t)                                                                                       \
  if(!SetCommTimeouts(fd, t)) {                                                                                        \
    fprintf(stderr, "SetCommTimeouts() %s\n", last_error());                                                           \
    return 1;                                                                                                          \
  }

#define GET_PORT_STATE(fd, pdcb)                                                                                       \
  memset(pdcb, 0, sizeof(DCB));                                                                                        \
  if(!GetCommState(fd, pdcb)) {                                                                                        \
    fprintf(stderr, "GetCommState() %s\n", last_error());                                                              \
    return 1;                                                                                                          \
  }

#define SET_PORT_STATE(fd, pdcb)                                                                                       \
  if(!SetCommState(fd, pdcb)) {                                                                                        \
    fprintf(stderr, "SetCommState() %s\n", last_error());                                                              \
    return 1;                                                                                                          \
  }

static unsigned int
port_timeout(HANDLE fd, unsigned int rt, unsigned int wt) {
  COMMTIMEOUTS t;

  GET_PORT_TIMEOUTS(fd, &t);

  t.ReadIntervalTimeout = 0;
  t.ReadTotalTimeoutMultiplier = 0;
  t.ReadTotalTimeoutConstant = rt;
  t.WriteTotalTimeoutMultiplier = 0;
  t.WriteTotalTimeoutConstant = wt;

  SET_PORT_TIMEOUTS(fd, &t);

  return 0;
}

#endif

int
serial_open(const char* port, unsigned int baud) {
#if WINDOWS_NATIVE
  HANDLE fd;
  char buf[1024] = {0};
  strcpy(buf, "\\\\.\\");
  strncat(buf, port, sizeof(buf) - 5);
  fd = CreateFile(buf,                          // port name
                  GENERIC_READ | GENERIC_WRITE, // Read/Write
                  0,                            // No Sharing
                  NULL,                         // No Security
                  OPEN_EXISTING,                // Open existing port only
                  0,                            // Non Overlapped I/O
                  NULL);                        // Null for Comm Devices

  if(fd != INVALID_HANDLE_VALUE) {
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;
    if(!GetCommState(fd, &dcbSerialParams)) {
      CloseHandle(fd); /* Sets port to null. Necessary? */
      return -1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    /* Using real values instead of CBR_BAUD_RATE for simplicity.
     * Allowed according to MSDN:
     * http://msdn.microsoft.com/en-us/library/windows/desktop/aa363214(v=vs.85).aspx */
    /* printf("Baud rate: %lu\n", baud_rate); */

    dcbSerialParams.BaudRate = baud;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fOutxDsrFlow = FALSE;
    dcbSerialParams.fDsrSensitivity = FALSE;
    dcbSerialParams.fAbortOnError = TRUE;

    /* Note to self: Forgot the indirection operator here- W. Jones... */
    if(!SetCommState(port, &dcbSerialParams)) {
      return -2;
    }

    /* Add if-else here? */
    GetCommTimeouts(port, &timeouts);
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if(!SetCommTimeouts(port, &timeouts)) {
      return -3;
    }
  }

  return fd;
#else
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
#ifdef B3500000
    case 3500000:
      cfsetispeed(&options, B3500000);
      cfsetospeed(&options, B3500000);
      break;
#endif
#ifdef B4000000
    case 4000000:
      cfsetispeed(&options, B4000000);
      cfsetospeed(&options, B4000000);
      break;
#endif
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
  options.c_iflag |= IXON;   // XON-XOFF Flow Control
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
#endif
}

void
serial_close(int fd) {
#if !WINDOWS_NATIVE

  tcflush(fd, TCIOFLUSH);
  close(fd);
#else
  CloseHandle(fd);
#endif
}

int
serial_has_char_timeout(int fd, int64 msecs) {
#if !WINDOWS_NATIVE
  struct pollfd fds;
  fds.fd = fd;
  fds.events = (POLLIN | POLLPRI); // Data may be read
  if(poll(&fds, 1, msecs) > 0) {
    return 1;
  } else {
    return 0;
  }
#else
  COMSTAT ComStat;
  DWORD errors = 0;
  if(!ClearCommError(fd, &errors, &ComStat))
    return 0;
  return ComStat.cbInQue > 0;
#endif
}

int
serial_has_char(int fd) {
  return serial_has_char_timeout(fd, 0);
}

void
serial_wait_until_sent(int fd) {
#if !WINDOWS_NATIVE
  while(tcdrain(fd) == -1) {
    fprintf(stderr, "Could not drain data: %s\n", strerror(errno));
  }
#else
  if(!PurgeComm(fd, PURGE_TXCLEAR)) {
    fprintf(stderr, "Could not drain data: %s\n", last_error());
  }
#endif
}

unsigned int
serial_write_raw(int fd, const char* d, unsigned int len) {
  unsigned int processed = 0;
  time_t start = time(NULL);

  while((processed < len) && (time(NULL) - start < TIMEOUT)) {
    int t;
#if !WINDOWS_NATIVE

    t = write(fd, (d + processed), (len - processed));
    if(t == -1)
#else
    DWORD sentSize;
    t = WriteFile(fd, (d + processed), (len - processed), &sentSize, NULL);
    if(!t)
#endif

    {
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
    int t;

#if !WINDOWS_NATIVE
    t = read(fd, (d + processed), (len - processed));

    if(t == -1)
#else
    DWORD bytesRead;
    t = ReadFile(fd, (d + processed), (len - processed), &bytesRead, NULL);

    if(!t)
#endif
    {
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
  while(*s)
    serial_write_char(fd, *(s++));
}

char**
get_serial_ports(void) {
  dir_t dir;
  char* entry;
  char** files;
  int size = 0;

  dir_open(&dir, "/dev/");

  while((entry = dir_read(&dir)) != NULL) {
#ifdef SEARCH
    if(strstr(entry, SEARCH) != NULL)
#endif
      size++;
  }
  dir_close(&dir);

  files = (char**)malloc((size + 1) * sizeof(char*));

  int i = 0;
  dir_open(&dir, "/dev/");
  while((entry = dir_read(&dir)) != NULL && (i < size)) {

#ifdef SEARCH
    if(strstr(entry, SEARCH) != NULL) {
#endif

      int tmp = strlen(entry) + 6;
      files[i] = (char*)malloc(tmp * sizeof(char));
      strcpy(files[i], "/dev/");
      strcpy(files[i] + 5, entry);
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
  dir_close(&dir);
  files[i] = NULL;
  return files;
}
