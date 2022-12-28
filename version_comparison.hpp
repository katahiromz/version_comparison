#pragma once

#include <string>
#include <vector>
#include <cassert>

namespace version_comparison
{
    namespace detail
    {
        template <typename T_STR_CONTAINER>
        inline void
        split(T_STR_CONTAINER& container,
              const typename T_STR_CONTAINER::value_type& str,
              const typename T_STR_CONTAINER::value_type& chars)
        {
            container.clear();
            size_t i = 0, k = str.find_first_of(chars);
            while (k != T_STR_CONTAINER::value_type::npos)
            {
                container.push_back(str.substr(i, k - i));
                i = k + 1;
                k = str.find_first_of(chars, i);
            }
            container.push_back(str.substr(i));
        }
    } // namespace detail

    inline int
    compare(const std::string& str1, const std::string& str2)
    {
        std::vector<std::string> vec1, vec2;
        std::string dot = ".";
        detail::split(vec1, str1, dot);
        detail::split(vec2, str2, dot);

        for (size_t i = 0; i < vec1.size() && i < vec2.size(); ++i)
        {
            std::string& s1 = vec1[i];
            std::string& s2 = vec2[i];
            int n1 = atoi(s1.c_str());
            int n2 = atoi(s2.c_str());
            if (n1 < n2)
                return -1;
            if (n1 > n2)
                return +1;
            if (s1 < s2)
                return -1;
            if (s1 > s2)
                return +1;
        }

        if (vec1.size() < vec2.size())
            return -1;
        if (vec1.size() > vec2.size())
            return -1;
        return 0;
    }

    inline int
    compare(const char *str1, const char *str2)
    {
        return compare(std::string(str1), std::string(str2));
    }

    inline void unit_test()
    {
        assert(compare("", "") == 0);
        assert(compare("1", "1") == 0);
        assert(compare("1", "2") < 0);
        assert(compare("2", "1") > 0);
        assert(compare("1", "1.0") < 0);
        assert(compare("1", "1.1") < 0);
        assert(compare("1", "1.2") < 0);
        assert(compare("10", "1.0") > 0);
        assert(compare("1.0", "1") < 0);
        assert(compare("1.1", "1") < 0);
        assert(compare("1.2", "1") < 0);
        assert(compare("1.0", "1.1") < 0);
        assert(compare("1.1", "1.0") > 0);
        assert(compare("1.0", "1.0 alpha") < 0);
        assert(compare("1.0 beta", "1.0 alpha") > 0);
        assert(compare("1.0 beta", "1.1 alpha") < 0);
        assert(compare("1.0.1", "1.0.0") > 0);
        assert(compare("1.0.0", "1.0.1") < 0);
        assert(compare("1.0.0", "1.1.1") < 0);
        assert(compare("1.10.0", "1.1.0") > 0);
    }
} // namespace version_comparison
