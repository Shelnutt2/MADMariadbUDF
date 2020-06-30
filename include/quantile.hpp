//
// Created by Seth Shelnutt on 5/26/17.
//

#ifndef QUANTILE_HPP
#define QUANTILE_HPP

#include <algorithm>
#include <cmath>
#include <common.hpp>
#include <cstddef>
#include <vector>

static inline double Lerp(double v0, double v1, double t) {
  return (1 - t) * v0 + t * v1;
}

template <typename T>
static std::vector<T> Quantile(std::vector<T> *inData,
                               const std::vector<double> &probs) {
  if (inData->empty()) {
    return std::vector<T>();
  }

  if (1 == inData->size()) {
    return std::vector<T>(1, inData->at(0));
  }

  std::sort(inData->begin(), inData->end());
  std::vector<T> quantiles;

  for (size_t i = 0; i < probs.size(); ++i) {
    T poi = Lerp(-0.5, inData->size() - 0.5, probs[i]);

    size_t left = std::max(int64_t(std::floor(poi)), int64_t(0));
    size_t right =
        std::min(int64_t(std::ceil(poi)), int64_t(inData->size() - 1));

    T datLeft = inData->at(left);
    T datRight = inData->at(right);

    T quantile = Lerp(datLeft, datRight, poi - left);

    quantiles.push_back(quantile);
  }

  return quantiles;
}
#endif // MAD_TESTS_QUANTILE_HPP
