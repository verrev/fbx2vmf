#include "CConverter.h"
int main(int argc, char *argv[])
{
	if (argc > 1){
		CConverter c;
		c.convertScene(std::string(argv[1]));
	}
}