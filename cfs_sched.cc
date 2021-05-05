#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "multimap.h"

class Scheduler { 
 public:
  Scheduler():tick(0), min_vruntime(0){}
  unsigned int tick;
  unsigned int min_vruntime;
};


class Task {
 public:
  Task(char id, unsigned int st, unsigned int dur) {
    identifier = id;
    start_time = st;
    duration = dur;
  }
  char Getid() {
    return identifier;
  }
  unsigned int Getst() {
    return start_time;
  }
  unsigned int Getdur() {
    return duration;
  }

  void VRunTimeInit(unsigned int n) {
    vruntime = n;
  }

  unsigned int runtime = 0;
  unsigned int vruntime;
        
 private:
    char identifier;
    unsigned int start_time;
    unsigned int duration;
};
 
// Helper - overload sorting method
struct Sorter {
    bool operator() (Task* &a, Task* &b) const {
      return a->Getid() > b->Getid();
    }
};

void Procedure(std::vector<Task*> &task, Scheduler *schedule, Multimap<unsigned int, Task*> &map){
    int prev = 0; 
    int working = 0;
    unsigned int task_size = task.size();
    std::vector<Task*> current_running; 
    std::vector<Task*> cycled_task;
    // Start of the actual scheduler keeps on going until there are no more tasks
    while (task_size != 0) {
      // CHECKING CHUNK START
      // In this checking chunk we see if there are elements that need
      // to be sorted by alphabet if they have the same starting time
      // if there are same starting times, sort them by alphabet and insert into Multimap
      // If not then its only one task or none so Insert it into multimap
      std::vector<Task*> same;
      int same_count = 0;

      for (unsigned int i = 0; i < task.size(); i++) {
        if (task[i]->Getst() == schedule->tick) {
          same.push_back(task[i]);
          same_count++;
        }
      }
      if (same_count >= 1) {
        std::sort(same.begin(), same.end(), Sorter());
        // Have to make a temp here otherwise might run
        // into comaring int and unsigned int problem
        unsigned int temp = same.size()-1;
        // we push the elements in reverse order 
        // since my container is now in descending order
        for (int k = temp; k >= 0; k--)  {
          // Initialize their virtual runtime to the current global min_vruntime
          same[k]->vruntime = schedule->min_vruntime;
          map.Insert(same[k]->vruntime, same[k]);
          // After inserting in multimap
          // Delete the pop the key in the back
          // which would be the key with the smallest
          // alphabetical order
          cycled_task.push_back(same[k]);
          working++;
          same.pop_back(); 
        }

        if (current_running.size() == 0) {
        } else if (current_running[0]->vruntime > schedule->min_vruntime) {
          schedule->min_vruntime = current_running[0]->vruntime;
        }
        if (current_running.size() > 0) {
          current_running.pop_back();
        }
    
      } else { // else if there is only one element
          if(same.size() == 1){
              same[0]->vruntime = schedule->min_vruntime;
              map.Insert(same[0]->vruntime, same[0]);
              cycled_task.push_back(same[0]);
              working++;
              same.pop_back();
              // moved it here such that initiation of vruntime is later 
              if (current_running.size() == 0) {

              } else if (current_running[0]->vruntime > schedule->min_vruntime) {
                schedule->min_vruntime = current_running[0]->vruntime;
              }
              if (current_running.size() > 0) {
                current_running.pop_back();
              }
          }
      }
      // CHECKING CHUNK END

      // Now scheduling starts 
      // Currently running function always has to be 0 initially
      // so we pop_back the one item from last loop 
      if(current_running.size() > 0) {
        current_running.pop_back();
      }

      // special case for when there is nothing to run
      if (map.Size() == 0){
        std::cout << schedule->tick << " [0]: _" << std::endl;
        schedule->tick++;
        continue;
      }
      // Only special case when activating the schedule 
      if (prev == 0 && map.Size() == 1) {
        current_running.push_back(map.Get(map.Min()));
        // remove first because its before incrementing 
        prev++;
        map.Remove(map.Min());
        current_running[0]->runtime++;
        current_running[0]->vruntime++;
        
        map.Insert(current_running[0]->vruntime, current_running[0]);
        std::cout << schedule->tick << " [" << working << "]: " << current_running[0]->Getid() << std::endl;
        schedule->tick++;
        continue;
      }
      // Get the smallest item which contains the task
      // check to see if there is a vruntime that is larger than it
      for (unsigned int i = 0; i < cycled_task.size(); i++) {
        if(cycled_task[i]->vruntime > schedule->min_vruntime) {
            schedule->min_vruntime = cycled_task[i]->vruntime;
        }
      }

      // Now general case where we take in smallest task
      current_running.push_back(map.Get(map.Min()));
      current_running[0]->runtime++;
      current_running[0]->vruntime++;

      // Check if task is done if it is done, we remove it from active tasks

      if (current_running[0]->runtime == current_running[0]->Getdur()) {
        std::cout << schedule->tick << " [" << working << "]: " << current_running[0]->Getid() << "*" << std::endl;
        working--; 
        task_size--;
        for (unsigned int n = 0; n < cycled_task.size(); n++) {
          if (cycled_task[n]->Getid() == current_running[0]->Getid()) {
            cycled_task.erase(cycled_task.begin()+n);
          }
        }
        schedule->tick++;
        map.Remove(map.Min());
        continue;
      }
      // Here we check if tasks have anything smaller than them otherwise
      // we can keep incrementing the current task until its 1 greater than
      // the min_vruntime 
      // if there is something less then we start the scheduling loop again

      int less_count = 0;
      for (unsigned int i = 0; i < cycled_task.size(); i++) {
        if (current_running[0]->vruntime > cycled_task[i]->vruntime) {   
          less_count++;
        }
      }

      if (less_count > 0) {
        map.Remove(map.Min());
        map.Insert(current_running[0]->vruntime, current_running[0]);
        std::cout << schedule->tick << " [" << working << "]: " << current_running[0]->Getid() << std::endl;
        schedule->tick++;
        continue; 

      } else {

        std::cout << schedule->tick << " [" << working << "]: " << current_running[0]->Getid() << std::endl;
        schedule->tick++;
        
        while (current_running[0]->vruntime <= schedule->min_vruntime) {
          current_running[0]->runtime++;
          current_running[0]->vruntime++;
          if(current_running[0]->runtime == current_running[0]->Getdur()){
            break;
          }
          std::cout << schedule->tick << " [" << working << "]: " << current_running[0]->Getid() << std::endl;
          schedule->tick++;
        }
        // repush things into tree 

        if(current_running[0]->runtime == current_running[0]->Getdur()){
          current_running[0]->runtime--;
          current_running[0]->vruntime--;
          continue;
        }
        map.Remove(map.Min());
        map.Insert(current_running[0]->vruntime, current_running[0]);
        continue;
      }

    }
}

// Function to read in data and pushes it into the class of tasks

bool ReadTaskData(std::string file, std::vector<Task*> &vector) {
  std::ifstream loadfile; 
  loadfile.open(file);
  if (loadfile.is_open()) {
  } else {
    return false;
  }

  char iden;
  unsigned int stime, duration;

  while (loadfile >> iden >> stime >> duration) {
    Task *taskname =  new Task{iden, stime, duration};
    // activate vruntime later when inserted into multiset
    vector.push_back(taskname);

  }
  return true;
}

int main(int argc, char*argv[]) {
  std::vector<Task*> tasks;
  std::string task_file;
  Multimap<unsigned int, Task*> m_map;
  Scheduler *sched = new Scheduler{};
  // scheduler should be passed in when schedule starts running 

  if(argc != 2){
    std::cerr<< "Usage: " << argv[0]
    << " <task_file.dat>" << std::endl;
    return 1;
  }

  task_file = argv[1];
  
  if (ReadTaskData(task_file, tasks) == false) {
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    return 1;
  }

  Procedure(tasks, sched, m_map);

  // delete all tasks when complete

  for (unsigned int z = 0; z < tasks.size(); z++){
    tasks.pop_back();
  }
  return 0;
}
