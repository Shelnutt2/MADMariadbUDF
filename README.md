# MADMariadbUDF
A User Defined Function which computes the Median absolute deviation

## Building

```sh
mkdir build
cd build
cmake ..
make
```

## Installing

```sh
make install
mysql -p
CREATE AGGREGATE FUNCTION mad RETURNS REAL SONAME 'libmad_udf.so';
```

## Removing

```sh
mysql -p
DROP FUNCTION mad;
```
