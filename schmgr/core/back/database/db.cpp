#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/utils.h"
#include "schmgr/core/core.h"

#include "schmgr/core/back/database/db.h"

namespace core::database
{


TableInfo::TableInfo()
{
    name = "";
    file = "";
}


TableInfo::TableInfo(mtl::string name)
{
    this->name = name;
    this->file = GetExePath() + "\\db\\" + name + ".csv";
}


bool Record::Loads(const mtl::string& line)
{
    size_t pos = 0;
    mtl::vector<mtl::string>().swap(*this);
    for (;;)
    {
        size_t pos2 = line.find(',', pos);
        if (pos2 == mtl::string::npos)
        {
            this->push_back(line.substr(pos));
            return true;
        }
        this->push_back(line.substr(pos, pos2 - pos));
        pos = pos2 + 1;
    }
    return true;
}

mtl::string Record::Dumps() const
{
    mtl::string result;
    const size_t N = this->size();
    for (size_t i = 0; i < N; i++)
    {
        result += (*this)[i];
        if (i != N - 1)
        {
            result += ",";
        }
    }
    return result;
}

mtl::vector<int64_t> Record::GetVectorData(size_t col) const {
    mtl::vector<int64_t> ret;
    if (col >= this->size()) { return ret; }
    mtl::string data = (*this)[col];
    size_t pos = 0;
    for (;;)
    {
        size_t pos2 = data.find('|', pos);
        if (pos2 == mtl::string::npos)
        {
            try
            {
                ret.push_back(std::stoll(data.substr(pos)));
            }
            catch (...)
            {
                ret.push_back(0);
            }
            return ret;
        }
        try
        {
            ret.push_back(std::stoll(data.substr(pos, pos2 - pos)));
        }
        catch (...)
        {
            ret.push_back(0);
        }
        pos = pos2 + 1;
    }
    return ret;
};

mtl::vector<mtl::string> Record::GetVectorString(size_t col) const {
    mtl::vector<mtl::string> ret;
    if (col >= this->size()) { return ret; }
    mtl::string data = (*this)[col];
    size_t pos = 0;
    for (;;)
    {
        size_t pos2 = data.find('|', pos);
        if (pos2 == mtl::string::npos)
        {
            ret.push_back(data.substr(pos));
            return ret;
        }
        ret.push_back(data.substr(pos, pos2 - pos));
        pos = pos2 + 1;
    }
    return ret;
};

void Record::SetVectorData(size_t col, const mtl::vector<int64_t>& data) {
    if (col >= this->size()) { return; }
    mtl::string result;
    const size_t N = data.size();
    for (size_t i = 0; i < N; i++)
    {
        result += std::to_string(data[i]);
        if (i != N - 1)
        {
            result += "|";
        }
    }
    (*this)[col] = result;
}

void Record::SetVectorString(size_t col, const mtl::vector<mtl::string>& data) {
    if (col >= this->size()) { return; }
    mtl::string result;
    const size_t N = data.size();
    for (size_t i = 0; i < N; i++)
    {
        result += data[i];
        if (i != N - 1)
        {
            result += "|";
        }
    }
    (*this)[col] = result;
}

Table::Table()
{
}

Table::Table(const mtl::string& name)
{
    SetName(name);
}

Table::~Table()
{
}

inline void Table::SetName(const mtl::string& name)
{
    info.SetName(name);
    info.SetFile(GetExePath() + "\\db\\" + name + ".csv");
}

inline mtl::string Table::GetName()
{
    return info.GetName();
}

bool Table::LoadTable()
{
    std::ifstream fileDb(info.GetFile());
    if (!fileDb.is_open())
    {
        std::ofstream fileDbNew(info.GetFile());
        if (!fileDbNew.is_open())
        {
            return false;
        }
        fileDbNew.close();
    }

    Clear();
    mtl::string line;
    while (std::getline(fileDb, line))
    {
        size_t pos = line.find(',');
        mtl::string keyStr = line.substr(0, pos);
        int64_t key = std::stoll(keyStr);
        records[key] = line;
    }

    return true;
}

bool Table::SaveTable()
{
    std::ofstream fileDb(info.GetFile());
    if (!fileDb.is_open())
    {
        return false;
    }

    for (auto it = records.begin(); it != records.end(); ++it)
    {
        fileDb << it->second.Dumps() << std::endl;
    }

    return true;
}

bool Table::AddFromCsv(const mtl::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        // ConsoleLog(file);
        return false;
    }

    mtl::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty())
        {
            continue;
        }
        int64_t key = GenUid();
        records[key] = Record(line);
        records[key][0] = std::to_string(key);
    }

    return true;
}

int64_t Table::AddRec(const Record& value)
{
    int64_t uid = std::stoll(value[0]);
    if (records.find(uid) != records.end())
    {
        return -1;
    }
    records.insert(std::make_pair(uid, value));
    return uid;
}


bool Table::DeleteRec(int64_t uid)
{
    if (records.find(uid) == records.end())
    {
        return false;
    }
    records.erase(uid);
    return true;
}

bool Table::UpdateRec(const Record& value)
{
    int64_t uid = std::stoll(value[0]);
    if (records.find(uid) == records.end())
    {
        return false;
    }
    records[uid] = value;
    return true;
}


} // namespace core::database
