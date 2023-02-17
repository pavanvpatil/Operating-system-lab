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
void grayScale(vector<vector<pixel*>>& pixels) {
    int width = pixels[0].size();
    int height = pixels.size();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            pixels[i][j]->r = avg;
            pixels[i][j]->g = avg;
            pixels[i][j]->b = avg;
        }
    }

}

void blueTone(vector<vector<pixel*>>& pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int b = pixels[i][j]->b;
            if (b + 50 > 255) {
                b = 255;
            } else {
                b += 50;
            }
            pixels[i][j]->b = b;
        }
    }

}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        cout << "Usage: ./part1 <input_file> <output_file>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    ifstream in;
    ofstream out;

    in.open(input_file);

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
    
    out.open(output_file);

    if (out.is_open()) {

        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;

        grayScale(pixels);
        blueTone(pixels);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                out << pixels[i][j]->r << " " << pixels[i][j]->g << " " << pixels[i][j]->b <<" ";
            }
            out << endl;
        }

    }

    out.close();

    return 0;
}