#ifndef GR_BOOST_H
#define GR_BOOST_H

#include <vector>
#include <string>
#include <set>
#include <random>
#include "Dataset.h"

class WeakClassifier {
public:
	size_t depth_;
	std::vector<size_t> splitting_features_;
	std::vector<double> leaf_answers_;
	WeakClassifier(size_t depth);
	std::vector<double> Predict(const Dataset& ds);
};

class GradientBoosting {
public:
	GradientBoosting(double learning_rate, size_t depth, int tree_size);
	GradientBoosting(std::string model_path);

	void Fit(const Dataset& ds);
	std::vector<double> Predict(const Dataset& dataset);
	void SaveModel(std::string save_path);
private:
	std::vector<WeakClassifier> trees_;
	double learning_rate_;
	int tree_number_;
	size_t depth_;
};
#undef GRADIENT_BOOSTING_H
#endif
