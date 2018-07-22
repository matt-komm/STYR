set(searchpath $ENV{DELPHES_DIR})

find_library(DELPHES_LIBRARY
              NAMES Delphes delphes
              HINTS ${searchpath}
              PATH_SUFFIXES lib)

find_path(DELPHES_INCLUDE_DIR
           NAMES classes/DelphesClasses.h modules/Delphes.h
           HINTS ${searchpath}
           PATH_SUFFIXES include)

unset(searchpath)

set(DELPHES_INCLUDE_DIRS ${DELPHES_INCLUDE_DIR})
set(DELPHES_LIBRARIES ${DELPHES_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Delphes DEFAULT_MSG DELPHES_INCLUDE_DIR DELPHES_LIBRARY)

mark_as_advanced(DELPHES_INCLUDE_DIR DELPHES_LIBRARY)

