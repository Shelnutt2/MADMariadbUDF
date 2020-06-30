//
// Created by Seth Shelnutt on 5/26/17.
//

#ifndef OUTLIERS_UDF_HPP
#define OUTLIERS_UDF_HPP

#include <common.hpp>
#include <iostream>
#include <numeric>
#include <quantile.hpp>
#include <vector>

/**
 *
 *
 * @tparam T
 * @param inData std::vector<T>
 * @return void return
 */
template <typename T> void remove_outliers(std::vector<T> *inData) {

  if (inData == NULL) {
    return;
  }
  if (inData->size() < 2) {
    return;
  }

  std::vector<T> quantiles = Quantile(inData, {0.25, 0.75});
  T IQR = quantiles[1] - quantiles[0];

  // curFiles is: vector < string > curFiles;

  typename std::vector<T>::iterator it = inData->begin();

  T lowerThreshold = quantiles[0] - (1.5 * IQR);
  T upperThreshold = quantiles[1] + (1.5 * IQR);
  /**
   * We want to exclude all data that is outside the range, and is an outlier.
   * We are excluding things that are < (Q1 - 1.5*IQR) AND > (Q3 + 1.5*IQR)
   * First loop through data starting at bottom of sorted vector
   * Once we have a value larger then the lower threshold stop and we'll start
   *again in reverse We can avoid iterating over the large number of non-outlier
   *values
   **/
  while (it < inData->end()) {
    if (*it < lowerThreshold)
      it = inData->erase(it);
    else
      break;
  }
  // Loop through values in descending order
  it = inData->end();
  while (it > inData->begin()) {
    it--;
    if (*it >= upperThreshold)
      it = inData->erase(it);
    else
      break;
  }
}

template <typename T> double avg(std::vector<T> *v) {
  return 1.0 * std::accumulate(v->begin(), v->end(), (T)0) / v->size();
}

template <typename T> double variance_population(std::vector<T> *v) {
  double mean = avg(v);
  double temp = 0;

  for (int i = 0; i < v->size(); i++) {
    temp += (v->at(i) - mean) * (v->at(i) - mean);
  }

  return temp / v->size();
}

template <typename T> double stddev_population(std::vector<T> *v) {
  return std::sqrt(variance_population(v));
}
#endif // OUTLIERS_UDF_HPP
