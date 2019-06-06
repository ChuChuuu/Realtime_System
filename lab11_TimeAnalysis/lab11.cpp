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
	float rate;
	float U;
};
//to record the life of task
struct Life{
	int id;
	int release;
	int start;
	int end;
};
struct Precedence{
	int early;
	int late;
};
//use to sort
bool Risesort_dead( Task a , Task b){
	return (a.deadline < b.deadline);
}
bool Risesort_id( Task a, Task b){
	return (a.id < b.id);
}
bool Risesort_exe( Task a , Task b){
	return (a.execution_time < b.execution_time);
}
bool Risesort_period( Task a, Task b){
	return (a.period < b.period);
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
void RM_P(struct Processor *processor,struct Task *task,int processor_num,int task_num);

void standard2(int *wait,int task_num , float avg){
	int i;
	float s2;
	float sigma;
	for( i = 0 ; i < task_num ; i++){
		s2 = (float)wait[i] - avg;
		s2 *= s2;
		sigma += s2;
	}
	sigma /= task_num-1;
	cout << "standard deviation :"<< sqrt(sigma)<<endl;
	cout << "variance :" << sigma <<endl;

}

int Time_analysis(struct Task *task,int processor_num,int task_num);
int main(){
	freopen("input-2.txt","r",stdin);

	int processor_num,task_num;
	int Case = 0;
	cout <<"test"<<endl;
    while(scanf("%d %d",&processor_num,&task_num) == 2){
		Case++;
		int trash;
		float U_total=0;
		float Urm=0;
		struct Processor processor[processor_num];
		struct Task task[task_num];
		int i;
		int schedule_flag = 0;
		for(i = 0 ; i < processor_num ; i++){
			scanf("%d %d",&processor[i].id,&processor[i].ability);
		}
		for(i = 0 ; i < task_num ; i++){
			scanf("%d %d %d %d %d %d %d",&task[i].id,&task[i].release_time,&task[i].execution_time,&task[i].deadline,&task[i].period,&task[i].preemption,&task[i].type);
			task[i].start = -1;
			task[i].end = -2;
		}
		scanf("%d",&trash);
		schedule_flag = Time_analysis(task,processor_num,task_num);
		if(schedule_flag == 1){
			cout <<"------------------------------------------"<<endl;
			cout << "Test case "<<Case<<" : schedulable"<<endl;
			cout <<"------------------------------------------"<<endl;
		}
		else{
		    cout <<"------------------------------------------"<<endl;
            cout << "Test case "<<Case<<" : not schedulable"<<endl;
            cout <<"------------------------------------------"<<endl;
		}
	}
}
int Time_analysis(struct Task *task,int processor_num,int task_num){
	int i;
	int all_schedule_flag = 1;
	int each_schedule_flag = 0;
	for(i = 1 ; i < task_num ; i++){
		int t,j;
		each_schedule_flag = 0;
		for(t = 1 ; t <= task[i].period ; t++){
			float Wt = task[i].execution_time;
			for( j = 0 ; j < i ; j++){
				Wt += ceil( (float)t/(float)task[j].period )*(float)task[j].execution_time;
			}
			if(Wt <= t){
				each_schedule_flag = 1;
			}
		}
		if(each_schedule_flag == 0){
			all_schedule_flag =0;
		}
	}
	if(all_schedule_flag == 0){
		return 0;
	}
	else{
		return 1;
	}
}
void RM_P(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int remain_time;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	int precedence_num = 0;
	int C=0;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
	//count HyperPeriod
	for( i = 0 ; i < task_num ; i++){
		//to avoid the situation of mod 0 or /0
		if(task[i].period > 0){
			HyperPeriod = lcm(HyperPeriod,task[i].period);
		}
	}
	remain_time = HyperPeriod;
	cout << "HyperPeriod = "<<HyperPeriod<<endl;
//do while until all tasks have been done

	while(remain_time > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			//because not every task is periodic,to avoid the situation of mod 0 or /0
			if(task[i].period > 0){
				if((time-task[i].release_time) % task[i].period == 0 || time == task[i].release_time){
					arrive.push_back(task[i]);
					arrive.at(arrive.size()-1).release_time = time;
					arrive.at(arrive.size()-1).deadline = time + arrive.at(arrive.size()-1).period;
				}
			}
			else{
				if(time == task[i].release_time){
					arrive.push_back(task[i]);
				}
			}
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			remain_time--;
			continue;
		}
		//put the smallest slack time at head
		sort(arrive.begin(),arrive.end(),Risesort_id);
		stable_sort(arrive.begin(),arrive.end(),Risesort_period);
		C = 0;

		//if start<0 means that the task haven't start
		if(arrive.at(C).start < 0){
			arrive.at(C).start = time;
		}

		if(arrive.at(C).id != lasttask){
			//if lasttask haven't  print
			if(print_flag == 0){
				cout <<setw(3)<< time_record << " task"<<lasttask<<" " << setw(3)<<time <<endl;
			}
			print_flag = 0;
			time_record = time;
		}
		//time+1 and execution time -1
		time++;
		arrive.at(C).execution_time--;
		remain_time--;
		//record lasttask
		lasttask = arrive.at(C).id;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(C).execution_time == 0){
			struct Life life_t;
			life_t.id = arrive.at(C).id;
			life_t.start = arrive.at(C).start;
			life_t.release = arrive.at(C).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<time_record << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			print_flag = 1;//test if print or not
			arrive.erase(arrive.begin()+C);
		}

	}
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);

		total_exe = total_exe + task[life[i].id].execution_time;
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	cout << "CPU utilization : " << fixed << setprecision(2) << (float)total_exe/HyperPeriod  << endl;

}
