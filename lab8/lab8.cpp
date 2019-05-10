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
	int slack;
};
//to record the life of task
struct Life{
	int id;
	int release;
	int start;
	int end;
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
bool Risesort_slack( Task a, Task b){
	return (a.slack < b.slack);
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
void EDF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num);
void EDF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num);
void SJF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num);
void SJF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num);
void LSTF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num);
void LSTF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num);

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


int main(){
	freopen("input.txt","r",stdin);

    int processor_num,task_num;
	int remain_time;
    scanf("%d %d",&processor_num,&task_num);

    struct Processor processor[processor_num];
    struct Task task[task_num];
    int i;
    for(i = 0 ; i < processor_num ; i++){
        scanf("%d %d",&processor[i].id,&processor[i].ability);
    }
    for(i = 0 ; i < task_num ; i++){
        scanf("%d %d %d %d %d %d %d",&task[i].id,&task[i].release_time,&task[i].execution_time,&task[i].deadline,&task[i].period,&task[i].preemption,&task[i].type);
		task[i].start = -1;
		task[i].end = -2;
    }
	printf("***EDF_P***\n");
	EDF_P(processor,task,processor_num,task_num);
	printf("***EDF_NP***\n");
	EDF_NP(processor,task,processor_num,task_num);
	printf("***SJF_P***\n");
	SJF_P(processor,task,processor_num,task_num);
	printf("***SJF_NP***\n");
	SJF_NP(processor,task,processor_num,task_num);
	printf("***LSTF_P***\n");
	LSTF_P(processor,task,processor_num,task_num);
	printf("***LSTF_NP***\n");
	LSTF_NP(processor,task,processor_num,task_num);

}

void EDF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int remain_time;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			remain_time--;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_dead);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}
	
		if(arrive.at(0).id != lasttask){
			//if lasttask haven't  print
			if(print_flag == 0){
				cout <<setw(3)<< time_record << " task"<<lasttask<<" " << setw(3)<<time <<endl;
			}
			print_flag = 0;
			time_record = time;
		}
		//time+1 and execution time -1
		time++;
		arrive.at(0).execution_time--;
		remain_time--;
		//record lasttask
		lasttask = arrive.at(0).id;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<time_record << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			print_flag = 1;//test if print or ont
			arrive.erase(arrive.begin());
		}
		
	}
	int miss = 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
		wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end >task[life[i].id].deadline){
			miss++;
		}
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;

}
void EDF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_dead);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}

		while(arrive.at(0).execution_time > 1){
			//time+1 and execution time -1
			time++;
			arrive.at(0).execution_time--;
			//test is there any task release while doing the task
			for(i = 0 ; i < task_num ; i++){
				if(time == task[i].release_time){
					arrive.push_back(task[i]);
				}
			}
		}
		
		time++;
		arrive.at(0).execution_time--;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<life_t.start << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			arrive.erase(arrive.begin());
		}
		
	}
	int miss = 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
        wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);  
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end > task[life[i].id].deadline){
            miss++;
        }
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;

}
void SJF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	int first_flag=0;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_exe);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}
	
		if(arrive.at(0).id != lasttask && first_flag == 1){
			//if lasttask haven't  print
			if(print_flag == 0){
				cout <<setw(3)<< time_record << " task"<<lasttask<<" " << setw(3)<<time <<endl;
			}
			print_flag = 0;
			time_record = time;
		}
		first_flag = 1;
		//time+1 and execution time -1
		time++;
		arrive.at(0).execution_time--;
		//record lasttask
		lasttask = arrive.at(0).id;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<time_record << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			print_flag = 1;//test if print or ont
			arrive.erase(arrive.begin());
		}
		
	}
	int miss= 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
        wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);  
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end > task[life[i].id].deadline){
            miss++;
        }
	}
	cout << "-------------------------------------------"<<endl;
	cout << life.size()<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;

}
void SJF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_exe);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}

		while(arrive.at(0).execution_time > 1){
			//time+1 and execution time -1
			time++;
			arrive.at(0).execution_time--;
			//test is there any task release while doing the task
			for(i = 0 ; i < task_num ; i++){
				if(time == task[i].release_time){
					arrive.push_back(task[i]);
				}
			}
		}
		
		time++;
		arrive.at(0).execution_time--;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<life_t.start << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			arrive.erase(arrive.begin());
		}
		
	}
	int miss = 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
        wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);  
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end > task[life[i].id].deadline){
            miss++;
        }
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;

}
void LSTF_P(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int remain_time;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//count slack time
		for( i = 0 ; i < arrive.size() ; i++){
			arrive.at(i).slack = arrive.at(i).deadline - time - arrive.at(i).execution_time;
		}
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			remain_time--;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_slack);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}
	
		if(arrive.at(0).id != lasttask){
			//if lasttask haven't  print
			if(print_flag == 0){
				cout <<setw(3)<< time_record << " task"<<lasttask<<" " << setw(3)<<time <<endl;
			}
			print_flag = 0;
			time_record = time;
		}
		//time+1 and execution time -1
		time++;
		arrive.at(0).execution_time--;
		remain_time--;
		//record lasttask
		lasttask = arrive.at(0).id;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<time_record << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			print_flag = 1;//test if print or ont
			arrive.erase(arrive.begin());
		}
		
	}
	int miss = 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
        wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);  
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end > task[life[i].id].deadline){
			miss++;
		}
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;



}

