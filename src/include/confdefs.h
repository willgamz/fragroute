#pragma once
#define FRAGROUTE_CONF "fragroute.conf";

#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
#define snprintf _snprintf
#endif /* Def WIN32 or Def WIN64 */