#include <iostream>
#include <string>
#include <fstream>

void render(int imageWidth, int imageHeight, std::ofstream& outFile){

    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for(int j = 0; j < imageHeight; j++){
        for(int i = 0; i < imageWidth; i++){
            auto r = double(i) / (imageWidth - 1);
            auto g = double(j) / (imageHeight - 1);
            auto b = 1.0;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            outFile << ir << " " << ig << " " << ib << "\n";
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