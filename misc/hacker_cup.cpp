#include <bit>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <tuple>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <cmath>
#include <complex>
#include <random>
#include <chrono>
#include <cstring>
#include <cctype>
#include <cassert>
#include <cstdlib>
#include <fstream>
using namespace std;

#ifdef natural_selection
#include "../libra/misc/dbg.h"
#else
#define debug(...)
#endif

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(NULL);

    int t = 1;
    cin >> t;
    for(int tt = 1; tt <= t; tt ++)
    {
        {{CURSOR}}
        // cout << "Case #" << tt << ": " << ans << endl;
    }
}

// compile with `clang++ -std=c++20 -Dnatural_selection -O3 -Wl,-stack_size,0x20000000 <file>.cpp`