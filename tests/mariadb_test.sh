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