

#pragma once


#include "ichar.h"
#include <map>


namespace Winprog
{
    typedef int Alias;

    class AliasList
    {
        friend class db;
    public:
        bool Reset()
        {
            m_it = m_list.begin();
            return true;
        }

        bool Next(Alias& id, TSTRING& s)
        {
            bool r = false;
            if(m_it != m_list.end())
            {
                r = true;
                AliasList_T::value_type& p = *m_it;
                id = p.first;
                s = p.second;

                m_it++;
            }
            return r;
        }

    private:
        bool _Clear()
        {
            m_list.clear();
            return true;
        }

        bool _Insert(const Alias& id, CTSTRING& s)
        {
            std::pair<AliasList_T::iterator, bool> insertResult;
            insertResult = m_list.insert(AliasList_T::value_type(id, s));
            return insertResult.second;
        }

        typedef std::map<Alias, TSTRING> AliasList_T;
        AliasList_T m_list;
        AliasList_T::iterator m_it;
    };
}// namespace Winprog


