#pragma once
#include <string>
#include "Camera.h"
#include "ImageBuffer.h"
#include "Renderer.h"
#include "Scene.h"

template <typename BuildFn>
int runScene(const std::string& outputName,
             const Camera& camera, 
             const RendererSettings& rendererSettings, 
             BuildFn build) {
    Scene scene = build();
    Renderer renderer(rendererSettings);
    ImageBuffer image(camera.imageWidth(), camera.imageHeight());
    renderer.render(scene, camera, image);
    image.writePNG("renders/" + outputName + ".png");
    return 0;
}