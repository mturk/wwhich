/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _WWHICH_H_INCLUDED_
#define _WWHICH_H_INCLUDED_

#define IS_INVALID_HANDLE(h) (((h) == 0 || (h) == INVALID_HANDLE_VALUE))
#define SAFE_FREE(_h)           \
    if ((_h) != NULL)           \
        free((_h));             \
    (_h) = NULL

/**
 * Version info
 */
#define WWHICH_MAJOR_VERSION  1
#define WWHICH_MINOR_VERSION  0
#define WWHICH_PATCH_VERSION  1
#if defined(_VENDOR_NUM)
# define WWHICH_MICRO_VERSION _VENDOR_NUM
#else
# define WWHICH_MICRO_VERSION 0
#endif

/**
 * Helper macros for properly quoting a value as a
 * string in the C preprocessor
 */
#define CPP_TOSTR_HELPER(n)     #n
#define CPP_TOSTR(n)            CPP_TOSTR_HELPER(n)

#if defined(_VENDOR_SFX)
# define WWHICH_VENDOR_SFX      CPP_TOSTR(_VENDOR_SFX)
#else
# define WWHICH_VENDOR_SFX      ""
#endif
/**
 * Set to zero for non dev versions
 */
#define WWHICH_ISDEV_VERSION    1

#if WWHICH_ISDEV_VERSION
# define WWHICH_VERSION_SFX     WWHICH_VENDOR_SFX "-dev"
#else
# define WWHICH_VERSION_SFX     WWHICH_VENDOR_SFX
#endif


/**
 * Macro for .rc files using numeric csv representation
 */
#define WWHICH_VERSION_CSV      WWHICH_MAJOR_VERSION,                       \
                                WWHICH_MINOR_VERSION,                       \
                                WWHICH_PATCH_VERSION,                       \
                                WWHICH_MICRO_VERSION

#define WWHICH_VERSION_STR                                                  \
                                CPP_TOSTR(WWHICH_MAJOR_VERSION) "."         \
                                CPP_TOSTR(WWHICH_MINOR_VERSION) "."         \
                                CPP_TOSTR(WWHICH_PATCH_VERSION)             \
                                WWHICH_VERSION_SFX

#define WWHICH_INTNAME         "wwhich"
#define WWHICH_DESCRIPTION     "Write the full path of COMMAND(s) to standard output"
#define WWHICH_AUTHOR          "Written by Mladen Turk (mturk apache.org)"
#define WWHICH_COPYRIGHT \
    "Copyright (c) 1964-2022 The Acme Corporation or its "                  \
    "licensors, as applicable."

#define WWHICH_COMPANY_NAME "Acme Corporation"

#define WWHICH_LICENSE_SHORT \
    "Licensed under the Apache-2.0 License"

#define WWHICH_LICENSE \
    "Licensed under the Apache License, Version 2.0 (the ""License""); "        \
    "you may not use this file except in compliance with the License. "         \
    "You may obtain a copy of the License at\r\n\r\n"                           \
    "http://www.apache.org/licenses/LICENSE-2.0\r\n\r\n"                        \
    "Unless required by applicable law or agreed to in writing, software "      \
    "distributed under the License is distributed on an ""AS IS"" BASIS, "      \
    "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. " \
    "See the License for the specific language governing permissions and "      \
    "limitations under the License."


#endif /* _WWHICH_H_INCLUDED_ */
