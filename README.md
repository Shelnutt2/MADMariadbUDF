# MADMariadbUDF [![Build Status](https://travis-ci.org/Shelnutt2/MADMariadbUDF.svg?branch=master)](https://travis-ci.org/Shelnutt2/MADMariadbUDF) [![Coverage Status](https://coveralls.io/repos/github/Shelnutt2/MADMariadbUDF/badge.svg?branch=master)](https://coveralls.io/github/Shelnutt2/MADMariadbUDF?branch=master)

A User Defined Function which computes the Median absolute deviation

## Building

```sh
git clone https://github.com/Shelnutt2/MADMariadbUDF.git
cd MADMariadbUDF
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
