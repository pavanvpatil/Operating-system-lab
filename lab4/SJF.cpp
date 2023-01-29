#include<bits/stdc++.h>

using namespace std;

int current_time = 0;
queue<int> ready_queue;
unordered_map<int,struct Process> PID_map;

struct Process{
    int PID;
    float turn_around_time;
    float waiting_time;
    queue<int> cpu_bound;
    queue<int> io_bound;
    int io_time;
    int cpu_time;
    int intial_arrival_time;
    int arrival_time;
    bool completed;
};

struct Process build_process(int PID,int arrival_time)
{
    struct Process proc;
    proc.PID = PID;
    proc.turn_around_time = -1;
    proc.waiting_time = -1;
    proc.completed = false;
    proc.io_time = 0;
    proc.cpu_time = 0;
    proc.arrival_time = arrival_time;
    proc.intial_arrival_time = arrival_time;
    return proc;
};


void Start_functioning()
{
    while(true)
    {
        for(auto i)
    }
}


int main()
{
    // extracting data from file
    ifstream file("process1.dat");
    string line;
    vector<vector<int>> input;
    while (getline(file, line)) {
        stringstream ss(line);
        int num;
        vector<int> temp;
        while (ss >> num) {
            temp.push_back(num);
        }
        if(temp.size()!=0)
            input.push_back(temp);
    }
    int PID = 0;
    for(auto i:input)
    {
        struct Process p = build_process(PID,i[0]);
        for(int j=1;j<i.size()-1;j++)
        {
            if(j%2 == 1)
                p.cpu_bound.push(i[j]);
            else
                p.io_bound.push(i[j]);
        }
        PID_map[PID] = p;
        PID++;
    }
    Start_functioning();
    return 0;
}
