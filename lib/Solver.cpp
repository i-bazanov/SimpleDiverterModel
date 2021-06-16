#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include "Solver.h"
#include "SystemPam.h"
#include "Cell.h"


void Solver::SetGrid() {
	//Создание сетки ячеек - вектора из ячеек, каждая из которых соответствует своему пласту
	for (int i = 0; i < params.number_layers; ++i) {
		Cell current_cell(init_cond, params, i);
		cells.push_back(current_cell);
	}
}

double Solver::CalcArea(const int& number_layer) {
	//Подсчет площади поверхности осаждения частиц дл¤ i-го пласта
	if (params.completion == 'O') {
		return (2 * params.PI * params.r_w * params.h[number_layer]);
	}
	else if (params.completion == 'C') {
		return (params.n_perf * params.h[number_layer] * 2 * params.PI * params.r_perf * params.L_perf);
	}
}

double Solver::CalcSigma(const int& number_layer) {
	//Подсчет массы или объема осажденного отклонителя на единцу площади поверхности на i-ом пласте в текущий момент времени
	return (((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / (params.mu * (log(params.r_e / params.r_w) + params.skin[number_layer] + cells[number_layer].pseudo_skin_init))) *
		(params.rho_div / CalcArea(number_layer) * cells[number_layer].delta_p_init) * (params.tau) + cells[number_layer].sigma_init);
}

double Solver::CalcVolume(const int& number_layer) {
	//Подсчет объема, закаченного в i-ый пласт в текущий момент времени
	return ((cells[number_layer].sigma_final * CalcArea(number_layer)) / params.rho_div);
}

double Solver::CalcResistance(const int& number_layer) {
	return (5 * pow(10, 12) * cells[number_layer].sigma_final);
}

//double Solver::CalcResistance(const int& number_layer) {
// //Подсчет сопротивления, полученного за счет отклонителя на i-ом пласте в текущий момент времени
// return (params.C_div / ((1 - params.phi_cake) * params.k_cake) * (cells[number_layer].sigma_final / params.C_div));
//}

double Solver::CalcPseudoSkin(const int& number_layer) {
	//Подсчет псевдо-скина, полученного на i-ом пласте в текущий момент времени
	return ((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / CalcArea(number_layer) * cells[number_layer].resistance_final);
}

double Solver::CalcDeltaP() {
	//Подсчет давления в текущий момент времени
	if (params.regime == 'Q') {
		double denominator = 0;
		for (int i = 0; i < params.number_layers; ++i) {
			denominator += ((params.k[i] * params.h[i]) / (log(params.r_e / params.r_w) + params.skin[i] + cells[i].pseudo_skin_final));
		}
		return (((params.cumulate_flow_rate * params.mu) / (2 * params.PI)) * (1 / denominator));
	}
	else if (params.regime == 'P') {
		return params.delta_p;
	}
}

double Solver::CalcCumulFlowRate() {
	//Подсчет общего потока в текущий момент времени
	if (params.regime == 'Q') {
		return params.cumulate_flow_rate;
	}
	else if (params.regime == 'P') {
		double denominator = 0;
		for (int i = 0; i < params.number_layers; ++i) {
			denominator += ((params.k[i] * params.h[i]) / (log(params.r_e / params.r_w) + params.skin[i] + cells[i].pseudo_skin_final));
		}
		return ((params.delta_p * 2 * params.PI) / params.mu * denominator);
	}
}

double Solver::CalcFlowRate(const int& number_layer) {
	//Подсчет потока в i-ый слой в текущий момент времени
	if (params.regime == 'Q') {
		return (((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / (params.mu * (log(params.r_e / params.r_w) + params.skin[number_layer] + cells[number_layer].pseudo_skin_final))) * cells[number_layer].delta_p_final);
	}
	else if (params.regime == 'P') {
		return (((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / (params.mu * (log(params.r_e / params.r_w) + params.skin[number_layer] + cells[number_layer].pseudo_skin_final))) * params.delta_p);
	}
}

void Solver::CalcAll() {
	//Подсчет всех величин в какой-то момент времени для всех слоев
	for (int i = 0; i < params.number_layers; ++i) {
		cells[i].sigma_final = CalcSigma(i);
		cells[i].volume_final = CalcVolume(i);
		cells[i].resistance_final = CalcResistance(i);
		cells[i].pseudo_skin_final = CalcPseudoSkin(i);
	}

	for (int i = 0; i < params.number_layers; ++i) {
		cells[i].delta_p_final = CalcDeltaP();
		cells[i].cumulate_flow_rate_final = CalcCumulFlowRate();
		cells[i].flow_rate_final = CalcFlowRate(i);
	}
}

void Solver::UpdateAll() {
	//Обновление параметров дл¤ просчета следующего шага
	for (int i = 0; i < params.number_layers; ++i) {
		cells[i].sigma_init = cells[i].sigma_final;
		cells[i].volume_init = cells[i].volume_final;
		cells[i].resistance_init = cells[i].resistance_final;
		cells[i].pseudo_skin_init = cells[i].pseudo_skin_final;
		cells[i].delta_p_init = cells[i].delta_p_final;
		cells[i].cumulate_flow_rate_init = cells[i].cumulate_flow_rate_final;
		cells[i].flow_rate_init = cells[i].flow_rate_final;
	}
}

void Solver::Solve(const std::string path) {
	//Вычисление солвера и вывод данных в файл
	std::ofstream out(path, std::ios::app);

	if (out.is_open()) {
		out << "time, sec; ";

		for (int i = 0; i < params.number_layers; ++i) {
			out << "cumulate flow rate, m^3/sec; "
				<< "flow rate " << i + 1 << ", m^3/day;"
				<< "pressure, Pa; "
				<< "pressure, bar; "
				<< "sigma " << i + 1 << ", m^3/m^2; "
				<< "volume " << i + 1 << ", m^3; "
				<< "resistance " << i + 1 << ", m^(-1); "
				<< "pseudo-skin " << i + 1 << "; ";
		}
		out << std::endl;
		out << current_time << " ; ";

		for (int i = 0; i < params.number_layers; ++i) {
			out << cells[i].cumulate_flow_rate_init << " ; "
				<< cells[i].flow_rate_init * 86400 << " ; "
				<< cells[i].delta_p_init << " ; "
				<< cells[i].delta_p_init * pow(10, -5) << " ; "
				<< cells[i].sigma_init << " ; "
				<< cells[i].volume_init << " ; "
				<< cells[i].resistance_init << " ; "
				<< cells[i].pseudo_skin_init << "; ";
		}
		out << std::endl;

		std::cout << "Calculating ..." << std::endl;
		// flag = 1;
		for (int step = 0; step < params.M; ++step) {
			CalcAll();

			//увеличение общего времени
			current_time += params.tau;
			if (step % params.output_step == 0) {
				out << current_time << " ; ";

				for (int i = 0; i < params.number_layers; ++i) {
					out << cells[i].cumulate_flow_rate_final << " ; "
						<< cells[i].flow_rate_final * 86400 << " ; "
						<< cells[i].delta_p_final << " ; "
						<< cells[i].delta_p_final * pow(10, -5) << " ; "
						<< cells[i].sigma_final << " ; "
						<< cells[i].volume_final << " ; "
						<< cells[i].resistance_final << " ; "
						<< cells[i].pseudo_skin_final << "; ";

					//if (cells[i].cumulate_flow_rate_final < 6.62 * pow(10, -4)) {
					// flag = 0;
					//}
				}
				out << std::endl;
			}

			UpdateAll();
			//if (flag == 0) {
			// break;
			//}
		}
	}
	out.close();
	std::cout << "Solver finished!!!" << std::endl;
}

//void Solver::SetSystemParams(std::string &namefile) {
// // загружаешь данные в структуру sys_params
//}

