#include <outliers.hpp>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <unordered_map>

TEST_CASE( "Outliers Integer", "[outliers]" ) {
  std::vector<int> inN = {1,2,3,4,5,6,7,8,9};
  std::vector<int> results = remove_outlier(inN);
  std::vector<int> expected = {1,2,3,4,5,6,7,8,9};
  REQUIRE(results == expected);
}

TEST_CASE( "Outliers Double", "[outliers]" ) {
  std::vector<double> inN = {53, 55, 51, 50, 60, 52};
  std::vector<double> results = remove_outlier(inN);
  std::vector<double> expected = {53, 55, 51, 50, 60, 52};
  REQUIRE(results == expected);
}

TEST_CASE( "Outliers Empty", "[outliers]" ) {
  std::vector<double> inN;
  std::vector<double> results = remove_outlier(inN);
  std::vector<double> expected;
  REQUIRE(results == expected);
}

TEST_CASE( "Outliers Single", "[outliers]" ) {
  std::vector<int> inN = {1};
  std::vector<int> results = remove_outlier(inN);
  std::vector<int> expected = {1};
  REQUIRE(results == expected);
}