#include <quantile.hpp>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <unordered_map>

//TEST_CASE( "Quantiles computed", "[quantiles]" ) {
//	std::vector<int> quantile_test_1 = {1,2,3,4,5,6};
//  std::vector<int> q1 = {};
//  std::vector<int> q3 = {};
//  std::unordered_map<float,std::vector<int>> quantiles;
//  quantiles.emplace(0.25, q1);
//  quantiles.emplace(0.75, q3);
//  REQUIRE( Quantile(quantile_test_1, {0.25, 0.75}) == quantiles );
//}

TEST_CASE( "Quantiles SimpleNumber", "[quantiles]" ) {
  std::vector<int> inN = {1,2,3,4,5,6,7,8,9};
  std::vector<double> probabilities = {0.5};
  std::vector<int> quantiles = Quantile(inN, probabilities);
  std::vector<int> expected = {5};
  REQUIRE(quantiles == expected);
}

TEST_CASE( "Quantiles SimpleNumber2", "[quantiles]" ) {
  std::vector<double> inN = {53, 55, 51, 50, 60, 52};
  std::vector<double> probabilities = {0.25, .75};
  std::vector<double> quantiles = Quantile(inN, probabilities);
  std::vector<double> expected = {51, 55};
  REQUIRE(quantiles == expected);
}
