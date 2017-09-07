#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

#define READY_nuestro 0
#define RUNNING_nuestro 1
#define BLOCKED_nuestro -1
#define FINISHED_nuestro -2


SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de quantums_actuales y sus cpu_quantum por parámetro
	int n = argn[0];

//cout << endl;
//for (size_t i = 0; i < argn.size(); i++) {
//	cout << argn[i] << " ";
//}
//cout << endl;

	for (int i = 0; i < n; i++) {
		int quantum = argn[i+1];
		quantums_actuales.push_back(quantum);
		quantums_originales.push_back(quantum);
	}
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
	procesos.push_back(READY_nuestro);
	cola.push_back(pid);
}

void SchedRR::unblock(int pid) {
	procesos[pid] = READY_nuestro;
	cola.push_back(pid);
}


int SchedRR::intercambiarProcesos(int cpu){
	int pid;
	if(!cola.empty()){
		pid = cola.front();
		procesos[pid] = RUNNING_nuestro;
		cola.pop_front();
	} else {
		pid = IDLE_TASK;
	}
	quantums_actuales[cpu] = quantums_originales[cpu];
	return pid;
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int pid = current_pid(cpu);

	if(m == TICK){
		//Si no es el idle:
		if(pid != IDLE_TASK){
			if (quantums_actuales[cpu] > 0){
				quantums_actuales[cpu]--;
			}
			if (quantums_actuales[cpu] == 0){
				procesos[pid] = READY_nuestro;
				cola.push_back(pid);
				pid = intercambiarProcesos(cpu);

			}
		//Si es el idle:
		} else {
			pid = intercambiarProcesos(cpu);
		}
	} else if(m == BLOCK){
		procesos[pid] = BLOCKED_nuestro;
		pid = intercambiarProcesos(cpu);
	} else if(m == EXIT){
		procesos[pid] = FINISHED_nuestro;
		pid = intercambiarProcesos(cpu);
	}


	return pid;
}

