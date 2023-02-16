#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

class pixel {
public:

    int r, g, b;

    pixel(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

};

// source: https://www.baeldung.com/cs/convert-rgb-to-grayscale
//  Luminosity Method
vector<std::vector<pixel*>> grayScale(std::vector<std::vector<pixel*>> pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    std::vector<std::vector<pixel*>> newPixels(height, std::vector<pixel*>(width, NULL));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            newPixels[i][j] = new pixel(avg, avg, avg);
        }
    }

    return newPixels;

}

std::vector<std::vector<pixel*>> blueTone(std::vector<std::vector<pixel*>> pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    std::vector<std::vector<pixel*>> newPixels(height, std::vector<pixel*>(width, NULL));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int b = pixels[i][j]->b;
            if (b + 50 > 255) {
                b = 255;
            } else {
                b += 50;
            }
            newPixels[i][j] = new pixel(pixels[i][j]->r, pixels[i][j]->g, b);
        }
    }

    return newPixels;

}

int main() {

    std::string file;
    std::cin >> file;

    std::ifstream in;
    std::ofstream out;

    in.open(file);

    std::string type;
    int width, height, maxColor;

    std::vector<std::vector<pixel*>> pixels;

    if (in.is_open()) {

        in >> type;
        in >> width >> height;
        in >> maxColor;

        pixels.resize(height, std::vector<pixel*>(width, NULL));
        
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r, g, b;
                in >> r >> g >> b;
                pixels[i][j] = new pixel(r, g, b);
            }
        }   

    }
    
    out.open("out1.ppm");

    if (out.is_open()) {

        out << type << std::endl;
        out << width << " " << height << std::endl;
        out << maxColor << std::endl;

        std::vector<std::vector<pixel*>> newPixels = blueTone(pixels);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                out << newPixels[i][j]->r << " " << newPixels[i][j]->g << " " << newPixels[i][j]->b << " ";
            }
            out << std::endl;
        }

    }

    out.close();

    return 0;
}