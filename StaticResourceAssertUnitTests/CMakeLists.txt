

# Catch3 lib target
add_library(
    Catch3
    STATIC
    catch_amalgamated.cpp
)

add_executable(
    StaticResourceAssertUnitTests
    StaticResourceUnitTest.cpp
)

if( WIN32 )
    target_compile_options(StaticResourceAssertUnitTests PUBLIC /bigobj)
else()
    target_compile_options(StaticResourceAssertUnitTests PUBLIC -ftemplate-depth=1024)
    target_compile_options(StaticResourceAssertUnitTests PUBLIC -fconstexpr-ops-limit=4294967296)
    target_compile_options(StaticResourceAssertUnitTests PUBLIC -fconstexpr-depth=8192)
endif()

target_include_directories(
    StaticResourceAssertUnitTests PUBLIC
    ../
)

target_link_libraries(
    StaticResourceAssertUnitTests
    Catch3
)
