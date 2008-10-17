/* Arquivo gerado automaticamente por ledc 2.5         */

#include <stdlib.h>
#include <stdarg.h>
#include <iup.h>

static Ihandle* named[    172 ]; 

static Ihandle* decl( char* name, Ihandle* elem, char* first, ...)
{
  char *attr, *val;
  va_list arg;
  va_start (arg, first);
  attr = first;
  while (attr)
  {
    val = va_arg(arg,char*);
    IupSetAttribute( elem, attr, val );
    attr = va_arg(arg,char*);
  }
  va_end (arg);
  if(name) IupSetHandle( name, elem );
  return elem;
}

static void image_imgLine (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgLine", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 0 );
}

static void image_imgClip (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgClip", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 0 );
}

static void image_imgImage (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0,
     0, 0, 0, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 2, 2, 2, 1, 3, 2, 2, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 2, 2, 1, 3, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 1, 3, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 1, 3, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 2, 1, 3, 1, 2, 1, 3, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 2, 2, 1, 3, 2, 1, 3, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 3, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgImage", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 
    "3", "153 153 153", 0 );
}

static void image_imgImageRGB (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgImageRGB", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "255 0 0", 
    "2", "0 255 0", 
    "3", "0 0 255", 0 );
}

static void image_imgBox (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgBox", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}

static void image_imgRect (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 2, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 2, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 2, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgRect", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}

static void image_imgSector (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgSector", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}

static void image_imgChord (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgChord", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}

static void image_imgArc (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 2, 2, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 2, 2, 2, 0, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgArc", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}

static void image_imgText (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0,
     0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 1, 1, 1, 1, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgText", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "255 255 255", 
    "2", "0 0 0", 0 );
}

static void image_imgPoly (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgPoly", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 0 );
}

static void image_imgMark (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgMark", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 0 );
}

static void image_imgPixel (void)
{
  char map[] = {
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  -1 };

  decl( "imgPixel", IupImage( 18, 18, map ), 
    "0", "BGCOLOR", 
    "1", "0 0 0", 
    "2", "255 255 255", 0 );
}


void cdtest_loadled (void)
{
  image_imgLine ();
  image_imgClip ();
  image_imgImage ();
  image_imgImageRGB ();
  image_imgBox ();
  image_imgRect ();
  image_imgSector ();
  image_imgChord ();
  image_imgArc ();
  image_imgText ();
  image_imgPoly ();
  image_imgMark ();
  image_imgPixel ();
  named[0] = decl( "mnOpen", IupMenu(
    IupItem( "MF...", "cmdPlayMF" ),
    named[1] = decl( "itPlayCGM", IupItem( "CGM...", "cmdPlayCGM" ), 
      "ACTIVE", "NO", 0 ),
    named[2] = decl( "itPlayEMF", IupItem( "EMF...", "cmdPlayEMF" ), 
      "ACTIVE", "NO", 0 ),
    named[3] = decl( "itPlayWMF", IupItem( "WMF...", "cmdPlayWMF" ), 
      "ACTIVE", "NO", 0 ),
  NULL), 0 );
  named[4] = decl( "mnSave", IupMenu(
    IupItem( "MF...", "cmdMF" ),
    named[5] = decl( "itPDF", IupItem( "PDF...", "cmdPDF" ), 
      "ACTIVE", "NO", 0 ),
    named[6] = decl( "itPS", IupItem( "PS...", "cmdPS" ), 
      "ACTIVE", "NO", 0 ),
    named[7] = decl( "itEPS", IupItem( "EPS...", "cmdEPS" ), 
      "ACTIVE", "NO", 0 ),
    named[8] = decl( "itCGMt", IupItem( "CGMt...", "cmdCGMt" ), 
      "ACTIVE", "NO", 0 ),
    named[9] = decl( "itCGMb", IupItem( "CGMb...", "cmdCGMb" ), 
      "ACTIVE", "NO", 0 ),
    named[10] = decl( "itDGN", IupItem( "DGN...", "cmdDGN" ), 
      "ACTIVE", "NO", 0 ),
    named[11] = decl( "itDXF", IupItem( "DXF...", "cmdDXF" ), 
      "ACTIVE", "NO", 0 ),
    named[12] = decl( "itEMF", IupItem( "EMF...", "cmdEMF" ), 
      "ACTIVE", "NO", 0 ),
    named[13] = decl( "itWMF", IupItem( "WMF...", "cmdWMF" ), 
      "ACTIVE", "NO", 0 ),
  NULL), 0 );
  named[14] = decl( "mnFile", IupMenu(
    decl( 0, IupSubmenu( "Open",
      named[0] /* mnOpen */
    ), 
      "KEY", "K_O", 0 ),
    IupSeparator(),
    decl( 0, IupSubmenu( "Save",
      named[4] /* mnSave */
    ), 
      "KEY", "K_S", 0 ),
    IupSeparator(),
    named[15] = decl( "itPrint", IupItem( "Print...", "cmdPrint" ), 
      "KEY", "K_P", 0 ),
    IupSeparator(),
    decl( 0, IupItem( "Exit", "cmdFileExit" ), 
      "KEY", "K_x", 0 ),
  NULL), 0 );
  named[16] = decl( "mnEdit", IupMenu(
    named[17] = decl( "itEditUndo", IupItem( "Undo", "cmdEditUndo" ), 0 ),
    IupSeparator(),
    named[18] = decl( "itClipBoard", IupItem( "Copy as EMF/WMF", "cmdClipBoard" ), 
      "ACTIVE", "NO", 0 ),
    named[19] = decl( "itClipBoardBitmap", IupItem( "Copy as Bitmap", "cmdClipBoardBitmap" ), 
      "ACTIVE", "NO", 0 ),
    named[20] = decl( "itClipBoardMetafile", IupItem( "Copy as CD Metafile", "cmdClipBoardMetafile" ), 
      "ACTIVE", "NO", 0 ),
    named[21] = decl( "itClipBoardPaste", IupItem( "Paste", "cmdClipBoardPaste" ), 
      "ACTIVE", "NO", 0 ),
    IupSeparator(),
    IupItem( "Clear", "cmdEditClear" ),
    IupSeparator(),
    IupItem( "Options...", "cmdOptions" ),
  NULL), 0 );
  named[22] = decl( "mnPrimitives", IupMenu(
    IupItem( "Pixel...", "cmdPixel" ),
    IupItem( "Mark...", "cmdMark" ),
    IupItem( "Line...", "cmdLine" ),
    IupItem( "Polygon...", "cmdPoly" ),
    IupItem( "Rect...", "cmdRect" ),
    IupItem( "Box...", "cmdBox" ),
    IupItem( "Arc...", "cmdArc" ),
    IupItem( "Sector...", "cmdSector" ),
    IupItem( "Chord...", "cmdChord" ),
    IupItem( "Text...", "cmdText" ),
    IupSeparator(),
    IupItem( "Server Image...", "cmdImage" ),
    IupItem( "RGB Image...", "cmdImageRGB" ),
  NULL), 0 );
  named[23] = decl( "mnDraw", IupMenu(
    decl( 0, IupSubmenu( "Primitives",
      named[22] /* mnPrimitives */
    ), 
      "KEY", "K_P", 0 ),
    IupSeparator(),
    IupItem( "Clipping...", "cmdClip" ),
    IupItem( "Attributes...", "cmdAttributes" ),
    IupSeparator(),
    named[24] = decl( "itWDCanvas", IupItem( "Show WD Canvas", "cmdWDCanvas" ), 0 ),
    named[25] = decl( "itPICCanvas", IupItem( "Show Picture Canvas", "cmdPICCanvas" ), 0 ),
  NULL), 
    "ISMENU", "YES", 0 );
  named[26] = decl( "mnHelp", IupMenu(
    IupItem( "About...", "cmdHelpAbout" ),
  NULL), 0 );
  named[27] = decl( "mnMain", IupMenu(
    decl( 0, IupSubmenu( "File",
      named[14] /* mnFile */
    ), 
      "KEY", "K_mF", 0 ),
    decl( 0, IupSubmenu( "Edit",
      named[16] /* mnEdit */
    ), 
      "KEY", "K_mE", 0 ),
    decl( 0, IupSubmenu( "Draw",
      named[23] /* mnDraw */
    ), 
      "KEY", "K_mD", 0 ),
    decl( 0, IupSubmenu( "Help",
      named[26] /* mnHelp */
    ), 
      "KEY", "K_mH", 0 ),
  NULL), 0 );
  named[28] = decl( "lbStatusLine", IupLabel( "Esta eh a barra de status do CDTest." ), 
    "EXPAND", "HORIZONTAL", 0 );
  named[29] = decl( "btLine", IupButton( "", "cmdLine" ), 
    "IMAGE", "imgLine", 
    "TIP", "Line", 0 );
  named[30] = decl( "btBox", IupButton( "", "cmdBox" ), 
    "IMAGE", "imgBox", 
    "TIP", "Box", 0 );
  named[31] = decl( "btRect", IupButton( "", "cmdRect" ), 
    "IMAGE", "imgRect", 
    "TIP", "Rect", 0 );
  named[32] = decl( "btArc", IupButton( "", "cmdArc" ), 
    "IMAGE", "imgArc", 
    "TIP", "Arc", 0 );
  named[33] = decl( "btSector", IupButton( "", "cmdSector" ), 
    "IMAGE", "imgSector", 
    "TIP", "Sector", 0 );
  named[34] = decl( "btChord", IupButton( "", "cmdChord" ), 
    "IMAGE", "imgChord", 
    "TIP", "Chord", 0 );
  named[35] = decl( "btPixel", IupButton( "", "cmdPixel" ), 
    "IMAGE", "imgPixel", 
    "TIP", "Pixel", 0 );
  named[36] = decl( "btMark", IupButton( "", "cmdMark" ), 
    "IMAGE", "imgMark", 
    "TIP", "Mark", 0 );
  named[37] = decl( "btText", IupButton( "", "cmdText" ), 
    "IMAGE", "imgText", 
    "TIP", "Text", 0 );
  named[38] = decl( "btPoly", IupButton( "", "cmdPoly" ), 
    "IMAGE", "imgPoly", 
    "TIP", "Polygon", 0 );
  named[39] = decl( "btImageRGB", IupButton( "", "cmdImageRGB" ), 
    "IMAGE", "imgImageRGB", 
    "TIP", "RGB Image", 0 );
  named[40] = decl( "btImage", IupButton( "", "cmdImage" ), 
    "IMAGE", "imgImage", 
    "TIP", "Server Image", 0 );
  named[41] = decl( "btClip", IupButton( "", "cmdClip" ), 
    "IMAGE", "imgClip", 
    "TIP", "Clipping Area", 0 );
  named[42] = decl( "btCurPrim", IupButton( "", "cmdShowDialog" ), 
    "IMAGE", "imgLine", 
    "TIP", "Show primitive dialog", 0 );
  named[43] = decl( "lbMousePos", IupLabel( "(    ,    )" ), 
    "SIZE", "90", 0 );
  named[44] = decl( "PrimBar", IupVbox(
    named[42] /* btCurPrim */,
    decl( 0, IupFill(), 
      "SIZE", "20", 0 ),
    named[35] /* btPixel */,
    named[36] /* btMark */,
    named[29] /* btLine */,
    named[38] /* btPoly */,
    named[31] /* btRect */,
    named[30] /* btBox */,
    named[32] /* btArc */,
    named[33] /* btSector */,
    named[34] /* btChord */,
    named[37] /* btText */,
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
    named[40] /* btImage */,
    named[39] /* btImageRGB */,
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
    named[41] /* btClip */,
    decl( 0, IupFill(), 
      "SIZE", "30", 0 ),
  NULL), 
    "GAP", "2", 0 );
  named[45] = decl( "cnvColorBar", IupCanvas( "cmdColorBarRepaint" ), 
    "BUTTON_CB", "cmdColorBarButtonCB", 
    "RESIZE_CB", "cmdColorBarResizeCB", 
    "SIZE", "220x12", 
    "BORDER", "NO", 
    "EXPAND", "NO", 0 );
  named[46] = decl( "cnvMain", IupCanvas( "cmdRepaint" ), 
    "MOTION_CB", "cmdMotionCB", 
    "BUTTON_CB", "cmdButtonCB", 
    "RESIZE_CB", "cmdResizeCB", 0 );
  named[47] = decl( "DeskTop", IupVbox(
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
    IupHbox(
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[44] /* PrimBar */,
      decl( 0, IupFill(), 
        "SIZE", "2", 0 ),
      IupVbox(
        decl( 0, IupFill(), 
          "SIZE", "2", 0 ),
        named[43] /* lbMousePos */,
        decl( 0, IupFill(), 
          "SIZE", "5", 0 ),
        named[46] /* cnvMain */,
        decl( 0, IupFill(), 
          "SIZE", "2", 0 ),
        named[45] /* cnvColorBar */,
        decl( 0, IupFill(), 
          "SIZE", "5", 0 ),
        named[28] /* lbStatusLine */,
      NULL),
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
    NULL),
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
  NULL), 0 );
  named[48] = decl( "dlgMain", IupDialog(
    named[47] /* DeskTop */
  ), 
    "TITLE", "CD Test 5.3", 
    "MENU", "mnMain", 
    "ICON", "CdTestIcon", 
    "K_cZ", "cmdEditUndo", 
    "GETFOCUS_CB", "cmdGetFocusCB", 
    "KILLFOCUS_CB", "cmdKillFocusCB", 
    "CLOSE_CB", "cmdFileExit", 0 );
  named[49] = decl( "cnvWDCanvas", IupCanvas( "cmdWDRepaint" ), 
    "BORDER", "NO", 0 );
  named[50] = decl( "dlgWDCanvas", IupDialog(
    named[49] /* cnvWDCanvas */
  ), 
    "TITLE", "WD Canvas", 
    "CLISE_CB", "cmdCloseWD", 
    "SIZE", "200x200", 
    "PARENTDIALOG", "dlgMain", 
    "GAP", "5x5", 0 );
  named[51] = decl( "cnvPICCanvas", IupCanvas( "cmdPICRepaint" ), 
    "BORDER", "NO", 0 );
  named[52] = decl( "dlgPICCanvas", IupDialog(
    named[51] /* cnvPICCanvas */
  ), 
    "TITLE", "PIC Canvas", 
    "CLISE_CB", "cmdClosePIC", 
    "SIZE", "200x200", 
    "PARENTDIALOG", "dlgMain", 
    "GAP", "5x5", 0 );
  named[53] = decl( "btOptionsHide", IupButton( "Hide", "cmdOptionsHide" ), 
    "SIZE", "30", 0 );
  named[54] = decl( "tgSimulate", IupToggle( "Simulate", "cmdSimulate" ), 
    "VALUE", "OFF", 
    "SIZE", "55", 0 );
  named[55] = decl( "tgStretchPlay", IupToggle( "Stretch Play", "cmdStretchPlay" ), 
    "VALUE", "OFF", 
    "SIZE", "70", 0 );
  named[56] = decl( "tgNoBuffering", IupToggle( "No buffering", "cmdNoBuffering" ), 
    "VALUE", "ON", 0 );
  named[57] = decl( "tgImageBuffer", IupToggle( "CD_DBUFFER", "cmdImageBuffer" ), 0 );
  named[58] = decl( "tgRGBBuffer", IupToggle( "CD_DBUFFERRGB", "cmdRGBBuffer" ), 0 );
  named[59] = decl( "rdBuffering", IupRadio(
    IupVbox(
      named[56] /* tgNoBuffering */,
      named[57] /* tgImageBuffer */,
      named[58] /* tgRGBBuffer */,
    NULL)
  ), 0 );
  named[60] = decl( "OptionsDesktop", IupVbox(
    decl( 0, IupVbox(
      decl( 0, IupFrame(
        named[59] /* rdBuffering */
      ), 
        "TITLE", "Buffering:", 
        "MARGIN", "5x5", 
        "SIZE", "80", 0 ),
      decl( 0, IupFill(), 
        "SIZE", "10", 0 ),
      named[54] /* tgSimulate */,
      named[55] /* tgStretchPlay */,
      decl( 0, IupFill(), 
        "SIZE", "10", 0 ),
      named[53] /* btOptionsHide */,
      decl( 0, IupFill(), 
        "SIZE", "2", 0 ),
    NULL), 
      "GAP", "2", 0 ),
  NULL), 0 );
  named[61] = decl( "dlgOptions", IupDialog(
    named[60] /* OptionsDesktop */
  ), 
    "TITLE", "Options", 
    "PARENTDIALOG", "dlgMain", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "MARGIN", "5x2", 0 );
  named[62] = decl( "lstTextAlignment", IupList( "cmdTextAlignment" ), 
    "1", "CD_NORTH", 
    "2", "CD_SOUTH", 
    "3", "CD_EAST", 
    "4", "CD_WEST", 
    "5", "CD_NORTH_EAST", 
    "6", "CD_NORTH_WEST", 
    "7", "CD_SOUTH_EAST", 
    "8", "CD_SOUTH_WEST", 
    "9", "CD_CENTER", 
    "10", "CD_BASE_LEFT", 
    "11", "CD_BASE_CENTER", 
    "12", "CD_BASE_RIGHT", 
    "DROPDOWN", "YES", 
    "VALUE", "10", 
    "SIZE", "89", 0 );
  named[63] = decl( "txtMarkSize", IupText( "cmdInteger" ), 
    "NC", "4", 
    "SIZE", "20x12", 
    "VALUE", "10", 0 );
  named[64] = decl( "lstMarkType", IupList( "cmdMarkType" ), 
    "1", "CD_PLUS", 
    "2", "CD_STAR", 
    "3", "CD_CIRCLE", 
    "4", "CD_X", 
    "5", "CD_BOX", 
    "6", "CD_DIAMOND", 
    "7", "CD_HOLLOW_CIRCLE", 
    "8", "CD_HOLLOW_BOX", 
    "9", "CD_HOLLOW_DIAMOND", 
    "DROPDOWN", "YES", 
    "VALUE", "2", 
    "SIZE", "80", 0 );
  named[65] = decl( "lstWriteMode", IupList( "cmdWriteMode" ), 
    "1", "CD_REPLACE", 
    "2", "CD_XOR", 
    "3", "CD_NOT_XOR", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[66] = decl( "lstFontTypeFace", IupList( "cmdFontTypeFace" ), 
    "1", "CD_SYSTEM", 
    "2", "CD_COURIER", 
    "3", "CD_TIMES_ROMAN", 
    "4", "CD_HELVETICA", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[67] = decl( "lstLineCap", IupList( "cmdLineCap" ), 
    "1", "CD_CAPFLAT", 
    "2", "CD_CAPSQUARE", 
    "3", "CD_CAPROUND", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[68] = decl( "lstLineJoin", IupList( "cmdLineJoin" ), 
    "1", "CD_BEVEL", 
    "2", "CD_MITER", 
    "3", "CD_ROUND", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[69] = decl( "lstFillMode", IupList( "cmdFillMode" ), 
    "1", "CD_EVENODD", 
    "2", "CD_WINDING", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[70] = decl( "lstFontStyle", IupList( "cmdFontStyle" ), 
    "1", "CD_PLAIN", 
    "2", "CD_BOLD", 
    "3", "CD_ITALIC", 
    "4", "CD_BOLD_ITALIC", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[71] = decl( "txtLineWidth", IupText( "cmdInteger" ), 
    "SIZE", "30", 
    "NC", "3", 
    "VALUE", "1", 0 );
  named[72] = decl( "txtFontSize", IupText( "cmdInteger" ), 
    "SIZE", "30", 
    "NC", "3", 
    "VALUE", "12", 0 );
  named[73] = decl( "txtTextOrientation", IupText( "cmdInteger" ), 
    "SIZE", "30", 
    "NC", "3", 
    "VALUE", "0", 0 );
  named[74] = decl( "lstLineStyle", IupList( "cmdLineStyle" ), 
    "1", "CD_CONTINUOUS", 
    "2", "CD_DASHED", 
    "3", "CD_DOTTED", 
    "4", "CD_DASH_DOT", 
    "5", "CD_DASH_DOT_DOT", 
    "6", "CD_CUSTOM", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 0 );
  named[75] = decl( "tgSolid", IupToggle( "CD_SOLID", "cmdSolid" ), 
    "VALUE", "ON", 0 );
  named[76] = decl( "tgHatch", IupToggle( "CD_HATCH", "cmdHatch" ), 0 );
  named[77] = decl( "tgStipple", IupToggle( "CD_STIPPLE", "cmdStipple" ), 0 );
  named[78] = decl( "tgPattern", IupToggle( "CD_PATTERN", "cmdPattern" ), 0 );
  named[79] = decl( "lstOpacity", IupList( "cmdOpacity" ), 
    "1", "CD_OPAQUE", 
    "2", "CD_TRANSPARENT", 
    "VALUE", "2", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 
    "ACTIVE", "YES", 0 );
  named[80] = decl( "lstHatchStyle", IupList( "cmdHatchStyle" ), 
    "1", "CD_HORIZONTAL", 
    "2", "CD_VERTICAL", 
    "3", "CD_FDIAGONAL", 
    "4", "CD_BDIAGONAL", 
    "5", "CD_CROSS", 
    "6", "CD_DIAGCROSS", 
    "VALUE", "1", 
    "DROPDOWN", "YES", 
    "SIZE", "80", 
    "ACTIVE", "YES", 0 );
  named[81] = decl( "btAttributesHide", IupButton( "Hide", "cmdAttributesHide" ), 
    "SIZE", "30", 0 );
  named[82] = decl( "rdInteriorStyle", IupRadio(
    IupVbox(
      named[75] /* tgSolid */,
      named[76] /* tgHatch */,
      named[77] /* tgStipple */,
      named[78] /* tgPattern */,
    NULL)
  ), 0 );
  named[83] = decl( "AttributesDesktop", IupVbox(
    IupHbox(
      decl( 0, IupVbox(
        IupLabel( "Write Mode:" ),
        named[65] /* lstWriteMode */,
        decl( 0, IupFill(), 
          "SIZE", "4", 0 ),
        decl( 0, IupFrame(
          IupVbox(
            IupLabel( "Back Opacity:" ),
            named[79] /* lstOpacity */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Fill Mode:" ),
            named[69] /* lstFillMode */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            decl( 0, IupFrame(
              named[82] /* rdInteriorStyle */
            ), 
              "TITLE", "Interior Style:", 
              "MARGIN", "5x5", 
              "SIZE", "80", 0 ),
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Hatch Style:" ),
            named[80] /* lstHatchStyle */,
          NULL)
        ), 
          "TITLE", "Fill Attributes", 
          "MARGIN", "5x5", 0 ),
        decl( 0, IupFill(), 
          "SIZE", "10", 0 ),
        decl( 0, IupFrame(
          IupVbox(
            IupLabel( "Mark Type:" ),
            named[64] /* lstMarkType */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Mark Size:" ),
            named[63] /* txtMarkSize */,
          NULL)
        ), 
          "TITLE", "Mark Attributes", 
          "MARGIN", "5x5", 0 ),
        decl( 0, IupFill(), 
          "SIZE", "10", 0 ),
        named[81] /* btAttributesHide */,
      NULL), 
        "GAP", "2", 0 ),
      decl( 0, IupVbox(
        decl( 0, IupFrame(
          IupVbox(
            IupLabel( "Font Typeface:" ),
            named[66] /* lstFontTypeFace */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Font Style:" ),
            named[70] /* lstFontStyle */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Font Size:" ),
            named[72] /* txtFontSize */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            decl( 0, IupLabel( "Text Alignment:" ), 
              "SIZE", "x9", 0 ),
            named[62] /* lstTextAlignment */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            decl( 0, IupLabel( "Text Orientation:" ), 
              "SIZE", "x9", 0 ),
            named[73] /* txtTextOrientation */,
          NULL)
        ), 
          "TITLE", "Text Attributes", 
          "MARGIN", "5x5", 0 ),
        decl( 0, IupFill(), 
          "SIZE", "10", 0 ),
        decl( 0, IupFrame(
          IupVbox(
            IupLabel( "Line Style:" ),
            named[74] /* lstLineStyle */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Line Width:" ),
            named[71] /* txtLineWidth */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Line Cap:" ),
            named[67] /* lstLineCap */,
            decl( 0, IupFill(), 
              "SIZE", "2", 0 ),
            IupLabel( "Line Join:" ),
            named[68] /* lstLineJoin */,
          NULL)
        ), 
          "TITLE", "Line Attributes", 
          "MARGIN", "5x5", 0 ),
      NULL), 
        "GAP", "2", 0 ),
    NULL),
  NULL), 0 );
  named[84] = decl( "dlgAttributes", IupDialog(
    named[83] /* AttributesDesktop */
  ), 
    "TITLE", "Attributes", 
    "PARENTDIALOG", "dlgMain", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdAttributesHide", 
    "MARGIN", "5x2", 0 );
  named[85] = decl( "txtLBX1", IupText( "cmdInteger" ), 
    "NC", "8", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[86] = decl( "txtLBX2", IupText( "cmdInteger" ), 
    "NC", "8", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[87] = decl( "txtLBY1", IupText( "cmdInteger" ), 
    "NC", "8", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[88] = decl( "txtLBY2", IupText( "cmdInteger" ), 
    "NC", "8", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[89] = decl( "btLBDraw", IupButton( "Draw", "cmdDraw" ), 
    "SIZE", "30", 0 );
  named[90] = decl( "btLBHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[91] = decl( "LBData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X1:" ), 
        "SIZE", "15x9", 0 ),
      named[85] /* txtLBX1 */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y1:" ), 
        "SIZE", "15x9", 0 ),
      named[87] /* txtLBY1 */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X2:" ), 
        "SIZE", "15x9", 0 ),
      named[86] /* txtLBX2 */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y2:" ), 
        "SIZE", "15x9", 0 ),
      named[88] /* txtLBY2 */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[92] = decl( "LBDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[91] /* LBData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[89] /* btLBDraw */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[90] /* btLBHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[93] = decl( "dlgLB", IupDialog(
    named[92] /* LBDeskTop */
  ), 
    "TITLE", "Line Parameters", 
    "PARENTDIALOG", "dlgMain", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "MARGIN", "5x5", 0 );
  named[94] = decl( "txtASXC", IupText( "cmdInteger" ), 
    "NC", "4", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[95] = decl( "txtASYC", IupText( "cmdInteger" ), 
    "NC", "4", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[96] = decl( "txtASW", IupText( "cmdInteger" ), 
    "NC", "4", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[97] = decl( "txtASH", IupText( "cmdInteger" ), 
    "NC", "4", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[98] = decl( "txtASAngle1", IupText( "cmdReal" ), 
    "NC", "10", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[99] = decl( "txtASAngle2", IupText( "cmdReal" ), 
    "NC", "10", 
    "SIZE", "30x12", 
    "VALUE", "360", 0 );
  named[100] = decl( "btASDraw", IupButton( "Draw", "cmdDraw" ), 
    "SIZE", "30", 0 );
  named[101] = decl( "btASHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[102] = decl( "ASData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Xc:" ), 
        "SIZE", "30x9", 0 ),
      named[94] /* txtASXC */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Yc:" ), 
        "SIZE", "30x9", 0 ),
      named[95] /* txtASYC */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "W:" ), 
        "SIZE", "30x9", 0 ),
      named[96] /* txtASW */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "H:" ), 
        "SIZE", "30x9", 0 ),
      named[97] /* txtASH */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Angle1:" ), 
        "SIZE", "30x9", 0 ),
      named[98] /* txtASAngle1 */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Angle2:" ), 
        "SIZE", "30x9", 0 ),
      named[99] /* txtASAngle2 */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[103] = decl( "ASDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[102] /* ASData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[100] /* btASDraw */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[101] /* btASHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[104] = decl( "dlgAS", IupDialog(
    named[103] /* ASDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Arc Parameters", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "MARGIN", "5x5", 0 );
  named[105] = decl( "txtPixelX", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[106] = decl( "txtPixelY", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[107] = decl( "btPixelDraw", IupButton( "Draw", "cmdDraw" ), 
    "SIZE", "30", 0 );
  named[108] = decl( "btPixelHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[109] = decl( "PixelData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X:" ), 
        "SIZE", "10x9", 0 ),
      named[105] /* txtPixelX */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y:" ), 
        "SIZE", "10x9", 0 ),
      named[106] /* txtPixelY */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[110] = decl( "PixelDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[109] /* PixelData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[107] /* btPixelDraw */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[108] /* btPixelHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[111] = decl( "dlgPixel", IupDialog(
    named[110] /* PixelDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Pixel Parameters", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "MARGIN", "5x5", 0 );
  named[112] = decl( "txtMarkX", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[113] = decl( "txtMarkY", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[114] = decl( "btMarkDraw", IupButton( "Draw", "cmdDraw" ), 
    "SIZE", "30", 0 );
  named[115] = decl( "btMarkHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[116] = decl( "MarkData", IupVbox(
    decl( 0, IupHbox(
      IupFill(),
      decl( 0, IupLabel( "X:" ), 
        "SIZE", "10x9", 0 ),
      named[112] /* txtMarkX */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y:" ), 
        "SIZE", "10x9", 0 ),
      named[113] /* txtMarkY */,
      IupFill(),
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[117] = decl( "MarkDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[116] /* MarkData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[114] /* btMarkDraw */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[115] /* btMarkHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[118] = decl( "dlgMark", IupDialog(
    named[117] /* MarkDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Mark Parameters", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "MARGIN", "5x5", 0 );
  named[119] = decl( "txtTextX", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[120] = decl( "txtTextY", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[121] = decl( "txtTextS", IupText( "cmdString" ), 
    "NC", "100", 
    "SIZE", "89x12", 
    "VALUE", "Text", 0 );
  named[122] = decl( "btTextDraw", IupButton( "Draw", "cmdDraw" ), 
    "SIZE", "30", 0 );
  named[123] = decl( "btTextHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[124] = decl( "TextData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X:" ), 
        "SIZE", "10x9", 0 ),
      named[119] /* txtTextX */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y:" ), 
        "SIZE", "10x9", 0 ),
      named[120] /* txtTextY */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    IupHbox(
      IupVbox(
        decl( 0, IupLabel( "Text:" ), 
          "SIZE", "25x9", 0 ),
        named[121] /* txtTextS */,
      NULL),
    NULL),
  NULL), 0 );
  named[125] = decl( "TextDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[124] /* TextData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[122] /* btTextDraw */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[123] /* btTextHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[126] = decl( "dlgText", IupDialog(
    named[125] /* TextDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Text Parameters", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "RESIZE", "NO", 
    "MARGIN", "5x5", 0 );
  named[127] = decl( "tgOpenLines", IupToggle( "CD_OPEN_LINES", "cmdOpenLines" ), 
    "VALUE", "ON", 0 );
  named[128] = decl( "tgClosedLines", IupToggle( "CD_CLOSED_LINES", "cmdClosedLines" ), 0 );
  named[129] = decl( "tgFilled", IupToggle( "CD_FILL", "cmdFill" ), 0 );
  named[130] = decl( "tgClip", IupToggle( "CD_CLIP", "cmdPolyClip" ), 0 );
  named[131] = decl( "tgBezier", IupToggle( "CD_BEZIER", "cmdPolyBezier" ), 0 );
  named[132] = decl( "btPolyHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[133] = decl( "rdMode", IupRadio(
    IupVbox(
      named[127] /* tgOpenLines */,
      named[128] /* tgClosedLines */,
      named[129] /* tgFilled */,
      named[130] /* tgClip */,
      named[131] /* tgBezier */,
    NULL)
  ), 0 );
  named[134] = decl( "PolyDesktop", IupVbox(
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
    IupHbox(
      IupFill(),
      decl( 0, IupFrame(
        IupHbox(
          decl( 0, IupFill(), 
            "SIZE", "5", 0 ),
          named[133] /* rdMode */,
        NULL)
      ), 
        "TITLE", "Polygon Mode:", 0 ),
      IupFill(),
    NULL),
    decl( 0, IupFill(), 
      "SIZE", "10", 0 ),
    IupHbox(
      IupFill(),
      named[132] /* btPolyHide */,
      IupFill(),
    NULL),
    decl( 0, IupFill(), 
      "SIZE", "5", 0 ),
  NULL), 0 );
  named[135] = decl( "dlgPoly", IupDialog(
    named[134] /* PolyDesktop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Polygon Parameters", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "CLOSE_CB", "cmdMsgHide", 0 );
  named[136] = decl( "txtClipXmin", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[137] = decl( "txtClipXmax", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[138] = decl( "txtClipYmin", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[139] = decl( "txtClipYmax", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[140] = decl( "btClipOff", IupButton( "Off", "cmdClipOff" ), 
    "SIZE", "30", 0 );
  named[141] = decl( "btClipArea", IupButton( "Area", "cmdClipArea" ), 
    "SIZE", "30", 0 );
  named[142] = decl( "btClipPoly", IupButton( "Polygon", "cmdClipPoly" ), 
    "SIZE", "50", 0 );
  named[143] = decl( "btClipHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[144] = decl( "ClipData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Xmin:" ), 
        "SIZE", "25x9", 0 ),
      named[136] /* txtClipXmin */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Ymin:" ), 
        "SIZE", "25x9", 0 ),
      named[138] /* txtClipYmin */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Xmax:" ), 
        "SIZE", "25x9", 0 ),
      named[137] /* txtClipXmax */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Ymax:" ), 
        "SIZE", "25x9", 0 ),
      named[139] /* txtClipYmax */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[145] = decl( "ClipDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[144] /* ClipData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[140] /* btClipOff */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[141] /* btClipArea */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[142] /* btClipPoly */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[143] /* btClipHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[146] = decl( "dlgClip", IupDialog(
    named[145] /* ClipDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Clipping", 
    "MAXBOX", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 
    "MARGIN", "5x5", 0 );
  named[147] = decl( "txtImageX", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[148] = decl( "txtImageY", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[149] = decl( "txtImageW", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[150] = decl( "txtImageH", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[151] = decl( "btImagePut", IupButton( "Put", "cmdImagePut" ), 
    "SIZE", "30", 0 );
  named[152] = decl( "btImageGet", IupButton( "Get", "cmdImageGet" ), 
    "SIZE", "30", 0 );
  named[153] = decl( "btImageHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[154] = decl( "ImageData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X:" ), 
        "SIZE", "30x9", 0 ),
      named[147] /* txtImageX */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y:" ), 
        "SIZE", "30x9", 0 ),
      named[148] /* txtImageY */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Width:" ), 
        "SIZE", "30x9", 0 ),
      named[149] /* txtImageW */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Height:" ), 
        "SIZE", "30x9", 0 ),
      named[150] /* txtImageH */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[155] = decl( "ImageDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[154] /* ImageData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[152] /* btImageGet */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[151] /* btImagePut */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[153] /* btImageHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[156] = decl( "dlgImage", IupDialog(
    named[155] /* ImageDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "Server Image", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "RESIZE", "NO", 
    "MARGIN", "5x5", 0 );
  named[157] = decl( "txtImageRGBX", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[158] = decl( "txtImageRGBY", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[159] = decl( "txtImageRGBW", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[160] = decl( "txtImageRGBH", IupText( "cmdInteger" ), 
    "NC", "5", 
    "SIZE", "30x12", 
    "VALUE", "0", 0 );
  named[161] = decl( "btImageRGBPut", IupButton( "Put", "cmdImageRGBPut" ), 
    "SIZE", "30", 0 );
  named[162] = decl( "btImageRGBGet", IupButton( "Get", "cmdImageRGBGet" ), 
    "SIZE", "30", 0 );
  named[163] = decl( "btImageRGBHide", IupButton( "Hide", "cmdMsgHide" ), 
    "SIZE", "30", 0 );
  named[164] = decl( "ImageRGBData", IupVbox(
    decl( 0, IupHbox(
      decl( 0, IupLabel( "X:" ), 
        "SIZE", "30x9", 0 ),
      named[157] /* txtImageRGBX */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Y:" ), 
        "SIZE", "30x9", 0 ),
      named[158] /* txtImageRGBY */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    decl( 0, IupHbox(
      decl( 0, IupLabel( "Width:" ), 
        "SIZE", "30x9", 0 ),
      named[159] /* txtImageRGBW */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      decl( 0, IupLabel( "Height:" ), 
        "SIZE", "30x9", 0 ),
      named[160] /* txtImageRGBH */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
  NULL), 0 );
  named[165] = decl( "ImageRGBDeskTop", IupVbox(
    IupHbox(
      IupFill(),
      named[164] /* ImageRGBData */,
      IupFill(),
    NULL),
    IupHbox(
      IupFill(),
      named[162] /* btImageRGBGet */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[161] /* btImageRGBPut */,
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      named[163] /* btImageRGBHide */,
      IupFill(),
    NULL),
  NULL), 0 );
  named[166] = decl( "dlgImageRGB", IupDialog(
    named[165] /* ImageRGBDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "RGB Image", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "CLOSE_CB", "cmdMsgHide", 
    "RESIZE", "NO", 
    "MARGIN", "5x5", 0 );
  named[167] = decl( "btCDTest", IupButton( "Close", "cmdCloseAbout" ), 0 );
  named[168] = decl( "AboutDeskTop", IupHbox(
    IupFill(),
    decl( 0, IupVbox(
      IupLabel( "CD Test 5.3" ),
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      IupLabel( "Antonio Scuri" ),
      IupLabel( "Diego Nehab" ),
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      IupLabel( "Tecgraf/PUC-Rio" ),
      decl( 0, IupFill(), 
        "SIZE", "5", 0 ),
      IupLabel( "CD Library Version" ),
      named[169] = decl( "lblVersion", IupLabel( "" ), 0 ),
      named[167] /* btCDTest */,
    NULL), 
      "ALIGNMENT", "ACENTER", 0 ),
    IupFill(),
  NULL), 
    "MARGIN", "5x5", 
    "GAP", "2", 0 );
  named[170] = decl( "dlgHelpAbout", IupDialog(
    named[168] /* AboutDeskTop */
  ), 
    "PARENTDIALOG", "dlgMain", 
    "TITLE", "About", 
    "MAXBOX", "NO", 
    "MINBOX", "NO", 
    "RESIZE", "NO", 0 );
}
