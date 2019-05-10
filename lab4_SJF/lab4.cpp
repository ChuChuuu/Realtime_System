#include <iostream>
#include <iomanip>
#include <stdio.h>
using namespace std;
struct Processor{
	int id;
	int ability;
};

struct Task{
	int id;
	int release_t;
	int execution_t;
	int deadline;
	int period;
	int preemption;
	int type;
};
struct Life{
	int start;
	int end;
};

int main(){
	freopen("input.txt","r",stdin);
//	freopen("output-2.txt","w",stdout);	
	int time = 0;
	int processor_num,task_num,remain_num;
	int i,j;
	int shortest_exe,oldest_age,now_task;
	int now_cpu_ability;
	int test_arrive = 0;
	int test_aging = 0;
	cin >> processor_num >> task_num;
	remain_num = task_num;
	struct Processor cpu[processor_num];
	struct Task task[task_num];
	struct Life life[task_num];
	int finish_flag[task_num];//0 -> not arrive, 1 -> arrive but not exe , 2 -? has executed
	int pri_flag[task_num];//to record waiting time
	int threshold = 10;
	int waiting_time = 0 , hit_num = 0;
	//initial array
	for(i = 0 ; i < task_num ; i++){
		finish_flag[i] = 0;
		pri_flag[i] = 0;
		life[i].start = -1;
		life[i].end = -2;
	}
	for(i = 0 ; i < processor_num ; i++)
		cin >> cpu[i].id >> cpu[i].ability;

	for(i = 0 ; i < task_num ; i++)
		cin >> task[i].id >> task[i].release_t >> task[i].execution_t >> task[i].deadline >> task[i].period >> task[i].preemption >> task[i].type;

	now_cpu_ability = cpu[0].ability;
	while(remain_num > 0){
		test_arrive = 0;
		test_aging = 0;
		//put coming task into ready queue
		for(i = 0 ; i < task_num ; i++){
			if(finish_flag[i] == 0 && task[i].release_t <= time)
				finish_flag[i] = 1;
		}
		//test if any tasks in ready queue	
		for(i = 0 ; i < task_num ; i ++){
			if(finish_flag[i] == 1)
				test_arrive++;
		}
		//if there is not task in ready queue,then set the time same as lastedt task
		if(test_arrive == 0){
			int lastest_arrive = 0;
			for(i = 1 ; i < task_num ; i++){
				if(task[i].release_t < task[lastest_arrive].release_t){
					lastest_arrive = i;
				}
			}
			time = task[lastest_arrive].release_t;
			finish_flag[lastest_arrive] = 1;
		}
		cout << setw(2) << time;
		shortest_exe = 9999999;
		oldest_age = 0;
		//test aging
		for( i = 0 ; i < task_num ; i++){
			if(finish_flag[i] == 1 && (time - task[i].release_t >= threshold)){
				pri_flag[i] = time - task[i].release_t;
				test_aging++;
			}
		}
		//if task need to be executed first
		if(test_aging > 0){
			//find which task has longest waiting time(because if more than one task have high priority,choose the smallest release time
			for(i = 0 ; i < task_num ; i++){
				if(finish_flag[i] == 1 && pri_flag[i] >= oldest_age){
					now_task = i;
					oldest_age = pri_flag[i];
				}
			}

		}
		else{
			for(i = 0 ; i < task_num ; i++){
				if(finish_flag[i] == 1 && task[i].execution_t <= shortest_exe){
					if(task[i].execution_t == shortest_exe){
						if(task[i].release_t < task[now_task].release_t){
							now_task = i;
							shortest_exe = task[i].execution_t;
						}
					}
					else{
						now_task = i;
						shortest_exe = task[i].execution_t;
					}
					
				}
			}
		}
		//finish the task
		finish_flag[now_task] = 2;
		//start time (before execute)	
		life[now_task].start = time;
		cout << " Task" << setw(2) << task[now_task].id << " "; // which task
		time = time + (task[now_task].execution_t / now_cpu_ability);
		life[now_task].end = time;
		cout << setw(2) << time << endl;//end time
		
		remain_num--;

	}

/*
	for( i = 0 ; i < task_num ; i++){
		cout << i <<":"<<life[i].start << " " << life[i].end << endl;
	}
*/
	//count effectiveness
	for(i = 0 ; i < task_num ; i++){
		waiting_time = waiting_time + (life[i].end - task[i].release_t - task[i].execution_t);
		if(life[i].end <= task[i].deadline){
			hit_num++;
		}
	}
	cout << "--------------------------------"<<endl;
	cout << "Avg. Waiting time : " << (float)waiting_time/task_num << "s"<<endl;
	cout << "Hit Rate : " << (float)hit_num/task_num << endl;

	return 0;

}
