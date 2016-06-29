#include <mad.hpp>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

TEST_CASE( "Medians are computed", "[med]" ) {
	std::vector<int> *med_test_1 = new std::vector<int>{1,1,2,2,4,6,9};
	REQUIRE( Median(med_test_1) == 2 );
	delete med_test_1;
}

TEST_CASE( "Median absolute deviations are computed", "[mad]" ) {
	std::vector<int> *mad_test_1 = new std::vector<int>{1,1,2,2,4,6,9};
	REQUIRE( Mad(mad_test_1) == 1 );
	delete mad_test_1;
}
