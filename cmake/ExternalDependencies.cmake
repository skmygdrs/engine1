include(ExternalProject)
project(ExternalDependencies)
##################################
## EIGEN                        ##
## Used for general matrix math ##
##################################

message( STATUS "External project - Eigen" )
set(Eigen_VERSION "3.3.3" )
set(Eigen_DIR "${CMAKE_BINARY_DIR}/Eigen/src/Eigen")
set(Eigen_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/eigen/install")

ExternalProject_Add( Eigen
  PREFIX Eigen
  URL "http://bitbucket.org/eigen/eigen/get/${Eigen_VERSION}.tar.gz"
  URL_HASH MD5=f21cee193e15e55cfd15ebbc16fc00a7
  INSTALL_DIR "${Eigen_INSTALL}"
  CMAKE_ARGS
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${Eigen_INSTALL}
        -DINCLUDE_INSTALL_DIR:STRING=${Eigen_INSTALL}/include
)
list(APPEND BioGears_DEPENDENCIES Eigen)
# Install Headers
install(DIRECTORY ${Eigen_INSTALL}/include
        DESTINATION ${INSTALL_INC})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/eigen/install)
message(STATUS "Eigen is here : ${Eigen_DIR}" )

###################################################
## LOG4CPP                                       ##
## General logging utility                       ##
## We have our own cmake file to build as dll/so ##
###################################################

message( STATUS "External project - Log4cpp" )
set(log4cpp_VERSION "1.1.2" )
set(log4cpp_DIR "${CMAKE_BINARY_DIR}/log4cpp/src/log4cpp")
set(log4cpp_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/log4cpp/install")

if(UNIX)
  set(CONFIGURE "./configure")
endif()
		
ExternalProject_Add( log4cpp
  PREFIX log4cpp
  URL "https://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.2.tar.gz"
  URL_HASH MD5=c70eac7334e2f3cbeac307dc78532be4
  UPDATE_COMMAND 
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/log4cpp.cmake ${log4cpp_DIR}/CMakeLists.txt
    COMMAND ${CONFIGURE}
  INSTALL_DIR "${log4cpp_INSTALL}"
  CMAKE_ARGS
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${log4cpp_INSTALL}
        -DINCLUDE_INSTALL_DIR:STRING=${log4cpp_INSTALL}/include
)
list(APPEND BioGears_DEPENDENCIES log4cpp)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/log4cpp/install)
# You should only be building the release versions of these libs
# This will copy those release versions to all supported configuration locations
# If you need to debug into these libraries, you can, just know that the release
# libs will be replaced with debug versions. 

# Install Headers
install(DIRECTORY ${log4cpp_INSTALL}/include
        DESTINATION ${INSTALL_INC})
# Install Bin
if(WIN32)
  set(L4C_DLL ${log4cpp_INSTALL}/bin/log4cpp.dll)
else()
  set(L4C_DLL ${log4cpp_INSTALL}/lib/liblog4cpp.so)
endif()

install(FILES ${L4C_DLL}
    DESTINATION ${INSTALL_BIN}/release${EX_CONFIG})
install(FILES ${L4C_DLL}
    DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG})
install(FILES ${L4C_DLL}
    DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG})
# Install Libs
if(WIN32)
  set(L4C_LIB ${log4cpp_INSTALL}/lib/log4cpp.lib)
else()
  set(L4C_LIB ${log4cpp_INSTALL}/lib/liblog4cpp.so)
endif()
install(FILES ${L4C_LIB}
  DESTINATION ${INSTALL_LIB}/release${EX_CONFIG})
install(FILES ${L4C_LIB}
  DESTINATION ${INSTALL_LIB}/debug${EX_CONFIG})
install(FILES ${L4C_LIB}
  DESTINATION ${INSTALL_LIB}/relwithdebinfo${EX_CONFIG})

message(STATUS "log4cpp is here : ${log4cpp_DIR}" )

################################
## Code Synthesis XSD         ##
## XSD to C++ Binding Classes ##
################################

message( STATUS "External project - XSD" )
set(xsd_VERSION "4.0.0" )
set(xsd_DIR "${CMAKE_BINARY_DIR}/xsd/src/xsd")
set(xsd_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/xsd/install")

if(MSVC)
  set(xsd_URL "http://www.codesynthesis.com/download/xsd/4.0/windows/i686/xsd-4.0.0-i686-windows.zip")
  set(xsd_HASH "d129469e109784c663387ca8bee5ac627434cfca")
elseif(APPLE)
  set(xsd_URL "http://www.codesynthesis.com/download/xsd/4.0/macosx/i686/xsd-4.0.0-i686-macosx.tar.bz2")
  set(xsd_HASH "bb96454da6acafb93180368220d555e2b9747023")
elseif(UNIX)
  set(xsd_URL "http://www.codesynthesis.com/download/xsd/4.0/linux-gnu/x86_64/xsd-4.0.0-x86_64-linux-gnu.tar.bz2")
  set(xsd_HASH "5eeb2eeca0d893949e3677bb374e7b96f19770d6")
endif()
		
ExternalProject_Add( xsd
  PREFIX xsd
  URL ${xsd_URL}
  URL_HASH SHA1=${xsd_HASH}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND 
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${xsd_DIR}/bin ${xsd_INSTALL}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${xsd_DIR}/libxsd ${xsd_INSTALL}/libxsd

)
list(APPEND BioGears_DEPENDENCIES xsd)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/xsd/install)
message(STATUS "xsd is here : ${xsd_DIR}" )

