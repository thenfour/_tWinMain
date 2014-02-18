

#pragma once


#include "where.h"
#include <vector>
#include "mssqltypes.h"
#include "tokenizer.h"


namespace CCADO
{
    // Storage class for record "UPDATE"s
    // UPDATE MyTable SET myfield=myval, myfield2=myval WHERE id=5
    class Update
    {
    public:
        /////////////////////////////////////////
        Update()
        {
        }

        ~Update()
        {
        }

        void assign(const Update& r)
        {
            m_sTable = r.m_sTable;
            m_Values = r.m_Values;
            m_Where.assign(r.m_Where);
        }

        TSTRING ToString()
        {
            CTokenBuilder s;// whole statement
            s.SetDelimiter(_T(" "));

            // Construct the value list
            CTokenBuilder vl;// value list
            vl.SetDelimiter(_T(", "));
            TSTRING sVal;// single value.

            ValueList_T::iterator it;
            for(it=m_Values.begin();it!=m_Values.end();it++)
            {
                std::pair<TSTRING, SQLSTRING>& val = *it;
                sVal.assign(val.first);
                sVal.push_back('=');
                sVal.append(val.second.s);
                vl.Append(sVal);
            }

            s.Append(_T("UPDATE"));
            s.Append(m_sTable);
            s.Append(_T("SET"));
            s.Append(vl.str());
            s.Append(_T("WHERE"));
            s.Append(m_Where.ToString());

            return s.str();
        }

        /////////////////////////////////////////
        CTSTRING& GetTable()
        {
            return m_sTable;
        }

        void SetTable(CTSTRING& s)
        {
            m_sTable = s;
        }

        void AddValue(CTSTRING& sName, const SQLSTRING& sValue)
        {
            m_Values.push_back(std::pair<TSTRING, SQLSTRING>(sName, sValue));
        }

        void SetWhere(const Where& w)
        {
            m_Where.assign(w);
        }

    private:
        TSTRING m_sTable;
        typedef std::vector<std::pair<TSTRING, SQLSTRING> > ValueList_T;
        ValueList_T m_Values;
        Where m_Where;
    };
}

