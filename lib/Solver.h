#pragma once
#include <string>
#include "SystemPam.h"
#include "Cell.h"


//Солвер
class Solver {
public:
	//Текущее время - параметр расчета солвера
	double current_time = 0;

	//Создание сетки
	void SetGrid();

	//Вычисление солвера и вывод данных в файл
	void Solve(std::string const path);

private:
	//Подсчет площади поверхности осаждения частиц для i-го пласта
	double CalcArea(const int& number_layer);

	//Подсчет массы или объема осажденного отклонител¤ на единцу площади поверхности на i-ом пласте в текущий момент времени
	double CalcSigma(const int& number_layer);

	//Подсчет объема, закаченного в i-ый пласт в текущий момент времени
	double CalcVolume(const int& number_layer);

	//Подсчет сопротивлени¤, полученного за счет отклонител¤ на i-ом пласте в текущий момент времени
	double CalcResistance(const int& number_layer);

	//Подсчет псевдо-скина, полученного на i-ом пласте в текущий момент времени
	double CalcPseudoSkin(const int& number_layer);

	//Подсчет давления в текущий момент времени
	double CalcDeltaP();

	//Подсчет общего потока (во все слои) в текущий момент времени
	double CalcCumulFlowRate();

	//Подсчет потока в i-ый слой в текущий момент времени
	double CalcFlowRate(const int& number_layer);

	//Подсчет всех величин в текущий момент времени для всех слоев
	void CalcAll();

	//Обновление параметров для просчета следующего шага
	void UpdateAll();

	//Сетка - вектор из всех ячеек рассчетов
	std::vector <Cell> cells;

	//Входные параметры модели
	System_parameters params;

	//Начальные условия расчетов
	Init_conditional init_cond;
};