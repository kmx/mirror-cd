/** \file
 * \brief Windows Unicode Encapsulation
 *
 * See Copyright Notice in cd.h
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

#include "cdwin_str.h"


static void* winStrGetMemory(int size)
{
#define MAX_BUFFERS 50
  static void* buffers[MAX_BUFFERS];
  static int buffers_sizes[MAX_BUFFERS];
  static int buffers_index = -1;

  int i;

  if (size == -1) /* Frees memory */
  {
    buffers_index = -1;
    for (i = 0; i < MAX_BUFFERS; i++)
    {
      if (buffers[i]) 
      {
        free(buffers[i]);
        buffers[i] = NULL;
      }
      buffers_sizes[i] = 0;
    }
    return NULL;
  }
  else
  {
    void* ret_buffer;

    /* init buffers array */
    if (buffers_index == -1)
    {
      memset(buffers, 0, sizeof(void*)*MAX_BUFFERS);
      memset(buffers_sizes, 0, sizeof(int)*MAX_BUFFERS);
      buffers_index = 0;
    }

    /* first alocation */
    if (!(buffers[buffers_index]))
    {
      buffers_sizes[buffers_index] = size+1;
      buffers[buffers_index] = malloc(buffers_sizes[buffers_index]);
    }
    else if (buffers_sizes[buffers_index] < size+1)  /* reallocate if necessary */
    {
      buffers_sizes[buffers_index] = size+1;
      buffers[buffers_index] = realloc(buffers[buffers_index], buffers_sizes[buffers_index]);
    }

    /* clear memory */
    memset(buffers[buffers_index], 0, buffers_sizes[buffers_index]);
    ret_buffer = buffers[buffers_index];

    buffers_index++;
    if (buffers_index == MAX_BUFFERS)
      buffers_index = 0;

    return ret_buffer;
  }
#undef MAX_BUFFERS
}

static void winStrWide2Char(const WCHAR* wstr, char* str, int len, int cd_utf8mode)
{
  if (cd_utf8mode)
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, len, str, 2*len, NULL, NULL);  /* str must has a large buffer */
  else
    len = WideCharToMultiByte(CP_ACP, 0, wstr, len, str, 2*len, NULL, NULL);

  if (len>0)
    str[len] = 0;
}

static void winStrChar2Wide(const char* str, WCHAR* wstr, int len, int cd_utf8mode)
{
  if (cd_utf8mode)
    len = MultiByteToWideChar(CP_UTF8, 0, str, len, wstr, len);
  else
    len = MultiByteToWideChar(CP_ACP, 0, str, len, wstr, len);

  if (len>0)
    wstr[len] = 0;
}

TCHAR* cdStrToSystem(int utf8mode, const char* str)
{
#ifdef UNICODE
  if (str)
  {
    int len = (int)strlen(str);
    WCHAR* wstr = (WCHAR*)winStrGetMemory((len+1) * sizeof(WCHAR));
    winStrChar2Wide(str, wstr, len, utf8mode);
    return wstr;
  }
  return NULL;
#else
  (void)utf8mode;
  return (char*)str;
#endif
}

char* cdStrFromSystem(int utf8mode, const TCHAR* wstr)
{
#ifdef UNICODE
  if (wstr)
  {
    int len = (int)wcslen(wstr);
    char* str = (char*)winStrGetMemory((2*len+1) * sizeof(char));    /* str must has a large buffer because the UTF-8 string can be larger than the original */
    winStrWide2Char(wstr, str, len, utf8mode);
    return str;
  }
  return NULL;
#else
  (void)utf8mode;
  return (char*)wstr;
#endif
}
