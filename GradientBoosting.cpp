#include <vector>
#include "SolveTree.cpp"

class GradientBoosting {
    std::vector<SolveTree> trees;
    
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
};

