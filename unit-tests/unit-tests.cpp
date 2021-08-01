// unit-tests.cpp : Source file for your target.
//


#define CATCH_CONFIG_RUNNER

#include <Catch2\catch_all.hpp>
#include "..\Vec.h"






int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	return result;
}
