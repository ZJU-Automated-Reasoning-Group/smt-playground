#pragma once

#include <memory>
#include <vector>

template<typename T>
class knn {
public:
    knn() = default;
    void set_k(int k);
    void set_train_samples(const std::vector<std::vector<T>> &samples, const std::vector<T> &labels);
    int predict(const std::vector<T> &sample, T &result) const;
private:
    int k = 3;
    int feature_length = 0;
    int samples_number = 0;
    std::unique_ptr<T[]> samples;
    std::unique_ptr<T[]> labels;
};
