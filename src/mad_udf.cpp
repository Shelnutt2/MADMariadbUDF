/*

  Copyright Seth Shelnutt 2016-06-27
  Licensed Under the GPL v3 or later

  returns the mad of the values in a distribution

  input parameters:
  data (real)

  output:
  mad value of the distribution (real)

  registering the function:
  CREATE AGGREGATE FUNCTION mad RETURNS REAL SONAME 'udf_mad.so';

  getting rid of the function:
  DROP FUNCTION mad;
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
#include <algorithm>
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>
#include <mad.hpp>

/*#ifdef HAVE_DLOPEN*/


#define BUFFERSIZE 1024


extern "C" {
my_bool mad_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void mad_deinit( UDF_INIT* initid );
void mad_clear( UDF_INIT* initid, char* is_null, char *error );
void mad_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
void mad_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
double mad( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
/*long long mad( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );*/
}

struct mad_data
{
  std::vector<double> *double_values;
  std::vector<long long> *int_values;
};

my_bool mad_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if (args->arg_count != 1)
  {
    strcpy(message,"wrong number of arguments: mad() requires one argument");
    return 1;
  }

  if (args->arg_type[0]!=REAL_RESULT && args->arg_type[1]!=INT_RESULT)
  {
    strcpy(message,"mad() requires a real or integer as parameter 1");
       return 1;
  }

  initid->decimals = NOT_FIXED_DEC;
  initid->maybe_null = 1;

  mad_data *buffer = new mad_data;
  initid->ptr = (char*)buffer;

  return 0;
}


void mad_deinit( UDF_INIT* initid )
{
  mad_data *buffer = (mad_data*)initid->ptr;

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


void mad_clear( UDF_INIT* initid, char* is_null, char* is_error )
{
  mad_data *buffer = (mad_data*)initid->ptr;
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


void mad_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  mad_clear(initid, is_null, is_error);
  mad_add( initid, args, is_null, is_error );
}


void mad_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  if (args->args[0]!=NULL)
  {
    mad_data *buffer = (mad_data*)initid->ptr;
    if (args->arg_type[0]==REAL_RESULT)
    {
      buffer->double_values->push_back(*((double*)args->args[0]));
    }
    else if (args->arg_type[1]==INT_RESULT)
    {
      buffer->int_values->push_back(*((long long*)args->args[0]));
    }
  }
}

double mad( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  mad_data* buffer = (mad_data*)initid->ptr;
  double mad;
  if (buffer->double_values != NULL) {
    mad = Mad(buffer->double_values);
  } else {
    mad = Mad(buffer->int_values);
  }
  return mad;
}

/*long long mad( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* is_error )
{
  mad_data* buffer = (mad_data*)initid->ptr;
  return median(buffer->int_values->begin(), buffer->int_values->end());
}*/


/* #endif */
