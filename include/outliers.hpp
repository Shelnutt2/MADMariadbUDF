//
// Created by Seth Shelnutt on 5/26/17.
//

#ifndef OUTLIERS_UDF_HPP
#define OUTLIERS_UDF_HPP

#include <vector>
#include <quantile.hpp>
#include <iostream>
#include <numeric>

/**
 *
 *
 * @tparam T
 * @param inData std::vector<T>
 * @return void return
 */
template<typename T>
void remove_outliers(std::vector<T> *inData) {

    if(inData == NULL) {
      return;
    }
    if(inData->size() < 2) {
      return;
    }

    std::vector<T> quantiles = Quantile(inData, {0.25, 0.75});
    T IQR = quantiles[1] - quantiles[0];

  // curFiles is: vector < string > curFiles;

  typename std::vector<T>::iterator it = inData->begin();

  while(it != inData->end()) {
        // We want to exclude all data that is outside the range, and is an outlier.
        // We are excluding things that are < (Q1 - 1.5*IQR) AND > (Q3 + 1.5*IQR)
        if((*it < (quantiles[0] - (1.5 * IQR))) || (*it > (quantiles[1] + (1.5 * IQR)))) {
          it = inData->erase(it);
        } else {
          it++;
        }
    }
}

template<typename T>
double avg(std::vector<T> *v) {
  return 1.0 * std::accumulate(v->begin(), v->end(), (T)0) / v->size();
}


template<typename T>
double variance_population(std::vector<T> *v) {
  double mean = avg(v);
  double temp = 0;

  for (int i = 0; i < v->size(); i++) {
    temp += (v->at(i) - mean) * (v->at(i) - mean);
  }

  return temp / v->size();
}

template<typename T>
double stddev_population(std::vector<T> *v) {
  return std::sqrt(variance_population(v));
}
#endif //OUTLIERS_UDF_HPP
