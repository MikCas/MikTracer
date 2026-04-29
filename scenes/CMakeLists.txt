function(add_miktracer_scene name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE miktracer_lib)
endfunction()

add_miktracer_scene(cornell_spheres)
add_miktracer_scene(blue)
add_miktracer_scene(red)
add_miktracer_scene(green)
