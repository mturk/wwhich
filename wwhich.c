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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include "wwhich.h"


static wchar_t *xwalloc(size_t size)
{
    wchar_t *p = (wchar_t *)calloc(size, sizeof(wchar_t));
    if (p == NULL)
        _exit(ERROR_OUTOFMEMORY);

    return p;
}

static void usage(void)
{
    fprintf(stderr, "Usage: " WWHICH_INTNAME " COMMAND[.exe] [...]\n");
    fprintf(stderr, "Write the full path of COMMAND(s) to standard output.\n\n");
    fprintf(stderr, WWHICH_INTNAME " " WWHICH_VERSION_STR " (" __DATE__ " " __TIME__ ")\n");
    fprintf(stderr, "Written by Mladen Turk (mturk@apache.org).\n");
}

static wchar_t *searchpathw(const wchar_t *path, const wchar_t *name, const wchar_t **fname)
{
    HANDLE   fh = NULL;
    wchar_t  buf[4];
    wchar_t  *sp, *fp;
    DWORD    ln, sz;

    ln = SearchPathW(path, name, L".exe", 4, buf, NULL);
    if (ln == 0)
        return NULL;

    sz = ln + 1;
    sp = xwalloc(sz);
    ln = SearchPathW(path, name, L".exe", sz, sp, NULL);
    if (ln == 0)
        goto finished;
    fh = CreateFileW(sp, GENERIC_READ, FILE_SHARE_READ, 0,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (IS_INVALID_HANDLE(fh))
        goto finished;
    SAFE_FREE(sp);
    sz = ln + 1;
    while (sp == NULL) {
        sp = xwalloc(sz);
        ln = GetFinalPathNameByHandleW(fh, sp, sz, VOLUME_NAME_DOS);
        if (ln == 0) {
            CloseHandle(fh);
            goto finished;
        }
        if (ln > sz) {
            SAFE_FREE(sp);
            sz = ln;
        }
    }
    CloseHandle(fh);
    if ((ln > 5) && (ln < _MAX_FNAME)) {
        /**
         * Strip leading \\?\ for short paths
         * but not \\?\UNC\* paths
         */
        if ((sp[0] == L'\\') &&
            (sp[1] == L'\\') &&
            (sp[2] == L'?')  &&
            (sp[3] == L'\\') &&
            (sp[5] == L':')) {
            wmemmove(sp, sp + 4, ln - 3);
            ln -= 4;
        }
    }
    fp = sp + ln;
    while (--fp > sp) {
        if (*fp == L'\\') {
            *(fp++) = L'\0';
            if (fname)
                *fname = fp;
            return sp;
        }
    }
finished:
    SAFE_FREE(sp);
    return NULL;
}

int wmain(int argc, const wchar_t **wargv)
{
    int      i, rc = 0;
    wchar_t *p;

    if (argc < 2) {
        usage();
        return ERROR_INVALID_PARAMETER;
    }
    if ((p = _wgetenv(L"PATH")) == NULL) {
        fputs(WWHICH_INTNAME ": missing PATH environment variable", stderr);
        return ERROR_ENVVAR_NOT_FOUND;
    }
    for (i = 1; i < argc; i++) {
        wchar_t *b, *f;
        if ((b = searchpathw(p, wargv[i], &f)) != NULL) {
            if (i > 1)
                fputc(L'\n', stdout);
#if defined(_PRINT_FILEPART)
            fputs(f, stdout);
            fputs(": ", stdout);
#endif
            fputws(b, stdout);
            free(b);
        }
        else {
            if (i > 1)
                fputc('\n', stderr);
            fprintf(stderr, WWHICH_INTNAME ": no %S in (%S)", wargv[i], p);
            rc = ERROR_FILE_NOT_FOUND;
            break;
        }
    }
    return rc;
}
