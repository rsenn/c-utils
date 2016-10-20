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

using boost::property_tree::ptree;
/*
std::string
get_second_std(const auto& field) {
  return field.second.get_value<std::string>();
}
*/

void
entry_to_v(boost::property_tree::ptree const& pt, std::list<std::string>& list)
{
  //std::transform(pt.begin(),pt.end(), list.begin(), get_second_std::string);

  ptree::const_iterator end = pt.end();
  for (ptree::const_iterator it = pt.begin(); it != end;)
    list.push_back(it->second.get_value<std::string>());
}

std::vector<std::string>
entry_to_v(boost::property_tree::ptree const& pt)
{
  std::vector<std::string> ret;
  for (const auto& field : pt) {
    ret.push_back(field.second.get_value<std::string>());
  }
  return ret;
}

void
print(boost::property_tree::ptree::value_type& v/* boost::property_tree::ptree const& pt*/)
{
  boost::property_tree::ptree const& pt = v.second;
  ptree::const_iterator end = pt.end();
  for (ptree::const_iterator it = pt.begin(); it != end;) {
    std::string value = it->second.get_value<std::string>();

    if (value.find(',') != std::string::npos)
      std::cout << std::quoted(value);
    else
      std::cout << value;

    //        std::cout << (value.find(',')==std::string::npos ? value : std::quoted(value));

    //  std::cout << '(';print(it->second); std::cout << ')';

    if (++it != end)
      std::cout << ",";
  }
  std::cout << std::endl;
}

int main()
{
  try {
    std::string input_file = "D:/Programs/MediathekView_11_2015.09.15/Einstellungen/.mediathek3/filme.json";
    // send your JSON above to the parser below, but populate ss first


    ptree pt;
    read_json(input_file, pt);


    /*ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
      std::list<std::string> fields;
      entry_to_v(it->second, fields);
      std::cout << boost::algorithm::join(fields, ",") << std::endl;
    }*/

    /*  BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt)
        print(v);*/
    std::for_each(pt.begin(), pt.end(), print);


    /*  BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt.get_child("particles.electron")) {
        assert(v.first.empty()); // array elements have no names
        std::cout << v.second.data() << std::endl;
        // etc
      }*/
    /*
        boost::property_tree::basic_ptree<std::string,std::string>::const_iterator iter = pt.begin(),iterEnd = pt.end();
    for(;iter != iterEnd;++iter)
    {
         iter->first; // Your key, at this level it will be "electron", "proton", "proton"
         iter->second; // The object at each step {"pos": [0,0,0], "vel": [0,0,0]}, etc.
    }*/
    return EXIT_SUCCESS;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_FAILURE;
}
