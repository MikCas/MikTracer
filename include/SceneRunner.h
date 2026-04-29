#pragma once
#include <string>
#include "ImageBuffer.h"
#include "Renderer.h"
#include "Scene.h"

template <typename BuildFn>
int runScene(const std::string& outputName,
             const RendererSettings& rendererSettings, 
             BuildFn build) 
{
    Scene scene = build();
    Renderer renderer(rendererSettings);
    ImageBuffer image(scene.camera.imageWidth(), scene.camera.imageHeight());
    renderer.render(scene, scene.camera, image);
    image.writePNG("renders/" + outputName + ".png");
    return 0;
}
