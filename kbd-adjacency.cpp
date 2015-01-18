#include <iostream>
#include <functional> 
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <locale>
#include <iterator>

#ifdef HAVE_INITIALIZER_LIST
#include <initializer_list>
#endif

#ifdef CXX11
#include <unordered_set>
#include <type_traits>
using std::enable_if;
using std::unordered_set;
#else
# include <boost/foreach.hpp>
# ifdef _MSC_VER
#  include <hash_set>
#  ifdef HAVE_XTR1COMMON
#   include <xtr1common>
using std::enable_if;
#  else
#    include <boost/utility/enable_if.hpp>
using boost::enable_if;
#  endif
using stdext::hash_set;
# else
#  include <ext/hash_set>
#  include <ext/type_traits.h>
using __gnu_cxx::hash_set;
using std::enable_if;
# endif
# define unordered_set hash_set
#endif

//-----------------------------------------------------------------------------
namespace std {
const char* begin(const char* s) { return s; }
const char* end(const char* s) { return begin(s)+std::strlen(s); }
}

//-----------------------------------------------------------------------------
typedef  std::vector<char> char_v;
typedef std::map<char,char_v> adjacency_t;



template<class Range>
char_v
range_to_v(Range r) {
	char_v v;
	std::copy(std::begin(r), std::end(r), std::back_inserter(v));
	return v;
}

#ifdef HAVE_INITIALIZER_LIST
template<class T>
std::vector<T>
range_to_v(std::initializer_list<T> il) {
	std::vector<T> v;
	std::copy(std::begin(il), std::end(il), std::back_inserter(v));
	return v;
}
#else
template<class T,template<class> class C>
std::vector<T>
range_to_v(C<T> il) {
	std::vector<T> v;
	std::copy(std::begin(il), std::end(il), std::back_inserter(v));
	return v;
}
#endif

//-----------------------------------------------------------------------------
template<class Char, class Container>
typename enable_if< 
std::is_same< Char, typename Container::value_type >::value,
std::basic_ostream<Char>&
>::type
operator<<(std::basic_ostream<Char>& os, const Container& vc) {
	std::copy(vc.begin(), vc.end(),  std::ostream_iterator<Char>(std::cout, ""));
	return os;
}

//-----------------------------------------------------------------------------
template<class Map>
void
dump_map(const Map& m) {
#ifdef CXX11
	for(auto& kv : m) {
#else 
  BOOST_FOREACH(typename Map::const_iterator it, m) {
#define kv (*it)
#endif
		std::cout << "m['" << kv.first <<  "'] = \"" << kv.second << "\"" << std::endl;
	}
}

//-----------------------------------------------------------------------------
template<class Char>
unordered_set<Char>
collect_chars(const std::map<Char,std::vector<Char> >& m) {
  typedef typename std::map<Char,std::vector<Char> > map_type;
	unordered_set<Char> r;
#ifdef CXX11
	for(auto& kv : m) {
#else 
  BOOST_FOREACH(typename map_type::const_iterator it, m) {
#define kv (*it)
#endif
		r.insert(kv.first);
#ifdef CXX11
		for(auto& ch : kv.second) {
#else 
  BOOST_FOREACH(typename std::vector<Char>::const_iterator it, kv.second) {
#define ch (*it)
#endif
			r.insert(ch);
		}
	}
	/*for(auto& kv : m) {
		r.insert(kv.first);
	}*/
	return r;
}


//-----------------------------------------------------------------------------
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

using namespace std::placeholders;

template<typename CharT>
std::function<bool(CharT)>
make_predicate(const std::ctype<CharT>& ct, std::ctype_base::mask m) {
	return std::bind(
		(bool(std::ctype<CharT>::*)(std::ctype_base::mask,CharT) const) &std::ctype<CharT>::is, &ct, m, _1);
}

int main() {

	using std::cout;
	using std::ctype;
	using std::copy;
	using std::endl;
	using std::ostream_iterator;

	adjacency_t m = make_adjacency_matrix();

	std::locale lc("de_CH.iso88591"); 
	const ctype<char>& ct = std::use_facet< ctype<char> >(lc); 

	dump_map(m);

	auto cl = collect_chars(m);

	for(auto mask : {
		ctype<char>::lower,
		ctype<char>::upper,
		ctype<char>::digit,
		ctype<char>::punct,
		ctype<char>::graph,
		ctype<char>::print
	})
	{
		copy_if(cl.begin(), cl.end(), ostream_iterator<char>(cout), make_predicate(ct, mask));
		cout << endl;
	}

	return 0;
}
