#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "DataObject.cpp"

class Dataset {
    std::vector<DataObject> data;

public:
    Dataset() {}
    
    Dataset(std::vector<DataObject> data) {
        this->data = data;
    }
    
    Dataset(std::string filepath, bool with_target = true) {
        std::ifstream in(filepath);
        data.clear();
        std::string line;
        
        while (std::getline(in, line)) {
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
                data.push_back(DataObject(result, target));
            } else {
                data.push_back(DataObject(result));
            }
        }
    }
    
    unsigned long get_size() const {
        return data.size();
    }
    
    std::vector<DataObject> get_batch(unsigned long start_index, unsigned long size) const {
        std::vector<DataObject> batch;
        for (unsigned long i = start_index; i < start_index + size; ++i) {
            batch.push_back(data[i]);
        }
        return batch;
    }
    
    DataObject get_element(int index) const {
        return data[index];
    }
    
    std::vector<DataObject> get_data() const {
        return data;
    }
    
    void save_to_csv(std::string filepath) const {
        std::ofstream out(filepath);
        
        for (auto data_object : data) {
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
        std::random_shuffle(data.begin(), data.end());
    }
    
    std::pair<Dataset, Dataset> train_val_split(double train_part=0.7) const {
        unsigned long train_size = get_size() * train_part;
        Dataset train_dataset = Dataset(get_batch(0, train_size));
        Dataset val_dataset = Dataset(get_batch(train_size, get_size() - train_size));
        return {train_dataset, val_dataset};
    }
    
    void add_data_object(DataObject data_object) {
        data.push_back(data_object);
    }
    
    void add_dataset(Dataset dataset) {
        std::vector<DataObject> data_to_add = dataset.get_data();
        data.insert(data.end(), data_to_add.begin(), data_to_add.end());
    }
    
};


