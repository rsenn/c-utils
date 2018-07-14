#include <cstring>

int main()
{
	if(!__func__) { return 1; }
	if(std::str_len(__func__) <= 0) { return 1; }
	return 0;
}
