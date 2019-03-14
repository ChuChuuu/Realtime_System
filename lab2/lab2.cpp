#include <iostream>
#include <stdio.h>
#include <vector>
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


int main(){
	freopen("input-2.txt","r",stdin);
	freopen("output-2.txt","w",stdout);

	int time = 0;
    int processor_num,task_num,remain_num;
	int shortest,nowtask,nowremain=0;
	vector <Task> arrive;

    scanf("%d %d",&processor_num,&task_num);
	remain_num = task_num;

    struct Processor processor[processor_num];
    struct Task task[task_num];
    int i,j,k=50;
    for(i = 0 ; i < processor_num ; i++){
        scanf("%d %d",&processor[i].id,&processor[i].ability);
    }
    for(i = 0 ; i < task_num ; i++){
        scanf("%d %d %d %d %d %d %d",&task[i].id,&task[i].release_time,&task[i].execution_time,&task[i].deadline,&task[i].period,&task[i].preemption,&task[i].type);
    }

	while(remain_num > 0){
		for( i = 0 ; i < task_num ; i++){
//			cout << "release " << task[i].release_time << endl;
			if(task[i].release_time == time){
				arrive.push_back(task[i]);
//				cout << "hi" <<endl;
			}
		}
		shortest = 9999;
		
		if(nowremain <= 0){
//			cout <<"size " << arrive.size() << endl;
			if(arrive.size() > 0){
				for( i = 0 ; i < arrive.size() ; i++){
					if(arrive.at(i).execution_time < shortest){
						shortest = arrive.at(i).execution_time;
						j = i;
						nowtask = arrive.at(i).id;
					}
				}
//				cout << "now task " << nowtask << endl;
				nowremain = shortest;
				vector<Task>::iterator it = arrive.begin() + j;
				arrive.erase(it);
				task[nowtask].start = time;
				cout <<task[nowtask].start << " task"<< task[nowtask].id << " ";  
			}

		}
		else{
			nowremain--;
			if(nowremain == 0){
				task[nowtask].end = time;
				cout << task[nowtask].end << endl;
				remain_num--;
				if(arrive.size() > 0){
                for( i = 0 ; i < arrive.size() ; i++){
                    if(arrive.at(i).execution_time < shortest){
                        shortest = arrive.at(i).execution_time;
                        j = i;
                        nowtask = arrive.at(i).id;
                    }
                }
  //              cout << "now task " << nowtask << endl;
                nowremain = shortest;
                vector<Task>::iterator it = arrive.begin() + j;
                arrive.erase(it);
                task[nowtask].start = time;
				cout <<task[nowtask].start << " task"<< task[nowtask].id << " ";
            }
			}
		}
		
		time ++;
//		cout << time <<endl;

	}
	


}
