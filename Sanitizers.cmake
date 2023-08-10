#Allow options to enable asan and ubsan
option(ENABLE_ASAN "Address Sanitizer" FALSE)
option(ENABLE_UBSAN "Undefined Behaviour Sanitizer" FALSE)
option(ENABLE_LSAN "Leak Sanitizer" FALSE)

#Conditional logic for activating address sanitizer
if(ENABLE_ASAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

#Conditional logic for activating undefined behaviour sanitizer
if(ENABLE_UBSAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

#Conditional logic for activating leak sanitizer
if(ENABLE_LSAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif()
