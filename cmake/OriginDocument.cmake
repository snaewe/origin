# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# This build module supports the generation of reference documentation using
# doxygen (or possibly a branched version supporting 0x syntax).

find_package(Doxygen REQUIRED)

# This variable maintains the list documentaiton directives that will be used
# to document the library.
set(ORIGIN_DOC_LIST)

# Documentation for Origin works pretty simply... There are a number of aggregating
# operations (origin_document_[file|dir|module]) that collect header files
# within a project. These cache the gathered documents in the ORIGIN_DOC_LIST
# variable, which is accessible on a per-project basis. Notet that the input to
# all of the origin_document_* modules are given relative to the include/origin
# directory (e.g., utility.hpp not origin/utility.hpp).

# Document individual files
macro(origin_document_header)
    foreach(file ${ARGV})
        list(APPEND ORIGIN_DOC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/origin/${file}")
    endforeach(file ${ARGV})
endmacro(origin_document_header)

# Recursively aggregate all headers in the given directories.
macro(origin_document_dir)
    foreach(dir ${ARGV})
        file(GLOB_RECURSE files "${CMAKE_CURRENT_SOURCE_DIR}/include/origin/${dir}/*.hpp")
        list(APPEND ORIGIN_DOC_LIST ${files})
    endforeach(dir ${ARGV})
endmacro(origin_document_dir)

# Aggregate the headers in the given module. A module is always defined by
# an aggregate header (e.g., origin/concepts.hpp) and its corresponding
# directory (e.g., origin/concepts). Headers from the directory are gathered
# recursively.
macro(origin_document_module)
    set(mod ${ARGV0})
    origin_document_header(${mod}.hpp)
    origin_document_dir(${mod})
endmacro(origin_document_module)

# Generate the build commands that will actually run Doxygen and generate the
# library docs.
macro(origin_build_documents)
    # Define and generate the Doxyfile. We generate this every time just in
    # case the caller wants to change the parameters. No big deal.
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    execute_process(
        COMMAND ${DOXYGEN_EXECUTABLE} -s -g ${doxyfile}
        OUTPUT_QUIET
        ERROR_QUIET)

    # Override the output generation
    file(APPEND ${doxyfile} "EXTRACT_ALL = YES\n")      # Force redocumentaiton for overrides
    file(APPEND ${doxyfile} "INHERIT_DOCS = NO\n")      # Force redocumentaiton for overrides
    file(APPEND ${doxyfile} "CLASS_GRAPH = NO\n")       # Not applicable for generic programs
    file(APPEND ${doxyfile} "SHOW_DIRECTORIES = YES\n") # TODO: Maybe?
    file(APPEND ${doxyfile} "STRIP_FROM_PATH = ${CMAKE_CURRENT_SOURCE_DIR}/include\n")     # Paths relative to some root.
    file(APPEND ${doxyfile} "GENERATE_HTML = YES\n")
    file(APPEND ${doxyfile} "GENERATE_HTML = YES\n")
    file(APPEND ${doxyfile} "OUTPUT_DIRECTORY = ${CMAKE_CURRENT_BINARY_DIR}/doc\n")
    file(APPEND ${doxyfile} "GENERATE_HTML = YES\n")
    file(APPEND ${doxyfile} "GENERATE_XML = YES\n")
    file(APPEND ${doxyfile} "GENERATE_LATEX = NO\n")
    # file(APPEND ${doxyfile} "QUIET = YES\n")

    # Build an input string based on the values given to the macro. Note that
    # the input values should be listed /relative/ to the top-level project
    # level source directory.
    string(REPLACE ";" " " out "${ORIGIN_DOC_LIST}")
    file(APPEND ${doxyfile} "INPUT = ${out}\n")

    # Add a command for actually building the doxygen documentation. This will
    # not build by default, so docs should be updated from the build separately.
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile})
endmacro(origin_build_documents)

# NOTE
# The generation and integration of reference documentation above is roughly
# one fourth of the total process. We need to transform the Doxygen XML into
# a BoostBook XML via a couple of XSL transforms.

# Also, we need to generate Quickbook documentation - this is actually pretty
# easy to do since all we have to do is run quickbook and store the results.
# The results of quickbook'ing documentation is BoostBook XML.

# The final step weaves together the BoostBook'd manual and reference and
# then styles them into HTML, PDF, etc according to the BoostBook stylesheets.

