#pragma once 

#include <vector>
#include <string>
#include <fstream>

#define MAX_STRINGN 15

struct Profile
{
    char login[MAX_STRINGN];
    char password[MAX_STRINGN];
    int money;
    int record;
};


class Profiles
{
private:
    std::vector<Profile> profiles;
public:
    Profiles(std::string filename);
    bool write(std::string filename);
    ~Profiles();
};

Profiles::Profiles(std::string filename)
{
    std::ifstream file;
    Profile profile;
    file.open(filename);
    if(!file)
        return;
    while(!file.eof())
        {
            file.read((char*)&profile,sizeof(Profile));
            profiles.push_back(profile);
        }    
    file.close();
}

bool Profiles::write(std::string filename)
{
    std::ofstream file;
    Profile profile;
    file.open(filename);
    if(!file)
        return false;
    for(auto it : profiles)
        {
            file.write((char*)&it,sizeof(Profile));
        } 
    file.close();
}


Profiles::~Profiles()
{
}
