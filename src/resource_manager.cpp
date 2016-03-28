//
// Created by Borin Ouch on 2016-03-27.
//


#include <string>
#include <sstream>
#include <map>

#include "resource_manager.h"


#ifdef __APPLE__

#include <CoreFoundation/CoreFoundation.h>

#endif//__APPLE__


namespace {

    std::string getBundlePath() {
#ifdef __APPLE__
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourceUrl = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourceUrl, TRUE, (UInt8*)path, PATH_MAX)) {
            return std::string();
        }
        CFRelease(resourceUrl);
        return std::string(path);
#else
        return "";
#endif//__APPLE__
    }


    static const std::string bundle_path = getBundlePath();

    std::string base_path;
    std::map<std::string, std::string> resource_map;

};

ResourceManager* ResourceManager::_instance = new ResourceManager();

std::string ResourceManager::getResourcePath(const std::string &filename) {
    std::stringstream ss;
    ss << bundle_path << "/";

    if (base_path.length() > 0) {
        ss << base_path << "/";
    }

    std::size_t dot = filename.find_last_of(".");
    if (dot != std::string::npos) {
        std::string ext = filename.substr(dot + 1);

        auto found = resource_map.find(ext);
        if (found != resource_map.end()) {
            ss << found->second << "/";
        }

    }

    ss << filename;

    return ss.str();
}

ResourceManager *ResourceManager::getInstance() {
    return nullptr;
}

void ResourceManager::setBasePath(const std::string &path) {
    base_path = path;
}

void ResourceManager::addResourceMapping(const std::string &extension, const std::string &path) {
    resource_map[extension] = path;
}

ResourceManager::ResourceManager() {
}
