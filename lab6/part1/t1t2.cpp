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
vector<vector<pixel*>> grayScale(vector<vector<pixel*>> pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    vector<vector<pixel*>> newPixels(height, vector<pixel*>(width, NULL));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            newPixels[i][j] = new pixel(avg, avg, avg);
        }
    }

    return newPixels;

}

vector<vector<pixel*>> blueTone(vector<vector<pixel*>> pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    vector<vector<pixel*>> newPixels(height, vector<pixel*>(width, NULL));

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

int main(int argc, char* argv[]) {

    string file;
    cin >> file;

    ifstream in;
    ofstream out;

    in.open(file);

    string type;
    int width, height, maxColor;

    vector<vector<pixel*>> pixels;

    if (in.is_open()) {

        in >> type;
        in >> width >> height;
        in >> maxColor;

        pixels.resize(height, vector<pixel*>(width, NULL));
        
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r, g, b;
                in >> r >> g >> b;
                pixels[i][j] = new pixel(r, g, b);
            }
        }   

    }
    
    out.open("out.ppm");

    if (out.is_open()) {

        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;

        vector<vector<pixel*>> newPixels = grayScale(pixels);
        newPixels = blueTone(newPixels);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                out << newPixels[i][j]->r << " " << newPixels[i][j]->g << " " << newPixels[i][j]->b <<" ";
            }
            out << endl;
        }

    }

    out.close();

    return 0;
}