/*
 * Copyright (C) Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 *
 * You must obey the GNU Affero General Public License in all respects
 * for all of the code used other than OpenSSL.  
 */

#include "config.h"

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "protocol.h"

/**
 *  Check the server for greeting code +OK, then send QUIT and check
 *  for code +OK. If alive return TRUE, else, return FALSE.
 *
 *  @author Jan-Henrik Haukeland, <hauk@tildeslash.com>
 *
 *  @file
 */
int check_pop(Socket_T s) {

  char buf[STRLEN];
  const char *ok= "+OK";

  ASSERT(s);
  
  if(!socket_readln(s, buf, sizeof(buf))) {
    LogError("POP: error receiving data -- %s\n", STRERROR);
    return FALSE;
  }

  Util_chomp(buf);
  
  if(strncasecmp(buf, ok, strlen(ok)) != 0) {
    LogError("POP error: %s\n", buf);
    return FALSE;
  }
  
  if(socket_print(s, "QUIT\r\n") < 0) {
    LogError("POP: error sending data -- %s\n", STRERROR);
    return FALSE;
  }

  if(!socket_readln(s, buf, sizeof(buf))) {
    LogError("POP: error receiving data -- %s\n", STRERROR);
    return FALSE;
  }

  Util_chomp(buf);
  
  if(strncasecmp(buf, ok, strlen(ok)) != 0) {
    LogError("POP error: %s\n", buf);
    return FALSE;
  }

  return TRUE;
  
}

