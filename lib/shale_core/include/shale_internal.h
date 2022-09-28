#ifndef _SHALE_INTERNAL_H
#define _SHALE_INTERNAL_H

#ifndef __SHALE_SOURCE__
#   error __file__ " should not be included outside of the SHALE codebase"
#endif

#ifndef _SHALE_H
#   error __file__ " must be included after shale.h"
#endif

#include <stdlib.h>

#define assert_class(pclass)
#define assert_driver(pdriver)
#define assert_device(pdevice)

#define shale_malloc malloc

#endif