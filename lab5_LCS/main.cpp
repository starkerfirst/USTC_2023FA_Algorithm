#include<fstream>
#include<iostream>
#include<string>
#include<cstring>

// longest common subsequence (space complexity: O(min(m,n)))
void LCS_quick(char *a, char *b, int n, int m){
    int *tmp = new int[m+1];
    int left_up = 0; // store the left-up LCS len of the current element
    int len = 0;
    
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            if (a[i-1] == b[j-1]){
                len = left_up + 1;
                left_up = tmp[j];
                tmp[j] = len;
            }
            else{
                len = std::max(tmp[j], tmp[j-1]);
                left_up = tmp[j];
                tmp[j] = len;
            }
        }
    }

    std::cout << tmp[m] << std::endl;
}

// normal LCS
void LCS(char *a_s, char *b_s, int n, int m){
    int c[1000][1000] = {0}; // store the length of LCS
    int b[1000][1000] = {0}; // 0: up, 1: left, 2: up-left

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            if (a_s[i-1] == b_s[j-1]){
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 2;
            }
            else if (c[i-1][j] >= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j] = 0;
            }
            else{
                c[i][j] = c[i][j-1];
                b[i][j] = 1;
            }
        }
    }

    // print the LCS
    int i = n, j = m;
    char out[1000];
    int count;
    while (i > 0 && j > 0){
        if (b[i][j] == 2){
            out[count++] = a_s[i-1];
            i--;
            j--;
        }
        else if (b[i][j] == 0){
            if (i == 1) out[count++] = a_s[i-1];
            i--;
        }
        else if (b[i][j] == 1){
            if (j == 1) out[count++] = b_s[j-1];
            j--;
        }
    }

    for (int i = count-1; i >= 0; i--){
        std::cout << out[i];
    }

    std::cout << std::endl;
    std::cout << c[n][m] << std::endl;
}



int main(){
    std::ifstream file("data.txt");
    auto a = new char[1000];
    auto b = new char[1000];
    int n, m;

    std::string str;
    std::getline(file, str);
    strcpy(a, str.c_str());
    n = strlen(a);

    std::getline(file, str);
    strcpy(b, str.c_str());
    m = strlen(b);

    std::cout << "LCS: " << std::endl;
    LCS(a, b, n, m);

    std::cout << "LCS_quick: " << std::endl;
    LCS_quick(a, b, n, m);

}