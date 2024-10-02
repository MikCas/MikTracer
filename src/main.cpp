#include <iostream>
#include <string>
#include <fstream>

#include "../include/Vec3.h"
#include "../include/Color.h"

void render(int imageWidth, int imageHeight, std::ofstream& outFile){

    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for(int j = 0; j < imageHeight; j++){
        for(int i = 0; i < imageWidth; i++){
            auto r = double(i) / (imageWidth - 1);
            auto g = double(j) / (imageHeight - 1);
            auto b = 0.0;

            writeColor(outFile, Vec3(r, g, b));
        }
    }
}

int main() {
    
    int imageWidth = 256;
    int imageHeight = 256;
    std::string outputFileName = "../image.ppm";

    std::ofstream outFile(outputFileName);

    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing :( \n";
        return 1;
    }

    render(imageWidth, imageHeight, outFile);

    outFile.close();

    return 0;
}