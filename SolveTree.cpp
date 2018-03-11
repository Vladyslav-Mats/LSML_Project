#include <vector>
#include "DataObject.cpp"
#include "Dataset.cpp"

class DecisionRule {
    int decision_feature;
    double threshold;
    
public:
    DecisionRule() {
        this->decision_feature = -1;
        this->threshold = 0;
    }
    
    DecisionRule(int decision_feature, double threshold) {
        this->decision_feature = decision_feature;
        this->threshold = threshold;
    }
    
    bool get_decision(const DataObject& data_object) const {
        return data_object.get_feature(decision_feature) >= threshold;
    }
};

class Vertex {
    Vertex *left_child=nullptr, *right_child=nullptr;
    DecisionRule decision_rule;
    double answer;
    
public:
    Vertex(Vertex *left_child=nullptr, Vertex *right_child=nullptr) {
        this->left_child = left_child;
        this->right_child = right_child;
    }
    
    Vertex(double answer) {
        this->answer = answer;
    }
    
    void add_decision_rule(int decision_feature, double threshold) {
        decision_rule = DecisionRule(decision_feature, threshold);
    }
    
    void add_left_child(Vertex* new_left_child) {
        left_child = new_left_child;
    }
    
    void add_right_child(Vertex* new_right_child) {
        right_child = new_right_child;
    }
    
    void add_answer(double answer) {
        this->answer = answer;
    }
    
    Vertex* move_by_decision_rule(DataObject data_object) {
        if (decision_rule.get_decision(data_object)) {
            return right_child;
        } else {
            return left_child;
        }
    }
    
    bool is_leaf() {
        return left_child || right_child;
    }
    
    double get_answer() {
        return answer;
    }
};

class SolveTreeFitParams {
public:
    int depth;
    
    SolveTreeFitParams(int depth=5) {
        this->depth = depth;
    }
};

class Prediction {
    double prediction;
    
public:
    Prediction(double prediction) {
        this->prediction = prediction;
    }
};

class SolveTree {
    Vertex *root;
    
    Prediction predict(const DataObject& data_object) {
        Vertex *current_vertex = root;
        while (!current_vertex->is_leaf()) {
            current_vertex = current_vertex->move_by_decision_rule(data_object);
        }
        Prediction prediction(current_vertex->get_answer());
        return prediction;
    }
    
public:
    SolveTree() {}
    
    void fit(const Dataset& dataset, SolveTreeFitParams params) {}
    
    std::vector<Prediction> predict(const Dataset& dataset) {
        std::vector<Prediction> predictions;
        for (DataObject data_object : dataset.get_data()) {
            predictions.push_back(predict(data_object));
        }
        return predictions;
    }
    
    
    
    
};










