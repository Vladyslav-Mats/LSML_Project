#include <vector>
#include <string>
#include "SolveTree.cpp"

class GradientBoosting {
    std::vector<SolveTree> trees;
    double learning_rate;
    int tree_number;
    int tree_size;
    
    
public:
    GradientBoosting(double learning_rate, int tree_number, int tree_size) {
        this->learning_rate = learning_rate;
        this->tree_number = tree_number;
        this->tree_size = tree_size;
    }
    
    GradientBoosting(std::string model_path) {
        // restore saved model
    }
    
    void fit(const Dataset& dataset) {}
    
    std::vector<Prediction> predict(const Dataset& dataset) {
        std::vector<Prediction> predictions(dataset.get_size());
        for (SolveTree solve_tree : trees) {
            std::vector<Prediction> predictions_for_tree = solve_tree.predict(dataset);
            
            for (int i = 0; i < predictions.size(); ++i) {
                predictions[i] += predictions_for_tree[i];
            }
        }
        return predictions;
    }
    
    void save_model(std::string save_path) {
        // save model
    }
};

