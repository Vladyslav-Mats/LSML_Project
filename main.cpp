#include <iostream>
#include <string>
#include "GradientBoosting.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Expected mode: fit or predict" << std::endl;
		return 0;
	}
    if (std::string(argv[1]) == "fit") {
        if (argc != 7) {
            std::cerr << "Expected 5 arguments after fit: csv path, learning rate, tree number, tree depth and save path" << std::endl;
			return 0;
        }
        
        std::string csv_path = argv[2];
        double learning_rate = atof(argv[3]);
        int tree_number = atoi(argv[4]);
        int tree_size = atoi(argv[5]);
        std::string save_path = argv[6];
        
        Dataset dataset(csv_path);
        GradientBoosting boosting_instance(learning_rate, tree_number, tree_size);
        boosting_instance.Fit(dataset);
        boosting_instance.SaveModel(save_path);
    } else if (std::string(argv[1]) == "predict") {
        if (argc != 5) {
            std::cerr << "Expected 3 arguments after predict: csv path, model path and results path" << std::endl;
			return 0;
        }
        
        std::string csv_path = argv[2];
        std::string model_path = argv[3];
        std::string results_path = argv[4];
        
        Dataset dataset(csv_path);
        GradientBoosting boosting_instance(model_path);
        std::vector<double> prediction = boosting_instance.Predict(dataset);
        std::ofstream out(results_path);

        for (auto y : prediction) {
            out << y << " " << std::endl;
        }
	}
	else {
		std::cerr << "Wrong mode: needed fit or predict.\n";
	}
	return 0;
}
