#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_arch.h"

/*-----------------------------------------------------------------------------
 [begin] extension to handle UTF-8 paths
 */
#if LJ_TARGET_WINDOWS
#include <windows.h>
#endif

wchar_t* cc2wc(const char *c)
{
#if LJ_TARGET_WINDOWS
  int wsz = MultiByteToWideChar(CP_UTF8,0,c,-1,0,0);
  wchar_t* wstr = (wchar_t*)malloc(wsz*sizeof(wchar_t));
  MultiByteToWideChar( CP_UTF8 , 0 , c, -1, wstr , wsz );
  return wstr;
#else
  (void) c;
  return 0;
#endif
}

char* wc2cc(const wchar_t* wc)
{
#if LJ_TARGET_WINDOWS
  int nc = WideCharToMultiByte(CP_UTF8,0,wc,-1,NULL,0,NULL,NULL);
  char* str = (char*)malloc(nc);
  WideCharToMultiByte(CP_UTF8,0,wc,-1,str,nc,NULL,NULL);
  return str;
#else
  (void) wc;
  return 0;
#endif
}

FILE* utf8_fopen(const char *filename, const char *mode)
{
#if LJ_TARGET_WINDOWS
    wchar_t *wfilename  = cc2wc(filename);
    wchar_t *wmode      = cc2wc(mode);
    FILE* fp = _wfopen(wfilename, wmode);
    free( wfilename );
    free( wmode );
    return fp;
#else
    return fopen(filename, mode);
#endif
}

FILE* utf8_popen(const char *filename, const char *mode)
{
#if LJ_TARGET_WINDOWS
    wchar_t *wfilename  = cc2wc(filename);
    wchar_t *wmode      = cc2wc(mode);
    FILE* fp = _wpopen(wfilename, wmode); 
    free( wfilename );
    free( wmode );
    return fp;
#else
    return popen(filename, mode); 
#endif
}

char* utf8_getenv(const char* varname)
{
#if LJ_TARGET_WINDOWS
    wchar_t *wvarname      = cc2wc(varname);
    wchar_t *wev = _wgetenv(wvarname);
    if (!wev) return NULL;
    char *ev = wc2cc(wev);
    free( wvarname );
    return ev;
#else
    // we're not allowed to free() the output of getenv (see getenv(3));
    // use strdup to keep the same caller-frees-ev semantics as on windows
    const char* ev = getenv(varname);
    return ev ? strdup(ev) : NULL;
#endif
}


/*-----------------------------------------------------------------------------
 [end] extension to handle UTF-8 paths
 */

