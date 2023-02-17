#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

class pixel {

    public:
        
        int r;
        int g;
        int b; 

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
        }

};

key_t key = 0x1324;
key_t limitKeyofT2 = 0x1325;

#define INIT_VAL 1
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

// source: https://www.baeldung.com/cs/convert-rgb-to-grayscale
//  Luminosity Method
void grayScale(key_t limitKeyofT2, key_t key, int pid, int height, int width){ 

    if (pid > 0) { return; }
    
    int SHMID = shmget(key, sizeof(pixel) * height * width, 0666 | IPC_CREAT);
    
    pixel* pixelPointer = NULL;
    pixelPointer = (pixel*) shmat(SHMID, NULL, 0);

    sem_t *sem = sem_open("SEM", O_RDWR);

    int* GOBACK = NULL;
    int shmid2 = shmget(limitKeyofT2, sizeof(int) * 2, 0666 | IPC_CREAT);
    GOBACK = (int*)shmat(shmid2, NULL, 0);

    for (int i = 0; i < height; i++)
    {    
        for (int j = 0; j < width; j++)
        { 

            sem_wait(sem);
            
            pixel pix;
            pix = pixelPointer[i * width + j];

            int r = pix.r;
            int g = pix.g;
            int b = pix.b;

            float avg = 0.3 * r + 0.59 * g + 0.11 * b;
            avg = (int)avg;

            pix.r = avg;
            pix.g = avg;
            pix.b = avg;

            pixelPointer[i * width + j] = pix;
            GOBACK[0] = i + 1; GOBACK[1] = j + 1;

            sem_post(sem); 
        }
    }
}

void edgeDetection(key_t limitKeyofT2, key_t key, int pid, int height, int width){ 
    
    if(pid > 0) { return; }
    
    int SHMID = shmget(key, sizeof(pixel) * height * width, 0666 | IPC_CREAT);

    pixel* pixelPointer;
    pixelPointer = (pixel*)shmat(SHMID, NULL, 0);

    pixel temp;
    vector<vector<pixel>> newPixels (height, vector<pixel>(width));

    sem_t *sem = sem_open("SEM", O_RDWR);

    int *GOBACK;
    int shmid2 = shmget(limitKeyofT2, sizeof(int) * 2, 0666 | IPC_CREAT);
    GOBACK = (int *)shmat(shmid2, NULL, 0);

    int workedPixel = 0;

    for(int i = 1; i <= height - 2; i++)
    {
        for(int j = 1; j <= width - 2; j++)
        {

            sem_wait(sem);

            if(j + 1 <= GOBACK[1] || i + 1 <= GOBACK[0])
            {
                newPixels[i][j].r = sqrt(pow((pixelPointer[(i - 1) * width + (j - 1)].r + 2 * pixelPointer[i * width + (j - 1)].r + pixelPointer[(i + 1) * width + (j - 1)].r) - (pixelPointer[(i - 1) * width + (j + 1)].r + 2 * pixelPointer[i * width + (j + 1)].r + pixelPointer[(i + 1) * width + (j + 1)].r), 2) + pow((pixelPointer[(i - 1) * width + (j - 1)].r + 2 * pixelPointer[(i - 1) * width + j].r + pixelPointer[(i - 1) * width + (j + 1)].r) - (pixelPointer[(i + 1) * width + (j - 1)].r + 2 * pixelPointer[(i + 1) * width + j].r + pixelPointer[(i + 1) * width + (j + 1)].r), 2));
                newPixels[i][j].g = sqrt(pow((pixelPointer[(i - 1) * width + (j - 1)].g + 2 * pixelPointer[i * width + (j - 1)].g + pixelPointer[(i + 1) * width + (j - 1)].g) - (pixelPointer[(i - 1) * width + (j + 1)].g + 2 * pixelPointer[i * width + (j + 1)].g + pixelPointer[(i + 1) * width + (j + 1)].g), 2) + pow((pixelPointer[(i - 1) * width + (j - 1)].g + 2 * pixelPointer[(i - 1) * width + j].g + pixelPointer[(i - 1) * width + (j + 1)].g) - (pixelPointer[(i + 1) * width + (j - 1)].g + 2 * pixelPointer[(i + 1) * width + j].g + pixelPointer[(i + 1) * width + (j + 1)].g), 2));
                newPixels[i][j].b = sqrt(pow((pixelPointer[(i - 1) * width + (j - 1)].b + 2 * pixelPointer[i * width + (j - 1)].b + pixelPointer[(i + 1) * width + (j - 1)].b) - (pixelPointer[(i - 1) * width + (j + 1)].b + 2 * pixelPointer[i * width + (j + 1)].b + pixelPointer[(i + 1) * width + (j + 1)].b), 2) + pow((pixelPointer[(i - 1) * width + (j - 1)].b + 2 * pixelPointer[(i - 1) * width + j].b + pixelPointer[(i - 1) * width + (j + 1)].b) - (pixelPointer[(i + 1) * width + (j - 1)].b + 2 * pixelPointer[(i + 1) * width + j].b + pixelPointer[(i + 1) * width + (j + 1)].b), 2));
                workedPixel = 1;
            } 
            else 
            {
                workedPixel = 0;
            }

            sem_post(sem);
            
            if(workedPixel == 0) {
                j -= 1;
            }
        }

        if (workedPixel == 0) 
        {
            i -= 1;
        }
    
    }

    for(int i = height - 1; i > 0; i--)
    {
        for(int j = 1; j <= width - 1; j++)
        {
            pixelPointer[i * width + j] = newPixels[i][j];
        }
    }

}

auto startTime(){
    return chrono::high_resolution_clock::now();
}
auto stopTime(){
    return chrono::high_resolution_clock::now();
}

int main(int argc, char** argv){

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
    int height, width, maxColor; 

    if (in.is_open()) {
        in >> type;
        in >> width;
        in >> height;
        in >> maxColor;
    }
    
   
    int SHMID = shmget(key, sizeof(pixel) * (height) * width, 0666 | IPC_CREAT);

    pixel *pixelPointer = NULL;
    pixelPointer = (pixel *)shmat(SHMID, NULL, 0);

    if (in.is_open()) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r, g, b;
                in >> r >> g >> b;
                pixel pix(r, g, b);
                pixelPointer[i * width + j] = pix;
            }
        }        
    }

    in.close();
    
    int *GOBACK;
    int shmid2 = shmget(limitKeyofT2, sizeof(int) * 2, 0666 | IPC_CREAT);
    GOBACK = (int *)shmat(shmid2, NULL, 0);
  
    auto start = startTime();

    sem_t *sem = sem_open("SEM", O_CREAT | O_EXCL, SEM_PERMS, INIT_VAL); //named semaphore

    edgeDetection(limitKeyofT2, key, fork(), height, width);
    grayScale(limitKeyofT2, key, fork(), height, width);
    wait(NULL); 
    wait(NULL);

    auto stop = stopTime();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Duration: " << duration.count() << " milliseconds" << endl;

    out.open(output_file);

    if (out.is_open()) {
        out << type << endl;
        out << width << " " << height << endl;
        out << maxColor << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pixel pix = pixelPointer[i * width + j];
                out << pix.r << " " << pix.g << " " << pix.b << " ";
            }
            out << endl;
        }
    }

    out.close();

    return 0;
}