/*

  Copyright Seth Shelnutt 2020-06-30
  Licensed Under the GPL v3 or later

  Common definitions
*/

#ifndef __COMMON_HPP
#define __COMMON_HPP

#define FLOATING_POINT_DECIMALS 31
#define NOT_FIXED_DEC FLOATING_POINT_DECIMALS
#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/

#endif
