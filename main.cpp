#include <iostream>
#include <vector>
#include <cstdlib>

#include "Cell.h"
#include "SystemPam.h"
#include "Solver.h"


int main() {
	Solver solver;
	solver.SetGrid();
	solver.Solve("C:\\First_diverter_model\\test.txt");

	system("pause");
	return 0;
}
