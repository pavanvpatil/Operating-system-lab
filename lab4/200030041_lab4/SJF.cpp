#include <bits/stdc++.h>

using namespace std;

int current_time = 0;

struct Process
{
    int PID;
    float turn_around_time;
    float waiting_time;
    float penalty_ratio;
    queue<int> cpu_bound;
    queue<int> io_bound;
    int io_time;
    int cpu_time;
    int intial_arrival_time;
    int arrival_time;
    bool cpu_proc;
    bool completed;
};

// pair<task_burst,PID>
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> CPU_queue, temp_CPU_queue;
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> IO_queue, temp_IO_queue;
unordered_map<int, struct Process> PID_map;

struct Process build_process(int PID, int arrival_time)
{
    struct Process proc;
    proc.PID = PID;
    proc.turn_around_time = -1;
    proc.waiting_time = 0;
    proc.penalty_ratio = -1;
    proc.completed = false;
    proc.io_time = 0;
    proc.cpu_time = 0;
    proc.cpu_proc = true;
    proc.arrival_time = arrival_time;
    proc.intial_arrival_time = arrival_time;
    return proc;
};

bool check_CPU_bound(int PID)
{
    struct Process proc = PID_map[PID];
    if (proc.arrival_time <= current_time && proc.cpu_proc && !proc.completed && !proc.io_time)
        return true;
    return false;
}

bool check_IO_bound(int PID)
{
    struct Process proc = PID_map[PID];
    if (proc.arrival_time <= current_time && !proc.cpu_proc && !proc.completed && !proc.cpu_time)
        return true;
    return false;
}

void add_to_ready_queue()
{
    for (auto &proc : PID_map)
    {
        if (check_CPU_bound(proc.first) && proc.second.cpu_time == 0)
        {
            proc.second.cpu_time = proc.second.cpu_bound.front();
            proc.second.cpu_bound.pop();
            proc.second.cpu_proc = false;
            CPU_queue.push({proc.second.cpu_time, proc.first});
        }
        else if (check_IO_bound(proc.first) && proc.second.io_time == 0)
        {
            proc.second.io_time = proc.second.io_bound.front();
            proc.second.io_bound.pop();
            proc.second.cpu_proc = true;
            IO_queue.push({proc.second.io_time, proc.first});
        }
        if (proc.second.cpu_bound.empty() && proc.second.io_bound.empty())
            proc.second.completed = true;
    }
}

void Start_functioning()
{
    int Curr_CPU_PID, Curr_IO_PID;
    int curr_CPU_burst_time = -1, curr_IO_burst_time = -1;
    while (true)
    {
        add_to_ready_queue();
        if (CPU_queue.empty() && IO_queue.empty() && curr_CPU_burst_time == -1 && curr_IO_burst_time == -1)
            break;
        if (!CPU_queue.empty() && (curr_CPU_burst_time == 0 || curr_CPU_burst_time == -1))
        {
            Curr_CPU_PID = CPU_queue.top().second;
            curr_CPU_burst_time = CPU_queue.top().first;
            CPU_queue.pop();
        }
        if (!IO_queue.empty() && (curr_IO_burst_time == 0 || curr_IO_burst_time == -1))
        {
            Curr_IO_PID = IO_queue.top().second;
            curr_IO_burst_time = IO_queue.top().first;
            IO_queue.pop();
        }
        if (curr_CPU_burst_time != 0 && curr_CPU_burst_time != -1)
        {
            curr_CPU_burst_time--;
            PID_map[Curr_CPU_PID].cpu_time--;
        }
        if (curr_CPU_burst_time == 0)
        {
            PID_map[Curr_CPU_PID].turn_around_time = current_time - PID_map[Curr_CPU_PID].intial_arrival_time;
            curr_CPU_burst_time = -1;
        }
        if (curr_IO_burst_time != 0 && curr_IO_burst_time != -1)
        {
            curr_IO_burst_time--;
            PID_map[Curr_IO_PID].io_time--;
        }
        if (curr_IO_burst_time == 0)
        {
            PID_map[Curr_IO_PID].turn_around_time = current_time - PID_map[Curr_IO_PID].intial_arrival_time;
            curr_IO_burst_time = -1;
        }

        temp_CPU_queue = CPU_queue;
        while (!temp_CPU_queue.empty())
        {
            if (temp_CPU_queue.top().second != Curr_CPU_PID)
                PID_map[temp_CPU_queue.top().second].waiting_time++;
            temp_CPU_queue.pop();
        }
        temp_IO_queue = IO_queue;
        while (!temp_IO_queue.empty())
        {
            if (temp_IO_queue.top().second != Curr_IO_PID)
                PID_map[temp_IO_queue.top().second].waiting_time++;
            temp_IO_queue.pop();
        }
        current_time++;
    }
}

int main(int argc, char *argv[])
{
    // extracting data from file
    string filename = "./test/process1.dat";
    if (argc == 2)
        filename = argv[1];
    ifstream file;
    try
    {
        file.open(filename);
        if (file.fail())
            throw exception();
    }
    catch (exception e)
    {
        cout << "File not found" << endl;
        return 0;
    }
    string line;
    vector<vector<int>> input;
    while (getline(file, line))
    {
        stringstream ss(line);
        int num;
        vector<int> temp;
        while (ss >> num)
        {
            temp.push_back(num);
        }
        if (temp.size() != 0)
            input.push_back(temp);
    }
    int PID = 0;
    for (auto i : input)
    {
        struct Process p = build_process(PID, i[0]);
        for (int j = 1; j < i.size() - 1; j++)
        {
            if (j % 2 == 1)
                p.cpu_bound.push(i[j]);
            else
                p.io_bound.push(i[j]);
        }
        PID_map[PID] = p;
        PID++;
    }
    unordered_map<int, int> total_actual_time;
    PID = 0;
    for (auto i : input)
    {
        for (auto j : i)
        {
            if (j == -1)
                break;
            total_actual_time[PID] += j;
        }
        PID++;
    }
    Start_functioning();
    for (auto &i : PID_map)
    {
        i.second.penalty_ratio = (float)i.second.turn_around_time / (float)total_actual_time[i.first];
    }
    float total_turn_around_time = 0;
    float total_waiting_time = 0;
    float total_penalty_ratio = 0;
    for (int i = 0; i < PID_map.size(); i++)
    {
        cout << "PID: " << i << ", ";
        cout << "Turn around time: " << PID_map[i].turn_around_time << ", ";
        total_turn_around_time += PID_map[i].turn_around_time;
        cout << "Waiting time: " << PID_map[i].waiting_time << ", ";
        total_waiting_time += PID_map[i].waiting_time;
        cout << "Penalty ratio: " << PID_map[i].penalty_ratio;
        total_penalty_ratio += PID_map[i].penalty_ratio;
        cout << endl;
    }
    cout << "___________________________________________________________________________" << endl
         << endl;
    cout << "Total Through put: " << (float)PID_map.size() / (current_time - 1) << endl;
    cout << "Average Turn around time: " << (float)total_turn_around_time / PID_map.size() << endl;
    cout << "Average Waiting time: " << (float)total_waiting_time / PID_map.size() << endl;
    cout << "Average Penalty ratio: " << (float)total_penalty_ratio / PID_map.size() << endl;
    cout << "___________________________________________________________________________" << endl
         << endl;
    return 0;
}