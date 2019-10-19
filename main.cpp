#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

struct Process {
	operator bool() {
		return remainingBurst <= 0;
	}

	int totalBurst;
	int remainingBurst;
};

using Processes = std::vector<Process>;
using Chrono = std::vector<size_t>;

struct Schedule {
	Processes processes;
	Chrono waitingTime;
	size_t totalTime = 0;
};

struct Fifo {
	void operator()(Schedule &schedule) {
		
	}
};

struct RoundRobin {
	constexpr static int quantum = 4;

	void operator()(Schedule &schedule) {
		auto &processes = schedule.processes;

		processes.front().remainingBurst -= quantum; //Applicera tidskvantum
		schedule.totalTime += quantum;

		if(int rem = processes.front().remainingBurst; rem < 0) {
			schedule.totalTime += rem;
		}

		if(processes.front() ) { //Om processen är färdigbehandlad
			//Beräkna processens väntetid
			schedule.waitingTime.push_back(schedule.totalTime - processes.front().totalBurst);
			processes.erase(processes.begin() ); //Pop front
		} else { //Annars roteras den längst bak
			std::rotate(processes.begin(), processes.begin() + 1, processes.end() );
		}
	}
};

template<typename T>
void runSchedule(Schedule &schedule, T &strategy) {

	auto &processes = schedule.processes;

	while(!processes.empty() ) {
		strategy(schedule);	//Behandla ett tick av algoritmen
	}
}

Schedule genSchedule(size_t n) {
	std::random_device rd;
	std::mt19937 mt(rd() );
	std::uniform_int_distribution<int> dist(1, 20);

	Schedule schedule;
	schedule.processes.resize(n);

	std::generate(schedule.processes.begin(), schedule.processes.end(), [&]() {
		int x = dist(mt);
		return Process{x, x};
	});

	return schedule;
}

int main() {
	size_t n = 3;

	Schedule scheduleA, scheduleB;
	scheduleA = scheduleB = genSchedule(n);

	auto printp = [](const Process pr) {
		std::cout <<  pr.totalBurst << ' ' << pr.remainingBurst << '\n';
	};

	auto printst = [](const size_t wt) {
		std::cout <<  wt << '\n';
	};

	std::for_each(scheduleA.processes.begin(), scheduleA.processes.end(), printp);
	std::for_each(scheduleA.waitingTime.begin(), scheduleA.waitingTime.end(), printst);
	puts("");
	RoundRobin rr;
	runSchedule(scheduleA, rr);
	std::for_each(scheduleA.waitingTime.begin(), scheduleA.waitingTime.end(), printst);
	//std::for_each(scheduleB.processes.begin(), scheduleB.processes.end(), print);
}
