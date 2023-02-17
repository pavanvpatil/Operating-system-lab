#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <semaphore.h>

// declare a semaphore
sem_t sem;

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
};

vector<vector<pixel *>> pixels;

auto startTime() {
    return chrono::high_resolution_clock::now();
}
auto endTime() {
    return chrono::high_resolution_clock::now();
}


// declare a
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

            sem_wait(&sem);
            float avg = 0.3 * pixels[i][j]->r + 0.59 * pixels[i][j]->g + 0.11 * pixels[i][j]->b;
            avg = (int)avg;
            pixels[i][j]->r = avg;
            pixels[i][j]->g = avg;
            pixels[i][j]->b = avg;
            sem_post(&sem);
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

            sem_wait(&sem);
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
            sem_post(&sem);
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

    in.open(input_file);

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

    out.open(output_file);

    if (out.is_open())
    {

        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;

        // init semaphore
        sem_init(&sem, 0, 1);

        auto start = startTime();
        thread t1(grayScale, ref(pixels));
        thread t2(blueTone, ref(pixels));
        t1.join();
        t2.join();
        auto end = endTime();

        cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        
        // grayScale(pixels);
        // blueTone(pixels);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                out << pixels[i][j]->r << " " << pixels[i][j]->g << " " << pixels[i][j]->b << " ";
            }
            out << endl;
        }
    }

    out.close();
    in.close();

    sem_destroy(&sem);

    return 0;
}