#include "directory_iterator.hpp"
#include "dir_internal.h"

#include <cstring>

//namespace util {

directory_iterator::directory_iterator(const string& path, const string& filter) {
	string p = path;
	if(dir_open(&m_handle, p.c_str())) {
		m_retcode = NULL;
	} else {
		next();
	}
}

directory_iterator::~directory_iterator() {
	close();
}

directory_iterator&
directory_iterator::operator ++() {
	if(m_retcode > 0) next();
	return *this;
}

bool
directory_iterator::operator==(const directory_iterator& other) const {
	return (other.m_retcode == NULL && m_retcode == NULL);
}

void
directory_iterator::close() {
	if(m_handle.dir_int != NULL) {
    dir_close(&m_handle);
    m_handle.dir_int = NULL;
	}
}

int
directory_iterator::next() {
	//int len = requested_buffer_length();
	//allocate(len);
	m_retcode = dir_read(&m_handle);
	//m_buffer.resize(len);
	return (m_retcode == NULL ? -1 : 0);
}

int
directory_iterator::requested_buffer_length() {
  if(m_retcode) return str_len(m_retcode)+1;
  return 0;
}

bool
directory_iterator::is_allocated() const {
	return m_buffer.capacity() > 1;
}

void
directory_iterator::allocate(const size_t size) {
	m_buffer.reserve(size + 1);
}

//} // end of namespace util
