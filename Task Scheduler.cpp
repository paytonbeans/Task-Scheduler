#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

char alpha[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

struct task{
 int task_id, /* alphabetic tid can be obtained as 'A'+(task_counter++)
*/
 arrival_time = 0,
 service_time = 0,
 remaining_time = 0,
 completion_time = 0,
 response_time = 0,
 wait_time = 0;
};
//10 B4 C2

void print_ln(int cycle, task * active, vector<task> * rdy_list){

  cout << setw(3) << right << cycle << "\t";

  // Print active task if there is one
  if(active == nullptr){
    cout << "  ";
  }
  else if(active->remaining_time == 0){
    cout << "  ";
  }
  else{
    cout << alpha[active->task_id] << active->remaining_time;
  }

  cout << "\t";

  if(rdy_list->size() <= 1){
    cout << "--";
  }
  else{
    string rdy_string = "";
    for(int i = 0; i < rdy_list->size(); i++){
      if(&rdy_list->at(i) != active){
        cout << alpha[rdy_list->at(i).task_id] << rdy_list->at(i).remaining_time;
        if(active == &rdy_list->at(rdy_list->size() - 1)){
          if(i != rdy_list->size() - 2){
            cout << ", ";
          }
        }
        else{
          if(i < rdy_list->size() - 1){
            cout << ", ";
          }
        }
      }
    }
    cout << rdy_string;
  }

  cout << endl;
  return;
}

// Move task obj from list1 to list2 and remove from list 1
void transfer_task(vector<task> * list1, vector<task> * list2, int index){
  list2->push_back(list1->at(index));
  list1->erase(list1->begin() + index);
}

void service_tasks(vector<task> * rdy_list, task * active, int cycle){
  for(int i = 0; i < rdy_list->size(); i++){
    if(&rdy_list->at(i) == active){
      // Service active task
      active->remaining_time--;
      if(active->remaining_time == 0){
        active->completion_time = cycle + 1;
        active->response_time = cycle - active->arrival_time + 1;
      }

    }
    else{
      rdy_list->at(i).wait_time++;
    }
  }
}

void add_rdy_tasks(vector<task> * task_list, vector<task> * rdy_list, int cycle){
  // Check for any tasks that are ready to start
    for(int i = 0; i < task_list->size(); i ++){
      if(cycle == task_list->at(i).arrival_time && task_list->at(i).remaining_time > 0){
        transfer_task(task_list, rdy_list, i);
        i--;
      }
    }
}

void remove_complete_tasks(vector<task> * task_list, vector<task> * rdy_list){
  //Check if any tasks on ready list are complete and can be moved back to task list
    for(int i = 0; i < rdy_list->size(); i ++){
      if(rdy_list->at(i).remaining_time == 0){
        transfer_task(rdy_list, task_list, i);
        i--;
      }
    }
}

bool remaining_tasks(vector<task> * list1, vector<task> * list2){
  bool flag = false;
  for(int i = 0; i < list1->size(); i++){
    if(list1->at(i).remaining_time > 0){
      flag = true;
    }
  }
  for(int i = 0; i < list2->size(); i++){
    if(list2->at(i).remaining_time > 0){
      flag = true;
    }
  }
  return flag;
}



vector<task> sjf(vector<task> task_list){
  vector<task> rdy_list;
  task * active = nullptr;

  cout << "SJF(preemptive) scheduling results" << endl <<
        "time    cpu     ready queue (tid/rst)" << endl <<
        "----------------------------------" << endl;

  int cycle = 0;
  while(remaining_tasks(&task_list, &rdy_list)){

    //Check for tasks ready to start
    add_rdy_tasks(&task_list, &rdy_list, cycle);



    // Select active task
    if(rdy_list.size() > 0){
      active = &rdy_list[0];
      for(int i = 0; i < rdy_list.size(); i++){
        if(active->remaining_time > rdy_list[i].remaining_time && rdy_list[i].remaining_time != 0){
         active = &rdy_list[i];
        }
      }

    }
    else{
      active = nullptr;
    }


    // Print line
    print_ln(cycle, active, &rdy_list);

    //service task
    if(active != nullptr){
      service_tasks(&rdy_list, active, cycle);
    }

    //Check for complete tasks to move back to task list
    remove_complete_tasks(&task_list, &rdy_list);


    //Increment CPU cycle
    cycle++;
  } //End while loop

  return task_list;
}


vector<task> fifo(vector<task> task_list){
  vector<task> rdy_list;
  task * active = nullptr;

  cout << "FIFO scheduling results" << endl <<
        "time    cpu     ready queue (tid/rst)" << endl <<
        "----------------------------------" << endl;

  int cycle = 0;
  while(remaining_tasks(&task_list, &rdy_list)){

    //Check for tasks ready to start
    add_rdy_tasks(&task_list, &rdy_list, cycle);


    // Select active task
    if(rdy_list.size() > 0){
      active = &rdy_list[0];
    }
    else{
      active = nullptr;
    }

    // Print line
    print_ln(cycle, active, &rdy_list);

    //service task
    if(active != nullptr){
      service_tasks(&rdy_list, active, cycle);
    }

    //Check for complete tasks to move back to task list
    remove_complete_tasks(&task_list, &rdy_list);

    //Increment CPU cycle
    cycle++;
  } // End while loop

  return task_list;
}


vector<task> rr(vector<task> task_list){
  vector<task> rdy_list;
  task * active = nullptr;

  cout << "RR scheduling results (time slice is 1)" << endl <<
          "time    cpu     ready queue (tid/rst)" << endl <<
          "----------------------------------" << endl;

  int cycle = 0;
  while(remaining_tasks(&task_list, &rdy_list)){

    //Check for tasks ready to start
    add_rdy_tasks(&task_list, &rdy_list, cycle);

    // Select active task
    if(rdy_list.size() == 0){
      active == nullptr;
    }
    else if(rdy_list.size() == 1){
      active = &rdy_list[0];
    }
    else{
      for(int i = 0; i < rdy_list.size() - 1; i++){
        rdy_list.push_back(rdy_list[0]);
        rdy_list.erase(rdy_list.begin());
        active = &rdy_list[rdy_list.size() - 1];
      }
    }

    // Print line
    print_ln(cycle, active, &rdy_list);

    //service task
    if(active != nullptr){
      service_tasks(&rdy_list, active, cycle);
    }

    //Increment CPU cycle
    cycle++;

    //Check for complete tasks to move back to task list
    remove_complete_tasks(&task_list, &rdy_list);

  } // End while loop

  return task_list;
}


int main(int argc, char * argv[]){
  // Read in file and build task list
  if(argc < 2){
    cout << "Invalid CMD line arguments (\"scheduling system\" < \"file name\")" << endl;
    return 0;
  }

  string schedule = argv[1];
  char alpha[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


  int number[2];
  vector<task> task_list;
  int id_counter = 0;

  // Read in file and create task structures
  /*while (myfile >> number[1] >> number[2]) {
      task new_task;

      new_task.arrival_time = number[1];

      new_task.remaining_time = number[2];
      new_task.service_time = number[2];

      new_task.task_id = id_counter;
      id_counter++;

      task_list.push_back(new_task);
    }
    myfile.close();
    */
  string s = "";
  while(getline(cin, s)){
    int ws = 0;
    for( int i = 0; i < s.length(); i++){
      if(s[i] == ' '){
        ws = i;
      }
    }
    int num1 = stoi(s.substr(0,ws));
    int num2 = stoi(s.substr(ws, s.length() - 1));

    task new_task;

    new_task.arrival_time = num1;

    new_task.remaining_time = num2;
    new_task.service_time = num2;

    new_task.task_id = id_counter;
    id_counter++;

    task_list.push_back(new_task);

  }



  if(schedule == "-sjf"){
    task_list = sjf(task_list);
  }
  else if(schedule == "-fifo"){
    task_list = fifo(task_list);
  }
  else if(schedule == "-rr"){
    task_list = rr(task_list);
  }
  else{
    cout << "Invalid scheduling system (-fifo, -sjf, -rr)" << endl;
  }

  //Print final output
  cout << endl <<
          "        arrival service completion response wait" << endl <<
          " tid    time    time    time       time     time" << endl <<
          "------------------------------------------------" << endl;
  for(int i = 0; i < task_list.size(); i++){
    int k = 0;
    while(i != task_list[k].task_id){
      k++;
    }
    cout << setw(3) << right << alpha[task_list[k].task_id] << setw(8) << task_list[k].arrival_time << setw(8) << task_list[k].service_time << setw(8) << task_list[k].completion_time << setw(11) << task_list[k].response_time << setw(9) << task_list[k].wait_time << endl;
  }

  //Print service times in ascending order
  int max_service = 0;
  for(int i = 0; i < task_list.size(); i++){
    if(task_list[i].service_time > max_service){
      max_service = task_list[i].service_time;
    }
  }

  cout << endl <<
          "service wait" << endl <<
          " time   time" << endl <<
          "------- ----" << endl;

  for(int i = 0; i <= max_service; i++){
    for(int k = 0; k < task_list.size(); k++){
      if(task_list[k].service_time == i){
        cout << right << setw(3) << task_list[k].service_time << setw(9) << task_list[k].wait_time << endl;
      }
    }
  }

  return 0;
}
