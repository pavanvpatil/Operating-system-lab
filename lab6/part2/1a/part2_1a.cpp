#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<thread>
#include<chrono>
#include<atomic>

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

vector<vector<pixel*>> pixels;

atomic_flag flag = ATOMIC_FLAG_INIT;

auto startTime() {
    return chrono::high_resolution_clock::now();
}
auto endTime() {
    return chrono::high_resolution_clock::now();
}

// source: https://www.baeldung.com/cs/convert-rgb-to-grayscale
//  Luminosity Method
void grayScale(vector<vector<pixel*>>& pixels) {
    int width = pixels[0].size();
    int height = pixels.size();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            while (atomic_flag_test_and_set(&flag));

            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            pixels[i][j]->r = avg;
            pixels[i][j]->g = avg;
            pixels[i][j]->b = avg;

            atomic_flag_clear(&flag);

        }
    }

}

void blueTone(vector<vector<pixel*>>& pixels) {

    int width = pixels[0].size();
    int height = pixels.size();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            while (atomic_flag_test_and_set(&flag));

            int b = pixels[i][j]->b;
            if (b + 50 > 255) {
                b = 255;
            } else {
                b += 50;
            }
            pixels[i][j]->b = b;

            atomic_flag_clear(&flag);

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
        
        auto start = startTime();
        thread t1(grayScale, ref(pixels));
        thread t2(blueTone, ref(pixels));
        t1.join();
        t2.join();
        auto end = endTime();

        cout << "Duration: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                out << pixels[i][j]->r << " " << pixels[i][j]->g << " " << pixels[i][j]->b <<" ";
            }
            out << endl;
        }

    }

    out.close();
    in.close();

    atomic_flag_clear(&flag);

    return 0;
}