#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <cmath>
#include <sys/stat.h>
#include <fcntl.h>

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
    pixel()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
};

pixel *pixels = NULL;
// file descriptor
int fds[2];

auto startTime()
{
    return chrono::high_resolution_clock::now();
}
auto endTime()
{
    return chrono::high_resolution_clock::now();
}

void grayScale(int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int buffer[3];
            read(fds[0], &buffer, sizeof(buffer));
            float avg = 0.3 * buffer[0] + 0.59 * buffer[1] + 0.11 * buffer[2];
            avg = (int)avg;
            pixels[i * width + j].r = avg;
            pixels[i * width + j].g = avg;
            pixels[i * width + j].b = avg;
        }
    }
}

void Edge_Detection(int width, int height)
{

    vector<vector<pixel>> newpixels(height, vector<pixel>(width));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel pix(0, 0, 0);
            newpixels[i][j] = pix;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
            {
                continue;
            }
            else
            {
                newpixels[i][j].r = sqrt(pow((pixels[(i - 1) * width + j - 1].r + 2 * pixels[i * width + j - 1].r + pixels[(i + 1) * width + j - 1].r) - (pixels[(i - 1) * width + j + 1].r + 2 * pixels[i * width + j + 1].r + pixels[(i + 1) * width + j + 1].r), 2) + pow((pixels[(i - 1) * width + j - 1].r + 2 * pixels[(i - 1) * width + j].r + pixels[(i - 1) * width + j + 1].r) - (pixels[(i + 1) * width + j - 1].r + 2 * pixels[(i + 1) * width + j].r + pixels[(i + 1) * width + j + 1].r), 2));

                newpixels[i][j].g = sqrt(pow((pixels[(i - 1) * width + j - 1].g + 2 * pixels[i * width + j - 1].g + pixels[(i + 1) * width + j - 1].g) - (pixels[(i - 1) * width + j + 1].g + 2 * pixels[i * width + j + 1].g + pixels[(i + 1) * width + j + 1].g), 2) + pow((pixels[(i - 1) * width + j - 1].g + 2 * pixels[(i - 1) * width + j].g + pixels[(i - 1) * width + j + 1].g) - (pixels[(i + 1) * width + j - 1].g + 2 * pixels[(i + 1) * width + j].g + pixels[(i + 1) * width + j + 1].g), 2));

                newpixels[i][j].b = sqrt(pow((pixels[(i - 1) * width + j - 1].b + 2 * pixels[i * width + j - 1].b + pixels[(i + 1) * width + j - 1].b) - (pixels[(i - 1) * width + j + 1].b + 2 * pixels[i * width + j + 1].b + pixels[(i + 1) * width + j + 1].b), 2) + pow((pixels[(i - 1) * width + j - 1].b + 2 * pixels[(i - 1) * width + j].b + pixels[(i - 1) * width + j + 1].b) - (pixels[(i + 1) * width + j - 1].b + 2 * pixels[(i + 1) * width + j].b + pixels[(i + 1) * width + j + 1].b), 2));
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int buffer[3];
            buffer[0] = newpixels[i][j].r;
            buffer[1] = newpixels[i][j].g;
            buffer[2] = newpixels[i][j].b;
            write(fds[1], &buffer, sizeof(buffer));
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

    if (!in.is_open())
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    in >> type;
    in >> width >> height;
    in >> maxColor;

    pixels = new pixel[width * height];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r, g, b;
            in >> r >> g >> b;
            pixel pix(r, g, b);
            pixels[i * width + j] = pix;
        }
    }

    out.open(output_file);

    if (!out.is_open())
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    out << type << endl;
    out << width << " " << height << endl;
    out << maxColor << endl;

    auto start = startTime();
    if (pipe(fds) == -1)
    {
        perror("pipe");
        exit(1);
    }

    int pid = fork();
    if (pid == 0)
    {
        close(fds[0]);
        Edge_Detection(width, height);
        close(fds[1]);
        exit(0);
    }
    else
    {
        close(fds[1]);
        grayScale(width, height);
        close(fds[0]);
        wait(NULL);
    }

    auto end = endTime();

    cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            out << pixels[i * width + j].r << " " << pixels[i * width + j].g << " " << pixels[i * width + j].b << " ";
        }
        out << endl;
    }

    out.close();
    in.close();

    return 0;
}