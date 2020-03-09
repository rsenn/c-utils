ssize_t
http_ssl_error(ssize_t ret, http* h, char** mptr) {

  if(ret < 0) {
    /* get error code */
    char buf[256];
    size_t n = sizeof(buf);
    int err;
    /* get error code */
    err = SSL_get_error(h->ssl, ret);

    ERR_error_string_n(err, buf, n);
    ERR_clear_error();

    if(mptr)
      *mptr = str_dup(buf);

    /* call ssl_read() again when socket gets readable */
    if(err == SSL_ERROR_WANT_READ) {
      io_wantread(h->sock);
      errno = EAGAIN;

      ret = -1;
      /* call ssl_read() again when socket gets writeable */
    } else if(err == SSL_ERROR_WANT_WRITE) {
      io_wantwrite(h->sock);
      errno = EWOULDBLOCK;

      ret = -1; /*
                 * EWOULDBLOCK, EINTR, EAGAIN are ignored because
                 * these say the handshake is in progress and needs
                 * more events.
                 */
    } else if(err == SSL_ERROR_SYSCALL) {
      /* ignore these */
      if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN) {
        // errno = EAGAIN;
      }
    } else if(err == SSL_ERROR_ZERO_RETURN) {
      ret = 0;
    } else if(err == SSL_ERROR_SSL) {
      io_wantwrite(h->sock);
      ret = 1;
    } else {
      ret = -1;
    }

    if(ret <= 0) {
      buffer_puts(buffer_2, "SSL ret: ");
      buffer_putlong(buffer_2, ret);
      buffer_puts(buffer_2, " err: ");
      buffer_putlong(buffer_2, err);
      buffer_puts(buffer_2, " : ");
      buffer_puts(buffer_2, buf);
      buffer_putnlflush(buffer_2);
    }
  }
  return ret;
}
ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, http* h) {
  ssize_t ret;
  char* msg = 0;

  if((ret = SSL_read(h->ssl, buf, len)) <= 0) {
    buffer_puts(buffer_2, "SSL read ");
    int err = http_ssl_error(ret, h);
    /*  if(msg) {
        buffer_puts(buffer_2, "read error: ");
        buffer_puts(buffer_2, msg);
        buffer_putnlflush(buffer_2);
      }*/
  }

  return ret;
}
ssize_t
http_ssl_write(fd_t fd, const void* buf, size_t n, http* h) {
  ssize_t ret;
  errno = 0;
  ret = SSL_write(h->ssl, buf, n);
  if(ret <= 0) {
    buffer_puts(buffer_2, "SSL write ");
    int err = http_ssl_error(ret, h);
  }
  return ret;
}

ssize_t http_ssl_connect(fd_t fd, http* h);
if(!h->connected) {
  if((ret = http_ssl_connect(h)) == 1) {

    buffer_putsflush(buffer_2, "SSL handshake done\n");
    h->connected = 1;
    io_wantwrite(h->sock);
    return ret;
  } else if(ret < 0) {
    return ret;
  }
}
}
#endif

if(freshen)
  buffer_freshen(&h->q.in);

if((r = h->response) == NULL) {
  return ret;
}

while(r->status == HTTP_RECV_HEADER) {
  if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
    break;

  stralloc_trimr(&r->data, "\r\n", 2);
  stralloc_nul(&r->data);

  // putline("Header", r->data.s, -r->data.len, &h->q.in);

  if(r->data.len == 0) {
    r->ptr = 0;
    r->status = HTTP_RECV_DATA;

    if(h->q.in.p < h->q.in.n) {
      ret = 1;
    } else {
      ret = 0;
      errno = EWOULDBLOCK;
    }
    break;
  }

  if(stralloc_startb(&r->data, "Content-Type: multipart", 23)) {
    size_t p = str_find(r->data.s, "boundary=");

    if(r->data.s[p]) {
      stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);
    }
    r->transfer = HTTP_TRANSFER_BOUNDARY;
  } else if(stralloc_startb(&r->data, "Content-Length:", 15)) {
    scan_ulonglong(&r->data.s[16], &r->content_length);
    r->transfer = HTTP_TRANSFER_LENGTH;
  } else {
    r->transfer = HTTP_TRANSFER_CHUNKED;
  }

  stralloc_zero(&r->data);
}

if(r->status == HTTP_RECV_DATA) {
  ret = 1;

  /*
      if(r->content_length) {
        size_t a;

        if(r->content_length < (a = r->data.a - r->data.len))
          a = r->content_length;
        if(a > 1024)
          a = 1024;

        stralloc_readyplus(&r->data, 1024);

        if((ret = buffer_get(&h->q.in, &r->data.s[r->data.len], r->data.a - r->data.len)) <= 0)
          break;

        putline("data", &r->data.s[r->data.len], 1, &h->q.in);

        r->data.len += ret;
        r->content_length -= ret;
        continue;
      }

      r->status = HTTP_STATUS_FINISH;*/
}

if(ret == -1) {
  err = errno;
  errno = 0;
} else {
  err = 0;
}

if(ret == 0) {
  r->status = HTTP_STATUS_FINISH;
}

if(err && err != EWOULDBLOCK)
  r->status = HTTP_STATUS_ERROR;

return ret;
}
