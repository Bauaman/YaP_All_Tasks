if(EXISTS "C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/antlr4_runtime/runtime/antlr4_tests[1]_tests.cmake")
  include("C:/Users/andre/YaP_All_Tasks/YaP_All_Tasks-1/Sprint15_1_Lesson5_Antlr/build/antlr4_runtime/runtime/antlr4_tests[1]_tests.cmake")
else()
  add_test(antlr4_tests_NOT_BUILT antlr4_tests_NOT_BUILT)
endif()
