#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <utility>
#include <iostream>
#include "DataObject.h"

class Dataset {
	std::vector<DataObject> data_;
	std::vector<std::vector<bool> > binary_data_;
	std::vector<std::vector<double> > thresholds_;
	size_t num_features_;
	const size_t BIN_COUNT = 16;

public:
	Dataset() {}

	Dataset(std::vector<DataObject>& data);

	Dataset(std::string filepath, size_t target_pos = std::string::npos);

	void binarize_data();

	size_t get_bin_count() const {
		return BIN_COUNT;
	}

	unsigned long get_size() const {
		return binary_data_.size();
	}

	double get_target(int ind) const {
		return data_[ind].get_target();
	}

	std::vector<DataObject> get_batch(unsigned long start_index, unsigned long size);

	std::vector<bool> operator[](int index) const {
		return binary_data_[index];
	}

	std::vector<std::vector<bool> > get_data() const {
		return binary_data_;
	}

	size_t features_count() const {
		return num_features_ * BIN_COUNT;
	}

	void save_to_csv(std::string filepath) const;

	void shuffle_dataset(int seed = 42);
	std::pair<Dataset, Dataset> train_val_split(double train_part = 0.7);
	void add_data_object(DataObject data_object);
};