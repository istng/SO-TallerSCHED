#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <deque>
#include <utility>
#include "basesched.h"

using namespace std;

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
		int intercambiarProcesos(int cpu);
		int dameNucleo();
	private:
		//first es estado, second es nucleo
		vector<pair<int, int > > procesos;
		vector<deque<int > > colas;
		vector<int > quantums_originales;
		vector<int > quantums_actuales;
};

#endif
