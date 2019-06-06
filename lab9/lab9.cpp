#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
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
//to record the life of task
struct Life{
	int id;
	int release;
	int start;
	int end;
};
//use to sort
bool Risesort( Task a , Task b){
	return (a.execution_time < b.execution_time);
}
bool Risesort_dead(Task a, Task b){
	return (a.deadline <b.deadline);
}
int gcd(int a,int b)  //最大公因數
{
     int temp;
     while(a%b!=0)
     {
         temp=a%b;
         a=b;
         b=temp;
     }
     return b;
}

int lcm(int a,int b)   //最小公倍數
{
    return a*b/gcd(a,b);
}
int main(){
	freopen("input-2.txt","r",stdin);
//	freopen("output-2.txt","w",stdout);

	int time = 0;
    int processor_num,task_num;
	int remain_time;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;
    scanf("%d %d",&processor_num,&task_num);
//	remain_num = task_num;

    struct Processor processor[processor_num];
    struct Task task[task_num];
//	struct Life life[task_num];//to record the life of task
    int i,j;
    for(i = 0 ; i < processor_num ; i++){
        scanf("%d %d",&processor[i].id,&processor[i].ability);
    }
    for(i = 0 ; i < task_num ; i++){
        scanf("%d %d %d %d %d %d %d",&task[i].id,&task[i].release_time,&task[i].execution_time,&task[i].deadline,&task[i].period,&task[i].preemption,&task[i].type);
		task[i].start = -1;
		task[i].start = -2;
	}
//	for( i = 0 ; i < task_num ; i++){
//		life[task[i].id].start = -1;
//		life[task[i].id].end = -2;
//	}
//count HyperPeriod
	for( i = 0 ; i < task_num ; i++){
		cout << task[i].period <<endl;
		if(task[i].period > 0){
			cout << HyperPeriod <<endl;
			HyperPeriod = lcm(HyperPeriod,task[i].period);
		}
	}

	remain_time = HyperPeriod;
	cout << "HyperPeriod = "<<HyperPeriod<<endl;
//count Frame
	int F_eq1 = 0;
	for( i = 0 ; i < task_num ; i++){
		if(task[i].period > 0){
			if(task[i].execution_time > F_eq1){
				F_eq1 = task[i].execution_time;
				cout <<"Feq1:"<<F_eq1<<endl;
			}
		}
	}
	vector <int> F_eq2;
	for( i = F_eq1 ; i <= HyperPeriod ; i++){
		if(HyperPeriod % i == 0){
			F_eq2.push_back(i);
		}
	}
	for ( i = 0 ; i < F_eq2.size() ; i++){
		cout << "F_eq2:"<<F_eq2.at(i)<<endl;
	}
	int Frame = 0;
	int find_flag = 0;
	for( i = 0 ; i < F_eq2.size() ;i++){
		for(j = 0; j < task_num ; j++){
			if(task[j].period > 0){
				if(2*F_eq2.at(i) - gcd(task[j].period,F_eq2.at(j)) <= task[i].period){
					Frame = F_eq2.at(i);
					find_flag = 1;
				}
				else{
					find_flag = 0;
					break;
				}
			}
		}
		if(find_flag ==1){
			break;
		}
	}
	cout <<"FRame:"<<Frame<<endl;
//do while until all tasks have been done
	int remain_frame = Frame;
	vector <Task> arrive_np;
	while(remain_time > 0){
		//check which task is come
		remain_frame = Frame;
		while(remain_frame > 0){
			for( i = 0 ; i < task_num ; i++){
				if(task[i].period > 0){
					if((time-task[i].release_time) % task[i].period == 0 || time == task[i].release_time){
						arrive.push_back(task[i]);
						arrive.at(arrive.size()-1).release_time = time;
						arrive.at(arrive.size()-1).deadline = time + arrive.at(arrive.size()-1).period;
					}
				}
				else{
					if(time == task[i].release_time){
						arrive_np.push_back(task[i]);
					}
				}
			}
			if(arrive.size() <= 0 && arrive_np.size() <= 0){
				time++;
				remain_time--;
				remain_frame--;
				continue;
			}
			//put the smallest execution time at head
			if(arrive.size() > 0){
				stable_sort(arrive.begin(),arrive.end(),Risesort_dead);
			}
			if(arrive_np.size() > 0){
				stable_sort(arrive_np.begin(),arrive_np.end(),Risesort_dead);
			}
			if(arrive.size() > 0){
				if(arrive.at(0).execution_time <= remain_frame){
					struct Life life_t;
					life_t.id = arrive.at(0).id;
					life_t.release = arrive.at(0).release_time;
					life_t.start = time;
					//non preemtive->finish the task;
					while(arrive.at(0).execution_time > 1){
						remain_time--;
						remain_frame--;
						arrive.at(0).execution_time--;
						time++;
						//test is there any task release while doing the task
						for( i = 0 ; i < task_num ; i++){
							if(task[i].period > 0){
								if((time-task[i].release_time) % task[i].period == 0 || time == task[i].release_time){
									arrive.push_back(task[i]);
									arrive.at(arrive.size()-1).release_time = time;
									arrive.at(arrive.size()-1).deadline = time + arrive.at(arrive.size()-1).period;
								}
							}
							else{
								if(time == task[i].release_time){
									arrive_np.push_back(task[i]);
								}
							}
						}
					}
		
		
					//time+1 and execution time -1
					time++;
					arrive.at(0).execution_time--;
					remain_time--;
					remain_frame--;
					life_t.end = time;
		

					//if execution_time = 0 means task done and print the result
					if(arrive.at(0).execution_time == 0){
						life.push_back(life_t);
						cout << life_t.start << " task"<<life_t.id<<" "<<life_t.end<<endl;
						arrive.erase(arrive.begin());
					}
				}
				else{
					remain_frame--;
					time++;
					remain_time--;
				}
			}
			else if(arrive_np.size()>0){
				if(arrive_np.at(0).execution_time <= remain_frame){
					struct Life life_t;
					life_t.id = arrive_np.at(0).id;
					life_t.release = arrive_np.at(0).release_time;
					life_t.start = time;
					//non preemtive->finish the task;
					while(arrive_np.at(0).execution_time > 1){
						remain_time--;
						remain_frame--;
						arrive_np.at(0).execution_time--;
						time++;
						//test is there any task release while doing the task
						for( i = 0 ; i < task_num ; i++){
							if(task[i].period > 0){
								if((time-task[i].release_time) % task[i].period == 0 || time == task[i].release_time){
									arrive.push_back(task[i]);
									arrive.at(arrive.size()-1).release_time = time;
									arrive.at(arrive.size()-1).deadline = time + arrive.at(arrive.size()-1).period;
								}
							}
							else{
								if(time == task[i].release_time){
									arrive_np.push_back(task[i]);
								}
							}
						}
					}
		
		
					//time+1 and execution time -1
					time++;
					arrive_np.at(0).execution_time--;
					remain_time--;
					remain_frame--;
					life_t.end = time;
		

					//if execution_time = 0 means task done and print the result
					if(arrive_np.at(0).execution_time == 0){
						life.push_back(life_t);
						cout << life_t.start << " task"<<life_t.id<<" "<<life_t.end<<endl;
						arrive_np.erase(arrive_np.begin());
					}
				}
				else{
					remain_frame--;
					time++;
					remain_time--;
				}
		
			}


		}
		
	}
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].start - life[i].release);
		total_exe = total_exe + task[life[i].id].execution_time;
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	cout << "CPU utilization : " << fixed << setprecision(2) << (float)total_exe/HyperPeriod  << endl;

}
