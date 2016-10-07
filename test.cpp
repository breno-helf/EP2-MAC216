#include<bits/stdc++.h>
std::vector<int> v;

int main() {
    for(int i = 0; i < 10; i++)
        v.push_back(i);
    for(int i = 0; i < 10000; i++) {
        for(int j = 0; j < 10; j++)
            printf("%d", v[j]);
        putchar('\n');
        std::next_permutation(v.begin(), v.end());
    }
}
