#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

struct Processor{
    int id;
    int ability;
};
struct Task{
    int id;
    int release_time;
    int execution_time;
    int deadline;
    int period;
    int preemption;
    int type;
	int start;
	int end;
};
struct Life{
	int start;
	int end;
};
bool Risesort( Task a , Task b){
	return (a.execution_time < b.execution_time);
}

int main(){
	freopen("input-2.txt","r",stdin);
//	freopen("output-2.txt","w",stdout);

	int time = 0;
    int processor_num,task_num,remain_num;
	int shortest,nowtask=-1,oldtask=-2,nowremain=0;
	int lasttask=0;
	int time_record = 0;
	int waiting_t = 0 , hit_num = 0;
	vector <Task> arrive;

    scanf("%d %d",&processor_num,&task_num);
	remain_num = task_num;

    struct Processor processor[processor_num];
    struct Task task[task_num];
	struct Life life[task_num];//to record the life of task
    int i;
    for(i = 0 ; i < processor_num ; i++){
        scanf("%d %d",&processor[i].id,&processor[i].ability);
    }
    for(i = 0 ; i < task_num ; i++){
        scanf("%d %d %d %d %d %d %d",&task[i].id,&task[i].release_time,&task[i].execution_time,&task[i].deadline,&task[i].period,&task[i].preemption,&task[i].type);
    }
	for( i = 0 ; i < task_num ; i++){
		life[task[i].id].start = -1;
		life[task[i].id].end = -2;
	}

	while(remain_num > 0){
		for( i = 0 ; i < task_num ; i++){
			if(task[i].release_time == time){
				arrive.push_back(task[i]);
			}
		}
		
		sort(arrive.begin(),arrive.end(),Risesort);
//context switch
		if(arrive.at(0).id != lasttask && (life[arrive.at(0).id].start >= 0 || life[lasttask].end < 0)){
			if(life[lasttask].end < 0){
				cout << time_record <<" task" <<lasttask<< " " <<time<<endl;
			}
			cout << time << " context switch ";
			time++;
			cout << time << endl;
			time_record = time;
			lasttask = arrive.at(0).id;
			continue;
		}
		if(life[arrive.at(0).id].start < 0){
			life[arrive.at(0).id].start = time;
		}
		
		time++;
		lasttask = arrive.at(0).id;
		arrive.at(0).execution_time--;
		if(arrive.at(0).execution_time == 0){
			life[arrive.at(0).id].end = time;
			remain_num--;
			cout << time_record << " task"<<arrive.at(0).id<<" "<<time<<" *finish*"<<endl;
		time_record = time;
			arrive.erase(arrive.begin());
		}
		
	}
	for(i = 0 ; i < task_num ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - task[i].release_time - task[i].execution_time);
		if(life[i].end <= task[i].deadline){
			hit_num++;
		}
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << (float)waiting_t/task_num <<"s"<<endl;
	cout << "Hit Rate : " << (float)hit_num/task_num << endl;
}
