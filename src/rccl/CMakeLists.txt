add_library(oomph_private_rccl_headers INTERFACE)
target_include_directories(oomph_private_rccl_headers INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
target_link_libraries(oomph_rccl PRIVATE oomph_private_rccl_headers)

list(TRANSFORM oomph_sources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/../
    OUTPUT_VARIABLE oomph_sources_rccl)
target_sources(oomph_rccl PRIVATE ${oomph_sources_rccl})
target_sources(oomph_rccl PRIVATE context.cpp hip_event_pool.cpp)
