/*

  Copyright Seth Shelnutt 2016-06-27
  Licensed Under the GPL v3 or later

  returns the median of the values in a distribution

  input parameters:
  data (real)

  output:
  median value of the distribution (real)

  registering the function:
  CREATE AGGREGATE FUNCTION median RETURNS REAL SONAME 'libmad_udf.so';

  getting rid of the function:
  DROP FUNCTION median;
*/


#ifdef STANDARD
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <vector>
#include <iostream>
#include <algorithm>
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>
#include <mad.hpp>


extern "C" {
my_bool median_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void median_deinit( UDF_INIT* initid );
void median_clear( UDF_INIT* initid, char* is_null, char *error );
void median_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
void median_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
double median( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
/*long long median( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );*/
}

struct median_data
{
  std::vector<double> *double_values;
  std::vector<long long> *int_values;
};

my_bool median_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 1)
  {
    strcpy(message,"wrong number of arguments: median() requires one argument");
    return 1;
  }

  if (args->arg_type[0]!=REAL_RESULT && args->arg_type[0]!=INT_RESULT && args->arg_type[0] != DECIMAL_RESULT)
  {
    if (args->arg_type[0] == STRING_RESULT)
      strcpy(message,"median() requires a real or integer as parameter 1, received STRING");
    else
      strcpy(message,"median() requires a decimal, real or integer as parameter 1");
    return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  median_data *buffer = new median_data;
  buffer->double_values = NULL;
  buffer->int_values = NULL;
  initid->ptr = (char*)buffer;

  return 0;
}


void median_deinit( UDF_INIT* initid )
{
  median_data *buffer = (median_data*)initid->ptr;

  if (buffer->double_values != NULL)
  {
    free(buffer->double_values);
    buffer->double_values=NULL;
  }
  if (buffer->int_values != NULL)
  {
    free(buffer->int_values);
    buffer->int_values=NULL;
  }
  delete initid->ptr;
}


void median_clear( UDF_INIT* initid, char* is_null, char* is_error )
{
  median_data *buffer = (median_data*)initid->ptr;
  *is_null = 0;
  *is_error = 0;

  if (buffer->double_values != NULL)
  {
    free(buffer->double_values);
    buffer->double_values=NULL;
  }
  if (buffer->int_values != NULL)
  {
    free(buffer->int_values);
    buffer->int_values=NULL;
  }

  buffer->double_values = new std::vector<double>;
  buffer->int_values = new std::vector<long long>;

}


void median_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  median_clear(initid, is_null, is_error);
  median_add( initid, args, is_null, is_error );
}


void median_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  if (args->args[0]!=NULL)
  {
    median_data *buffer = (median_data*)initid->ptr;
    if (args->arg_type[0]==REAL_RESULT || args->arg_type[0]==DECIMAL_RESULT)
    {
      buffer->double_values->push_back(*((double*)args->args[0]));
    }
    else if (args->arg_type[0]==INT_RESULT)
    {
      buffer->int_values->push_back(*((long long*)args->args[0]));
    }
  }
}

double median( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  median_data* buffer = (median_data*)initid->ptr;
  double median;
  if (buffer->double_values != NULL && buffer->double_values->size() > 0) {
    median = Median(buffer->double_values);
  } else if (buffer->int_values != NULL && buffer->int_values->size() > 0) {
    median = Median(buffer->int_values);
  } else {
    std::cerr << "median() internal error, all vectors were null in computation" << std::endl;
    *is_error = 1;
  }
  return median;
}

/*long long median( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  median_data* buffer = (median_data*)initid->ptr;
  return median(buffer->int_values->begin(), buffer->int_values->end());
}*/


/* #endif */
