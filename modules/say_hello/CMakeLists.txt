# Check if SAY_HELLO is set in Kconfig
if(CONFIG_SAY_HELLO)

    # Add the say_hello module to the build
    zephyr_include_directories(.)

    # Add the source file for the say_hello module
    zephyr_library_sources(say_hello.c)

endif()