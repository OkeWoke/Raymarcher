#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include "../Vec.h"

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	return result;
}
