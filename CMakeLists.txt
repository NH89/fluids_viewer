
cmake_minimum_required(VERSION 3.8  FATAL_ERROR)  
project(fluids_viewer LANGUAGES C CXX)

find_package(HDF5 REQUIRED)
#include_directories( )

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "./install" CACHE PATH "..." FORCE)
endif()

add_executable(fluids_viewer
        read_hdf5.cpp 
        
        )

target_include_directories(fluids_viewer PRIVATE  ${CMAKE_CURRENT_BINARY_DIR} )

target_compile_features(fluids_viewer PUBLIC cxx_std_11)  

target_link_libraries(fluids_viewer PRIVATE ${HDF5_LIBRARIES} ${HDF5_CXX_LIBRARIES} )
