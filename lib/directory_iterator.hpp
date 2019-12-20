#ifndef DIRECTORY_ITERATOR_HPP_
#define DIRECTORY_ITERATOR_HPP_

#include <string>
#include "dir.h"

using std::string;

// namespace util {

class directory_iterator {
public:
  typedef struct dir_s handle_type;
  typedef char* retcode_type;

  static const size_t buffer_size = 4096;

  directory_iterator() : m_retcode(0) {}
  directory_iterator(const string& path, const string& filter = "*");
  ~directory_iterator();

  directory_iterator& operator++();

  directory_iterator
  operator++(int) {
    directory_iterator temp = *this;
    ++*this;
    return temp;
  }

  bool operator==(const directory_iterator& other) const;

  bool
  operator!=(const directory_iterator& other) const {
    return !(*this == other);
  }

  const string& operator*() const { return m_buffer; }

  handle_type
  handle() const {
    return m_handle;
  }

  retcode_type
  retcode() const {
    return m_retcode;
  }

protected:
  void close();

  int next();

private:
  int requested_buffer_length();

  bool is_allocated() const;

  void allocate(const size_t size = buffer_size);

  handle_type m_handle;
  string m_buffer;
  char* m_retcode;
};

//} /* end of namespace util */

#endif /* DIRECTORY_ITERATOR_HPP_ */
