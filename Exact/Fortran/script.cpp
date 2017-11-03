#include <iostream>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <stdlib.h>

using namespace std::chrono;
using namespace std;

int main(){
	steady_clock::time_point t1;
	steady_clock::time_point t2;
	duration<double> time_span;
	ofstream resultsFile;
	resultsFile.open ("results.txt");

	t1 = steady_clock::now();
	system("./program < tai12a.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai12a time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai12b.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai12b time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai15a.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai15a time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai15b.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai15b time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai17a.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai17a time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai20a.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai20a time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai20b.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai20b time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai25a.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai25a time: " << time_span.count() << "seconds.\n";

	t1 = steady_clock::now();
	system("./program < tai25b.dat");
	t2 = steady_clock::now();

	time_span = duration_cast<duration<double>>(t2 - t1);
	resultsFile << "Tai25b time: " << time_span.count() << "seconds.\n";

	resultsFile.close();

	return 0;
}