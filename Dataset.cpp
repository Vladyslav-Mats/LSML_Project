#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <utility>
#include <iostream>
#include "DataObject.cpp"

class Dataset {
    std::vector<DataObject> data_;
	std::vector<std::vector<bool> > binary_data_;
	std::vector<std::vector<double> > thresholds_;
	size_t num_features_;
	const size_t BIN_COUNT = 16;

public:
    Dataset() {}
    
    Dataset(std::vector<DataObject>& data) {
        this->data_ = data;
		num_features_ = data[0].features_count();
		binarize_data();
    }
    
    Dataset(std::string filepath, bool with_target = true) {
        std::ifstream in(filepath);
        data_.clear();
        std::string line;
		size_t lines_read = 0;
        
		//TODO: rewrite with fscanf
        while (std::getline(in, line) && lines_read < 5000) {
			++lines_read;

            std::vector<double> result;
            std::getline(in, line);
            std::stringstream lineStream(line);
            std::string cell;
            
            while(std::getline(lineStream,cell, ','))
            {
                result.push_back(std::atof(cell.c_str()));
            }
            
            if (with_target) {
                double target = result.back();
                result.pop_back();
                data_.push_back(DataObject(result, target));
            } else {
                data_.push_back(DataObject(result));
            }
			num_features_ = data_[0].features_count();
        }
		
		binarize_data();
		std::cout << "Data reading complete\n";
    }

	void binarize_data() {
		binary_data_ = std::vector<std::vector<bool> >(data_.size(),
			std::vector<bool>(16 * num_features_));
		thresholds_ = std::vector<std::vector<double> >(num_features_,
			std::vector<double>(16));

		for (int j = 0; j < num_features_; ++j) {
			std::vector<double> feature_values(data_.size());
			for (int i = 0; i < data_.size(); ++i) {
				feature_values[i] = data_[i][j];
			}

			std::sort(feature_values.begin(), feature_values.end());

			for (int i = 0; i < BIN_COUNT - 1; ++i) {
				thresholds_[j][i] = feature_values[(i + 1) * data_.size() / BIN_COUNT];
			}
			thresholds_[j][BIN_COUNT - 1] = DBL_MAX;

			for (int i = 0; i < data_.size(); ++i) {
				for (int l = 0; l < BIN_COUNT; ++l) {
					binary_data_[i][BIN_COUNT * j + l] = data_[i][j] < thresholds_[j][l];
				}
			}
		}

	}
    
	size_t get_bin_count() const {
		return BIN_COUNT;
	}

    unsigned long get_size() const {
        return binary_data_.size();
    }

	double get_target(int ind) const {
		return data_[ind].get_target();
	}
    
    std::vector<DataObject> get_batch(unsigned long start_index, unsigned long size) const {
        std::vector<DataObject> batch;
        for (unsigned long i = start_index; i < start_index + size; ++i) {
            batch.push_back(data_[i]);
        }
        return batch;
    }
    
    std::vector<bool> operator[](int index) const {
        return binary_data_[index];
    }
    
    std::vector<std::vector<bool> > get_data() const {
        return binary_data_;
    }
    
	size_t features_count() const {
		return num_features_ * BIN_COUNT;
	}

    void save_to_csv(std::string filepath) const {
        std::ofstream out(filepath);
        
        for (auto data_object : data_) {
            std::string csv_string = "";
            for (double feature : data_object.get_features()) {
                csv_string += std::to_string(feature) + ",";
            }
            
            if (data_object.has_target()) {
                csv_string += std::to_string(data_object.get_target()) + ",";
            }
            
            csv_string.pop_back();
            
            out << csv_string << std::endl;
        }
    }
    
    void shuffle_dataset(int seed=42) {
        std::srand(seed);
        std::random_shuffle(data_.begin(), data_.end());
    }
    
    std::pair<Dataset, Dataset> train_val_split(double train_part=0.7) const {
        unsigned long train_size = get_size() * train_part;
        Dataset train_dataset = Dataset(get_batch(0, train_size));
        Dataset val_dataset = Dataset(get_batch(train_size, get_size() - train_size));
        return {train_dataset, val_dataset};
    }
    
    void add_data_object(DataObject data_object) {
        data_.push_back(data_object);
    }
};


