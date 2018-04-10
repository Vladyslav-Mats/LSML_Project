#include <vector>

class DataObject {
    std::vector<double> features;
    bool known_target;
    double target;
    
public:
    DataObject(const std::vector<double>& features) {
        this->features = features;
        known_target = false;
    }
    
    DataObject(const std::vector<double>& features, const double& target) {
        this->features = features;
        this->target = target;
        known_target = true;
    }
    
    const std::vector<double>& get_features() const {
        return features;
    }
    
    double operator[](int num_of_feature) const {
        return features[num_of_feature];
    }
    
	size_t features_count() const {
		return features.size();
	}

    const bool has_target() const {
        return known_target;
    }
    
    const double get_target() const {
        if (known_target) {
            return target;
        }
        return 0;
    }
};
