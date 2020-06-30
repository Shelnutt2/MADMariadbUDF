/*

  Copyright Seth Shelnutt 2016-06-27
  Licensed Under the GPL v3 or later

  returns the mad of the values in a distribution
*/
#ifndef __mad_h
#define __mad_h

#include <algorithm>
#include <common.hpp>
#include <vector>

#ifdef __clang__
#include <cstdlib>
#endif

// Get the median of an unordered set of numbers of arbitrary
// type (this will modify the underlying dataset).
template <typename T> T Median(std::vector<T> *v) {
  auto size = std::distance(v->begin(), v->end());
  std::nth_element(v->begin(), v->begin() + size / 2, v->end());
  return *std::next(v->begin(), size / 2);
}

// Get the mad of an unordered set of numbers of arbitrary
// type (this will modify the underlying dataset).
template <typename T> T Mad(std::vector<T> *v) {
  T med = Median(v);
  std::vector<T> mads;
  for (auto it : *v) {
    mads.push_back(T(std::abs(med - it)));
  }
  auto size = std::distance(mads.begin(), mads.end());
  std::nth_element(mads.begin(), mads.begin() + size / 2, mads.end());
  return *std::next(mads.begin(), size / 2);
}

#endif // __mad_h
