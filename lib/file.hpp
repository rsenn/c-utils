#ifndef FILE_HPP_
#define FILE_HPP_

#include <string>

using std::string;

class file {
public:
  typedef int handle_type;

  enum {
  	read_only = 0,
  	read_write = 1,
  	create = 2,
  	append = 4,
  	trunc = 8,
  };

  file(const string& name, int access);
  ~file();

  void
  close();

  int
  write(const char* b, const unsigned long n);

  int
  read(char* b, const unsigned long n);

  void
  put_line(const char* s);

  void
  put_line(const string& s)	{
  	put_line(s.c_str());
  }

  handle_type
  handle() const { return m_handle; }

  int
  retcode() const { return m_retcode; }

protected:
  handle_type m_handle;
  int m_access;
  int m_retcode;
};

//} // end of namespace util

#endif
