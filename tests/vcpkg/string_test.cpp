#include <string>
#include <benchmark/benchmark.h>

static void BM_STRING(benchmark::State& state)
{
  for (auto _ : state) {
	for (int ii = 0; ii < 1000000; ++ii)
	{
	  std::string x("https://blog.csdn.net/mythma");
	  x.c_str();
	}
  }
}
BENCHMARK(BM_STRING);

BENCHMARK_MAIN();
