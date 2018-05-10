#include "GradientBoosting.h"
#include <chrono>
#include <fstream>
#include <omp.h>

WeakClassifier::WeakClassifier(size_t depth) : depth_(depth) {
	splitting_features_ = std::vector<size_t>();
	leaf_answers_ = std::vector<double>(1 << (depth_ + 1), 0.0);
}

std::vector<double> WeakClassifier::Predict(const Dataset& ds) {
	std::vector<double> res(ds.GetSize());
	for (int i = 0; i < ds.GetSize(); ++i) {
		size_t mask = 0;
		for (int j = 0; j < depth_; ++j) {
			mask += (ds[i][splitting_features_[j]] << (depth_ - j - 1));
		}
		res[i] = leaf_answers_[mask];
	}
	return res;
}


GradientBoosting::GradientBoosting(double learning_rate, size_t depth, int tree_size) {
	this->learning_rate_ = learning_rate;
	this->tree_number_ = tree_size;
	this->depth_ = depth;
}

GradientBoosting::GradientBoosting(std::string model_path) {
    std::ifstream in(model_path);
    in >> depth_ >> tree_number_ >> learning_rate_;
    for (int tree = 0; tree < tree_number_; ++tree) {
        int depth;
        in >> depth;
        WeakClassifier new_classifier(depth);
        int leaf_answer_length;
        in >> leaf_answer_length;
        new_classifier.leaf_answers_ = std::vector<double>(leaf_answer_length);
        for (int i = 0; i < leaf_answer_length; ++i) {
            in >> new_classifier.leaf_answers_[i];
        }
        int splitting_feature_length;
        in >> splitting_feature_length;
        new_classifier.splitting_features_ = std::vector<size_t>(splitting_feature_length);
        for (int i = 0; i < splitting_feature_length; ++i) {
            in >> new_classifier.splitting_features_[i];
        }
        trees_.push_back(new_classifier);
    }
}

