#ifndef CORE_DATABASE_DB_H_
#define CORE_DATABASE_DB_H_

#include "schmgr/core/mtl/mtl.h"

namespace core::database
{

class TableInfo
{

private:
    mtl::string name;
    mtl::string file;

public:
    TableInfo();
    // ~TableInfo();

    TableInfo(mtl::string name);
    void SetName(mtl::string n) { name = n; };
    mtl::string GetName() { return name; };
    void SetFile(mtl::string f) { file = f; };
    mtl::string GetFile() { return file; };

};

template <class Kty, class Ty>
using Index = mtl::multimap<Kty, Ty>;


class Record : public mtl::vector<mtl::string>
{
public:
    Record() {};
    Record(const mtl::string& line) { Loads(line); };
    bool Loads(const mtl::string& line);
    mtl::string Dumps() const;
    mtl::vector<int64_t> GetVectorData(size_t col) const;
    mtl::vector<mtl::string> Record::GetVectorString(size_t col) const;
    void SetVectorData(size_t col, const mtl::vector<int64_t>& data);
    void SetVectorString(size_t col, const mtl::vector<mtl::string>& data);

    bool isEmpty() const { return size() == 0; };
};

class Table
{
private:
    TableInfo info;
    mtl::map<int64_t, Record> records;
public:

    Table();
    // ~Table() = default;

    Table(const mtl::string& name);

    ~Table();

    void SetName(const mtl::string& name);
    mtl::string GetName();

    bool LoadTable();

    virtual bool CalcIndex() = 0;

    template <class Kty>
    void RemoveIndex(Index<Kty, int64_t>& index, int64_t uid)
    {
        for (auto it = index.begin();it != index.end();++it)
        {
            if (it->second == uid)
            {
                index.erase(it);
                return;
            }
        }
    }

    template <class Kty>
    void AddIndex(Index<Kty, int64_t>& index, int64_t uid, Kty key)
    {
        index.insert(std::make_pair(key, uid));
    }

    template <class Kty>
    mtl::string DumpsIndex(Index<Kty, int64_t>& index) const
    {
        mtl::string ret;
        for (auto it = index.begin();it != index.end();++it)
        {
            ret += std::to_string(it->first) + ":" + std::to_string(it->second) + "\r";
        }
        return ret;
    }

    bool SaveTable();

    bool AddFromCsv(const mtl::string& file);

    int64_t AddRec(const Record& value);

    bool DeleteRec(int64_t uid);

    bool UpdateRec(const Record& value);

    Record Find(int64_t uid) {
        auto it = records.find(uid);
        if (it == records.end())
        {
            return Record();
        }
        return records.find(uid)->second;
    };

    const mtl::map<int64_t, Record>& GetRecords() const { return records; };

    void Clear()
    {
        // mtl::map<int64_t, Record>().swap(records);
        records.clear();
    };

    void SetInfo(const TableInfo& i) { info = i; };

};

} // namespace core::database


#endif // CORE_DATABASE_DB_H_