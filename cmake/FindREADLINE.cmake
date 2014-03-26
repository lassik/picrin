# This file used to be a part of LuaJit project.

# ===============================================================================
# Picrin Scheme -- a lightweight scheme implementation.

# Copyright (C) 2014 Yuichi Nishiwaki. All rights reserved.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# [ MIT license: http://www.opensource.org/licenses/mit-license.php ]

# ===============================================================================
# LuaJIT -- a Just-In-Time Compiler for Lua. http://luajit.org/

# Copyright (C) 2005-2013 Mike Pall. All rights reserved.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# [ MIT license: http://www.opensource.org/licenses/mit-license.php ]

# ===============================================================================
# [ LuaJIT includes code from Lua 5.1/5.2, which has this license statement: ]

# Copyright (C) 1994-2012 Lua.org, PUC-Rio.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# ===============================================================================
# [ LuaJIT includes code from dlmalloc, which has this license statement: ]

# This is a version (aka dlmalloc) of malloc/free/realloc written by
# Doug Lea and released to the public domain, as explained at
# http://creativecommons.org/licenses/publicdomain

# ===============================================================================



# - Try to find Readline
# Once done this will define
#  READLINE_FOUND - System has readline
#  READLINE_INCLUDE_DIRS - The readline include directories
#  READLINE_LIBRARIES - The libraries needed to use readline
#  READLINE_DEFINITIONS - Compiler switches required for using readline

find_package ( PkgConfig )
pkg_check_modules ( PC_READLINE QUIET readline )
set ( READLINE_DEFINITIONS ${PC_READLINE_CFLAGS_OTHER} )

find_path ( READLINE_INCLUDE_DIR readline/readline.h
      HINTS ${PC_READLINE_INCLUDEDIR} ${PC_READLINE_INCLUDE_DIRS}
      PATH_SUFFIXES readline )

find_library ( READLINE_LIBRARY NAMES readline
      HINTS ${PC_READLINE_LIBDIR} ${PC_READLINE_LIBRARY_DIRS} )

set ( READLINE_LIBRARIES ${READLINE_LIBRARY} )
set ( READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR} )

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set READLINE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( readline DEFAULT_MSG READLINE_LIBRARY READLINE_INCLUDE_DIR )

mark_as_advanced ( READLINE_INCLUDE_DIR READLINE_LIBRARY )
