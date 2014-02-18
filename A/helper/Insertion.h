

#pragma once


#include "mssqltypes.h"
#include "tokenizer.h"


namespace CCADO
{
    // Storage class for record "INSERT"s
    // here we store INSERT INTO MyTable (MyColumn1, MyColumn2) VALUES (1, 2)"
    class Insertion
    {
    public:
        Insertion()
        {
        }
        ~Insertion()
        {
        }
        void assign(const Insertion& r)
        {
            m_sTableName = r.m_sTableName;
            m_Values = r.m_Values;
        }
        TSTRING ToString()
        {
            CTokenBuilder s;// whole statement
            s.SetDelimiter(_T(" "));

            // BUILD THE COLUMNS LIST and VALUES LIST simultaneously
            CTokenBuilder c;// Column list
            CTokenBuilder v;// Value list

            ValueList_T::iterator it;
            for(it=m_Values.begin();it!=m_Values.end();it++)
            {
                std::pair<TSTRING, SQLSTRING>& val = *it;
                c.Append(val.first);
                v.Append(val.second.s);
            }

            // Add the surrounding parenthesis
            TSTRING sc(_T("("));
            sc.append(c.str());
            sc.push_back(_T(')'));
            TSTRING sv(_T("("));
            sv.append(v.str());
            sv.push_back(_T(')'));

            s.Append(_T("INSERT INTO"));
            s.Append(m_sTableName);
            s.Append(sc);
            s.Append(_T("VALUES"));
            s.Append(sv);

            return s.str();
        }

        void SetTableName(CTSTRING& s)
        {
            m_sTableName = s;
        }
        CTSTRING& GetTableName()
        {
            return m_sTableName;
        }
        void AddPair(CTSTRING& sColumn, SQLSTRING& sValue)
        {
            m_Values.push_back(std::pair<TSTRING, SQLSTRING>(sColumn, sValue));
        }

    private:
        TSTRING m_sTableName;
        typedef std::vector<std::pair<TSTRING, SQLSTRING> > ValueList_T;
        ValueList_T m_Values;
    };
}
