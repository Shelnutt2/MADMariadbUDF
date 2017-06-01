#!/bin/bash

set -e

. assert.sh

mysql_mad_int_test=$(mysql -uroot -ss test -e "select mad(value) FROM test_int_mad;")
mysql_mad_double_test=$(mysql -uroot -ss test -e "select mad(value) FROM test_double_mad;")
mysql_mad_decimal_test=$(mysql -uroot -ss test -e "select mad(value) FROM test_decimal_mad;")
mysql_median_int_test=$(mysql -uroot -ss test -e "select median(value) FROM test_int_mad;")
mysql_median_double_test=$(mysql -uroot -ss test -e "select median(value) FROM test_double_mad;")
mysql_median_decimal_test=$(mysql -uroot -ss test -e "select median(value) FROM test_decimal_mad;")

assert $mysql_mad_int_test 1
assert $mysql_mad_double_test 1
assert $mysql_mad_decimal_test 1
assert $mysql_median_int_test 2
assert $mysql_median_double_test 2
assert $mysql_median_decimal_test 2

mysql_quantile_int_test=$(mysql -uroot -ss test -e "select quantile(value, 0.5) FROM test_int_outlier;")
mysql_quantile_double_test=$(mysql -uroot -ss test -e "select quantile(value, 0.25) FROM test_double_outlier;")
mysql_quantile_double_test2=$(mysql -uroot -ss test -e "select quantile(value, 0.75) FROM test_double_outlier;")

assert $mysql_quantile_int_test 5
assert $mysql_quantile_double_test 51
assert $mysql_quantile_double_test2 55

mysql_mean_no_outliers_int_test=$(mysql -uroot -ss test -e "select mean_no_outliers(value) FROM test_int_outlier;")
mysql_mean_no_outliers_double_test=$(mysql -uroot -ss test -e "select mean_no_outliers(value) FROM test_double_outlier;")

assert $mysql_mean_no_outliers_int_test 5
assert $mysql_mean_no_outliers_double_test 53.5

mysql_stddev_no_outliers_int_test=$(mysql -uroot -ss test -e "select stddev_no_outliers(value) FROM test_int_outlier;")
mysql_stddev_no_outliers_double_test=$(mysql -uroot -ss test -e "select stddev_no_outliers(value) FROM test_double_outlier;")

assert $mysql_stddev_no_outliers_int_test 2.5820
assert $mysql_stddev_no_outliers_double_test 3.3040379335998358

mysql_count_no_outliers_int_test=$(mysql -uroot -ss test -e "select count_no_outliers(value) FROM test_int_outlier;")
mysql_count_no_outliers_double_test=$(mysql -uroot -ss test -e "select count_no_outliers(value) FROM test_double_outlier;")

assert $mysql_count_no_outliers_int_test 9
assert $mysql_count_no_outliers_double_test 6