# set all RCCL related options and values

#------------------------------------------------------------------------------
# Enable RCCL support
#------------------------------------------------------------------------------
set(OOMPH_WITH_RCCL OFF CACHE BOOL "Build with RCCL backend")

if (OOMPH_WITH_RCCL)
    find_package(hip REQUIRED)
    find_package(RCCL REQUIRED)
    add_library(oomph_rccl SHARED)
    add_library(oomph::rccl ALIAS oomph_rccl)
    oomph_shared_lib_options(oomph_rccl)
    target_link_libraries(oomph_rccl PUBLIC RCCL::rccl hip::host)
    install(TARGETS oomph_rccl
        EXPORT oomph-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
