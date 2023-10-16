#include<chrono>
#include<iostream>
#include<fstream>
#include<cstdlib>

#define K 50

// insert sort
void insertSort(int* arr, int left, int right) {
    int tmp;
    for (int i = left + 1; i <= right; i++) {
        tmp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > tmp) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = tmp;
    }
}

// sample
int sampler(int* arr, int left, int right, int option){
    int mid;
    switch (option)
    {
        // fix
        case 0:
            return right;
            break;
        // random
        case 1:
            return left + rand() % (right - left + 1);
            break;
        // median of three
        case 2:
            mid = (left + right) / 2;
            if (arr[left] > arr[mid]) {
                if (arr[mid] > arr[right]) return mid;
                else if (arr[left] > arr[right]) return right;
                else return left;
            }
            else {
                if (arr[left] > arr[right]) return left;
                else if (arr[mid] > arr[right]) return right;
                else return mid;
            }
            break;
        default:
            return right;
            break;
    }
        

}

// quick sort
void quickSort(int* arr, int left, int right, int option) {
    auto i = left, j = right-1;
    int tmp;
    auto sample = sampler(arr, left, right, option);
    auto baseline = arr[sample];

    // boundary
    if (right - left + 1 <= 1) return;
    if (right - left + 1 == 2){
        if (arr[left] > arr[right]){
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
        }
        return;
    }
        
    // swap sample and right
    tmp = arr[sample];
    arr[sample] = arr[right];
    arr[right] = tmp;

    // partition
    while (i <= j) {
        while (arr[i] <= baseline) i++;
        while (arr[j] > baseline) j--;
        if (i <= j) {
            // swap
            if (i < right && j >= left){
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            i++;
            j--;
        }
    };
    
    // swap back
    if (i < right){
        tmp = arr[i];
        arr[i] = arr[right];
        arr[right] = tmp;
    }

    // recursion (if length < K, stop quicksort and move to insertsort)
    if (left < j && !(option == 3 && right - left + 1 < K)) quickSort(arr, left, j, option);
    if (i < right && !(option == 3 && right - left + 1 < K)) quickSort(arr, i+1, right, option);
}

int main(){
    // read file
    std::ifstream fin("data.txt", std::ios::in);
    int n;
    fin >> n;
    int* data = new int[n];
    for (int i = 0; i < n; i++) fin >> data[i];
    fin.close();

    // fix
    int* arr = new int[n];
    for (int i = 0; i < n; i++) arr[i] = data[i];
    auto start = std::chrono::steady_clock::now();
    quickSort(arr, 0, n-1, 0);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "fix: " << diff.count() << " s" << std::endl;
    std::ofstream fout_1("output_fix.txt", std::ios::out);
    for (int i = 0; i < n; i++) fout_1 << arr[i] << " ";
    fout_1.close();

    // random
    for (int i = 0; i < n; i++) arr[i] = data[i];
    start = std::chrono::steady_clock::now();
    quickSort(arr, 0, n-1, 1);
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "random: " << diff.count() << " s" << std::endl;
    std::ofstream fout_2("output_random.txt", std::ios::out);
    for (int i = 0; i < n; i++) fout_2 << arr[i] << " ";
    fout_2.close();

    // median of three
    for (int i = 0; i < n; i++) arr[i] = data[i];
    start = std::chrono::steady_clock::now();
    quickSort(arr, 0, n-1, 2);
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "median of three: " << diff.count() << " s" << std::endl;
    std::ofstream fout_3("output_median.txt", std::ios::out);
    for (int i = 0; i < n; i++) fout_3 << arr[i] << " ";
    fout_3.close();

    // early exit
    for (int i = 0; i < n; i++) arr[i] = data[i];
    start = std::chrono::steady_clock::now();
    quickSort(arr, 0, n-1, 3);
    insertSort(arr, 0, n-1);
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "early exit (" << K << "): " << diff.count() << " s" << std::endl;
    std::ofstream fout_4("output_exit.txt", std::ios::out);
    for (int i = 0; i < n; i++) fout_4 << arr[i] << " ";
    fout_4.close();

    delete[] arr;
    return 0;

}

