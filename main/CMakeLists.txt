idf_component_register(
    SRCS 
        "main.c"
        "../git_hash/generated/git_version.cpp"
    INCLUDE_DIRS 
        "."
        "../git_hash"
        )
