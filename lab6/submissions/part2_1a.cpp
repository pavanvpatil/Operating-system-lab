#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <math.h>
#include <cmath>

using namespace std;

class pixel
{
public:
    int r, g, b;

    pixel(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    pixel() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    };
};

vector<vector<pixel *>> pixels;

atomic_flag flag = ATOMIC_FLAG_INIT; // atomic flag 

auto startTime()
{
    return chrono::high_resolution_clock::now();
}
auto endTime()
{
    return chrono::high_resolution_clock::now();
}

// source: https://www.baeldung.com/cs/convert-rgb-to-grayscale
//  Luminosity Method
void grayScale(vector<vector<pixel *>> &pixels)
{
    int width = pixels[0].size();
    int height = pixels.size();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            while (atomic_flag_test_and_set(&flag)); // spin until flag is clear

            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            pixels[i][j]->r = avg;
            pixels[i][j]->g = avg;
            pixels[i][j]->b = avg;

            atomic_flag_clear(&flag); // clear flag
        }
    }
}

void blueTone(vector<vector<pixel *>> &pixels)
{

    int width = pixels[0].size();
    int height = pixels.size();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            while (atomic_flag_test_and_set(&flag));   // spin until flag is clear

            int b = pixels[i][j]->b;
            if (b + 50 > 255)
            {
                b = 255;
            }
            else
            {
                b += 50;
            }
            pixels[i][j]->b = b;

            atomic_flag_clear(&flag); // clear flag
        }
    }
}

void edgeDetection(vector<vector<pixel *>> &pixels)
{
    
    int width = pixels[0].size();
    int height = pixels.size();

    vector<vector<pixel *>> modifiedPixels(height, vector<pixel *>(width, NULL));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            modifiedPixels[i][j] = new pixel();
        }
    }

    for (int i = 1; i <= height - 2; i++)
    {
        for (int j = 1; j <= width - 2; j++)
        {

            modifiedPixels[i][j]->r = sqrt(pow((pixels[i - 1][j - 1]->r + 2 * pixels[i][j - 1]->r + pixels[i + 1][j - 1]->r) - (pixels[i - 1][j + 1]->r + 2 * pixels[i][j + 1]->r + pixels[i + 1][j + 1]->r), 2) + pow((pixels[i - 1][j - 1]->r + 2 * pixels[i - 1][j]->r + pixels[i - 1][j + 1]->r) - (pixels[i + 1][j - 1]->r + 2 * pixels[i + 1][j]->r + pixels[i + 1][j + 1]->r), 2));
            modifiedPixels[i][j]->g = sqrt(pow((pixels[i - 1][j - 1]->g + 2 * pixels[i][j - 1]->g + pixels[i + 1][j - 1]->g) - (pixels[i - 1][j + 1]->g + 2 * pixels[i][j + 1]->g + pixels[i + 1][j + 1]->g), 2) + pow((pixels[i - 1][j - 1]->g + 2 * pixels[i - 1][j]->g + pixels[i - 1][j + 1]->g) - (pixels[i + 1][j - 1]->g + 2 * pixels[i + 1][j]->g + pixels[i + 1][j + 1]->g), 2));
            modifiedPixels[i][j]->b = sqrt(pow((pixels[i - 1][j - 1]->b + 2 * pixels[i][j - 1]->b + pixels[i + 1][j - 1]->b) - (pixels[i - 1][j + 1]->b + 2 * pixels[i][j + 1]->b + pixels[i + 1][j + 1]->b), 2) + pow((pixels[i - 1][j - 1]->b + 2 * pixels[i - 1][j]->b + pixels[i - 1][j + 1]->b) - (pixels[i + 1][j - 1]->b + 2 * pixels[i + 1][j]->b + pixels[i + 1][j + 1]->b), 2));
        }
    }

    for (int i = height - 1; i > 0; i--)
    {
        for (int j = 1; j <= width - 1; j++)
        {
            while (atomic_flag_test_and_set(&flag)); // spin until flag is clear

            pixels[i][j]->r = modifiedPixels[i][j]->r;
            pixels[i][j]->g = modifiedPixels[i][j]->g;
            pixels[i][j]->b = modifiedPixels[i][j]->b;

            atomic_flag_clear(&flag);   // clear the flag
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        cout << "Usage: ./part1 <input_file> <output_file>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    ifstream in;
    ofstream out;

    in.open(input_file); // open the input file

    string type;
    int width, height, maxColor;

    if (in.is_open())
    {

        in >> type;
        in >> width >> height;
        in >> maxColor;

        pixels.resize(height, vector<pixel *>(width, NULL));

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int r, g, b;
                in >> r >> g >> b;
                pixels[i][j] = new pixel(r, g, b);
            }
        }
    }

    out.open(output_file); // open the output file

    if (out.is_open())
    {

        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;

        auto start = startTime();
        thread t1(grayScale, ref(pixels));
        thread t2(edgeDetection, ref(pixels));
        t1.join();
        t2.join();
        auto end = endTime();

        cout << "Duration: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                out << pixels[i][j]->r << " " << pixels[i][j]->g << " " << pixels[i][j]->b << " ";
            }
            out << endl;
        }
    }

    out.close(); // close the output file
    in.close(); // close the input file

    atomic_flag_clear(&flag); // clear the flag

    return 0;
}