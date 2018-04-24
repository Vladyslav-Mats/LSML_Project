#include "Dataset.h"
#include <chrono>

Dataset::Dataset(std::vector<DataObject>& data) {
	this->data_ = data;
	num_features_ = data[0].features_count();
	BinarizeData();
}
    
Dataset::Dataset(std::string filepath, size_t target_pos) {
	data_.clear();
	size_t lines_read = 0;
	num_features_ = 0;

	FILE * pFile;
	const size_t LINE_MAX = 100000;
	char line[LINE_MAX];

	pFile = fopen(filepath.c_str(), "r");

	std::chrono::high_resolution_clock::time_point t1 = 
		std::chrono::high_resolution_clock::now();

	//TODO: rewrite with fscanf
	while (fgets(line, LINE_MAX, pFile) != NULL) {
		++lines_read;
		if (lines_read % 10000 == 0) {
			std::cerr << "\rlines read: " << lines_read;
		}

		std::vector<double> result;
		char buf[100];
		size_t ptr = 0;
		double target = DBL_MAX;
		bool target_passed = false;

		for (int i = 0; line[i] != '\0'; ++i) {
			if (line[i] == ',') {
				buf[ptr] = '\0';
				if (!target_passed && target_pos == result.size()) {
					target = std::atof(buf);
					target_passed = true;
				}
				else {
					result.push_back(std::atof(buf));
				}
				ptr = 0;
			}
			else {
				buf[ptr++] = line[i];
			}
		}

		if (target_pos == std::string::npos) {
			target = result.back();
			result.pop_back();
		}

		data_.push_back(DataObject(result, target));
		num_features_ = data_[0].features_count();
	}

	BinarizeData();
	std::chrono::high_resolution_clock::time_point t2 =
		std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	std::cout << "\nData reading complete, time elapsed: " << duration << " seconds\n";
}

void Dataset::BinarizeData() {
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
   
std::vector<DataObject> Dataset::GetBatch(unsigned long start_index, unsigned long size) const {
    std::vector<DataObject> batch;
    for (unsigned long i = start_index; i < start_index + size; ++i) {
        batch.push_back(data_[i]);
    }
    return batch;
}

void Dataset::SaveToCSV(std::string filepath) const {
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
    
void Dataset::ShuffleDataset(int seed) {
    std::srand(seed);
    std::random_shuffle(data_.begin(), data_.end());
}
    
std::pair<Dataset, Dataset> Dataset::TrainValSplit(double train_part) const {
    unsigned long train_size = GetSize() * train_part;
    Dataset train_dataset = Dataset(GetBatch(0, train_size));
    Dataset val_dataset = Dataset(GetBatch(train_size, GetSize() - train_size));
    return {train_dataset, val_dataset};
}
    
void Dataset::AddDataObject(DataObject data_object) {
    data_.push_back(data_object);
}


