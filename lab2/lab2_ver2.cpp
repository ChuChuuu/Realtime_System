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

int main(){
	freopen("input-2.txt","r",stdin);
	freopen("output-2.txt","w",stdout);	
	int time = 0;
	int processor_num,task_num,remain_num;
	int i,j;
	int shortest_exe,now_task;
	int now_cpu_ability;
	int test_arrive = 0;
	cin >> processor_num >> task_num;
	remain_num = task_num;
	struct Processor cpu[processor_num];
	struct Task task[task_num];
	int finish_flag[task_num];//0 -> not arrive, 1 -> arrive but not exe , 2 -? has executed
	for(i = 0 ; i < task_num ; i++) finish_flag[i] = 0;
	for(i = 0 ; i < processor_num ; i++)
		cin >> cpu[i].id >> cpu[i].ability;

	for(i = 0 ; i < task_num ; i++)
		cin >> task[i].id >> task[i].release_t >> task[i].execution_t >> task[i].deadline >> task[i].period >> task[i].preemption >> task[i].type;

	now_cpu_ability = cpu[0].ability;
	while(remain_num > 0){
		test_arrive = 0;
		for(i = 0 ; i < task_num ; i++){
			if(finish_flag[i] == 0 && task[i].release_t <= time)
				finish_flag[i] = 1;
//			cout << "fin" << i << ":"<<finish_flag[i] << endl;
		}
		
		for(i = 0 ; i < task_num ; i ++){
			if(finish_flag[i] == 1)
				test_arrive++;
		}
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
		finish_flag[now_task] = 2;
		cout << " Task" << setw(2) << task[now_task].id << " "; // whidh task
		time = time + (task[now_task].execution_t / now_cpu_ability);
		cout << setw(2) << time << endl;//end time
		
		remain_num--;

	}

	return 0;

}
