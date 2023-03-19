#include <bits/stdc++.h>

using namespace std;

int vpages, pframes, swap_size;
vector<int> data;
queue<int> FIFO;
int page_faults = 0;
int page_hits = 0;
unordered_set<int> RAM;

int main(int argc, char *argv[])
{
    argc--;
    argv++;
    if (argc != 4)
    {
        cout << "Error in Usage: ./a.out <virtual pages> <page frames> <blocks in swap space> <filename>" << endl;
        return 0;
    }
    vpages = atoi(argv[0]);
    pframes = atoi(argv[1]);
    swap_size = atoi(argv[2]);
    string filename = argv[3];
    if (vpages > pframes + swap_size)
    {
        cout << "Error: virtual pages > page frames + swap space" << endl;
        return 0;
    }

    // Open the input file
    ifstream infile(filename);
    if (!infile)
    {
        cerr << "Error: input file not found" << endl;
        return 1;
    }

    // Read the data from the file into a vector
    int num;
    while (infile >> num)
    {
        data.push_back(num);
    }

    // Close the input file
    infile.close();

    // implement FIFO
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] > vpages)
        {
            cout << "segmentation fault: virtual page number greater than virtual pages" << endl;
            return 0;
        }
        if (RAM.find(data[i]) == RAM.end())
        {
            if (RAM.size() < pframes)
            {
                RAM.insert(data[i]);
                FIFO.push(data[i]);
            }
            else
            {
                int temp = FIFO.front();
                FIFO.pop();
                RAM.erase(temp);
                RAM.insert(data[i]);
                FIFO.push(data[i]);
            }
            page_faults++;
        }
        else
        {
            page_hits++;
        }
    }

    cout << "Page Faults: " << page_faults << endl;
    cout << "Page Hits: " << page_hits << endl;
    cout << "Page Fault Rate: " << (float)page_faults / (float)(page_faults + page_hits) << endl;
    cout << "Page Hit Rate: " << (float)page_hits / (float)(page_faults + page_hits) << endl;

    return 0;
}