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

using namespace std;

key_t pixels_key = 0x1256;
key_t sem_key = 0x1258;

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

auto startTime()
{
    return chrono::high_resolution_clock::now();
}
auto endTime()
{
    return chrono::high_resolution_clock::now();
}

void grayScale(int width, int height, int size)
{

    int shmid_pixel = shmget(pixels_key, size, 0666);
    if (shmid_pixel < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }

    pixel *pixels = (pixel *)shmat(shmid_pixel, NULL, 0);

    int shmid_sem = shmget(sem_key, sizeof(sem_t), 0666);
    if (shmid_sem < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }

    sem_t *sem = (sem_t *)shmat(shmid_sem, NULL, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sem_wait(sem); // wait for semaphore (decrement)
            float avg = 0.3 * pixels[i * width + j].r + 0.59 * pixels[i * width + j].g + 0.11 * pixels[i * width + j].b;
            avg = (int)avg;
            pixels[i * width + j].r = avg;
            pixels[i * width + j].g = avg;
            pixels[i * width + j].b = avg;
            sem_post(sem); // post to semaphore (increment)
        }
    }

}

void Edge_Detection(int width, int height, int size)
{

    int shmid_pixel = shmget(pixels_key, size, 0666);
    if (shmid_pixel < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }
  
    pixel *pixels = (pixel *)shmat(shmid_pixel, NULL, 0);

    int shmid_sem = shmget(sem_key, sizeof(sem_t), 0666);
    if (shmid_sem < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }

    sem_t *sem = (sem_t *)shmat(shmid_sem, NULL, 0);

    vector<vector<pixel>> modifiedPixels(height, vector<pixel>(width));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel pix;
            modifiedPixels[i][j] = pix;
        }
    }

    for (int i = 1; i <= height - 2; i++)
    {
        for (int j = 1; j <= width - 2; j++)
        {
            modifiedPixels[i][j].r = sqrt(pow((pixels[(i - 1) * width + j - 1].r + 2 * pixels[i * width + j - 1].r + pixels[(i + 1) * width + j - 1].r) - (pixels[(i - 1) * width + j + 1].r + 2 * pixels[i * width + j + 1].r + pixels[(i + 1) * width + j + 1].r), 2) + pow((pixels[(i - 1) * width + j - 1].r + 2 * pixels[(i - 1) * width + j].r + pixels[(i - 1) * width + j + 1].r) - (pixels[(i + 1) * width + j - 1].r + 2 * pixels[(i + 1) * width + j].r + pixels[(i + 1) * width + j + 1].r), 2));

            modifiedPixels[i][j].g = sqrt(pow((pixels[(i - 1) * width + j - 1].g + 2 * pixels[i * width + j - 1].g + pixels[(i + 1) * width + j - 1].g) - (pixels[(i - 1) * width + j + 1].g + 2 * pixels[i * width + j + 1].g + pixels[(i + 1) * width + j + 1].g), 2) + pow((pixels[(i - 1) * width + j - 1].g + 2 * pixels[(i - 1) * width + j].g + pixels[(i - 1) * width + j + 1].g) - (pixels[(i + 1) * width + j - 1].g + 2 * pixels[(i + 1) * width + j].g + pixels[(i + 1) * width + j + 1].g), 2));

            modifiedPixels[i][j].b = sqrt(pow((pixels[(i - 1) * width + j - 1].b + 2 * pixels[i * width + j - 1].b + pixels[(i + 1) * width + j - 1].b) - (pixels[(i - 1) * width + j + 1].b + 2 * pixels[i * width + j + 1].b + pixels[(i + 1) * width + j + 1].b), 2) + pow((pixels[(i - 1) * width + j - 1].b + 2 * pixels[(i - 1) * width + j].b + pixels[(i - 1) * width + j + 1].b) - (pixels[(i + 1) * width + j - 1].b + 2 * pixels[(i + 1) * width + j].b + pixels[(i + 1) * width + j + 1].b), 2));
        }
    }

    for (int i = height - 1; i > 0; i--)
    {
        for (int j = 1; j <= width - 1; j++)
        {

            sem_wait(sem); // wait for semaphore

            pixels[i * width + j].r = modifiedPixels[i][j].r;
            pixels[i * width + j].g = modifiedPixels[i][j].g;
            pixels[i * width + j].b = modifiedPixels[i][j].b;

            sem_post(sem); // post semaphore
        
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

    in.open(input_file); // open input file

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

    int shmid_pixel = shmget(pixels_key, sizeof(pixel) * height * width, IPC_CREAT | 0666);
    if (shmid_pixel < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }

    pixel *pixels = (pixel *)shmat(shmid_pixel, NULL, 0);

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

    int semid = shmget(sem_key, sizeof(sem_t), IPC_CREAT | 0666);

    if (semid < 0) // if shared memory is not created
    {
        perror("shmget");
        exit(1);
    }
    sem_t *sem = (sem_t *)shmat(semid, NULL, 0);

    // initialize semaphores
    sem_init(sem, 0, 1);

    auto start = startTime();

    int pid = fork();
    if (pid == 0) // child process
    {
        grayScale(width, height, sizeof(pixel) * height * width);
        exit(0);
    }
    else // parent process
    {
        Edge_Detection(width, height, sizeof(pixel) * height * width);
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

    shmctl(shmid_pixel, IPC_RMID, NULL); // remove shared memory of pixels
    shmctl(semid, IPC_RMID, NULL);       // remove shared memory of semaphores

    out.close(); // close output file
    in.close(); // close input file

    return 0;
}