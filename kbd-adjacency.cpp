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

static adjacency_t
make_adjacency_matrix() {
  adjacency_t m;

  m['1'] = m['+'] = range_to_v("§°2\"qQ");
  m['2'] = m['"'] = range_to_v("1+3*wWqQ");
  m['3'] = m['*'] = range_to_v("2\"4çeE¿wW");
  m['4'] = m['ç'] = range_to_v("3*5%rReE¿");
  m['5'] = m['%'] = range_to_v("4ç6&tTrR");
  m['6'] = m['&'] = range_to_v("5%7/zZtT");
  m['7'] = m['/'] = range_to_v("6&8(uUzZ");
  m['8'] = m['('] = range_to_v("7/9)iIuU");
  m['9'] = m[')'] = range_to_v("8(0=oOiI");
  m['0'] = m['='] = range_to_v("9)'?pPoO");
  m['\''] = m['?'] = range_to_v("0=üèpP");
  m['^'] = m['`'] = range_to_v("'?üè");
  //m['"'] = m['!'] = range_to_v("'üè$£äà");

	m['q'] = m['Q'] = range_to_v( {'1', '2', 'w', 'W', 'A', 'a'} );
	m['w'] = m['W'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );
	m['e'] = m['E'] = range_to_v( {'q', 'Q', '2', '3', 'e', 'E', 's', 'S', 'a', 'A'} );
	m['r'] = m['R'] = range_to_v("eE45tTfFdD");
	m['t'] = m['T'] = range_to_v("rR56zZgGfF");
	m['z'] = m['Z'] = range_to_v("tT67uUhHgG");
	m['u'] = m['U'] = range_to_v("zZ78iIjJhH");
	m['i'] = m['I'] = range_to_v("uU89oOkKjJ");
	m['o'] = m['O'] = range_to_v("iI90pPlLkK");
	m['p'] = m['P'] = range_to_v("oO0'üèöélL");
	m['ü'] = m['è'] = range_to_v("pP'äàöé");

//	m['a'] = m['A'] = range_to_v("qQwWsSyY<>");
	m['a'] = m['A'] = range_to_v("qQwWsSyY");
	m['s'] = m['S'] = range_to_v("aAwWeEdDxXyY");
	m['d'] = m['D'] = range_to_v("sSeErRfFcCxX");
	m['f'] = m['F'] = range_to_v("dDrRtTgGvVcC");
	m['g'] = m['g'] = range_to_v("fFtTzZhHbBvV");
	m['h'] = m['H'] = range_to_v("gGzZuUjJnNbB");
	m['j'] = m['J'] = range_to_v("hHuUiIkKmMnN");
	m['k'] = m['k'] = range_to_v("jJiIoOlL,;mM");
	m['l'] = m['L'] = range_to_v("kKoOpPöé.:,;");
	m['ö'] = m['é'] = range_to_v("lLpPüèäà-_.:");
	m['ä'] = m['à'] = range_to_v("öéüè$£-_");
	m['$'] = m['£'] = range_to_v("äà");
	m['<'] = m['>'] = range_to_v("aAyY");
	m['y'] = m['Y'] = range_to_v("<>aAsSxX");
  m['x'] = m['X'] = range_to_v("yYsSdDcC ");
  m['c'] = m['C'] = range_to_v("xXdDfFvV ");
  m['v'] = m['V'] = range_to_v("cCfFgGbB ");
  m['b'] = m['B'] = range_to_v("vVgGhHnN ");
  m['n'] = m['N'] = range_to_v("bBhHjJmM ");
  m['m'] = m['M'] = range_to_v("nNjJkK,; ");
  m[','] = m[';'] = range_to_v("mMkKlL.: ");
  m['.'] = m[':'] = range_to_v(",;lLöé-_");
  m['-'] = m['_'] = range_to_v(".:öéäà");


	return m;
};

int main() {
  adjacency_t m = make_adjacency_matrix();

	return 0;
}
