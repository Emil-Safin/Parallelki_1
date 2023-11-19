#include <iostream>
#include <cmath>
#include <time.h>
#include <omp.h>
#include <vector>
using namespace std;
// /O2 /Qpar /Qpar-report:2 
int main() 
{
    setlocale(LC_ALL, "ru");
    int N;
    cout << "Simple" << endl << endl << "Enter N: ";
    cin >> N;
    clock_t t1, t2;
    double time;
    long double sum = 0;
    /*omp_set_num_threads(1);
    cout << "Number of threads: " << omp_get_num_threads() << endl;*/
    t1 = clock();
    for (int n = 1; n <= N; n++)
    {
        //sum += pow(-1, (n - 1)) * ((2 * n) + 1) / (n * (n + 1));
        //sum += pow(-1, n) * (n + 1) / ((n + 1) * pow((n + 1), 0.5) - 1);
        sum += sin((2 * n) + 1) / (pow((n + 1), 2) * pow((n + 2), 2));
    }
    t2 = clock();
    time = (t2 - t1) / (double)CLOCKS_PER_SEC;

    cout << "Result sum (SIMPLE): " << sum << endl;
    cout << "Time Work (SIMPLE): " << time << endl << endl;


    cout << "OMP" << endl << endl << "Enter N: ";
    cin >> N;


    sum = 0;
    t1 = clock();
    omp_set_num_threads(12);
#pragma omp parallel
    {

#pragma omp single
        cout << "Number of threads: " << omp_get_num_threads() << endl;


#pragma omp for reduction(+ : sum)
        for (int n = 1; n <= N; n++)
        {
           // sum += pow(-1, n) * (n + 1) / ((n + 1) * pow((n + 1), 0.5) - 1);
            sum += sin((2 * n) + 1) / (pow((n + 1), 2) * pow((n + 2), 2));
        }
    }

    t2 = clock();
    time = (t2 - t1) / (double)CLOCKS_PER_SEC;

    cout << "Result sum(OMP): " << sum << endl;
    cout << "Time Work (OMP): " << time << endl << endl;


    cout << "schedule" << endl << endl << "Enter N: ";
    cin >> N;

    sum = 0;


    vector<vector<double>> times;


    double chunks[] = { 1, 100, 10000 };



    for (int chunkIndex = 0; chunkIndex < 3; chunkIndex++)
    {
        t1 = clock();

#pragma omp parallel for reduction(+ : sum) schedule(static, static_cast<int>(chunks[chunkIndex]))
        for (int n = 1; n <= N; n++)
        {
            //sum += pow(-1, n) * (n + 1) / ((n + 1) * pow((n + 1), 0.5) - 1);
            sum += sin((2 * n) + 1) / (pow((n + 1), 2) * pow((n + 2), 2));
        }

        t2 = clock();
        time = (t2 - t1) / (double)CLOCKS_PER_SEC;


        times.push_back({ chunks[chunkIndex], time });


        sum = 0;
    }
    cout << "static" << endl << "chunk\ttime" << endl;
    for (const auto& entry : times)
    {
        cout << entry[0] << "\t" << entry[1] << "\t" << "seconds" << endl;
    }
    cout << endl;
    times.clear();
    for (int chunkIndex = 0; chunkIndex < 3; chunkIndex++)
    {
        t1 = clock();

#pragma omp parallel for reduction(+ : sum) schedule(dynamic, static_cast<int>(chunks[chunkIndex]))
        for (int n = 1; n <= N; n++)
        {
           // sum += pow(-1, n) * (n + 1) / ((n + 1) * pow((n + 1), 0.5) - 1);
            sum += sin((2 * n) + 1) / (pow((n + 1), 2) * pow((n + 2), 2));
        }

        t2 = clock();
        time = (t2 - t1) / (double)CLOCKS_PER_SEC;


        times.push_back({ chunks[chunkIndex], time });

        sum = 0;
    }
    cout << "dynamic" << endl << "chunk\ttime" << endl;
    for (const auto& entry : times)
    {
        cout << entry[0] << "\t" << entry[1] << "\t" << "seconds" << endl;
    }
    times.clear();
    cout << endl;
    for (int chunkIndex = 0; chunkIndex < 3; chunkIndex++)
    {
        t1 = clock();


#pragma omp parallel for reduction(+ : sum) schedule(guided, static_cast<int>(chunks[chunkIndex]))
        for (int n = 1; n <= N; n++)
        {
           // sum += pow(-1, n) * (n + 1) / ((n + 1) * pow((n + 1), 0.5) - 1);
            sum += sin((2 * n) + 1) / (pow((n + 1), 2) * pow((n + 2), 2));
        }

        t2 = clock();
        time = (t2 - t1) / (double)CLOCKS_PER_SEC;


        times.push_back({ chunks[chunkIndex], time });


        sum = 0;
    }
    cout << "guided" << endl << "chunk\ttime" << endl;
    for (const auto& entry : times)
    {
        cout << entry[0] << "\t" << entry[1] << "\t" << "seconds" << endl;
    }
    times.clear();


    return 0;
}
