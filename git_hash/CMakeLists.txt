# CMakeLists.txt for git_hash
idf_component_register(
    SRCS 
        "generated/git_version.cpp" 
    INCLUDE_DIRS 
        "."
        "generated/"
        )
