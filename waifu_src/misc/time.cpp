auto start = std::chrono::high_resolution_clock::now();
// Code to benchmark
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
std::cout << "Execution time: " << duration << " microseconds" << std::endl;