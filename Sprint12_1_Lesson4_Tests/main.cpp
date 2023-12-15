#include <iostream>

int main() {
    int users = 100000;
    int pages = 1000;
    int queries = 1000000;

    std::cout << queries << std::endl;

    int k = 1;
    int q = 1;
    for (int i=1; i<queries; ++i) {
        if (i <= 100000) {
            std::cout << "READ " << i << " " << k << std::endl;
            ++k;
            if (k==1001) k=1;
        }
        else {
            std::cout << "CHEER " << q << std::endl;
            ++q;
            if (q==100001) q=1;
        }
    }
}