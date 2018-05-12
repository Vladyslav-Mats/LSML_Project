# LSML_Project

It's the repo with team project for LSML course in YSDA. Our team's theme is "Multy-thread gradient boosting". The code is written in C++

# Usage

To run Gradient boosting you need to clone repo and compile it. Intended method of compilation is to make something like

/usr/local/bin/g++-7 -O2 -std=c++11 -fopenmp -o out -I . *.cpp

It has 2 modes - Fit and Predict.

### Example of running Fit mode:

./a.out fit csv_path learning_rate tree_number tree_size save_path

### Args description:

string csv_path - path to csv file with train dataset, last column should be target function

double learning_rate - gradient boosting learning rate

int tree_number - number of trees to fit

int tree_size - maximal depth of trees

string save_path - path to save a fitted model

### Example of running Predict mode:

./a.out predict csv_path model_path results_path

### Args description:

string csv_path - path to csv with test dataset

string model_path - path to saved fitted model

string results_path - path where to save csv wiht results

# Done experiments to compare with others

Benchmarks: TODO

Full Higgs dataset (10500000 samples in train and 500000 in test):

Our algorithm results:

| Tree number | Depth | Threads | Dataset reading and binarizing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|--------------------------------|------------------------|-----------|----------|
| 1           | 3     | 4       | 179                            | 24                     | 0.226591  | 0.226617 |
| 3           | 3     | 4       | 179                            | 76                     | 0.208841  | 0.208555 |
| 3           | 6     | 4       | 179                            | 167                    | 0.200677  | 0.200362 |
| 30          | 6     | 4       | 179                            | 1409                   | 0.182976  | 0.182697 |
| 10          | 18    | 4       | 179                            | 1417                   | 0.1702    | 0.178566 |

XGBoost results:

| Tree number | Depth | Threads | Dataset reading and preprocessing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|-----------------------------------|------------------------|-----------|----------|
| 1           | 3     | 4       | 145                               | 7                      | 0.221908  | 0.221799 |
| 3           | 3     | 4       | 145                               | 20                     | 0.208278  | 0.208066 |
| 3           | 6     | 4       | 145                               | 44                     | 0.194487  | 0.194325 |
| 30          | 6     | 4       | 145                               | 480                    | 0.179546  | 0.179736 |
| 10          | 18    | 4       | 145                               | 1800                   | 0.102137  | 0.22046  |


# Used papers

https://arxiv.org/pdf/1603.02754.pdf
