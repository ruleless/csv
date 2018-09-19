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
#include <typeinfo>

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

struct ConvertException : public std::exception
{
    virtual const char *what() const throw()
    {
        return "invalid conversion type";
    }
};

template<typename T>
struct ConvertToString
{
    std::string tostr(const T &val) const
    {
        if (typeid(T) == typeid(int) ||
            typeid(T) == typeid(long) ||
            typeid(T) == typeid(long long) ||
            typeid(T) == typeid(unsigned) ||
            typeid(T) == typeid(unsigned long) ||
            typeid(T) == typeid(unsigned long long) ||
            typeid(T) == typeid(float) ||
            typeid(T) == typeid(double) ||
            typeid(T) == typeid(long double) ||
            typeid(T) == typeid(char))
        {
            return (std::ostringstream() << val).str();
        } else {
            throw ConvertException();
        }
    }
};

template<typename T>
struct ConvertToValue
{
    T operator()(const std::string &s) const
    {
        try {
            if (typeid(T) == typeid(int)) {
                return static_cast<T>(std::stoi(s));
            } else if (typeid(T) == typeid(long)) {
                return static_cast<T>(std::stol(s));
            } else if (typeid(T) == typeid(long long)) {
                return static_cast<T>(std::stoll(s));
            } else if (typeid(T) == typeid(unsigned)) {
                return static_cast<T>(std::stoul(s));
            } else if (typeid(T) == typeid(unsigned long)) {
                return static_cast<T>(std::stoul(s));
            } else if (typeid(T) == typeid(unsigned long long)) {
                return static_cast<T>(std::stoull(s));
            } else if (typeid(T) == typeid(float)) {
                return static_cast<T>(std::stof(s));
            } else if (typeid(T) == typeid(double)) {
                return static_cast<T>(std::stod(s));
            } else if (typeid(T) == typeid(long double)) {
                return static_cast<T>(std::stold(s));
            } else if (typeid(T) == typeid(char)) {
                return static_cast<T>(s.at(0));
            } else {
                throw ConvertException();
            }
        } catch (...) {
            throw;
        }
    }
};

class Parser
{
  public:
    using ROW = std::vector<std::string>;
    using ROWS = std::vector<ROW>;

    Parser()
            :separator_(',')
            ,rows_()
            ,colnum_(0)
    { }

    virtual ~Parser() { }

    inline size_t rownum() const { return rows_.size(); }
    inline size_t colnum() const { return colnum_; }

    std::string get(size_t r, size_t l) const
    {
        return rows_.at(r).at(l);
    }

    int getInt(size_t r, size_t l) const
    {
        return ConvertToValue<int>()(rows_.at(r).at(l));
    }
    long getLong(size_t r, size_t l) const
    {
        return ConvertToValue<long>()(rows_.at(r).at(l));
    }
    long long getInt64(size_t r, size_t l) const
    {
        return ConvertToValue<long long>()(rows_.at(r).at(l));
    }
    unsigned getUInt(size_t r, size_t l) const
    {
        return ConvertToValue<unsigned>()(rows_.at(r).at(l));
    }
    unsigned long getULong(size_t r, size_t l) const
    {
        return ConvertToValue<unsigned long>()(rows_.at(r).at(l));
    }
    unsigned long long getUInt64(size_t r, size_t l) const
    {
        return ConvertToValue<unsigned long long>()(rows_.at(r).at(l));
    }
    float getFloat(size_t r, size_t l) const
    {
        return ConvertToValue<float>()(rows_.at(r).at(l));
    }
    double getDouble(size_t r, size_t l) const
    {
        return ConvertToValue<double>()(rows_.at(r).at(l));
    }
    char getChar(size_t r, size_t l) const
    {
        return ConvertToValue<char>()(rows_.at(r).at(l));
    }

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
        clear();

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
        return trim(ele);
    }

  private:
    char separator_;
    ROWS rows_;
    size_t colnum_;
};

} // namespace csv

#endif // __CSV_H__
