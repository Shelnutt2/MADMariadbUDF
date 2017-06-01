#include <quantile.hpp>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <unordered_map>

TEST_CASE( "Quantiles Integer", "[quantiles]" ) {
  std::vector<int> inN = {1,2,3,4,5,6,7,8,9};
  std::vector<double> probabilities = {0.5};
  std::vector<int> quantiles = Quantile(&inN, probabilities);
  std::vector<int> expected = {5};
  REQUIRE(quantiles == expected);
}

TEST_CASE( "Quantiles Doubles", "[quantiles]" ) {
  std::vector<double> inN = {53, 55, 51, 50, 60, 52};
  std::vector<double> probabilities = {0.25, .75};
  std::vector<double> quantiles = Quantile(&inN, probabilities);
  std::vector<double> expected = {51, 55};
  REQUIRE(quantiles == expected);
}

TEST_CASE( "Quantiles Empty", "[quantiles]" ) {
  std::vector<double> inN;
  std::vector<double> probabilities = {0.25, .75};
  std::vector<double> quantiles = Quantile(&inN, probabilities);
  std::vector<double> expected;
  REQUIRE(quantiles == expected);
}