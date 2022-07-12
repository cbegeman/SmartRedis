project(SmartRedis)

cmake_minimum_required(VERSION 3.10)
enable_language(Fortran)

option(BUILD_PYTHON "Build the python module" ON)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 19)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
message("Fortran_COMPILER_NAME = ${Fortran_COMPILER_NAME}")

add_link_options(-lpthread)

add_compile_options(-fvisibility=default)
add_compile_options(-g -O0)

if (WERROR)
    add_compile_options(-Wall -Werror)
endif()

find_library(REDISPP redis++ PATHS ${CMAKE_SOURCE_DIR}/install/lib NO_DEFAULT_PATH REQUIRED)
find_library(HIREDIS hiredis PATHS ${CMAKE_SOURCE_DIR}/install/lib NO_DEFAULT_PATH REQUIRED)

set(EXT_CLIENT_LIBRARIES ${REDISPP} ${HIREDIS})

set(CLIENT_SRC
    src/c/c_client.cpp
    src/c/c_dataset.cpp
    src/c/c_error.cpp
    src/cpp/client.cpp
    src/cpp/dataset.cpp
    src/cpp/command.cpp
    src/cpp/keyedcommand.cpp
    src/cpp/nonkeyedcommand.cpp
    src/cpp/multikeycommand.cpp
    src/cpp/singlekeycommand.cpp
    src/cpp/compoundcommand.cpp
    src/cpp/addressatcommand.cpp
    src/cpp/addressanycommand.cpp
    src/cpp/clusterinfocommand.cpp
    src/cpp/dbinfocommand.cpp
    src/cpp/gettensorcommand.cpp
    src/cpp/commandlist.cpp
    src/cpp/metadata.cpp
    src/cpp/tensorbase.cpp
    src/cpp/tensorpack.cpp
    src/cpp/dbnode.cpp
    src/cpp/commandreply.cpp
    src/cpp/redisserver.cpp
    src/cpp/rediscluster.cpp
    src/cpp/redis.cpp
    src/cpp/metadatafield.cpp
    src/cpp/stringfield.cpp
)
set(FORTRAN_CLIENT_SRC
    ${CLIENT_SRC}
    src/fortran/fortran_c_interop.F90
    src/fortran/dataset.F90
    src/fortran/client.F90
)

include_directories(SYSTEM
    include
    install/include
    src/fortran
)

# Build dynamic library
add_library(smartredis SHARED ${CLIENT_SRC})
set_target_properties(smartredis PROPERTIES SUFFIX ".so")
target_link_libraries(smartredis PUBLIC ${EXT_CLIENT_LIBRARIES})

add_library(smartredis-fortran SHARED ${FORTRAN_CLIENT_SRC})
target_link_libraries(smartredis-fortran PUBLIC ${EXT_CLIENT_LIBRARIES})
set_target_properties(smartredis-fortran PROPERTIES SUFFIX ".so")

# Install dynamic library and headers
install(TARGETS smartredis
	LIBRARY DESTINATION lib)
install(TARGETS smartredis-fortran
	LIBRARY DESTINATION lib)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/"
        DESTINATION "include"
        FILES_MATCHING
        PATTERN "*.h" PATTERN "*.tcc" PATTERN "*.inc" PATTERN "*.mod"
)

if(BUILD_PYTHON)
	message("-- Python client build enabled")
	add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/pybind
                     ${CMAKE_SOURCE_DIR}/third-party/pybind/build)

    add_library(smartredis_static STATIC ${CLIENT_SRC})

	pybind11_add_module(smartredisPy
	                    src/python/src/pyclient.cpp
                        src/python/src/pydataset.cpp
                        ${CLIENT_SRC}
                        src/python/bindings/bind.cpp)

	target_link_libraries(smartredisPy PUBLIC ${EXT_CLIENT_LIBRARIES})
	install(TARGETS smartredisPy
	        LIBRARY DESTINATION lib)
else()
	message("-- Skipping Python client build")
endif()
