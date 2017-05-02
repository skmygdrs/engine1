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

# Install Headers
install(DIRECTORY ${EIGEN3_INCLUDE_DIR}
        DESTINATION ${INSTALL_INC})

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

# You should only be building the release versions of these libs
# This will copy those release versions to all supported configuration locations
# If you need to debug into these libraries, you can, just know that the release
# libs will be replaced with debug versions. 

# Install Headers
install(DIRECTORY ${LOG4CPP_INCLUDE_DIR}
        DESTINATION ${INSTALL_INC})
# Install Bin
file(GLOB L4C_BIN 
  ${log4cpp_INSTALL}/bin/*.dll)
install(FILES ${L4C_BIN}
  DESTINATION ${INSTALL_BIN}/release${EX_CONFIG})
install(FILES ${L4C_BIN}
  DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG})
install(FILES ${L4C_BIN}
  DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG})
# Install Libs
file(GLOB L4C_LIB 
  ${log4cpp_INSTALL}/lib/*.lib
  ${log4cpp_INSTALL}/lib/*.so)
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
  set(xerces_TRANSCODER "icu")
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
        -Dtranscoder:STRING=${xerces_TRANSCODER}
        -DCMAKE_INSTALL_PREFIX:STRING=${xerces_INSTALL}
        -DINCLUDE_INSTALL_DIR:STRING=${xerces_INSTALL}/include
)

# Install Headers
install(DIRECTORY ${XercesC_INCLUDE_DIR}
        DESTINATION ${INSTALL_INC})
# Install Bin
file(GLOB X_BIN 
  ${xerces_INSTALL}/bin/*.dll)
install(FILES ${X_BIN}
  DESTINATION ${INSTALL_BIN}/release${EX_CONFIG})
install(FILES ${X_BIN}
  DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG})
install(FILES ${X_BIN}
  DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG})
# Install Libs
file(GLOB X_LIB 
  ${xerces_INSTALL}/lib/*.lib
  ${xerces_INSTALL}/lib/*.so)
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


# Install Headers
install(DIRECTORY ${DIRENT_INCLUDE_DIR}
        DESTINATION ${INSTALL_INC})
endif()

## Generate the BioGears project after dependencies have been built
#ExternalProject_Add( SetupBioGears
#    PREFIX SetupBioGears
#    DEPENDS Eigen log4cpp xsd xerces dirent
#    SOURCE_DIR ${CMAKE_SOURCE_DIR}
#    BINARY_DIR ${CMAKE_BINARY_DIR}
#    CMAKE_ARGS
#          -DSUPERBUILD:BOOL=OFF
#  )

  ## Generate the BioGears project after dependencies have been built
#add_custom_target(SetupBioGears
#                  DEPENDS eigen log4cpp xsd xerces dirent)
#add_custom_command(TARGET SetupBioGears PRE_BUILD
#  COMMAND cmake -DSUPERBUILD:BOOL=OFF  ${CMAKE_SOURCE_DIR}/CMakeLists.txt
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/BioGearz)
#message(STATUS "Where I want to put BG build : ${CMAKE_BINARY_DIR}")
#add_custom_command(TARGET MakeBioGears POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -DConfig:STRING=GenData -P ${CMAKE_SOURCE_DIR}/cmake/devScripts.cmake)