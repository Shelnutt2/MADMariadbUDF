/*

  Copyright Seth Shelnutt 2017-05-26
  Licensed Under the GPL v3 or later

  returns the quantile of the values in a distribution

  input parameters:
  data (real, interger)
  qauntile (real)

  output:
  quantile value of the distribution (real)

  registering the function:
  CREATE AGGREGATE FUNCTION quantile RETURNS REAL SONAME 'libmad_udf.so';

  getting rid of the function:
  DROP FUNCTION quantile;
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
#include <quantile.hpp>


extern "C" {
my_bool quantile_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void quantile_deinit( UDF_INIT* initid );
void quantile_clear( UDF_INIT* initid, char* is_null, char *error );
void quantile_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
void quantile_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
double quantile( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
/*long long quantile( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );*/
}

struct quantile_data
{
    std::vector<double> *double_values;
    std::vector<long long> *int_values;
    double quantile;

};

my_bool quantile_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 2)
  {
    strcpy(message,"wrong number of arguments: quantile() requires two argument");
    return 1;
  }

  if (args->arg_type[0]!=REAL_RESULT && args->arg_type[0]!=INT_RESULT && args->arg_type[0]!=DECIMAL_RESULT)
  {
    if (args->arg_type[0] == STRING_RESULT)
      strcpy(message,"quantile() requires a real, decimal, double or integer as parameter 1, received STRING");
    else
      strcpy(message,"quantile() requires a real, decimal, double or integer as parameter 1, received Decimal");
    return 1;
  }

  if (args->arg_type[1]!=REAL_RESULT && args->arg_type[1]!=INT_RESULT && args->arg_type[1]!=DECIMAL_RESULT)
  {
    if (args->arg_type[1] == STRING_RESULT)
      strcpy(message,"quantile() requires a real, decimal, double or integer as parameter 2, received STRING");
    else
      strcpy(message,"quantile() requires a real, decimal, double or integer as parameter 2, received unknown");
    return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  quantile_data *buffer = new quantile_data;
  buffer->double_values = NULL;
  buffer->int_values = NULL;
  initid->ptr = (char*)buffer;

  return 0;
}


void quantile_deinit( UDF_INIT* initid )
{
  quantile_data *buffer = (quantile_data*)initid->ptr;

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


void quantile_clear( UDF_INIT* initid, char* is_null, char* is_error )
{
  quantile_data *buffer = (quantile_data*)initid->ptr;
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


void quantile_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  quantile_clear(initid, is_null, is_error);
  quantile_add( initid, args, is_null, is_error );
}


void quantile_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  if (args->args[0]!=NULL)
  {
    quantile_data *buffer = (quantile_data*)initid->ptr;
    if (args->arg_type[0]==REAL_RESULT || args->arg_type[0]==DECIMAL_RESULT)
    {
      buffer->double_values->push_back(*((double*)args->args[0]));
    }
    else if (args->arg_type[0]==INT_RESULT)
    {
      buffer->int_values->push_back(*((long long*)args->args[0]));
    }
  }

  if(args->args[1]!=NULL)
  {
    quantile_data *buffer = (quantile_data*)initid->ptr;
    buffer->quantile = *((double*)args->args[1]);
  }
}

double quantile( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  quantile_data* buffer = (quantile_data*)initid->ptr;

  if (buffer->double_values != NULL && buffer->double_values->size() > 0) {
    std::vector<double> quantile = Quantile(buffer->double_values, {buffer->quantile});
    return quantile[0];
  } else if (buffer->int_values != NULL && buffer->int_values->size() > 0) {
    std::vector<long long> quantile = Quantile(buffer->int_values, {buffer->quantile});
    return quantile[0];
  }
  std::cerr << "quantile() internal error, all vectors were null in computation" << std::endl;
  *is_error = 1;
  return 0;
}

/*long long quantile( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  quantile_data* buffer = (quantile_data*)initid->ptr;
  return quantile(buffer->int_values->begin(), buffer->int_values->end());
}*/


/* #endif */
