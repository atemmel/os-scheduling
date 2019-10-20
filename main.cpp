#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <random>
#include <cmath>

template<typename T>
T square(T value) {
	return value * value;
}

using Burst = int64_t;
using Chrono = uint64_t;

struct Process {
	operator bool() {
		return remainingBurst <= 0;
	}

	Burst totalBurst;
	Burst remainingBurst;
};

using Processes = std::vector<Process>;
using Chronos = std::vector<Chrono>;

struct Schedule {
	double mean() const {
		return std::accumulate(waitingTime.begin(), waitingTime.end(), 0.) 
			/ static_cast<double>(waitingTime.size() );
	}

	double stdev() const {
		const double my = mean();
		double sigma = 0.;

		for(double d : waitingTime) {
			sigma += square(d - my);
		}

		sigma = std::sqrt(sigma / static_cast<double>(waitingTime.size() ) );


		return sigma;
	}

	Processes processes;
	Chronos waitingTime;
	size_t totalTime = 0;
};

struct Fifo {
	void operator()(Schedule &schedule) {
		schedule.totalTime += schedule.processes.front().totalBurst;
		schedule.waitingTime.push_back(schedule.totalTime - schedule.processes.front().totalBurst);
		schedule.processes.erase(schedule.processes.begin() );
	}
};

struct RoundRobin {
	constexpr static Burst quantum = 4;

	void operator()(Schedule &schedule) {
		auto &processes = schedule.processes;

		processes.front().remainingBurst -= quantum; //Applicera tidskvantum
		schedule.totalTime += quantum;

		if(Burst rem = processes.front().remainingBurst; rem < 0) {
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

Schedule generateSchedule(size_t n) {
	std::random_device rd;
	std::mt19937 mt(rd() );
	std::uniform_int_distribution<Burst> dist(1, 20);

	Schedule schedule;
	schedule.processes.resize(n);

	std::generate(schedule.processes.begin(), schedule.processes.end(), [&]() {
		Burst x = dist(mt);
		return Process{x, x};
	});

	return schedule;
}

int main() {
	std::ofstream fifoFile("fifo.csv"), rrFile("rr.csv");
	if(!fifoFile.is_open() || !rrFile.is_open() ) {
		std::cerr << "File could not be opened, exiting...\n";
		std::exit(EXIT_FAILURE);
	}

	constexpr size_t n = 1e3;
	constexpr size_t m = 1e3;

	std::cout << "N is " << n << '\n';
	std::cout << "M is " << m << '\n';

	for(size_t i = 0; i < m; i++) {
		Schedule scheduleA, scheduleB;
		scheduleA = scheduleB = generateSchedule(n);

		Fifo fifo;
		runSchedule(scheduleA, fifo);
		RoundRobin rr;
		runSchedule(scheduleB, rr);

		const double fifoMean = scheduleA.mean();
		const double rrMean = scheduleB.mean();
		fifoFile << fifoMean << '\n';
		rrFile << rrMean << '\n';

		std::cout << "i is " << i << '\n';
		std::cout << "Round Robin mean: " << scheduleA.mean() << '\n';
		std::cout << "First-in First-out mean: " << scheduleB.mean() << "\n\n";
	}
}
