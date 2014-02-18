

// represents a where clause
#pragma once
#include <vector>
#include "apputil.h"


namespace CCADO
{
    // Generates a NAME=Value pair
    template<typename T>
    TSTRING WhereEq(CTSTRING& name, const T& value)
    {
        SQLSTRING ssql = ToSQL(value);
        TSTRING r;
        sprintf(r, _T("%s=%s"), name.c_str(), ssql.s.c_str());
        return r;
    }

    // Generates a NAME LIKE Value pair
    inline TSTRING WhereLike(CTSTRING& name, CTSTRING& value)
    {
        SQLSTRING ssql = ToSQL(value);
        TSTRING r;
        sprintf(r, _T("%s LIKE %s"), name.c_str(), ssql.s.c_str());
        return r;
    }

    // Otherwise known as criteria, or a "search condition"
    class Where
    {
    public:
        Where()
        {
        }
        ~Where()
        {
        }
        void assign(const Where& r)
        {
            m_sCriteria = r.m_sCriteria;
        }
        TSTRING ToString()
        {
            return m_sCriteria;
        }
        void SetCriteria(CTSTRING& s)
        {
            m_sCriteria = s;
        }
        void AddToken(CTSTRING& s)
        {
            if(!m_sCriteria.empty())
            {
                m_sCriteria.push_back(_T(' '));
            }
            m_sCriteria.append(s);
        }
    private:

        TSTRING m_sCriteria;
    };
}

