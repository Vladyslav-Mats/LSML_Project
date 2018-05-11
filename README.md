# LSML_Project

It's the repo with team project for LSML course in YSDA. Our team's theme is "Multy-thread gradient boosting". The code is written in C++

# Usage

To run Gradient boosting you need to clone repo and compile it. Intended method of compilation is to make something like

/usr/local/bin/g++-7 -O2 -std=c++11 -fopenmp -o out -I . *.cpp

It has 2 modes - Fit and Predict.

### Example of running Fit mode:

./a.out fit csv_path learning_rate tree_number tree_size save_path

### Args description:

csv_path - path to csv file with train dataset, last column should be target function

learning_rate - double variable learning rate

tree_number - int variable number of trees to fit

tree_size - maximal depth of trees

save_path - path to save a fitted model

### Example of running Predict mode:

./a.out predict csv_path model_path results_path

### Args description:

csv_path - path to csv with test dataset

model_path - path to saved fitted model

results_path - path where to save csv wiht results

# Done experiments to compare with others

Benchmarks: TODO

Higgs:

Our algorithm results:

| Tree number | Depth | Threads | Total time | Time for tree building only | Train error | Test error |
|-------------|-------|---------|------------|-----------------------------|-------------|------------|
|             |       |         |            |                             |             |            |
|             |       |         |            |                             |             |            |
|             |       |         |            |                             |             |            |

LightGBM results:

| Tree number | Depth | Threads | Total time | Time for tree building only | Train error | Test error |
|-------------|-------|---------|------------|-----------------------------|-------------|------------|
|             |       |         |           |                             |             |            |
|             |       |         |            |                             |             |            |
|             |       |         |            |                             |             |            |


# Used papers

https://arxiv.org/pdf/1603.02754.pdf
