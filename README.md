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

string results_path - path where to save csv with results

# Done experiments to compare with others

All experiments are done on 4 2,8 GHz Intel Core i7 CPUs

## Benchmarks:

## Higgs dataset from Kaggle competition (200000 samples in train and 50000 in test):

Our algorithm with single thread results:

| Tree number | Depth | Threads | Dataset reading and binarizing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|--------------------------------|------------------------|-----------|----------|
| 10          | 3     | 1       | 2                              | 8                      | 0.131936  | 0.131422 |
| 30          | 3     | 1       | 2                              | 25                     | 0.124073  | 0.123846 |
| 30          | 6     | 1       | 2                              | 48                     | 0.116751  | 0.118213 |
| 30          | 9     | 1       | 2                              | 84                     | 0.108761  | 0.122416 |
| 30          | 12    | 1       | 2                              | 108                    | 0.0822861 | 0.147608 |
| 30          | 18    | 1       | 2                              | 194                    | 0.00186316| 0.227627 |

Our algorithm with multy threads results:

| Tree number | Depth | Threads | Dataset reading and binarizing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|--------------------------------|------------------------|-----------|----------|
| 10          | 3     | 4       | 2                              | 2                      | 0.131936  | 0.131422 |
| 30          | 3     | 4       | 2                              | 8                      | 0.124073  | 0.123846 |
| 30          | 6     | 4       | 2                              | 17                     | 0.116751  | 0.118213 |
| 30          | 9     | 4       | 2                              | 25                     | 0.108761  | 0.122416 |
| 30          | 12    | 4       | 2                              | 34                     | 0.0822861 | 0.147608 |
| 30          | 18    | 4       | 2                              | 90                     | 0.00186316| 0.227627 |

XGBoost results:

| Tree number | Depth | Threads | Dataset reading and preprocessing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|-----------------------------------|------------------------|-----------|----------|
| 10          | 3     | 4       | 1                                 | 2                      | 0.127176  | 0.127550 |
| 30          | 3     | 4       | 1                                 | 6                      | 0.120089  | 0.121364 |
| 30          | 6     | 4       | 1                                 | 15                     | 0.107211  | 0.141912 |
| 30          | 9     | 4       | 1                                 | 22                     | 0.082569  | 0.199075 |
| 30          | 12    | 4       | 1                                 | 30                     | 0.0398245 | 0.237417 |
| 30          | 18    | 4       | 1                                 | 62                     | 0.00037853| 0.254394 |



## Full Higgs dataset (10500000 samples in train and 500000 in test):

Our algorithm with single thread results:

| Tree number | Depth | Threads | Dataset reading and binarizing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|--------------------------------|------------------------|-----------|----------|
| 1           | 3     | 1       | 179                            | 49                     | 0.226591  | 0.226617 |
| 3           | 3     | 1       | 179                            | 159                    | 0.208841  | 0.208555 |
| 3           | 6     | 1       | 179                            | 281                    | 0.200677  | 0.200362 |
| 30          | 6     | 1       | 179                            | 3116                   | 0.182976  | 0.182697 |
| 10          | 12    | 1       | 179                            | 2123                   | 0.181193  | 0.181422 |
| 10          | 18    | 1       | 179                            | 3541                   | 0.1702    | 0.178566 |

Our algorithm with multy threads results:

| Tree number | Depth | Threads | Dataset reading and binarizing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|--------------------------------|------------------------|-----------|----------|
| 1           | 3     | 4       | 179                            | 24                     | 0.226591  | 0.226617 |
| 3           | 3     | 4       | 179                            | 76                     | 0.208841  | 0.208555 |
| 3           | 6     | 4       | 179                            | 167                    | 0.200677  | 0.200362 |
| 30          | 6     | 4       | 179                            | 1409                   | 0.182976  | 0.182697 |
| 10          | 12    | 4       | 179                            | 899                    | 0.181193  | 0.181422 |
| 10          | 18    | 4       | 179                            | 1417                   | 0.1702    | 0.178566 |

XGBoost results:

| Tree number | Depth | Threads | Dataset reading and preprocessing | Time for tree building | Train MSE | Test MSE |
|-------------|-------|---------|-----------------------------------|------------------------|-----------|----------|
| 1           | 3     | 4       | 145                               | 7                      | 0.221908  | 0.221799 |
| 3           | 3     | 4       | 145                               | 20                     | 0.208278  | 0.208066 |
| 3           | 6     | 4       | 145                               | 44                     | 0.194487  | 0.194325 |
| 30          | 6     | 4       | 145                               | 480                    | 0.179546  | 0.179736 |
| 10          | 12    | 4       | 145                               | 532                    | 0.16965   | 0.178883 |
| 10          | 18    | 4       | 145                               | 1800                   | 0.102137  | 0.22046  |

## Conclusions:

1) Our multithreading works good, 2-3 times reducing fitting time with 4 CPU cores.

2) On Small Higgs our speed is almost the same as speed of XGBoost, while results are quite different. This is because of using oblivious decision trees we can't overfit so much with trees of big depth, what we can see at tables above.

3) On full Higgs we can see that we have not only huge results difference, but in fitting time too. We works like 3-4 times slower on such giant dataset with small tree depth, but our time is almost linear to tree_number * depth, while time of XGBoost isn't, here it looks exponential from depth. This is because of dataset is big enough to have a lot of deep leafs, so XGBoost works much slower, while our algorithm on oblivious decision trees doesn't have such problems, and works faset than XGBoost on trees with big depth.

4) Even more, we can look on overfitting value from 2 last columns and see that our algorithm doesn't overfit on much bigger tree depth values, while XGBoost fails. On such a big dataset as full Higgs it looks most obvious.

# Used papers

https://arxiv.org/pdf/1603.02754.pdf
