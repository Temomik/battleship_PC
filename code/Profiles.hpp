#pragma once 

#include <vector>
#include <string>
#include <fstream>
#include <cstring>

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
    void addProfile(Profile& profile);
    bool isConsistLogin(Profile& profile);
    Profile getProfile(Profile& profile);
    void update(Profile& profile);
    bool isConsistProfile(Profile& profile);
    ~Profiles();
};

Profiles::Profiles(std::string filename)
{
    std::ifstream file;
    Profile profile;
    file.open(filename);
    if(!file)
        return;
    do
    {
            file.read((char*)&profile,sizeof(Profile));
            profiles.push_back(profile);
    }while(!file.eof());
    profiles.pop_back();    
    file.close();
}

void Profiles:: update(Profile& profile)
{
    for(int i = 0 ; i < profiles.size();i++)
    {
        if(strcmp(profile.login,profiles[i].login) == 0)
        {
            profiles[i].record = profile.record;
            profiles[i].money = profile.money;
            break;
        }
    }
}

Profile Profiles::getProfile(Profile& profile)
{
    for(auto it : profiles)
    {
        if(strcmp(profile.login,it.login) == 0)
            return it;
    }
}

void Profiles::addProfile(Profile& profile)
{
    profiles.push_back(profile);
}

bool Profiles::isConsistLogin(Profile& profile)
{
    for(auto it : profiles)
    {
        if(strcmp(it.login,profile.login) == 0)
            return true;
    }
    return false;
}

bool Profiles::isConsistProfile(Profile& profile)
{
    for(auto it : profiles)
    {
        if(strcmp(it.login,profile.login) == 0 && strcmp(it.password,profile.password) == 0)
            return true;
    }
    return false;
}

bool Profiles::write(std::string filename)
{
    std::ofstream file;
    Profile profile;
    file.open(filename);
    if(!file)
        return false;
    if(profiles.size() > 0)
        for(auto it : profiles)
            {
                file.write((char*)&it,sizeof(Profile));
            } 
    file.close();
}


Profiles::~Profiles()
{
}
