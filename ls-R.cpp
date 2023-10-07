#include <iostream>
#include <experimental/filesystem>

int
main(int argc, char* argv[]) {
  using std::experimental::filesystem::recursive_directory_iterator;

  std::string myPath(".");

  if(argc > 1)
    myPath = argv[1];

  for(auto& dirEntry : recursive_directory_iterator(myPath))
    std::cout << dirEntry << std::endl;
}
