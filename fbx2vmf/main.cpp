#include "CConverter.h"
//#define DBG 
int main(int argc, char *argv[])
{
#ifdef DBG
	CConverter c;
	c.convertScene("man.FBX");
	return 0;
#else
	if (argc > 1){
		CConverter c;
		c.convertScene(std::string(argv[1]));
	}
#endif
}