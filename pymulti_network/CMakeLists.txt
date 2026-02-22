# pymulti_network/CMakeLists.txt

if ( HAVE_PYTHON )

  if ( CYTHON_FOUND )
    include( UseCython )
    set_source_files_properties(pymulti_network.pyx PROPERTIES CYTHON_IS_CXX TRUE)

    # Generate pyconfig.pxi (Cython compile-time config)
    add_custom_command(
      OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pyconfig.pxi
      COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests.py
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      VERBATIM
    )

    add_custom_target(pymulti_network_pyconfig ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pyconfig.pxi
    )

    cython_add_module(pymulti_network pymulti_network.pyx)

    set_source_files_properties(
    pybuffer.pyx
    PROPERTIES CYTHON_IS_CXX TRUE )

    cython_add_module( pybuffer pybuffer.pyx )

    add_dependencies(pymulti_network pymulti_network_pyconfig)
    add_dependencies(pybuffer pymulti_network_pyconfig)

  else ()
    # require the source pymulti_network/pymulti_network.cxx precompiled
    set( pymulti_network_generated_file "NOTFOUND" )
    if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pymulti_network.cxx )
      set( pymulti_network_generated_file pymulti_network.cxx )
    elseif ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pymulti_network.cpp )
      set( pymulti_network_generated_file pymulti_network.cpp )
    endif ()

    if ( pymulti_network_generated_file STREQUAL "NOTFOUND" )
      message( FATAL_ERROR "idk" )
    endif ()

    python_add_module( pymulti_network ${pymulti_network_generated_file} pymulti_network.pxd )
    if ( APPLE )
      set_target_properties( pymulti_network PROPERTIES LINK_FLAGS "-undefined dynamic_lookup" )
    else ()
      set_target_properties( pymulti_network PROPERTIES LINK_FLAGS "-Wl,--no-as-needed" )
      target_link_libraries( pymulti_network ${Python_LIBRARIES} )
    endif ()
  endif ()

  target_link_libraries( pymulti_network
      multi_network
      )

  target_link_libraries( pybuffer
      ${MPI_CXX_LIBRARIES}
      )

  target_include_directories( pymulti_network PRIVATE
      ${PROJECT_SOURCE_DIR}/multi_network/include
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${Python_INCLUDE_DIRS}
      )

  target_include_directories( pybuffer PRIVATE
      ${PROJECT_SOURCE_DIR}/multi_network/include
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${Python_INCLUDE_DIRS}
      )

  install( DIRECTORY nest_multi_network/
      DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYEXECDIR}/nest_multi_network
  )
  install( TARGETS pymulti_network DESTINATION ${PYEXECDIR}/nest_multi_network/ )

  install( TARGETS pybuffer DESTINATION ${PYEXECDIR}/nest_multi_network/ )

endif ()