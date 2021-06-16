#pragma once
#include "SystemPam.h"

class Cell {
public:
	//Начальный и конечный суммарный поток
	double cumulate_flow_rate_init;
	double cumulate_flow_rate_final;

	//Начальное и конечное изменение давление в ячейке за текущий шаг
	double delta_p_init; 
	double delta_p_final;

	//Начальный и конечный поток в ячейке за текущий шаг
	double flow_rate_init;
	double flow_rate_final;

	//Начальный и конечный объем или масса на ед. площади поверхности пласта в ячейке за текущий шаг
	double sigma_init;
	double sigma_final;

	//Начальный и конечный объем, закаченный в ячейки пласта за текущий шаг
	double volume_init;
	double volume_final;

	//Начальное и конечное сопротивление в ячейке за текущий шаг
	double resistance_init;
	double resistance_final;

	//Начальный и конечный псевдо-скин в ячейке за текущий шаг
	double pseudo_skin_init;
	double pseudo_skin_final;

	//Конструктор для создания ячейки в начальный момент времени
	Cell(const Init_conditional& cond, const System_parameters& param, const int& number_layer);
};
