#pragma once
#include <cmath>
#include <vector>


struct System_parameters {

	const double PI = 3.1415926535897932384626433832795;

	const int output_step = 1; //параметр, задающий шаг вывода

	const double M = 16 * 60 * output_step; // number of iterations

	const int number_layers = 3; // number of layers
	const std::vector <double> h = { 3, 12, 15 }; // height of layers - m
	const std::vector <double> k = { 5 * pow(10, -13), pow(10, -13), 5 * pow(10, -14) }; // permeability of layers - mD = 10^(-15) m^2
	const std::vector <double> skin = { 5, 10, 10 }; // skin-factor of layers

	const double cumulate_flow_rate = 5.3 * pow(10, -3); // total flow rate - m^3/sec !!!задаем в м/3 на сутки и переводим в м/3 на секунды!!!
	double delta_p = 0; //drop dpessure - Pa

	const double r_w = 76.2 * pow(10, -3); // welbore radious - m
	const double r_e = 610; // external radious - m
	const double C_div = 2 * pow(10, -3); // volume concentration of diverter - m^3/m^3
	const double rho_div = 18; //5.47 * pow(10, -4) * (h[0] + h[1] + h[2]) / cumulate_flow_rate; // mass concentration of diverter - kg/m^3 3.1 kg/m^3
	const double mu = pow(10, -3); // viscosity - Pa * sec
	//const double phi_cake = 0.9; // porosity of cake - m^3/m^3 0.1
	//const double k_cake = 2 * pow(10, -15); // permeability of cake - m^2 ---- 1mD 2 * pow(10, -14)

	const double n_perf = 12; // количество перфораций на метр
	const double r_perf = pow(10, -2); // радиус перфорации
	const double L_perf = 254 * pow(10, -3); // длина перфорации

	const double T = 16 * 60; // all time - sec
	const double tau = T / M;

	//Режим закачки в пласт: 'Q' - посто¤нный общий поток, 'P' - посто¤нное давление
	const char regime = 'Q';

	//Тип завершени¤ скважины: 'O' - open hole (необсажденна¤ скважина), 'C' - cased (обсажденна¤ скважина с перфораци¤ми)
	const char completion = 'C';
};

struct Init_conditional 
{
	double sigma_init = 0;
	double volume_init = 0;
	double resistance_init = 0;
	double pseudo_skin_init = 0;
};