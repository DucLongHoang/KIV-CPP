#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <vector>
#include <atomic>
#include <execution>
#include <numeric>

constexpr  size_t ITERATIONS = 100000;

void test_without_synch() {
    unsigned long cislo = 0;
    std::thread thr1([&cislo]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            cislo += 1;
        }
    });
    std::thread thr2([&cislo]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            cislo += 2;
        }
    });

    thr1.join();
    thr2.join();

    std::cout << "vysledek je: " << cislo << std::endl;
}

void test_atomic() {
    std::atomic<unsigned long > cislo = 0;


    std::thread thr1([&cislo]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            cislo += 1;
        }
    });
    std::thread thr2([&cislo]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            cislo += 2;
        }
    });

    thr1.join();
    thr2.join();

    std::cout << "vysledek je: " << cislo << std::endl;
}

void test_mutex() {
    unsigned long cislo = 0;

    std::mutex mtx;

    std::thread thr1([&cislo, &mtx]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            std::lock_guard<std::mutex> lck(mtx);
            cislo += 1;
        }
    });
    std::thread thr2([&cislo, &mtx]() {
        for (size_t i = 0; i < ITERATIONS; ++i) {
            std::lock_guard<std::mutex> lck(mtx);
            cislo += 2;
        }
    });

    thr1.join();
    thr2.join();

    std::cout << "vysledek je: " << cislo << std::endl;
}

void sort_parallel(std::vector<double>& work) {
    std::sort(std::execution::par_unseq, work.begin(), work.end());
}

void sort_serial(std::vector<double>& work) {
    std::sort(std::execution::seq, work.begin(), work.end());
}

void accumulate_parallel(std::vector<double>& work) {
    auto res = std::reduce(std::execution::par_unseq, work.begin(), work.end(), 0.0);
    std::cout << "vysledek: " << res << std::endl;
}

void accumulate_serial(std::vector<double>& work) {
    auto res = std::accumulate(work.begin(), work.end(), 0.0);
    std::cout << "vysledek: " << res << std::endl;
}

int main(int argc, char** argv) {

//    {
//        auto beg = std::chrono::high_resolution_clock::now();
//        test_without_synch();
//        auto end = std::chrono::high_resolution_clock::now();
//        std::cout << "cas: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "ms" << std::endl;
//    }
//
//    {
//        auto beg = std::chrono::high_resolution_clock::now();
//        test_mutex();
//        auto end = std::chrono::high_resolution_clock::now();
//        std::cout << "cas: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "ms" << std::endl;
//    }
//
//    {
//        auto beg = std::chrono::high_resolution_clock::now();
//        test_atomic();
//        auto end = std::chrono::high_resolution_clock::now();
//        std::cout << "cas: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "ms" << std::endl;
//    }

    std::vector<double> work(10000000);
    std::random_device rdev;
    std::default_random_engine reng(rdev());
    std::uniform_real_distribution<double> rdist(-1000, 1000);
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = rdist(reng);
    }

    std::vector<double> work_local1 = work;
    std::vector<double> work_local2 = work;


    {
        auto beg = std::chrono::high_resolution_clock::now();
//        sort_serial(work_local1);
        accumulate_serial(work_local1);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "cas: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "ms" << std::endl;
    }

    {
        auto beg = std::chrono::high_resolution_clock::now();
//        sort_parallel(work_local2);
        accumulate_parallel(work_local1);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "cas: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "ms" << std::endl;
    }

//    std::cout << "CPU count: " << std::thread::hardware_concurrency() << std::endl;
//
//    std::mutex mtx;
//
//    std::thread thr([&mtx] () {
//        int pocet = 100;
//        for (int i = 0; i < pocet; ++i) {
//            // gets unlocked out of scope
//            std::unique_lock<std::mutex> lck(mtx);
//            std::cout << "A" << "h" << "o" << "j";
//        }
//    });
//    std::thread thr2([&mtx] () {
//        int pocet = 100;
//        for (int i = 0; i < pocet; ++i) {
//            std::unique_lock<std::mutex> lck(mtx);
//            std::cout << "B" << "y" << "e";
//        }
//    });
//
////    thr.detach();
////    std::this_thread::sleep_for(std::chrono::seconds(3));
//
//    if (thr.joinable())
//        thr.join();
//
//    if (thr2.joinable())
//        thr2.join();
//
//    if (thr.joinable())
//        thr.join();

    return EXIT_SUCCESS;
}