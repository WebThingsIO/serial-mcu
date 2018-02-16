/*
 * Log.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Log_h)
#define Log_h

#if defined(USE_LOG)

#include <stdio.h>

#define LOG(...) printf(__VA_ARGS__)

#else

#define  LOG(...)

#endif
#endif // Log_h
