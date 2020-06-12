#include <algorithm>
#include "Eratosthenes.h"
#include "Bitmap.h"

using namespace MyLib;

void MyLib::Eratosthenes(int n, const char * const file)
{
    Bitmap B(n);

    B.set(0);
    B.set(1);

    for (int i = 2; i < n; i++)
    {
        if (!B.test(i))
        {
            for (int j = std::min(i, 46340) * std::min(i, 46340); j < n; j += i)
            {
                B.set(j);
            }
        }
    }

    B.dump(file);
}