void GradientBoosting::Fit(const Dataset& ds) {
	std::chrono::high_resolution_clock::time_point t1 =
		std::chrono::high_resolution_clock::now();

	trees_.clear();
	std::vector<double> cur_pred(ds.GetSize(), 0), temp_pred(ds.GetSize(), 0);
	std::vector<double> targets(ds.GetSize());
	for (int i = 0; i < ds.GetSize(); ++i) {
		targets[i] = ds.GetTarget(i);
	}

	const int NUM_THREADS = 4;

	for (int t = 0; t < tree_number_; ++t) {
		WeakClassifier wc(depth_);
		std::vector<int> leaf_ind(ds.GetSize(), 0);
		for (int d = 0; d < depth_; ++d) {
			std::vector<int> best_leaf_ind(ds.GetSize(), 0);
			std::vector<double> best_leaf_ans(1 << (d + 1), 0);
			std::set<size_t> used_features;
			size_t best_feature = 0;
			double best_mse = __DBL_MAX__, best_true_mse = __DBL_MAX__;
			std::vector<double> best_leaf_sum;
			std::vector<int> best_leaf_count;
            #pragma omp parallel for num_threads(NUM_THREADS)
			for (int j = 0; j < ds.GetNumFeatures(); ++j) {
				if (omp_get_thread_num() == 0) {
					int progress = j * NUM_THREADS*100.0 / ds.GetNumFeatures();
					std::cerr << "\rTree " << t+1 << '/' << tree_number_ << ' '
							  << "Depth " << d+1 << '/' << depth_ << ' ' 
						      << "Progress " << progress << "%";
				}				
				std::vector<int> temp_leaf_ind(ds.GetSize(), 0);
				std::vector<double> leaf_sum(1 << (d + 1), 0.0);
				std::vector<int> leaf_count(1 << (d + 1), 0);
				std::vector<double> leaf_ans(1 << (d + 1), 0);
				double this_mse = 0.0;
				//skip used features
				if (used_features.count(j / ds.GetBinCount()) > 0) {
					continue;
				}
				//traverse dataset				
				for (int i = 0; i < ds.GetSize(); ++i) {
					temp_leaf_ind[i] = leaf_ind[i] * 2 + ds[i][j];
					leaf_sum[temp_leaf_ind[i]] += targets[i] - cur_pred[i];
					++leaf_count[temp_leaf_ind[i]];
				}
				//calc optimal leaf answers given this tree structure
				for (int i = 0; i < leaf_ans.size(); ++i) {
					if (leaf_count[i] == 0) {
						leaf_ans[i] = 0;
					}
					else {
						leaf_ans[i] = leaf_sum[i] / leaf_count[i];
					}				
					this_mse += leaf_ans[i] * (leaf_count[i] * leaf_ans[i] - 2 * leaf_sum[i]);
				}

				//update best feature
				#pragma omp critical
				{
					if (this_mse < best_mse) {
						best_mse = this_mse;
						best_feature = j;
						best_leaf_ans = leaf_ans;
						best_leaf_ind = temp_leaf_ind;
						best_leaf_sum = leaf_sum;
						best_leaf_count = leaf_count;
					}
				}				
			}
			wc.splitting_features_.push_back(best_feature);
			wc.leaf_answers_ = best_leaf_ans;
			used_features.insert(best_feature / ds.GetBinCount());
			leaf_ind = best_leaf_ind;
			//output MSE for debugging, move this lower later
			double MSE = 0.0;
			std::cerr << "\rTree " << t + 1 << '/' << tree_number_ << ' '
				<< "Depth " << d + 1 << '/' << depth_ << "                                           \n";
			std::cerr << "Leaf answers: ";
			for (int i = 0; i < best_leaf_ans.size(); ++i) {
				std::cerr << best_leaf_ans[i] << ' ';
			}
			std::cerr << "\n";
			for (int i = 0; i < ds.GetSize(); ++i) {
				temp_pred[i] = cur_pred[i] + learning_rate_ * best_leaf_ans[best_leaf_ind[i]];
				MSE += (ds.GetTarget(i) - temp_pred[i]) * (ds.GetTarget(i) - temp_pred[i]);
			}
			MSE /= ds.GetSize();
			std::cerr << "Resulting MSE " << MSE << "\n\n";
		}
		cur_pred = temp_pred;
		trees_.push_back(wc);
	}
	std::chrono::high_resolution_clock::time_point t2 =
		std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	std::cout << "\nFitting complete, time elapsed: " << duration << " seconds\n";
}

std::vector<double> GradientBoosting::Predict(const Dataset& dataset) {
	std::vector<double> predictions(dataset.GetSize());
	for (WeakClassifier solve_tree : trees_) {
		std::vector<double> predictions_for_tree = solve_tree.Predict(dataset);
		for (int i = 0; i < predictions.size(); ++i) {
			predictions[i] += learning_rate_ * predictions_for_tree[i];
		}
	}
    
    double mse = 0;
    for (int i = 0; i < predictions.size(); ++i) {
        mse += (predictions[i] - dataset.GetTarget(i)) * (predictions[i] - dataset.GetTarget(i)) / predictions.size();
    }
    std::cerr << "Resulting MSE " << mse << "\n\n";
	return predictions;
}

void GradientBoosting::SaveModel(std::string save_path) {
    std::ofstream out(save_path);
    out << depth_ << " " << tree_number_ << " " << learning_rate_ << std::endl;
    for (auto tree : trees_) {
        out << tree.depth_ << std::endl;
        out << tree.leaf_answers_.size() << std::endl;
        for (auto answer : tree.leaf_answers_) {
            out << answer << " ";
        }
        out << std::endl << tree.splitting_features_.size() << std::endl;

        for (auto feature : tree.splitting_features_) {
            out << feature << " ";
        }
        out << std::endl;
    }
}
