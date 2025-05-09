#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <iostream>
using namespace std;
//g++ my_dlopen.cpp
int main(int argc, char **argv) {
    void *handle;
    char *error;

    handle = dlopen ("/lib/x86_64-linux-gnu/libm.so.6", RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }

    auto sym = dlsym(handle, "cos");
    double (*cosine)(double) = reinterpret_cast<double(*)(double)>(sym);
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(1);
    }

    printf ("%f\n", (*cosine)(2.0));
    dlclose(handle);
    cout<< "...."<<endl;
}