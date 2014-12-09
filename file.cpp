#include "file.hpp"
#include "open.h"

#include <cstring>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

//namespace util {

file::file(const string& name, int access)
		: m_handle((access & append ? open_append : (access & trunc ? open_trunc : (access & read_write ? open_rw : open_read)))(name.c_str()))
    ,	m_access(access)
    , m_retcode(0) {
	if(m_handle < 0) {
		m_retcode = m_handle;
	}
}

file::~file() {
	if(m_handle >= 0) close();
}

void
file::close() {
  ::close(m_handle);
	m_handle = -1;
}

int
file::write(const char* b, const unsigned long n) {
	int ret;
	if((ret = ::write(m_handle, b, n)) < 0)
		m_retcode = ret;
	return ret;
}

int
file::read(char* b, const unsigned long n) {
	int ret;
	if((ret = ::read(m_handle, b, n)) < 0)
		m_retcode = ret;
	return ret;
}

void
file::put_line(const char* s) {
	write(s, strlen(s));
	write("\r\n", 2);
}

//} // end of namespace util
