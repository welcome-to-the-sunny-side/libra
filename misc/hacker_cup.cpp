#include<bits/stdc++.h>
using namespace std;

#ifdef natural_selection
#include "../libra/misc/dbg.h"
#else
#define debug(...)
#define endl "\n"
#endif

void main_()
{
    ios_base::sync_with_stdio(false), cin.tie(NULL);
    int t = 1;
    cin >> t;
    for(int tt = 1; tt <= t; tt ++)
    {
        ifstream fin("inp.txt");
        ofstream fout("out.txt");

        {{CURSOR}}
    }
}

static void run_with_stack_size(void (*func)(void), size_t stsize)
{
    char *stack, *send;
    stack = (char *)malloc(stsize);
    send = stack + stsize - 16;
    send = (char *)((uintptr_t)send / 16 * 16);
    asm volatile(
        "mov %%rsp, (%0)\n"
        "mov %0, %%rsp\n"
        :
        : "r"(send));
    func();
    asm volatile("mov (%0), %%rsp\n" : : "r"(send));
    free(stack);
}

int32_t main()
{
    run_with_stack_size(main_, 1024 * 1024 * 1024); // run with a 1 GiB stack
    return 0;
}