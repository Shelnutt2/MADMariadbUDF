//
// Created by Seth Shelnutt on 5/26/17.
//

#ifndef QUANTILE_HPP
#define QUANTILE_HPP

#include <vector>
#include <cmath>

static inline double Lerp(double v0, double v1, double t)
{
  return (1 - t)*v0 + t*v1;
}

template<typename T>
static inline std::vector<T> Quantile(const std::vector<T>& inData, const std::vector<double>& probs) {
  if (inData.empty()) {
    return std::vector<T>();
  }

  if (1 == inData.size()) {
    return std::vector<T>(1, inData[0]);
  }

  std::vector <T> data = inData;
  std::sort(data.begin(), data.end());
  std::vector <T> quantiles;

  for (size_t i = 0; i < probs.size(); ++i) {
    T poi = Lerp(-0.5, data.size() - 0.5, probs[i]);

    size_t left = std::max(int64_t(std::floor(poi)), int64_t(0));
    size_t right = std::min(int64_t(std::ceil(poi)), int64_t(data.size() - 1));

    T datLeft = data.at(left);
    T datRight = data.at(right);

    T quantile = Lerp(datLeft, datRight, poi - left);

    quantiles.push_back(quantile);
  }

  return quantiles;
}
#endif //MAD_TESTS_QUANTILE_HPP
