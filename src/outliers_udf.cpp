/*

  Copyright Seth Shelnutt 2017-05-26
  Licensed Under the GPL v3 or later

  returns the mean and stddev with outliers removed

  input parameters:
  data (real)

  output:
  stddev value of the distribution (real)

  registering the function:
  CREATE AGGREGATE FUNCTION stddev_no_outliers RETURNS REAL SONAME
  'liboutliers_udf.so'; CREATE AGGREGATE FUNCTION mean_no_outliers RETURNS REAL
  SONAME 'liboutliers_udf.so';

  getting rid of the function:
  DROP FUNCTION stddev_no_outliers;
  DROP FUNCTION mean_no_outliers;
*/

#include <iostream>
#include <mysql.h>
#include <outliers.hpp>
#include <stdio.h>
#include <string.h>
#include <vector>

/*
 *
 * Mean with outlier removal
 *
 */

extern "C" {
my_bool mean_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void mean_no_outliers_deinit(UDF_INIT *initid);
void mean_no_outliers_clear(UDF_INIT *initid, char *is_null, char *error);
void mean_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                            char *error);
void mean_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                          char *error);
double mean_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                        char *error);
}

struct mean_no_outliers_data {
  std::vector<double> *double_values;
  std::vector<long long> *int_values;
};

my_bool mean_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
  if (args->arg_count != 1) {
    strcpy(
        message,
        "wrong number of arguments: mean_no_outliers() requires one argument");
    return 1;
  }

  if (args->arg_type[0] != REAL_RESULT && args->arg_type[0] != INT_RESULT &&
      args->arg_type[0] != DECIMAL_RESULT) {
    if (args->arg_type[0] == STRING_RESULT)
      strcpy(message, "mean_no_outliers() requires a real, decimal, double or "
                      "integer as parameter 1, received STRING");
    else
      strcpy(message, "mean_no_outliers() requires a real, decimal, double or "
                      "integer as parameter 1, received Decimal");
    return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  mean_no_outliers_data *buffer = new mean_no_outliers_data;
  buffer->double_values = NULL;
  buffer->int_values = NULL;
  initid->ptr = (char *)buffer;

  return 0;
}

void mean_no_outliers_deinit(UDF_INIT *initid) {
  mean_no_outliers_data *buffer = (mean_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }
  delete initid->ptr;
}

void mean_no_outliers_clear(UDF_INIT *initid, char *is_null, char *is_error) {
  mean_no_outliers_data *buffer = (mean_no_outliers_data *)initid->ptr;
  *is_null = 0;
  *is_error = 0;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }

  buffer->double_values = new std::vector<double>;
  buffer->int_values = new std::vector<long long>;
}

void mean_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                            char *is_error) {
  mean_no_outliers_clear(initid, is_null, is_error);
  mean_no_outliers_add(initid, args, is_null, is_error);
}

void mean_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                          char *is_error) {
  if (args->args[0] != NULL) {
    mean_no_outliers_data *buffer = (mean_no_outliers_data *)initid->ptr;
    if (args->arg_type[0] == REAL_RESULT ||
        args->arg_type[0] == DECIMAL_RESULT) {
      buffer->double_values->push_back(*((double *)args->args[0]));
    } else if (args->arg_type[0] == INT_RESULT) {
      buffer->int_values->push_back(*((long long *)args->args[0]));
    } else {
      // std::cerr << "mean_no_outliers() Error in adding type was not real( "
      // << REAL_RESULT << "), decimal("
      //         << DECIMAL_RESULT << ") or int(" << INT_RESULT << ")!! instead
      //         got: " << args->arg_type[0] << std::endl;
    }
  } else {
    // std::cerr << "mean_no_outliers() Argument in add received is null!" <<
    // std::endl;
  }
}

double mean_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                        char *is_error) {
  mean_no_outliers_data *buffer = (mean_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL && !buffer->double_values->empty()) {
    remove_outliers(buffer->double_values);
    return avg(buffer->double_values);
  } else if (buffer->int_values != NULL && !buffer->int_values->empty()) {
    remove_outliers(buffer->int_values);
    return avg(buffer->int_values);
  } else {
    std::cerr << "mean_no_outliers() internal error, all vectors were null in "
                 "computation"
              << std::endl;
    *is_null = 1;
    //    *is_error = 1;
  }
  return 0;
}

/*
 *
 * Stddev with outlier removal
 *
 */

extern "C" {
my_bool stddev_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args,
                                char *message);
void stddev_no_outliers_deinit(UDF_INIT *initid);
void stddev_no_outliers_clear(UDF_INIT *initid, char *is_null, char *error);
void stddev_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                              char *error);
void stddev_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                            char *error);
double stddev_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                          char *error);
}

struct stddev_no_outliers_data {
  std::vector<double> *double_values;
  std::vector<long long> *int_values;
};

