# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-src"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-build"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
