#ifndef DATASET_H
#define DATASET_H

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

	Dataset(const std::vector<DataObject>& data);

	Dataset(std::string filepath, size_t target_pos = std::string::npos);

	void BinarizeData();

	size_t GetBinCount() const {
		return BIN_COUNT;
	}

	unsigned long GetSize() const {
		return binary_data_.size();
	}

	double GetTarget(int ind) const {
		return data_[ind].get_target();
	}

	std::vector<DataObject> GetBatch(unsigned long start_index, unsigned long size) const;

	std::vector<bool> operator[](int index) const {
		return binary_data_[index];
	}

	std::vector<std::vector<bool> > GetData() const {
		return binary_data_;
	}

	size_t GetNumFeatures() const {
		return num_features_ * BIN_COUNT;
	}

	void SaveToCSV(std::string filepath) const;

	void ShuffleDataset(int seed = 42);
	std::pair<Dataset, Dataset> TrainValSplit(double train_part = 0.7) const;
	void AddDataObject(DataObject data_object);
};
#undef DATASET_H
#endif
