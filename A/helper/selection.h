#pragma once


#include "where.h"
#include <vector>
#include "tokenizer.h"


namespace CCADO
{
    // Storage class for record criteria
    // SELECT [fieldlist] FROM [Table] WHERE [where]
    class Selection
    {
    public:
        Selection()
        {
            m_bTop = false;
            m_nTopRows = 0;
        }

        ~Selection()
        {
        }

        TSTRING ToString()
        {
            CTokenBuilder s;// whole statement, space separated
            s.SetDelimiter(_T(" "));

            // field list
            CTokenBuilder fl;// field list, comma separated
            fl.SetDelimiter(_T(", "));
            if(m_FieldList.empty())
            {
                fl.Append(_T("*"));
            }
            else
            {
                FieldList_T::iterator it;
                for(it=m_FieldList.begin();it!=m_FieldList.end();it++)
                {
                    fl.Append(*it);
                }
            }

            TSTRING sWhere = m_Where.ToString();

            s.Append(_T("SELECT"));
            if(m_bTop)
            {
                s.Append(_T("TOP"));
                s.Append(ToSQL(m_nTopRows).s);
            }
            s.Append(fl.str());
            s.Append(_T("FROM"));
            s.Append(m_sTable);
            if(!sWhere.empty())
            {
                s.Append(_T("WHERE"));
                s.Append(m_Where.ToString());
            }

            return s.str();
        }

        void SetTop(bool b, int nRows)
        {
            m_bTop = b;
            m_nTopRows = nRows;
        }

        void assign(const Selection& r)
        {
            m_FieldList = r.m_FieldList;
            m_sTable = r.m_sTable;
            m_Where.assign(r.m_Where);
        }

        // If no fields are present, all will be returned.
        // Just add "*" for all fields
        void AddField(CTSTRING& s)
        {
            m_FieldList.push_back(s);
        }

        void SetTableName(CTSTRING& s)
        {
            m_sTable = s;
        }

        CTSTRING& GetTableName()
        {
            return m_sTable;
        }

        void SetWhere(const Where& x)
        {
            m_Where.assign(x);
        }

    private:
        typedef std::vector<TSTRING> FieldList_T;
        FieldList_T m_FieldList;
        TSTRING m_sTable;
        Where m_Where;

        bool m_bTop;
        int m_nTopRows;
    };
}

