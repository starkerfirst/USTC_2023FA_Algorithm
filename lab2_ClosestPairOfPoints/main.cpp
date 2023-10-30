#include<iostream>
#include<fstream>
#include<algorithm>
#include<chrono>
#include<cassert>

double pow(double x, int n) {
    double result = 1;
    for (int i = 0; i < n; ++i) {
        result *= x;
    }
    return result;
}

// baseline function for ClosestPairOfPodoubles
void Baseline(double n, double *x, double *y) {
    printf("Baseline\n");
    double min = 0x7fffffff;
    int p1, p2;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; ++j) {
            double dist = (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]);
            if (dist < min) {
                min = dist;
                p1 = i;
                p2 = j;
            }
        }
    }
    printf("Closest pair: (%d, %f, %f), (%d, %f, %f) dist: %f\n", p1, x[p1], y[p1], p2, x[p2], y[p2], min);
}


// my implementation for ClosestPairOfPodoubles
double* ClosestPairOfPoints(int n, double *x, double *y) {
    // // boundary condition
    double *result = new double[4];
    double min = 0x7fffffff;
    // if (n <= 1) return result;
    // if (n == 2) {
    //     result[0] = x[0];
    //     result[1] = y[0];
    //     result[2] = x[1];
    //     result[3] = y[1];
    //     return result;
    // }

    // short cut at 40
    if (n < 10){
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                double dist = pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);
                if (dist < min) {
                    min = dist;
                    result[0] = x[i];
                    result[1] = y[i];
                    result[2] = x[j];
                    result[3] = y[j];
                    if(result[0] == result[2]){
                        int a =1;
                        a++;
                    }
                }
            }
        }
        return result;
    }

    double *x_sorted = new double[n];
    // double *y_sorted = new double[n];

    // find the median of x
    std::copy(x, x+n, x_sorted);
    std::sort(x_sorted, x_sorted+n); // sort x
    // std::copy(y, y+n, y_sorted);
    // std::sort(y_sorted, y_sorted+n); // sort y
    double median = x_sorted[n/2];

    // partition
    double *x_left = new double[n/2];
    double *y_left = new double[n/2];
    double *x_right = new double[n - n/2];
    double *y_right = new double[n - n/2];
    double *result_left;
    double *result_right;
    std::copy(x_sorted, x_sorted+n/2, x_left);
    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n; ++j) {
            if (x_left[i] == x[j]) {
                // check if y[j] in y_left
                bool flag = false;
                for (int k = 0; k < i; ++k) {
                    if (y_left[k] == y[j]) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    y_left[i] = y[j];
                    break;
                }
            }
        }
    }
    std::copy(x_sorted+n/2+1, x_sorted+n, x_right);
    for (int i = 0; i < n - n/2-1; ++i) {
        for (int j = 0; j < n; ++j) {
            if (x_right[i] == x[j] && y_left[i-1] != y[j]) {
                 // check if y[j] in y_left
                bool flag = false;
                for (int k = 0; k < i; ++k) {
                    if (y_right[k] == y[j]) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    y_right[i] = y[j];
                    break;
                }
            }
        }
    }
    

    // recursive
    result_left = ClosestPairOfPoints(n/2, x_left, y_left);
    result_right = ClosestPairOfPoints(n - n/2 - 1, x_right, y_right);
    delete[] x_left;
    delete[] y_left;
    delete[] x_right;
    delete[] y_right;

    // merge
    double delta_left = pow(result_left[0] - result_left[2], 2) + pow(result_left[1] - result_left[3], 2);
    double delta_right = pow(result_right[0] - result_right[2], 2) + pow(result_right[1] - result_right[3], 2);
    double delta;
    
    if (delta_left < delta_right) {
        delta = delta_left;
        result[0] = result_left[0];
        result[1] = result_left[1];
        result[2] = result_left[2];
        result[3] = result_left[3];
    } else {
        delta = delta_right;
        result[0] = result_right[0];
        result[1] = result_right[1];
        result[2] = result_right[2];
        result[3] = result_right[3];
    }

    // get the podoubles in the strip
    int *index = new int[n];
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (abs(x[i]-median) < delta) {
            index[count] = i;
            count++;
        }
    }
    // loop over all podoubles in the strip by y
    for (int i = 0; i < count; ++i) {
        for (int j = i+1; j < count; ++j) {
            double dist = pow(x[index[i]] - x[index[j]], 2) + pow(y[index[i]] - y[index[j]], 2);
            if (dist < delta) {
                delta = dist;
                result[0] = x[index[i]];
                result[1] = y[index[i]];
                result[2] = x[index[j]];
                result[3] = y[index[j]];
            }
        }
    }

    delete[] index;
    delete[] x_sorted;
    // delete[] y_sorted;

    return result;

}


int main(){
    int n = 10000;
    double *x, *y;
    double *result = new double[4];
    double tmp;
    std::ifstream fin("data.txt");
    x = new double[n];
    y = new double[n];
    for (int i = 0; i < n; ++i) {
        fin >> tmp;
        fin >> x[i];
        fin >> y[i];
    }
    fin.close();

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    Baseline(n, x, y);
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    // printf("Baseline time: %f ms\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count()*1000);
    printf("\nMy implementation\n");
    t1 = std::chrono::steady_clock::now();
    result = ClosestPairOfPoints(n, x, y); // return index of closest pair
    int index_l, index_r;
    for (int i = 0; i < n; ++i) {
        if (x[i] == result[0] && y[i] == result[1]) {
            index_l = i;
        }
        if (x[i] == result[2] && y[i] == result[3]) {
            index_r = i;
        }
    }
    double dist = pow(result[0] - result[2], 2) + pow(result[1] - result[3], 2);
    printf("Closest pair: (%d, %f, %f), (%d, %f, %f) dist: %f\n", index_l, result[0], result[1], index_r, result[2], result[3], dist);
    t2 = std::chrono::steady_clock::now();
    // printf("My implementation time: %f ms\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count()*1000);

    delete[] x;
    delete[] y;
    return 0;
}