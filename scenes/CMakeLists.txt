function(add_miktracer_scene name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE miktracer_lib)
endfunction()

add_miktracer_scene(sphere_grid)
add_miktracer_scene(three_spheres)
add_miktracer_scene(cornell_spheres)