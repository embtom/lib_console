project(lib_console)

######################################################################################
#Project config
######################################################################################
SET(LIB_CONSOLE_SOURCE_C   	    src/lib_console.c
                                 src/lib_console_factory.c)
SET(LIB_CONSOLE_HEADER          "include")
SET(LIB_CONSOLE_HEADER_INTERNAL "internal_include")

######################################################################################
# Selector to choose the linkage base
######################################################################################
SET(PROJECT_LINK_LIBRARIES lib_serial)

if (TARGET lib_thread) 
    LIST(APPEND PROJECT_LINK_LIBRARIES lib_thread )
else()
    find_package(osbase REQUIRED)
    LIST(APPEND PROJECT_LINK_LIBRARIES  EMBTOM::osbase)
endif()

if (TARGET lib_convention) 
    LIST(APPEND PROJECT_LINK_LIBRARIES lib_convention )
else()
    find_package(osbase REQUIRED)
    LIST(APPEND PROJECT_LINK_LIBRARIES  EMBTOM::osbase)
endif()

if (TARGET lib_mini_printf) 
    LIST(APPEND PROJECT_LINK_LIBRARIES lib_mini_printf )
else()
    find_package(osbase REQUIRED)
    LIST(APPEND PROJECT_LINK_LIBRARIES  EMBTOM::osbase)    
endif()

LIST(REMOVE_DUPLICATES PROJECT_LINK_LIBRARIES)

######################################################################################
#Build target settings
######################################################################################
add_library(${PROJECT_NAME} STATIC ${LIB_CONSOLE_SOURCE_C})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBRARIES}) 
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_CONSOLE_HEADER})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_CONSOLE_HEADER_INTERNAL})