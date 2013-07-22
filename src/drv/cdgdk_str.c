/** \file
 * \brief GDK UTF-8 Encapsulation
 *
 * See Copyright Notice in cd.h
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gdk/gdk.h>

#include "cdgdk_str.h"          


static char* strLastConvertUTF8 = NULL;

static int cdgdkStrIsAscii(const char* str)
{
  while(*str)
  {
    int c = *str;
    if (c < 0)
      return 0;
    str++;
  }
  return 1;
}

static char* cdgdkStrToUTF8(const char *str, const char* charset)
{
  return g_convert(str, -1, "UTF-8", charset, NULL, NULL, NULL);
}

static char* cdgdkStrFromUTF8(const char *str, const char* charset)
{
  return g_convert(str, -1, charset, "UTF-8", NULL, NULL, NULL);
}

char* cdStrToSystem(int utf8mode, const char* str)  /* From CD to GDK */
{
  if (!str || *str == 0)
    return (char*)str;

  if (!utf8mode)  
  {
    const char *charset = NULL;
    if (g_get_charset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (strLastConvertUTF8)
          g_free(strLastConvertUTF8);
        strLastConvertUTF8 = cdgdkStrToUTF8(str, "ISO8859-1");   /* if string is not UTF-8, assume ISO8859-1 */
        if (!strLastConvertUTF8) return (char*)str;
        return strLastConvertUTF8;
      }
    }
    else
    {
      if (cdgdkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (strLastConvertUTF8)
          g_free(strLastConvertUTF8);
        strLastConvertUTF8 = cdgdkStrToUTF8(str, charset);
        if (!strLastConvertUTF8) return (char*)str;
        return strLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}

char* cdStrFromSystem(int utf8mode, const char* str)  /* From GDK to CD */
{
  if (!str || *str == 0)
    return (char*)str;

  if (!utf8mode)  
  {
    const gchar *charset = NULL;
    if (g_get_charset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (strLastConvertUTF8)
          g_free(strLastConvertUTF8);
        strLastConvertUTF8 = cdgdkStrFromUTF8(str, "ISO8859-1");  /* if string is not UTF-8, assume ISO8859-1 */
        if (!strLastConvertUTF8) return (char*)str;
        return strLastConvertUTF8;
      }
    }
    else
    {
      if (cdgdkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (strLastConvertUTF8)
          g_free(strLastConvertUTF8);
        strLastConvertUTF8 = cdgdkStrFromUTF8(str, charset);
        if (!strLastConvertUTF8) return (char*)str;
        return strLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}
