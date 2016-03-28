//
// Created by Borin Ouch on 2016-03-27.
//

#ifndef SFML_TEST_RESOURCE_MANAGER_H
#define SFML_TEST_RESOURCE_MANAGER_H


#include <string>


class ResourceManager {

public:
    static ResourceManager* getInstance();

public:
    void setBasePath(const std::string& path);
    void addResourceMapping(const std::string& extension, const std::string& path);

    std::string getResourcePath(const std::string& filename);

private:
    ResourceManager();

    static ResourceManager* _instance;
};


#endif //SFML_TEST_RESOURCE_MANAGER_H
