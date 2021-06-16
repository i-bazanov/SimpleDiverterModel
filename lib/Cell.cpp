#include <cmath> 
#include "Solver.h"
#include "SystemPam.h"
#include "Cell.h"

Cell::Cell(const Init_conditional& init_cond, const System_parameters& params, const int& number_layer) {
	//Конструктор для создания ячейки в начальный момент времени
	sigma_init = init_cond.sigma_init;
	volume_init = init_cond.volume_init;
	resistance_init = init_cond.resistance_init;
	pseudo_skin_init = init_cond.pseudo_skin_init;

	double denominator = 0;
	for (int i = 0; i < params.number_layers; ++i) {
		denominator += ((params.k[i] * params.h[i]) / (log(params.r_e / params.r_w) + params.skin[i]));
	}
	if (params.regime == 'Q') {
		//Режим с постоянной суммарной закачной
		cumulate_flow_rate_init = params.cumulate_flow_rate;
		delta_p_init = (((params.cumulate_flow_rate * params.mu) / (2 * params.PI)) * (1 / denominator));
		flow_rate_init = ((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / (params.mu * (log(params.r_e / params.r_w) + params.skin[number_layer] + pseudo_skin_init))) * delta_p_init;
	}
	else if (params.regime == 'P') {
		//Режим с посто¤нным давлением
		delta_p_init = params.delta_p;
		cumulate_flow_rate_init = (params.delta_p * 2 * params.PI) / params.mu * denominator;
		flow_rate_init = ((2 * params.PI * params.k[number_layer] * params.h[number_layer]) / (params.mu * (log(params.r_e / params.r_w) + params.skin[number_layer] + pseudo_skin_init))) * params.delta_p;
	}
	
}


//delta_p_init = (((params.Q * params.mu) / (2 * params.PI)) * (1 / ((params.k[number_layer] * params.h[number_layer]) / (log(params.r_e / params.r_w) + params.skin[number_layer] + pseudo_skin_init))));