#ifndef UTIL_H
#define UTIL_H

#include <vector>

namespace ClankerSim {

// Template helper functions demonstrating generic programming.
template <typename T>
inline size_t countAlive(const std::vector<T*>& items) {
    size_t count = 0;
    for (auto* item : items) {
        if (item && !item->isDestroyed()) {
            ++count;
        }
    }
    return count;
}

template <typename T>
inline T* firstAlive(const std::vector<T*>& items) {
    for (auto* item : items) {
        if (item && !item->isDestroyed()) {
            return item;
        }
    }
    return nullptr;
}

}

#endif // UTIL_H
