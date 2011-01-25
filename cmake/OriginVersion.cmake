# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

## Parse a version string, creating a number of variables describing its
## contents.
##
##   parse_vsersion(prefix version)
##
## where prefix is the prefix of the output variables and version is the version
## string (which need not be quoted). The version string must be of the form
## a.b.c where a, b, and c are the major version, minor version, and patch
## level, respectively.
##
## For example, writing parse_version(X a.b.c) will result in the variables
## X_VERSION (a.b.c), X_VERSION_MAJOR (a), # X_VERSION_MINOR (b), and
## X_VERSION_PATCH (c).
macro(parse_version prefix version)
  string(REPLACE "." " " space ${version})
  separate_arguments(nums UNIX_COMMAND ${space})
  list(GET nums 0 ${prefix}_VERSION_MAJOR)
  list(GET nums 1 ${prefix}_VERSION_MINOR)
  list(GET nums 2 ${prefix}_VERSION_PATCH)
  set(${prefix}_VERSION ${version})
endmacro()
