#include <map>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <cstring>

namespace std {
const char* begin(const char* s) { return s; }
const char* end(const char* s) { return begin(s)+std::strlen(s); }
}

typedef  std::vector<char> char_v;
typedef std::map<char,char_v> adjacency_t;

template<class Range>
char_v
range_to_v(Range r) {
	char_v v;
	std::copy(std::begin(r), std::end(r), std::back_inserter(v));
	return v;
}

template<class T>
std::vector<T>
range_to_v(std::initializer_list<T> il) {
	std::vector<T> v;
	std::copy(std::begin(il), std::end(il), std::back_inserter(v));
	return v;
}

static  adjacency_t adjacency_matrix() {
  adjacency_t m;

	m['q'] = m['Q'] = range_to_v( {'1', '2', 'w', 'W', 'A', 'a'} );
	m['w'] = m['W'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );
	m['e'] = m['E'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );

	return m;
};

int main() {

	return 0;
}
