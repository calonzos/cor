cmake_minimum_required(VERSION 2.8.12.2)

#set version

#Project name
project(CorProject)

#Set Warning All and debugg flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -g ")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#Include directory
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/libraries/fuzzylite/)
include_directories(/usr/local/include/mongocxx/v0.3 /usr/local/include/bsoncxx/v0.3)


#${CMAKE_SOURCE_DIR}/libraries/liblocalsearch/trunk/include ${CMAKE_SOURCE_DIR}/libraries/gtest-1.7.0
#${CMAKE_SOURCE_DIR}/libraries/gtest-1.7.0/include/ ${CMAKE_SOURCE_DIR}/libraries/libaco/trunk/include

#Set source files paths
set(SEARCH_SPACE_PATH source/search_space/)
set(COR_FUZZY_PATH source/fuzzy/)
set(COR_ACO_PATH source/aco/)
set(DATOSBD_PATH ${CMAKE_SOURCE_DIR}/source/datosBD/)
set(LIBRARIES_PATH ${CMAKE_SOURCE_DIR}/libraries/)
set(FUZZYLITE_BUILD_PATH ${LIBRARIES_PATH}fuzzylite/bin/)
#set(LIBACO_BUILD_PATH ${LIBRARIES_PATH}libaco/trunk/build/)
#set(GTEST_BUILD_PATH ${LIBRARIES_PATH}gtest-1.7.0/mybuild/)
#set(ACO_LIBRARY ${LIBACO_BUILD_PATH}libaco.a)

set(SEARCH_SPACE_SOURCE ${SEARCH_SPACE_PATH}Sample.cpp ${SEARCH_SPACE_PATH}SampleGenerator.cpp ${SEARCH_SPACE_PATH}TermsGenerator.cpp ${SEARCH_SPACE_PATH}Subspace.cpp ${SEARCH_SPACE_PATH}SubspaceGenerator.cpp ${SEARCH_SPACE_PATH}SearchSpace.cpp  ${COR_FUZZY_PATH}RuleBlockGenerator.cpp ${COR_FUZZY_PATH}CorEngine.cpp ${COR_ACO_PATH}CorProblem.cpp  ${COR_ACO_PATH}BestWorstACO.cpp ${COR_ACO_PATH}AntSolution.cpp ${DATOSBD_PATH}mongoDB.cpp)



set(MAIN_SOURCE source/main.cpp)


find_library(MONGODB_LIBRARY mongocxx)
find_library(BSON_LIBRARY bsoncxx)


#Add subdirectories
add_subdirectory(${LIBRARIES_PATH}fuzzylite)
#add_subdirectory(${LIBRARIES_PATH}gtest-1.7.0)

#Importing libraries properties
add_library(fuzzylite STATIC IMPORTED)
set_property(TARGET fuzzylite PROPERTY IMPORTED_LOCATION ${FUZZYLITE_BUILD_PATH}libfuzzylite-static.a )

#Add executables
add_executable(CorProject ${MAIN_SOURCE} ${SEARCH_SPACE_SOURCE} ${FUZZYLITE_BUILD_PATH}libfuzzylite-static.a)


#Linking static libraries to executables
target_link_libraries(CorProject fuzzylite )
#target_link_libraries(CorProject ${CMAKE_SOURCE_DIR}/libraries/libaco/trunk/build/libaco.a )
#target_link_libraries(CorProject ${CMAKE_SOURCE_DIR}/libraries/liblocalsearch/trunk/build/liblocalsearch.a )
target_link_libraries(CorProject ${MONGODB_LIBRARY} ${BSON_LIBRARY})