# Install Headers
install(DIRECTORY ${xsd_INSTALL}/libxsd/xsd
        DESTINATION ${INSTALL_INC}/include)

###############################
## XERCES                    ##
## XML Serialization support ##
###############################
## NOTE : This is using a branch that supports CMake    ##
## Hopefully this branch will be merged to main in 2017 ##

message( STATUS "External project - XERCES" )
if(WIN32)
  set(xerces_TRANSCODER "windows")
else()
  set(xerces_TRANSCODER "iconv")
endif()

set(xerces_VERSION "3.1.x" )
set(xerces_DIR "${CMAKE_BINARY_DIR}/xerces/src/xerces")
set(xerces_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/xerces/install")
ExternalProject_Add( xerces
  PREFIX xerces
  GIT_REPOSITORY "https://github.com/rleigh-codelibre/xerces-c.git"
  GIT_TAG "cmake-trunk"
  INSTALL_DIR "${xerces_INSTALL}"
  CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=ON
        -Dnetwork:BOOL=OFF
        -Dtranscoder:STRING=${xerces_TRANSCODER}
        -DCMAKE_INSTALL_PREFIX:STRING=${xerces_INSTALL}
        -DINCLUDE_INSTALL_DIR:STRING=${xerces_INSTALL}/include
)
list(APPEND BioGears_DEPENDENCIES xerces)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/xerces/install)
# Install Headers
install(DIRECTORY ${xerces_INSTALL}/include
        DESTINATION ${INSTALL_INC})
# Install Binaries
if(WIN32)
  set(X_DLL ${xerces_INSTALL}/bin/xerces-c.dll)
  set(X_DLL_output xerces-c.dll)
else()
  set(X_DLL ${xerces_INSTALL}/lib/libxerces-c.so.3.1)
  set(X_DLL_output libxerces-c.so)
endif()
install(FILES ${X_DLL}
  DESTINATION ${INSTALL_BIN}/release${EX_CONFIG} RENAME ${X_DLL_output})
install(FILES ${X_DLL}
  DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG} RENAME ${X_DLL_output})
install(FILES ${X_DLL}
  DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG} RENAME ${X_DLL_output})
# Install Libs
if(WIN32)
  set(X_LIB ${xerces_INSTALL}/lib/xerces-c.lib)
else()
  set(X_LIB ${xerces_INSTALL}/lib/libxerces-c.so
            ${xerces_INSTALL}/lib/libxerces-c.so.3.1)
endif()
install(FILES ${X_LIB}
  DESTINATION ${INSTALL_LIB}/release${EX_CONFIG})
install(FILES ${X_LIB}
  DESTINATION ${INSTALL_LIB}/debug${EX_CONFIG})
install(FILES ${X_LIB}
  DESTINATION ${INSTALL_LIB}/relwithdebinfo${EX_CONFIG})

message(STATUS "xerces is here : ${xerces_DIR}" )
if(WIN32)
  ##########################################
  ## DIRENT                               ##
  ## Platform generic file system utility,##
  ## that is not included in msvc/mingw   ##
  ##########################################

  message( STATUS "External project - dirent" )
  set(dirent_DIR "${CMAKE_BINARY_DIR}/dirent/src/dirent")
  set(dirent_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/dirent/install")
  ExternalProject_Add( dirent
    PREFIX dirent
    GIT_REPOSITORY "https://github.com/tronkko/dirent.git"
    GIT_SHALLOW 1
    INSTALL_DIR "${dirent_INSTALL}"
    CMAKE_ARGS
          -DBUILD_SHARED_LIBS:BOOL=OFF
          -DCMAKE_INSTALL_PREFIX:STRING=${dirent_INSTALL}
          -DINCLUDE_INSTALL_DIR:STRING=${dirent_INSTALL}/include
  )
  message(STATUS "dirent is here : ${dirent_DIR}" )
  list(APPEND BioGears_DEPENDENCIES dirent)
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/dirent/install)
  # Install Headers
  install(DIRECTORY ${dirent_INSTALL}/include
          DESTINATION ${INSTALL_INC})
endif()

# Support ccache
if( CMAKE_VERSION VERSION_LESS 3.4 )
  set( CMAKE_CXX_COMPILER_LAUNCHER_FLAG )
  set( CMAKE_C_COMPILER_LAUNCHER_FLAG )
else()
  set( CMAKE_CXX_COMPILER_LAUNCHER_FLAG
    -DCMAKE_CXX_COMPILER_LAUNCHER:FILEPATH=${CMAKE_CXX_COMPILER_LAUNCHER} )
  set( CMAKE_C_COMPILER_LAUNCHER_FLAG
    -DCMAKE_C_COMPILER_LAUNCHER:FILEPATH=${CMAKE_C_COMPILER_LAUNCHER} )
endif()

# ExternalProject_Add doesn't like to work with lists: it keeps only the first element
string(REPLACE ";" "::" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

# Generate the BioGears project after dependencies have been built
ExternalProject_Add( SetupBioGears
    DEPENDS Eigen ${BioGears_DEPENDENCIES}
    DOWNLOAD_COMMAND ""
    DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}/SetupBioGears-build
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    BUILD_AWAYS 1
    LIST_SEPARATOR ::
    CMAKE_ARGS
          -DSUPERBUILD:BOOL=OFF
          -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
          -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
          -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
          -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
          -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
          -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
          ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
          ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
          -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
          -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
          -DBUILD_SHARED_LIBS:BOOL=${shared}
          -DBUILD_TESTING:BOOL=${BUILD_TESTING}
          -DMAKECOMMAND:STRING=${MAKECOMMAND}
  )
