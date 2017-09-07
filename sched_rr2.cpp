#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

#define READY_nuestro 0
#define RUNNING_nuestro 1
#define BLOCKED_nuestro -1
#define FINISHED_nuestro -2


SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de quantums_actuales y sus cpu_quantum por parámetro
	int n = argn[0];

	for (size_t i = 0; i < size_t(n); i++) {
		int quantum = argn[i+1];
		quantums_actuales.push_back(quantum);
		quantums_originales.push_back(quantum);

		deque<int >* cola = new deque<int >();
		colas.push_back(*cola);
	}
}

SchedRR2::~SchedRR2() {
	for (size_t i = 0; i < colas.size(); i++) {
		delete &colas[i];
	}
}

int SchedRR2::dameNucleo(){
	int min,cpu = 0;
	bool primero = true;
	for (size_t i = 0; i < colas.size(); ++i){
		int count = 0;
		for (size_t j = 0; j < procesos.size(); ++j){
			if (procesos[j].first != FINISHED_nuestro && procesos[j].second == int(i) ){
				count++;
			}
		}
		if (count < min || primero){
			min = count;
			cpu = i;
			primero = false;
		}
	}

	return cpu;
}


void SchedRR2::load(int pid) {
	int cpu = dameNucleo();
	procesos.push_back(make_pair(READY_nuestro,cpu));
	colas[cpu].push_back(pid);
}

void SchedRR2::unblock(int pid) {
	procesos[pid].first = READY_nuestro;
	colas[procesos[pid].second].push_back(pid);
}


int SchedRR2::intercambiarProcesos(int cpu){
	int pid;
	if(!colas[cpu].empty()){
		pid = colas[cpu].front();
		procesos[pid].first = RUNNING_nuestro;
		colas[cpu].pop_front();
	} else {
		pid = IDLE_TASK;
	}
	quantums_actuales[cpu] = quantums_originales[cpu];
	return pid;
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	int pid = current_pid(cpu);

	if(m == TICK){
		//Si no es el idle:
		if(pid != IDLE_TASK){
			if (quantums_actuales[cpu] > 0){
				quantums_actuales[cpu]--;
			}
			if (quantums_actuales[cpu] == 0){
				procesos[pid].first = READY_nuestro;
				colas[cpu].push_back(pid);
				pid = intercambiarProcesos(cpu);
			}
		//Si es el idle:
		} else {
			pid = intercambiarProcesos(cpu);
		}
	} else if(m == BLOCK){
		procesos[pid].first = BLOCKED_nuestro;
		pid = intercambiarProcesos(cpu);
	} else if(m == EXIT){
		procesos[pid].first = FINISHED_nuestro;
		pid = intercambiarProcesos(cpu);
	}


	return pid;
}

