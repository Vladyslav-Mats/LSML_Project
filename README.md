# LSML_Project

It's the repo with team project for LSML course in YSDA. Our team's theme is "Multy-thread gradient boosting". The code is written in C++

# Usage

To run Gradient boosting you need to clone repo and compile main. It has 2 modes - Fit and Predict.

Example of running Fit mode:

./a.out fit csv_path learning_rate tree_number tree_size save_path

Args description:

csv_path - path to csv file with train dataset, last column should be target function

learning_rate - double variable learning rate

tree_number - int variable number of trees to fit

tree_size - maximal depth of trees

save_path - path to save a fitted model

Example of running Predict mode:

./a.out predict csv_path model_path results_path

Args description:

csv_path - path to csv with test dataset

model_path - path to saved fitted model

results_path - path where to save csv wiht results

# Done experiments to compare with others

Benchmarks: TODO

Football: TODO

Fraud detection: TODO

BCI: TODO

Planet Kaggle: TODO

Higgs: TODO

Airline: TODO

# Used papers

https://arxiv.org/pdf/1603.02754.pdf
