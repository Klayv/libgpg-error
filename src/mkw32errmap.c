/* mkw32errmap.c - Generate mapping sources for Windows.
   Copyright (C) 2010 g10 Code GmbH

   This file is part of libgpg-error.

   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef RESOLVE_MACROS
# include <winerror.h>
#endif
#include <stdio.h>
#include <stdlib.h>

static const char header_gpg_extra_errno_h[] =
  "/* errno.h - WindowsCE errno.h substitute\n"
  "   Copyright (C) 2010 g10 Code GmbH\n"
  "\n"
  "   This file is free software; as a special exception the author gives\n"
  "   unlimited permission to copy and/or distribute it, with or without\n"
  "   modifications, as long as this notice is preserved.\n"
  "\n"
  "   This file is distributed in the hope that it will be useful, but\n"
  "   WITHOUT ANY WARRANTY, to the extent permitted by law; without even\n"
  "   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR\n"
  "   PURPOSE.\n"
  "\n"
  "   +++ Do not edit!  File has been generated by mkw32errmap.c +++\n"
  "\n"
  "   This file is intended to be used with ming32ce-gcc to implement an\n"
  "   errno substitute under WindowsCE.  It must be included via gcc's\n"
  "   -idirafter option.  The gpg-error-config script emits the\n"
  "   appropriate option snippet.  The actual implementation of the errno\n"
  "   related functions are part of libgpg-error.  A separate header file\n"
  "   is required because errno.h is often included before gpg-error.h.\n"
  " */\n"
  "\n"
  "#ifndef _GPG_ERROR_EXTRA_ERRNO_H\n"
  "#define _GPG_ERROR_EXTRA_ERRNO_H\n"
  "\n"
  "/* Due to peculiarities in W32 we can't implement ERRNO as an\n"
  "   writable lvalue.  This also allows us to easily find places\n"
  "   where ERRNO is being written to.  See also gpg_err_set_errno.  */\n"
  "int _gpg_w32ce_get_errno (void);\n"
  "#define errno (_gpg_w32ce_get_errno ())\n"
  "\n";
static const char footer_gpg_extra_errno_h[] =
  "\n"
  "#endif /*_GPG_ERROR_EXTRA_ERRNO_H*/\n";


/* The table below is used in two modes.  First we run the host
   preprocessor over it to generate a new include file.  This include
   file has the same content but the Windows error macros are
   resolved.  This is so that we don't need to include winerror.h into
   the generated errno.h.  The mkw32errmap_marker variable is only
   here to have something to grep for after preprocessing.  */
static int mkw32errmap_marker;
struct table_s
{
  int *dummy;
  const char *name;
  int w32code;
};

struct table_s table[] = 
  {
#ifdef RESOLVE_MACROS
#define X(a,b) \
    {&mkw32errmap_marker, (a), (b)}
   X( "EPERM",	ERROR_CANNOT_MAKE                 ), 
   X( "ENOENT",	ERROR_FILE_NOT_FOUND              ),
   X( "EINTR",	ERROR_INVALID_AT_INTERRUPT_TIME   ),
   X( "EIO",	ERROR_IO_DEVICE                   ),
   X( "ENXIO",	ERROR_FILE_INVALID                ),
   X( "EBADF",	ERROR_INVALID_HANDLE              ),
   X( "EAGAIN",	ERROR_MORE_DATA                   ),
   /* FIXME:  Add WSAEWOULDBLOCK */

   X( "ENOMEM",	ERROR_NOT_ENOUGH_MEMORY           ),
   X( "EACCES",	ERROR_ACCESS_DENIED               ),
   X( "EFAULT",	ERROR_PROCESS_ABORTED             ),
   X( "EBUSY",	ERROR_BUSY                        ),
   X( "EEXIST",	ERROR_FILE_EXISTS                 ),
   /* FIXME: Add WSAEADDRINUSE.  */

   X( "EXDEV",	ERROR_NOT_SAME_DEVICE             ),
   X( "ENODEV",	ERROR_BAD_DEVICE                  ),
   /* FIXME: ERROR_DEV_NOT_EXIST */

   X( "ENOTDIR",ERROR_DIRECTORY                   ),
   X( "EINVAL",	ERROR_INVALID_PARAMETER           ),
   X( "ENFILE",	ERROR_NO_MORE_FILES               ),
   X( "EMFILE",	ERROR_TOO_MANY_OPEN_FILES         ),
   X( "ENOSPC",	ERROR_DISK_FULL                   ),
   X( "EROFS",	ERROR_WRITE_PROTECT               ),
   X( "EPIPE",	ERROR_BROKEN_PIPE                 ),
   X( "ERANGE",	ERROR_ARITHMETIC_OVERFLOW         ),
   X( "EDEADLOCK",ERROR_POSSIBLE_DEADLOCK         ),
   X( "ENAMETOOLONG", ERROR_FILENAME_EXCED_RANGE  ),
   X( "ENOLCK",	ERROR_SHARING_BUFFER_EXCEEDED     ),
   X( "ENOSYS",	ERROR_NOT_SUPPORTED               ),
   X( "ENOTEMPTY",ERROR_DIR_NOT_EMPTY             ),
   X( "ESPIPE",  ERROR_SEEK_ON_DEVICE             ),
#if 0 /* FIXME: Find appropriate mappings.  */
   X( "EILSEQ",		), 
   X( "EDOM",		), 
   X( "EMLINK",		), 
   X( "ESRCH",		), /* No such process */
   X( "E2BIG",		), /* Arg list too long */
   X( "ENOEXEC",	), /* Exec format error */
   X( "ECHILD",		), /* No child processes */
   X( "EISDIR",		), /* Is a directory */
   X( "ENOTTY",		), /* Inappropriate I/O control operation */
   X( "EFBIG",		), /* File too large */
#endif
#undef X
#else /*!RESOLVE_MACROS*/
# include "mkw32errmap.tab.h"   
#endif /*!RESOLVE_MACROS*/
   { NULL, 0 }
  };



static int
compare_table (const void *a_v, const void *b_v)
{
  const struct table_s *a = a_v;
  const struct table_s *b = b_v;

  return (a->w32code - b->w32code);
}     


int
main (int argc, char **argv)
{
  int idx;

  for (idx=0; table[idx].name; idx++)
    ;
  qsort (table, idx, sizeof *table, compare_table);

  fputs (header_gpg_extra_errno_h, stdout);
  for (idx=0; table[idx].name; idx++)
    printf ("#define %-12s %5d\n", table[idx].name, table[idx].w32code);
  fputs (footer_gpg_extra_errno_h, stdout);

  return 0;
}
