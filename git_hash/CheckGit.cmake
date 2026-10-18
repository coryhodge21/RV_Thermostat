
message(STATUS "CheckGitHash")

# Set variable CURRENT_LIST_DIR = ESP32-General_Controller/cmake
set(CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
#message(STATUS ${CURRENT_LIST_DIR})


# Pre config = current dir
if (NOT DEFINED pre_configure_dir)
    set(pre_configure_dir ${CMAKE_CURRENT_LIST_DIR})
endif ()

# post config = ESP32-General_Controller/build/generated
if (NOT DEFINED post_configure_dir)
    set(post_configure_dir ${CMAKE_CURRENT_LIST_DIR}/generated)
endif ()
#message(STATUS ${CMAKE_BINARY_DIR})
#message(STATUS ${post_configure_dir})

# path to source file with build variable reference
set(pre_configure_file ${pre_configure_dir}/git_version.cpp.in)

# path to source file with actual git hash written in
set(post_configure_file ${post_configure_dir}/git_version.cpp)

###
# FX : CheckGitWrite(git_hash)
#   This writes the git hash to a file ../build/generated/git-state.txt
###
function(CheckGitWrite git_hash)
    file(WRITE ${post_configure_dir}/git-state.txt ${git_hash})
endfunction()

###
# FX : CheckGitRead(git_hash)
#       This reads ../build/git-state.txt into CONTENT
#       CONTENT is parsed for element 0 (the git hash), 
#           and this is written into var
#       var is used to assign the hash string to git_hash
###
function(CheckGitRead git_hash)
    if (EXISTS ${post_configure_dir}/git-state.txt)
        file(STRINGS ${post_configure_dir}/git-state.txt CONTENT)
        LIST(GET CONTENT 0 var)

        set(${git_hash} ${var} PARENT_SCOPE)
    endif ()
endfunction()

###
# FX : CheckGitVersion
#
###
function(CheckGitVersion)
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    CheckGitRead(GIT_HASH_CACHE)

    if (NOT EXISTS ${post_configure_dir})
        file(MAKE_DIRECTORY ${post_configure_dir})
    endif ()

    if (NOT EXISTS ${post_configure_dir}/git_version.h)
        file(COPY ${pre_configure_dir}/git_version.h DESTINATION ${post_configure_dir})
    endif()

    if (NOT DEFINED GIT_HASH_CACHE)
        set(GIT_HASH_CACHE "INVALID")
    endif ()

    # Only update the git_version.cpp if the hash has changed. This will
    # prevent us from rebuilding the project more than we need to.
    if (NOT ${GIT_HASH} STREQUAL ${GIT_HASH_CACHE} OR NOT EXISTS ${post_configure_file})
        # Set che GIT_HASH_CACHE variable the next build won't have
        # to regenerate the source file.
        CheckGitWrite(${GIT_HASH})

        configure_file(${pre_configure_file} ${post_configure_file} @ONLY)
    endif ()

endfunction()

#####
# function : CheckGitSetup
# brief : 
#####
function(CheckGitSetup)

    add_custom_target(AlwaysCheckGit COMMAND ${CMAKE_COMMAND}
        -DRUN_CHECK_GIT_VERSION=1
        -Dpre_configure_dir=${pre_configure_dir}
        -Dpost_configure_file=${post_configure_dir}
        -DGIT_HASH_CACHE=${GIT_HASH_CACHE}
        -P ${CURRENT_LIST_DIR}/CheckGit.cmake
        BYPRODUCTS ${post_configure_file}
        )

    add_library(git_version ${CURRENT_LIST_DIR}/generated/git_version.cpp)
    target_include_directories(git_version PUBLIC ${CURRENT_LIST_DIR}/generated)
    add_dependencies(git_version AlwaysCheckGit)

    CheckGitVersion()
endfunction()

# This is used to run this function from an external cmake process.
if (RUN_CHECK_GIT_VERSION)
    CheckGitVersion()
endif ()