void LSTF_NP(struct Processor *processor,struct Task *task,int processor_num,int task_num){
	int time = 0;
	int lasttask=0;
	int time_record = 0;
	int HyperPeriod = 1;
	int total_exe = 0;
	int waiting_t = 0 , hit_num = 0;
	int print_flag = 0;
	int remain_job;
	//ready queue
	vector <Task> arrive;
	vector <Life> life;

	remain_job = task_num;
    int i;
//do while until all tasks have been done
	while(remain_job > 0){
		//check which task is come
		for( i = 0 ; i < task_num ; i++){
			if(time == task[i].release_time){
				arrive.push_back(task[i]);
			}
		}
		//count slack time
        for( i = 0 ; i < arrive.size() ; i++){
            arrive.at(i).slack = arrive.at(i).deadline - time - arrive.at(i).execution_time;
        }
		//if no task ->time++
		if(arrive.size() <= 0){
			time++;
			continue;
		}
		//put the smallest  deadline at head
		sort(arrive.begin(),arrive.end(),Risesort_id);	
		stable_sort(arrive.begin(),arrive.end(),Risesort_slack);
		
		//if start<0 means that the task haven't start
		if(arrive.at(0).start < 0){
			arrive.at(0).start = time;
		}

		while(arrive.at(0).execution_time > 1){
			//time+1 and execution time -1
			time++;
			arrive.at(0).execution_time--;
			//test is there any task release while doing the task
			for(i = 0 ; i < task_num ; i++){
				if(time == task[i].release_time){
					arrive.push_back(task[i]);
				}
			}
		}
		
		time++;
		arrive.at(0).execution_time--;

		//if execution_time = 0 means task done and print the result
		if(arrive.at(0).execution_time == 0){
			remain_job--;
			struct Life life_t;
			life_t.id = arrive.at(0).id;
			life_t.start = arrive.at(0).start;
			life_t.release = arrive.at(0).release_time;
			life_t.end = time;
			life.push_back(life_t);
			cout << setw(3)<<life_t.start << " task"<<life_t.id<<" "<<setw(3)<<life_t.end<<endl;
			arrive.erase(arrive.begin());
		}
		
	}
	int miss = 0;
	int wait[task_num];
	for(i = 0 ; i < life.size() ; i++){
		//count waiting
		waiting_t = waiting_t + (life[i].end - life[i].release - task[life[i].id].execution_time);
        wait[life[i].id]=(life[i].end - life[i].release - task[life[i].id].execution_time);  
		total_exe = total_exe + task[life[i].id].execution_time;
		if(life[i].end > task[life[i].id].deadline){
            miss++;
        }
	}
	cout << "-------------------------------------------"<<endl;
	cout << "Avg. Waiting Time : " << fixed << setprecision(2) << (float)waiting_t/life.size()  <<"s"<<endl;
	standard2(wait,task_num,(float)waiting_t/life.size());
	cout << "Miss Rate : " << fixed << setprecision(2) << (float)miss/task_num  << endl;


}

