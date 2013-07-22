/** \file
 * \brief UTF8 definitions
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDWIN_STR_H
#define __CDWIN_STR_H

#ifdef __cplusplus
extern "C" {
#endif


TCHAR* cdStrToSystem(int utf8mode, const char* str);
char* cdStrFromSystem(int utf8mode, const TCHAR* wstr);


#ifdef __cplusplus
}
#endif

#endif /* ifndef __CDWIN_STR_ */


