#ifndef KML_RESOURCE_MANAGER_H
#define KML_RESOURCE_MANAGER_H

#include "ABI.h"

#include <unordered_map>
#include <string>

/**
 * @file    ResourceManager.h
 * @brief   Provides global storage abstraction
 */ 

namespace KML {
    template <typename T> class KML_API ResourceManager {
      protected:
        std::unordered_map<std::string, T> resources;
        void* uptr = nullptr;
        void (*add_callback)(const std::string&, void*) = nullptr;
        void (*remove_callback)(T, void*) = nullptr;
      public:
        ResourceManager(void (*acb)(const std::string&, void*), void (*rcb)(T, void*), void* user_pointer) {
            add_callback = acb;
            remove_callback = rcb;
            uptr = user_pointer;
        }

        inline T Get(std::string key) {
            if(resources.find(key) == resources.end()) return T{};
            return resources.at(key);
        }
        inline void Add(std::string key, T value) {
            if(resources.find(key) != resources.end()) return;
            if(add_callback) add_callback(key, uptr);
            resources.emplace(key, value);
        }
        inline void Remove(std::string key) {
            if(resources.find(key) == resources.end()) return;
            if(remove_callback) remove_callback(resources[key], uptr);
            resources.erase(key);
        }
        inline void Clear() {
            if(remove_callback) for(const auto& i : resources) remove_callback(i.second, uptr);
            resources.clear();
        }
        inline void SetAddCallback(void (*cb)(const std::string&, void*)) { add_callback = cb; }
        inline void SetRemoveCallback(void (*cb)(T, void*)) { remove_callback = cb; }
    };
}

#endif