my_bool stddev_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args,
                                char *message) {
  if (args->arg_count != 1) {
    strcpy(message, "wrong number of arguments: stddev_no_outliers() requires "
                    "one argument");
    return 1;
  }

  if (args->arg_type[0] != REAL_RESULT && args->arg_type[0] != INT_RESULT &&
      args->arg_type[0] != DECIMAL_RESULT) {
    if (args->arg_type[0] == STRING_RESULT)
      strcpy(message, "stddev_no_outliers() requires a real, decimal, double "
                      "or integer as parameter 1, received STRING");
    else
      strcpy(message, "stddev_no_outliers() requires a real, decimal, double "
                      "or integer as parameter 1, received Decimal");
    return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  stddev_no_outliers_data *buffer = new stddev_no_outliers_data;
  buffer->double_values = NULL;
  buffer->int_values = NULL;
  initid->ptr = (char *)buffer;

  return 0;
}

void stddev_no_outliers_deinit(UDF_INIT *initid) {
  stddev_no_outliers_data *buffer = (stddev_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }
  delete initid->ptr;
}

void stddev_no_outliers_clear(UDF_INIT *initid, char *is_null, char *is_error) {
  stddev_no_outliers_data *buffer = (stddev_no_outliers_data *)initid->ptr;
  *is_null = 0;
  *is_error = 0;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }

  buffer->double_values = new std::vector<double>;
  buffer->int_values = new std::vector<long long>;
}

void stddev_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                              char *is_error) {
  stddev_no_outliers_clear(initid, is_null, is_error);
  stddev_no_outliers_add(initid, args, is_null, is_error);
}

void stddev_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                            char *is_error) {
  if (args->args[0] != NULL) {
    stddev_no_outliers_data *buffer = (stddev_no_outliers_data *)initid->ptr;
    if (args->arg_type[0] == REAL_RESULT ||
        args->arg_type[0] == DECIMAL_RESULT) {
      buffer->double_values->push_back(*((double *)args->args[0]));
    } else if (args->arg_type[0] == INT_RESULT) {
      buffer->int_values->push_back(*((long long *)args->args[0]));
    }
  }
}

double stddev_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                          char *is_error) {
  stddev_no_outliers_data *buffer = (stddev_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL && !buffer->double_values->empty()) {
    remove_outliers(buffer->double_values);
    return stddev_population(buffer->double_values);
  } else if (buffer->int_values != NULL && !buffer->int_values->empty()) {
    remove_outliers(buffer->int_values);
    return stddev_population(buffer->int_values);
  } else {
    std::cerr << "stddev_no_outliers() internal error, all vectors were null "
                 "in computation"
              << std::endl;
    *is_null = 1;
    // *is_error = 1;
  }
  return 0;
}

/*
 *
 * Count with outlier removal
 *
 */

extern "C" {
my_bool count_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void count_no_outliers_deinit(UDF_INIT *initid);
void count_no_outliers_clear(UDF_INIT *initid, char *is_null, char *error);
void count_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                             char *error);
void count_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                           char *error);
double count_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                         char *error);
}

struct count_no_outliers_data {
  std::vector<double> *double_values;
  std::vector<long long> *int_values;
};

my_bool count_no_outliers_init(UDF_INIT *initid, UDF_ARGS *args,
                               char *message) {
  if (args->arg_count != 1) {
    strcpy(
        message,
        "wrong number of arguments: count_no_outliers() requires one argument");
    return 1;
  }

  if (args->arg_type[0] != REAL_RESULT && args->arg_type[0] != INT_RESULT &&
      args->arg_type[0] != DECIMAL_RESULT) {
    if (args->arg_type[0] == STRING_RESULT)
      strcpy(message, "count_no_outliers() requires a real, decimal, double or "
                      "integer as parameter 1, received STRING");
    else
      strcpy(message, "count_no_outliers() requires a real, decimal, double or "
                      "integer as parameter 1, received Decimal");
    return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  count_no_outliers_data *buffer = new count_no_outliers_data;
  buffer->double_values = NULL;
  buffer->int_values = NULL;
  initid->ptr = (char *)buffer;

  return 0;
}

void count_no_outliers_deinit(UDF_INIT *initid) {
  count_no_outliers_data *buffer = (count_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }
  delete initid->ptr;
}

void count_no_outliers_clear(UDF_INIT *initid, char *is_null, char *is_error) {
  count_no_outliers_data *buffer = (count_no_outliers_data *)initid->ptr;
  *is_null = 0;
  *is_error = 0;

  if (buffer->double_values != NULL) {
    delete buffer->double_values;
    buffer->double_values = NULL;
  }
  if (buffer->int_values != NULL) {
    delete buffer->int_values;
    buffer->int_values = NULL;
  }

  buffer->double_values = new std::vector<double>;
  buffer->int_values = new std::vector<long long>;
}

void count_no_outliers_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                             char *is_error) {
  count_no_outliers_clear(initid, is_null, is_error);
  count_no_outliers_add(initid, args, is_null, is_error);
}

void count_no_outliers_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                           char *is_error) {
  if (args->args[0] != NULL) {
    count_no_outliers_data *buffer = (count_no_outliers_data *)initid->ptr;
    if (args->arg_type[0] == REAL_RESULT ||
        args->arg_type[0] == DECIMAL_RESULT) {
      buffer->double_values->push_back(*((double *)args->args[0]));
    } else if (args->arg_type[0] == INT_RESULT) {
      buffer->int_values->push_back(*((long long *)args->args[0]));
    }
  }
}

double count_no_outliers(UDF_INIT *initid, UDF_ARGS *args, char *is_null,
                         char *is_error) {
  count_no_outliers_data *buffer = (count_no_outliers_data *)initid->ptr;

  if (buffer->double_values != NULL && !buffer->double_values->empty()) {
    remove_outliers(buffer->double_values);
    return buffer->double_values->size();
  } else if (buffer->int_values != NULL && !buffer->int_values->empty()) {
    remove_outliers(buffer->int_values);
    return buffer->int_values->size();
  } else {
    std::cerr << "count_no_outliers() internal error, all vectors were null in "
                 "computation"
              << std::endl;
    *is_null = 1;
    //  *is_error = 1;
  }
  return 0;
}
