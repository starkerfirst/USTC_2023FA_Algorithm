#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<cassert>
#include<utility>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;

int min_time = INT32_MAX;
vector<pair<int, vector<int>>> best_schedule;

void Backtracking(int &num_task, int &num_machine, vector<int> &task_time, int cur_step, vector<pair<int, vector<int>>>& schedule, int debug_index = 0){
    // boundary condition
    if (cur_step == num_task) {
        int time = max_element(schedule.begin(), schedule.end())->first;
        if (time < min_time){
            min_time = time;
            best_schedule = schedule;
        }
        return;
    }

    // recursive condition
    int index = 0;
    int max_task_time = *max_element(task_time.begin() + cur_step, task_time.end());
    int min_machine_time = min_element(schedule.begin(), schedule.end())->first;
    for(auto &machine: schedule){ 
        // pruning
        if (max_element(schedule.begin(), schedule.end())->first < min_time and max_task_time + min_machine_time < min_time and index <= cur_step + 1){
            machine.second.push_back(cur_step);
            machine.first += task_time[cur_step];
            Backtracking(num_task, num_machine, task_time, cur_step + 1, schedule, debug_index = index);
            machine.second.pop_back();
            machine.first -= task_time[cur_step];
        }
        index++;
    }
}

// initialize best_schedule with greedy algorithm
void initialize(int num_task, int num_machine, vector<int> &task_time){
    best_schedule = vector<pair<int, vector<int>>>(num_machine, pair<int, vector<int>>(0, vector<int>(0)));
    for (int i = 0; i < num_task; i++){
        int min_machine_time = min_element(best_schedule.begin(), best_schedule.end())->first;
        int index = find(best_schedule.begin(), best_schedule.end(), *min_element(best_schedule.begin(), best_schedule.end())) - best_schedule.begin();
        best_schedule[index].second.push_back(i);
        best_schedule[index].first += task_time[i];
    }
}

// Backtracking for best scheduling scheme
// three pruning methods are applied
int main(){
    int num_task, num_machine;
    
    // test1
    cout << "test1" << endl;
    ifstream fin("test1.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    fin >> num_task >> num_machine;
    vector<int> task_time;
    vector<int> task_time_copy;
    vector<pair<int, vector<int>>> schedule = vector<pair<int, vector<int>>>(num_machine, pair<int, vector<int>>(0, vector<int>(0)));
    task_time.resize(num_task);
    task_time_copy.resize(num_task);
    for (int i = 0; i < num_task; i++){
        fin >> task_time[i];
    }
    // sort task_time in descending order (greedy)
    copy(task_time.begin(), task_time.end(), task_time_copy.begin());
    sort(task_time.begin(), task_time.end());
    fin.close();
    initialize(num_task, num_machine, task_time);
    Backtracking(num_task, num_machine, task_time, 0, schedule);
    cout << "min_time: " << min_time << endl;
    int count = 1;
    int index;
    for (auto &machine: best_schedule){
        cout << "machine " << count << ": ";
        for (auto &task: machine.second){
            index = find(task_time_copy.begin(), task_time_copy.end(), task_time[task]) - task_time_copy.begin();
            cout << task_time[task] << "(task " << index+1 << ")" << " ";
            task_time_copy[index] = -1;
        }
        cout << endl;
        count++;
    }
    schedule.clear();

    // test2
    cout << "test2" << endl;
    fin.open("test2.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    fin >> num_task >> num_machine;
    schedule = vector<pair<int, vector<int>>>(num_machine, pair<int, vector<int>>(0, vector<int>(0)));
    min_time = INT32_MAX;
    best_schedule.clear();
    task_time.resize(num_task);
    task_time_copy.resize(num_task);
    for (int i = 0; i < num_task; i++){
        fin >> task_time[i];
    }
    fin.close();
    // sort task_time in descending order
    copy(task_time.begin(), task_time.end(), task_time_copy.begin());
    sort(task_time.begin(), task_time.end(), greater<int>());
    initialize(num_task, num_machine, task_time);
    Backtracking(num_task, num_machine, task_time, 0, schedule);
    cout << "min_time: " << min_time << endl;
    count = 1;
    for (auto &machine: best_schedule){
        cout << "machine " << count << ": ";
        for (auto &task: machine.second){
            int index = find(task_time_copy.begin(), task_time_copy.end(), task_time[task]) - task_time_copy.begin();
            cout << task_time[task] << "(task " << index+1 << ")" << " ";
            task_time_copy[index] = -1;
        }
        cout << endl;
        count++;
    }
    schedule.clear();
    

    // test3
    cout << "test3" << endl;
    fin.open("test3.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    fin >> num_task >> num_machine;
    schedule = vector<pair<int, vector<int>>>(num_machine, pair<int, vector<int>>(0, vector<int>(0)));
    min_time = INT32_MAX;
    best_schedule.clear();
    task_time.resize(num_task);
    task_time_copy.resize(num_task);
    for (int i = 0; i < num_task; i++){
        fin >> task_time[i];
    }
    fin.close();
    // sort task_time in descending order
    copy(task_time.begin(), task_time.end(), task_time_copy.begin());
    sort(task_time.begin(), task_time.end(), greater<int>());
    initialize(num_task, num_machine, task_time);
    Backtracking(num_task, num_machine, task_time, 0, schedule);
    cout << "min_time: " << min_time << endl;
    count = 1;
    for (auto &machine: best_schedule){
        cout << "machine " << count << ": ";
        for (auto &task: machine.second){
            int index = find(task_time_copy.begin(), task_time_copy.end(), task_time[task]) - task_time_copy.begin();
            cout << task_time[task] << "(task " << index+1 << ")" << " ";
            task_time_copy[index] = -1;
        }
        cout << endl;
        count++;
    }
    
    return 0;
}