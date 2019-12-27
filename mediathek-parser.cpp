#ifdef _MSC_VER
#include <boost/config/compiler/visualc.hpp>
#endif
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <iomanip>
#include <algorithm>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/algorithm/string.hpp>

struct dateparser {
  dateparser(std::string fmt) {
    /* set format */
    using namespace boost::local_time;
    local_time_input_facet* input_facet = new local_time_input_facet();
    input_facet->format(fmt.c_str());
    ss.imbue(std::locale(ss.getloc(), input_facet));
  }

  bool
  operator()(std::string const& text) {
    ss.clear();
    ss.str(text);

    ss >> pt;
    bool ok = !pt.is_not_a_date_time();

    if(ok) {
      auto tm = to_tm(pt);
      year = tm.tm_year;
      month = tm.tm_mon + 1; /* for 1-based (1:jan, .. 12:dec) */
      day = tm.tm_mday;
      hour = tm.tm_hour;
      min = tm.tm_min;
      sec = tm.tm_sec;
    }

    return ok;
  }

  boost::posix_time::ptime pt;
  unsigned year, month, day;
  unsigned hour, min, sec;

private:
  std::stringstream ss;
};

using boost::property_tree::ptree;
/*
std::string
get_second_std(const auto& field) {
  return field.second.get_value<std::string>();
}
*/

void
entry_to_v(boost::property_tree::ptree const& pt, std::list<std::string>& list) {
  std::cerr << "pt.size=" << pt.size() << std::endl;
  // std::transform(pt.begin(),pt.end(), list.begin(), get_second_std::string);

  ptree::const_iterator end = pt.end();
  for(ptree::const_iterator it = pt.begin(); it != end;) list.push_back(it->second.get_value<std::string>());
}

std::vector<std::string>
entry_to_v(boost::property_tree::ptree const& pt) {

  std::cerr << "pt.size=" << pt.size() << ",value=" << pt.get_value<std::string>() << std::endl;
  std::vector<std::string> ret;
  for(const auto& field : pt) {
    ret.push_back(field.second.get_value<std::string>());
  }
  return ret;
}

void
print(boost::property_tree::ptree::value_type& v /* boost::property_tree::ptree const& pt*/) {
  boost::property_tree::ptree const& pt = v.second;
  ptree::const_iterator end = pt.end();
  for(ptree::const_iterator it = pt.begin(); it != end;) {
    std::string value = it->second.get_value<std::string>();

    if(value.find(',') != std::string::npos)
      std::cout << std::quoted(value);
    else
      std::cout << value;

    /*        std::cout << (value.find(',')==std::string::npos ? value : std::quoted(value)); */

    /*  std::cout << '(';print(it->second); std::cout << ')'; */

    if(++it != end)
      std::cout << ",";
  }
  std::cout << std::endl;
}

int
main() {
  try {
    std::string input_file = "D:/Programs/MediathekView_11_2015.09.15/Einstellungen/.mediathek3/filme.json";
    std::ifstream input_stream(input_file);
    std::string line;
    std::size_t lineno = 0;

    dateparser d_parser("%d.%m.%Y");
    dateparser t_parser("%H:%M:%S");

    /* send your JSON above to the parser below, but populate ss first */

    while(std::getline(input_stream, line) && ++lineno < 4) {
      ptree pt;
      std::stringstream ss;

      boost::trim_if(line, boost::is_any_of(",\r"));
      ss << "{ " << line << " }";
      /*  ss .str(line); */
      /*  ss.str(line); */

      std::cerr << "Line: " << line << std::endl;

      try {
        read_json(ss, pt);

        /*ptree::const_iterator end = pt.end();
        for(ptree::const_iterator it = pt.begin(); it != end; ++it) {*/
        std::list<std::string> fields;
        entry_to_v(pt.front().second, fields);
        std::cout << boost::algorithm::join(fields, " ||| ") << std::endl;
        /*   }*/

        /*  BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt)
            print(v);*/
      } catch(boost::property_tree::json_parser_error const& parse_error) {
        std::cerr << parse_error.what() << std::endl;
      }
    }
    /*    std::for_each(pt.begin(), pt.end(), print); */

    /*  BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt.get_child("particles.electron")) {
        assert(v.first.empty()); /* array elements have no names */
    std::cout << v.second.data() << std::endl;
    /* etc */
  }
  * /
      /*
          boost::property_tree::basic_ptree<std::string,std::string>::const_iterator iter = pt.begin(),iterEnd =
      pt.end(); for(;iter != iterEnd;++iter)
      {
           iter->first; /* Your key, at this level it will be "electron", "proton", "proton" */
      iter->second; /* The object at each step {"pos": [0,0,0], "vel": [0,0,0]}, etc. */
}
* / return EXIT_SUCCESS;
}
catch(std::exception& e) {
  std::cerr << e.what() << std::endl;
}
return EXIT_FAILURE;
}
