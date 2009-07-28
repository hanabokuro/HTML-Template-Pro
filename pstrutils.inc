/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id$
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pbuffer.h"
#include "pstring.h"

static
PSTRING
lowercase_pstring (pbuffer* pBuffer ,PSTRING pstring) {
  size_t size=pstring.endnext-pstring.begin;
  char* buf=pbuffer_resize(pBuffer, size+1);
  char* inbuf=buf;
  char* i=pstring.begin;
  PSTRING retval;
  while (i<pstring.endnext) {
    *inbuf++=tolower(*i++);
  }
  *inbuf=0;
  retval.begin=buf;
  retval.endnext=buf+size;
  return retval;
}

static
int
is_pstring_true (PSTRING s) {
  if (s.begin == NULL || s.begin == s.endnext) return 0;
  if (1==s.endnext-s.begin) {
    if (*(s.begin)=='0') return 0; else return 1;
  } else if (3==s.endnext-s.begin) {
    if ('0'==*(s.begin) && '.'==*(s.begin+1) && '0'==*(s.begin+2)) return 0; else return 1;
  } else return 1;
}
