#include <vector>
#include "DataObject.cpp"

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
};

class SolveTree {
    Vertex *root;
};
