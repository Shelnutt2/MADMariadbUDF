#include <outliers.hpp>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <unordered_map>

TEST_CASE( "Outliers Integer", "[outliers]" ) {
  std::vector<int> inN = {1,2,3,4,5,6,7,8,9};
  remove_outliers(&inN);
  std::vector<int> expected = {1,2,3,4,5,6,7,8,9};
  REQUIRE(inN == expected);
}

TEST_CASE( "Outliers Double", "[outliers]" ) {
  std::vector<double> inN = {53, 55, 51, 50, 60, 52};
  remove_outliers(&inN);
  std::vector<double> expected = {50.0, 51.0, 52.0, 53.0, 55.0, 60.0};
  REQUIRE(inN == expected);
}

TEST_CASE( "Outliers Double - 2 Outliers", "[outliers]" ) {
  std::vector<double> inN = {53, 55, 51, 50, 60, 52, 100, 0};
  remove_outliers(&inN);
  std::vector<double> expected = {50.0, 51.0, 52.0, 53.0, 55.0, 60.0};
  REQUIRE(inN == expected);
}

TEST_CASE( "Outliers Empty", "[outliers]" ) {
  std::vector<double> inN;
  remove_outliers(&inN);
  std::vector<double> expected;
  REQUIRE(inN == expected);
}

TEST_CASE( "Outliers Single", "[outliers]" ) {
  std::vector<int> inN = {1};
  remove_outliers(&inN);
  std::vector<int> expected = {1};
  REQUIRE(inN == expected);
}