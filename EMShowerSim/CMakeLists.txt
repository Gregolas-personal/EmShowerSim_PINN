cmake_minimum_required(VERSION 3.16)
project(EMShowerSim)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# HDF5 is required for .h5 output. Geant4 must also be built with
# -DGEANT4_USE_HDF5=ON. If unavailable, change the filename in
# RunAction.cc to .root (requires ROOT) or .csv.
find_package(HDF5 QUIET COMPONENTS C CXX)
if(HDF5_FOUND)
    message(STATUS "HDF5 found — .h5 output enabled")
else()
    message(WARNING "HDF5 not found. Change the output filename in RunAction.cc "
                    "from .h5 to .root or .csv, or install HDF5 and rebuild Geant4 "
                    "with -DGEANT4_USE_HDF5=ON.")
endif()

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(EMShower EMShower.cc ${sources} ${headers})
target_link_libraries(EMShower ${Geant4_LIBRARIES})
target_include_directories(EMShower PRIVATE include)

if(HDF5_FOUND)
    target_link_libraries(EMShower ${HDF5_CXX_LIBRARIES})
    target_include_directories(EMShower PRIVATE ${HDF5_INCLUDE_DIRS})
endif()
