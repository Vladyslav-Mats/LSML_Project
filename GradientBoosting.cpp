#include <vector>
#include <string>
#include <set>
#include <random>
#include "Dataset.cpp"
class WeakClassifier {
public:
	size_t depth_;
	std::vector<size_t> splitting_features_;
	std::vector<double> leaf_answers_;
	WeakClassifier(size_t depth) : depth_(depth) {
		splitting_features_ = std::vector<size_t>();
		leaf_answers_ = std::vector<double>(1 << (depth_ + 1), 0.0);
	}
	std::vector<double> Predict(const Dataset& ds) {
		std::vector<double> res(ds.get_size());
		for (int i = 0; i < ds.get_size(); ++i) {
			size_t mask = 0;
			for (int j = 0; j < depth_; ++j) {
				mask += (ds[i][splitting_features_[j]] << (depth_ - j));
			}
			res[i] = leaf_answers_[mask];
		}
		return res;
	}
};
class GradientBoosting {
	std::vector<WeakClassifier> trees_;
	double learning_rate;
	int tree_number;
	size_t depth_;
public:
	GradientBoosting(double learning_rate, size_t depth, int tree_size) {
		this->learning_rate = learning_rate;
		this->tree_number = tree_size;
		this->depth_ = depth;
	}
	GradientBoosting(std::string model_path) {
		// restore saved model
	}
	void fit(const Dataset& ds) {
		trees_.clear();
		std::vector<double> cur_pred(ds.get_size(), 0), temp_pred(ds.get_size(), 0);

		for (int t = 0; t < tree_number; ++t) {
			WeakClassifier wc(depth_);
			std::vector<int> leaf_ind(ds.get_size(), 0);
			for (int d = 0; d < depth_; ++d) {
				std::vector<int> temp_leaf_ind(ds.get_size(), 0), best_leaf_ind(ds.get_size(), 0);
				std::vector<double> leaf_ans(1 << (d + 1), 0), best_leaf_ans(1 << (d + 1), 0);
				std::set<size_t> used_features;
				size_t best_feature = 0;
				double best_mse = DBL_MAX, best_true_mse = DBL_MAX;
				std::vector<double> best_leaf_sum;
				std::vector<int> best_leaf_count;
				//std::cout << "before feature loop\n";
				for (int j = 0; j < ds.features_count(); ++j) {
					std::vector<double> leaf_sum(1 << (d + 1), 0.0);
					std::vector<int> leaf_count(1 << (d + 1), 0);
					double this_mse = 0.0, this_true_mse = 0.0;
					//skip used features
					if (used_features.count(j / ds.get_bin_count()) > 0) {
						continue;
					}
					//traverse dataset
					for (int i = 0; i < ds.get_size(); ++i) {
						temp_leaf_ind[i] = leaf_ind[i] * 2 + ds[i][j];
						leaf_sum[temp_leaf_ind[i]] += ds.get_target(i) - cur_pred[i];
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
				used_features.insert(best_feature / ds.get_bin_count());
				leaf_ind = best_leaf_ind;
				//output MSE for debugging, move this lower later
				double MSE = 0.0;
				for (int i = 0; i < best_leaf_ans.size(); ++i) {
					std::cout << best_leaf_ans[i] << ' ';
				}
				std::cout << '\n';
				for (int i = 0; i < ds.get_size(); ++i) {
					temp_pred[i] = cur_pred[i] + best_leaf_ans[best_leaf_ind[i]];
					MSE += (ds.get_target(i) - temp_pred[i]) * (ds.get_target(i) - temp_pred[i]);
				}
				MSE /= ds.get_size();
				std::cout << "depth " << d << " MSE " << MSE << "\n\n";
			}
			cur_pred = temp_pred;
			trees_.push_back(wc);
		}
	}
	std::vector<double> predict(const Dataset& dataset) {
		std::vector<double> predictions(dataset.get_size());
		for (WeakClassifier solve_tree : trees_) {
			std::vector<double> predictions_for_tree = solve_tree.Predict(dataset);
			for (int i = 0; i < predictions.size(); ++i) {
				predictions[i] += predictions_for_tree[i];
			}
		}
		return predictions;
	}
	void save_model(std::string save_path) {
		// save model
	}
};
