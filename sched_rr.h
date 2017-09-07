#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <deque>
#include "basesched.h"

using namespace std;

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
		int intercambiarProcesos(int cpu);
	private:
		vector<int > procesos;
		deque<int > cola;
		vector<int > quantums_originales;
		vector<int > quantums_actuales;
};

#endif
