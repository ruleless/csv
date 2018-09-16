#ifndef __CSV_H__
#define __CSV_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cctype>

namespace csv {

// trim from start
static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
            s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

class Parser
{
  public:
    using ROW = std::vector<std::string>;
    using ROWS = std::vector<ROW>;

    Parser()
            :separator_(',')
            ,rows_()
            ,colnum_(0)
    {}

    virtual ~Parser() {}

    inline size_t rownum() const { return rows_.size(); }
    inline size_t colnum() const { return colnum_; }

    bool loadcsv(const std::string &path)
    {
        std::ifstream fs(path);
        return parseStream(fs);
    }

    bool parseString(const std::string &str)
    {
        std::istringstream is(str);
        return parseStream(is);
    }

    bool parseStream(std::istream &is)
    {
        std::string line;
        while (std::getline(is, line)) {
            if (trim(line).empty()) {
                continue;
            }

            // add a row
            rows_.push_back(ROW {});
            ROW &row = rows_.back();
            std::cout<<line<<std::endl;

            auto i = line.begin();
            auto j = std::find(i, line.end(), separator_);
            for (; j != line.end();
                 i = ++j, j = std::find(i, line.end(), separator_))
            {
                addElement(row, i, j);
            }
            addElement(row, i, j);

            if (!colnum_) {
                colnum_ = row.size();
            } else if (colnum_ != row.size()) {
                clear();
                return false;
            }
        }

        return true;
    }

    void clear()
    {
        rows_.clear();
        colnum_ = 0;
    }

  private:
    template<typename InputIterator>
    std::string &addElement(ROW &row, InputIterator first, InputIterator last)
    {
        row.push_back(std::string {});
        std::string &ele = row.back();
        std::copy(first, last, std::back_inserter(ele));
        return ele;
    }

  private:
    char separator_;
    ROWS rows_;
    size_t colnum_;
};

} // namespace csv

#endif // __CSV_H__
