/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/benchmark.h>
#include <taskbench/utils/format.h>
#include <taskbench/utils/statistics.h>

namespace taskbench {

// _____________________________________________________________________________________________________________________
BenchmarkResult::BenchmarkResult(std::string name, uint64_t data_size, uint64_t num_operations)
    : _name(std::move(name)), _data_size(data_size), _num_operations(num_operations) {}

// _____________________________________________________________________________________________________________________
void BenchmarkResult::add_runtime(taskbench::seconds runtime) { _runtimes.push_back(runtime); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::bps_max() const { return static_cast<double>(_data_size) / utils::min(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::bps_mean() const { return static_cast<double>(_data_size) / utils::mean(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::bps_stdev() const { return static_cast<double>(_data_size) / utils::stdev(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::bps_min() const { return static_cast<double>(_data_size) / utils::max(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::ops_max() const { return static_cast<double>(_num_operations) / utils::min(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::ops_mean() const { return static_cast<double>(_num_operations) / utils::mean(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::ops_stdev() const { return static_cast<double>(_num_operations) / utils::stdev(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::ops_min() const { return static_cast<double>(_num_operations) / utils::max(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::runtime_max() const { return utils::max(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::runtime_mean() const { return utils::mean(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::runtime_stdev() const { return utils::stdev(_runtimes); }

// _____________________________________________________________________________________________________________________
double BenchmarkResult::runtime_min() const { return utils::min(_runtimes); }

// _____________________________________________________________________________________________________________________
const std::vector<seconds>& BenchmarkResult::runtimes() const { return _runtimes; }

// _____________________________________________________________________________________________________________________
nlohmann::json BenchmarkResult::json() {
  std::vector<double> runtimes_double(_runtimes.size());
  std::transform(_runtimes.begin(), _runtimes.end(), runtimes_double.begin(),
                 [](auto val) -> double { return val.count(); });
  nlohmann::json j;
  j["name"] = _name;
  j["data_size"] = _data_size;
  j["iterations"] = _runtimes.size();
  j["runtimes"] = runtimes_double;
  return j;
}

// _____________________________________________________________________________________________________________________
const std::string& BenchmarkResult::name() const { return _name; }

// _____________________________________________________________________________________________________________________
uint64_t BenchmarkResult::data_size() const { return _data_size; }

// === AbstractBenchmark ===============================================================================================
// _____________________________________________________________________________________________________________________
void AbstractBenchmark::reset() { _benchmark_result.clear(); }

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::set_verbosity(taskbench::VERBOSITY verbosity) { _verbosity = verbosity; }

// _____________________________________________________________________________________________________________________
std::map<std::string, BenchmarkResult> AbstractBenchmark::results() { return _benchmark_result; }

// _____________________________________________________________________________________________________________________
const std::map<std::string, BenchmarkResult>& AbstractBenchmark::results() const { return _benchmark_result; }

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_register_benchmark(uint64_t data_size, uint64_t num_operations, const std::string& name) {
  _benchmark_result.insert({name, BenchmarkResult(name, data_size, num_operations)});
}
// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_add_result(const std::string& key, seconds value) {
  if (!_benchmark_result.contains(key)) {
    throw std::runtime_error("Benchmark must be registered before results can be added.");
  }
  _benchmark_result.at(key).add_runtime(value);
}

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_print_runtime(const taskbench::BenchmarkResult& bm_res) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print("\r                                                             ");
    fmt::print(fg(fmt::color::azure), "\r    {:40} ", bm_res.name());
    fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", bm_res.runtime_mean(), bm_res.runtime_stdev());
    std::cout << std::flush;
  }
}

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_print_gib_per_second(const taskbench::BenchmarkResult& bm_res) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]", bm_res.bps_max() / S_1_GiB);
    std::cout << std::flush;
  }
}

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_print_o_per_second(const taskbench::BenchmarkResult& bm_res) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::blue_violet), "[{}]", utils::pretty_ops(bm_res.ops_max()));
    std::cout << std::flush;
  }
}

}  // namespace taskbench