#include <iostream>
#include <stdio.h>
using namespace std;

struct Processor{
	int id;
	int ability;
};
struct Attribute{
	int id;
	int release_time;
	int execution_time;
	int deadline;
	int period;
	int preemption;
	int task_kind;
};

int main(){
	freopen("input.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int processor_num,task_num;

	scanf("%d %d",&processor_num,&task_num);
	struct Processor processor[processor_num];
	struct Attribute attribute[task_num];
	int i,j;
	for(i = 0 ; i < processor_num ; i++){
		scanf("%d %d",&processor[i].id,&processor[i].ability);
	}
	for(i = 0 ; i < task_num ; i++){
		scanf("%d %d %d %d %d %d %d",&attribute[i].id,&attribute[i].release_time,&attribute[i].execution_time,&attribute[i].deadline,&attribute[i].period,&attribute[i].preemption,&attribute[i].task_kind);
	}

	cout << "number of process : "<< processor_num << endl;
	cout << "number of tasks : " << task_num << endl;
	for(i = 0 ; i < processor_num ; i++){
		cout << "Processor " << processor[i].id << " :ability--" << processor[i].ability << endl;
	}
	for(i = 0 ; i < task_num ; i++){
		cout << "Task " << attribute[i].id << ":";
		cout << "release time--"<<attribute[i].release_time<<",exectuion time--"<<attribute[i].execution_time<<",deadline--"<<attribute[i].deadline<<",period--"<<attribute[i].period<<",preemption--"<<attribute[i].preemption<<",type--"<<attribute[i].task_kind<<endl;
	}

	fclose(stdin);
	fclose(stdout);
	return 0;


}
