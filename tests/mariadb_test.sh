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
