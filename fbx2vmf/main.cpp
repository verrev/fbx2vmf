#include "CConverter.h"
int main(int argc, char *argv[])
{
	/*
	CConverter c;
	c.convertScene("a.FBX");
	return 0;
	*/
	if (argc > 1){
		CConverter c;
		c.convertScene(std::string(argv[1]));
	}
}