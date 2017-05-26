//
// Created by Seth Shelnutt on 5/26/17.
//

#ifndef OUTLIERS_UDF_HPP
#define OUTLIERS_UDF_HPP

#include <vector>
#include <quantile.hpp>
#include <iostream>
template<typename T>
std::vector<T> remove_outlier(std::vector<T> inData) {

    if(inData.size() < 2) {
      return inData;
    }
    std::vector<T> data;

    std::vector<T> quantiles = Quantile(inData, {0.25, 0.75});
    T IQR = quantiles[1] - quantiles[0];

    for(T dataPoint : inData) {
        // We want to exclude all data that is outside the range, and is an outlier.
        // We are excluding things that are < (Q1 - 1.5*IQR) AND > (Q3 + 1.5*IQR)
      std::cout << "dataPoint: " << dataPoint << " lower: "
                << quantiles[0] - (1.5 * IQR) << " upper: "
                << quantiles[1] + (1.5 * IQR) << std::endl;
        if((dataPoint > (quantiles[0] - (1.5 * IQR))) || (dataPoint < (quantiles[1] + (1.5 * IQR)))) {
            data.push_back(dataPoint);
        }
    }

    return data;
}

#endif //OUTLIERS_UDF_HPP
