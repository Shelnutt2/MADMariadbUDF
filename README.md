# Mariadb-Statistical-UDFs  [![Build Status](https://travis-ci.org/Shelnutt2/Mariadb-Statistical-UDFs.svg?branch=master)](https://travis-ci.org/Shelnutt2/Mariadb-Statistical-UDFs ) [![Coverage Status](https://coveralls.io/repos/github/Shelnutt2/Mariadb-Statistical-UDFs/badge.svg?branch=master)](https://coveralls.io/github/Shelnutt2/Mariadb-Statistical-UDFs?branch=master)

A User Defined Function which computes the Median absolute deviation

## Building

```sh
git clone https://github.com/Shelnutt2/Mariadb-Statistical-UDFs.git
cd Mariadb-Statistical-UDFs
mkdir build
cd build
cmake ..
make
```

## Installing

```sh
make install
mysql -p < install.sql
```

## Removing

```sh
mysql -p < uninstall.sql
```
