# StaticResourceAssert
StaticResourceAssert (sra) generates a header file containing an API that allows for statically asserting the existence of a directory or file.

The API is updated only when StaticResourceAssert runs, so any changes to the filesystem after the header is generated cannot be statically asserted.

This project is just an experiment. Its use cases are probably minimal at best. Please enjoy.

# Using

## sra

### Command line arguments
- -d : Required. The directory to recursively search through and catalog entries
- -o : Name of the output api header. Always include the .h extention. Default is: static_resource_assert_api.h
- -h : Print version and command line options

    ### Example usage
    `sra -d inputDir -o outputDir/output_header.h`

## sra generated header
Include the generated header to get the static assert API.

### API reference
`STATIC_RESOURCE_ASSERT("file_or_directory")` Statically asserts that the file or directory path is in the API header.

`STATIC_RESOURCE_ASSERT_MSG("file_or_directory", "assert message")`  Statically asserts that the file or directory path is in the API header. Custom assert message is printed if assert fails.

`STATIC_RESOURCE_FIND("file_or_directory")` Statically searches for `file_or_directory` and returns true if the file or directory was found, else false.

Example:
```c++
STATIC_RESOURCE_ASSERT("dir0/file0");
STATIC_RESOURCE_ASSERT("dir0/file1");
STATIC_RESOURCE_ASSERT("dir0/other_dir");

STATIC_RESOURCE_ASSERT_MSG("dir0/file0", "file 0 not found!");
STATIC_RESOURCE_ASSERT_MSG("dir0/file1", "file 1 not found!");
STATIC_RESOURCE_ASSERT_MSG("dir0/other_dir", "other_dir not found!");

constexpr bool val_true = STATIC_RESOURCE_FIND("dir0/file0");
constexpr bool val_false = STATIC_RESOURCE_FIND("dir0/file5");
constexpr bool val_true = STATIC_RESOURCE_FIND("dir0/other_dir");
constexpr bool val_false = STATIC_RESOURCE_FIND("dir0/other_dir123");
```

# Building
## sra
1) Open StaticResourceAssert directory in VSCode or use cmake from the command line
2) Use cmake to configure
3) Build target `sra`

## UnitTests
1) Run `sra -d StaticResourceAssertUnitTests/exampleDirectory` from the repo root. This will generate the static API header needed by unit tests
2) Build target `StaticResourceAssertUnitTests`


# Limits
In cases where the directory cataloged is large, the compiler may display errors like: `fatal error: template instantiation depth exceeds maximum of X`.
When this occurs use the following GCC flags to change constexpr evaluation limits.
- `-ftemplate-depth=X` set the maximum instantiation depth for template classes to X.
- `-fconstexpr-ops-limit=X` sets the maximum number of operations during a single constexpr evaluation to X.
- `-fconstexpr-depth=X` sets the maximum nested evaluation depth for C++11 constexpr functions to X

