/** \file
 * \brief UTF8 definitions
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDGDK_STR_H
#define __CDGDK_STR_H

#ifdef __cplusplus
extern "C" {
#endif


char* cdStrToSystem(int utf8mode, const char* str);
char* cdStrFromSystem(int utf8mode, const char* str);


#ifdef __cplusplus
}
#endif

#endif /* ifndef __CDGDK_STR_ */


