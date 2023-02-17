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
};

// vector<vector<pixel *>> pixels;

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
void grayScale(int width, int height, int size, int pid, pixel **pixels)
{
    if (pid > 0)
    {
        return;
    }
    // shared memory

    // shared semaphores memory
    int shmid_sem = shmget(sem_key, sizeof(sem_t), 0666);
    cout << "shmid_semg: " << shmid_sem << endl;
    if (shmid_sem < 0)
    {
        perror("shmget");
        exit(1);
    }
    // attach shared memory

    sem_t *sem = (sem_t *)shmat(shmid_sem, NULL, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sem_wait(sem);
            float avg = 0.3 * pixels[i * width + j]->r + 0.59 * pixels[i * width + j]->g + 0.11 * pixels[i * width + j]->b;
            avg = (int)avg;
            pixels[i * width + j]->r = avg;
            pixels[i * width + j]->g = avg;
            pixels[i * width + j]->b = avg;
            sem_post(sem);
        }
    }

    exit(0);
}

void blueTone(int width, int height, int size, int pid, pixel **pixels)
{
    if (pid > 0)
    {
        return;
    }

    // shared semaphores memory
    int shmid_sem = shmget(sem_key, sizeof(sem_t), IPC_CREAT | 0666);
    cout << "shmid_semb: " << shmid_sem << endl;
    if (shmid_sem < 0)
    {
        perror("shmget");
        exit(1);
    }
    // attach shared memory
    sem_t *sem = (sem_t *)shmat(shmid_sem, NULL, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sem_wait(sem);
            int b = pixels[i * width + j]->b;
            if (b + 50 > 255)
            {
                b = 255;
            }
            else
            {
                b += 50;
            }
            pixels[i * width + j]->b = b;
            sem_post(sem);
        }
    }

    exit(0);
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
    }

    int shmid_pixel = shmget(pixels_key, sizeof(pixel *) * height * width, IPC_CREAT | 0666);
    cout << "shmid_pixelf: " << shmid_pixel << endl;
    if (shmid_pixel < 0)
    {
        perror("shmget");
        exit(1);
    }

    // attach shared memory
    pixel **pixels = (pixel **)shmat(shmid_pixel, NULL, 0);
    cout << "pixelsf: " << pixels << endl;

    if (in.is_open())
    {

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int r, g, b;
                in >> r >> g >> b;
                pixels[i * width + j] = new pixel(r, g, b);
            }
        }
    }

    out.open(output_file);

    if (out.is_open())
    {

        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;

        // create shared memory for semaphores
        int semid = shmget(sem_key, sizeof(sem_t), IPC_CREAT | 0666);
        if (semid < 0)
        {
            perror("shmget");
            exit(1);
        }

        // attach shared memory for semaphores
        sem_t *semaphores = (sem_t *)shmat(semid, NULL, 0);

        // initialize semaphores
        sem_init(semaphores, 1, 1);

        // declare vector for pixels

        auto start = startTime();

        // int pid = fork();

        // if (pid == 0)
        // {
        //     // child process

        //     grayScale(width, height, sizeof(pixel *) * height * width);
        //     exit(0);
        // }
        // else
        // {
        //     // parent process
        //     wait(NULL);
        //     blueTone(width, height, sizeof(pixel *) * height * width);

        // }
        grayScale(width, height, sizeof(pixel *) * height * width, fork(), pixels);
        blueTone(width, height, sizeof(pixel *) * height * width, fork(), pixels);
        wait(NULL);
        wait(NULL);
        auto end = endTime();

        cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                out << pixels[i * width + j]->r << " " << pixels[i * width + j]->g << " " << pixels[i * width + j]->b << " ";
            }
            out << endl;
        }
        // remove shared memory
        shmctl(shmid_pixel, IPC_RMID, NULL);
        shmctl(semid, IPC_RMID, NULL);
    }

    out.close();
    in.close();

    return 0;
}