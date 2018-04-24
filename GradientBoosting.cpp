#include "GradientBoosting.h"
#include <chrono>

WeakClassifier::WeakClassifier(size_t depth) : depth_(depth) {
	splitting_features_ = std::vector<size_t>();
	leaf_answers_ = std::vector<double>(1 << (depth_ + 1), 0.0);
}

std::vector<double> WeakClassifier::Predict(const Dataset& ds) {
	std::vector<double> res(ds.GetSize());
	for (int i = 0; i < ds.GetSize(); ++i) {
		size_t mask = 0;
		for (int j = 0; j < depth_; ++j) {
			mask += (ds[i][splitting_features_[j]] << (depth_ - j));
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
	// restore saved model
}

void GradientBoosting::Fit(const Dataset& ds) {
	std::chrono::high_resolution_clock::time_point t1 =
		std::chrono::high_resolution_clock::now();

	trees_.clear();
	std::vector<double> cur_pred(ds.GetSize(), 0), temp_pred(ds.GetSize(), 0);

	for (int t = 0; t < tree_number_; ++t) {
		WeakClassifier wc(depth_);
		std::vector<int> leaf_ind(ds.GetSize(), 0);
		for (int d = 0; d < depth_; ++d) {
			std::vector<int> temp_leaf_ind(ds.GetSize(), 0), best_leaf_ind(ds.GetSize(), 0);
			std::vector<double> leaf_ans(1 << (d + 1), 0), best_leaf_ans(1 << (d + 1), 0);
			std::set<size_t> used_features;
			size_t best_feature = 0;
			double best_mse = DBL_MAX, best_true_mse = DBL_MAX;
			std::vector<double> best_leaf_sum;
			std::vector<int> best_leaf_count;
			//std::cout << "before feature loop\n";
			for (int j = 0; j < ds.GetNumFeatures(); ++j) {
				std::vector<double> leaf_sum(1 << (d + 1), 0.0);
				std::vector<int> leaf_count(1 << (d + 1), 0);
				double this_mse = 0.0, this_true_mse = 0.0;
				//skip used features
				if (used_features.count(j / ds.GetBinCount()) > 0) {
					continue;
				}
				//traverse dataset
				#pragma omp parallel num_threads(4)
				for (int i = 0; i < ds.GetSize(); ++i) {
					temp_leaf_ind[i] = leaf_ind[i] * 2 + ds[i][j];
					leaf_sum[temp_leaf_ind[i]] += ds.GetTarget(i) - cur_pred[i];
					++leaf_count[temp_leaf_ind[i]];
				}
				//calc optimal leaf answers given this tree structure
				bool ok = true;
				for (int i = 0; i < leaf_ans.size() && ok; ++i) {
					if (leaf_count[i] == 0) {
						ok = false;
						break;
					}
					leaf_ans[i] = leaf_sum[i] / leaf_count[i];
					this_mse += leaf_ans[i] * (leaf_count[i] * leaf_ans[i] - 2 * leaf_sum[i]);
				}

				//update best feature
				if (this_mse < best_mse && ok) {
					best_mse = this_mse;
					best_feature = j;
					best_leaf_ans = leaf_ans;
					best_leaf_ind = temp_leaf_ind;
					best_leaf_sum = leaf_sum;
					best_leaf_count = leaf_count;
				}
			}
			//std::cout << "after feature loop\n";
			wc.splitting_features_.push_back(best_feature);
			wc.leaf_answers_ = best_leaf_ans;
			used_features.insert(best_feature / ds.GetBinCount());
			leaf_ind = best_leaf_ind;
			//output MSE for debugging, move this lower later
			double MSE = 0.0;
			for (int i = 0; i < best_leaf_ans.size(); ++i) {
				std::cerr << best_leaf_ans[i] << ' ';
			}
			std::cerr << '\n';
			for (int i = 0; i < ds.GetSize(); ++i) {
				temp_pred[i] = cur_pred[i] + best_leaf_ans[best_leaf_ind[i]];
				MSE += (ds.GetTarget(i) - temp_pred[i]) * (ds.GetTarget(i) - temp_pred[i]);
			}
			MSE /= ds.GetSize();
			std::cerr << "depth " << d << " MSE " << MSE << "\n\n";
		}
		cur_pred = temp_pred;
		trees_.push_back(wc);
	}
	std::chrono::high_resolution_clock::time_point t2 =
		std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	std::cout << "\nData reading complete, time elapsed: " << duration << " seconds\n";
}

std::vector<double> GradientBoosting::Predict(const Dataset& dataset) {
	std::vector<double> predictions(dataset.GetSize());
	for (WeakClassifier solve_tree : trees_) {
		std::vector<double> predictions_for_tree = solve_tree.Predict(dataset);
		for (int i = 0; i < predictions.size(); ++i) {
			predictions[i] += predictions_for_tree[i];
		}
	}
	return predictions;
}

void GradientBoosting::SaveModel(std::string save_path) {
	// save model
}
