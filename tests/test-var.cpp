#include <iostream>

using namespace std;

int main()
{
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            cout << ((i >> j) & 0x01) << " ";
        }
        cout << "\n";
    }
    return 0;
}