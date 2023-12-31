#include <algorithm>
#include <vector>
#include <thread>

template <typename RandomAccessIterator>
using TrairsValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;

template <typename RandomAccessIterator>
void insertionSort(RandomAccessIterator begin, RandomAccessIterator end) {
    for (auto i = begin; i != end; ++i) {
        for (auto j = i; j != begin && *j < *(j - 1); --j) {
            std::iter_swap(j, j - 1);
        }
    }
}

template <typename RandomAccessIterator>
void merge(RandomAccessIterator begin, RandomAccessIterator middle, RandomAccessIterator end) {
    std::vector<TrairsValueType<RandomAccessIterator>> left(begin, middle);
    std::vector<TrairsValueType<RandomAccessIterator>> right(middle, end);

    auto leftIt = left.begin();
    auto rightIt = right.begin();
    auto dest = begin;

    while (leftIt != left.end() && rightIt != right.end()) {
        if (*leftIt < *rightIt) {
            *dest = *leftIt;
            ++leftIt;
        } else {
            *dest = *rightIt;
            ++rightIt;
        }
        ++dest;
    }

    std::copy(leftIt, left.end(), dest);
    std::copy(rightIt, right.end(), dest);
}

template <typename RandomAccessIterator>
void timsort(RandomAccessIterator begin, RandomAccessIterator end) {
    const int minRun = 32;

    auto size = std::distance(begin, end);

    for (auto i = begin; i < end; i += minRun) {
        auto runEnd = std::min(i + minRun, end);
        insertionSort(i, runEnd);
    }

    std::vector<std::thread> threads;

    auto contSize = std::distance(begin, end);

    for (auto size = minRun; size < contSize; size *= 2) {
        for (auto i = begin; i < end; i += 2 * size) {
            auto middle = i + size;
            auto endRun = std::min(i + 2 * size, end);

            if (middle < endRun) {
                threads.emplace_back(merge<RandomAccessIterator>, i, middle, endRun);
            }
        }

        for (auto& thread : threads) {
            thread.join();
        }

        threads.clear();
    }
}
