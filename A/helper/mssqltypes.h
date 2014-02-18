

#pragma once


#include <sstream>
#include "apputil.h"


namespace CCADO
{
    /*
        These functions return a fully self-contained string representing the
        given value which can be used in a SQL statement.  The caller knows nothing
        about the output format except that it can be put directly after an operator
        like =.

        So, strings will be returned with surrounding single-quotes, etc.
    */

    // to truly differentiate between normal strings and ones that have been
    // formatted for SQL statements, this is the type to use.
    class SQLSTRING
    {
    public:
        SQLSTRING()
        {
        }
        SQLSTRING(const SQLSTRING& r) :
            s(r.s)
        {
        }
        ~SQLSTRING()
        {
        }

        SQLSTRING& operator =(const SQLSTRING& r)
        {
            s.assign(r.s);
            return *this;
        }

        TSTRING s;// the string itself
    };


    inline SQLSTRING ToSQL(int n)
    {
        SQLSTRING r;
        _tstringstream ss;
        ss << n;
        r.s = ss.str();
        return r;
    }


    inline SQLSTRING ToSQL(bool n)
    {
        SQLSTRING r;
        if(n)
        {
            r.s = _T("1");
        }
        else
        {
            r.s = _T("0");
        }
        return r;
    }


    inline SQLSTRING ToSQL(const SQLSTRING& s)
    {
        return s;
    }


    inline SQLSTRING ToSQL(const SYSTEMTIME& s)
    {
        TCHAR tszDatetime[50] = {0};
        WORD wHour = s.wHour;
        SQLSTRING r;
        // convert to a nice string
        PCTSTR tszAMPM = _T("AM");
        if(wHour > 12)
        {
            wHour -= 12;
            tszAMPM = _T("PM");
        }
        _stprintf(tszDatetime, _T("'%04d%02d%02d %02d:%02d:%02d.%003d'"), s.wYear, s.wMonth, s.wDay, wHour, s.wMinute, s.wSecond, s.wMilliseconds);
        r.s = tszDatetime;
        return r;
    }


    inline SQLSTRING ToSQL(CTSTRING& s)
    {
        SQLSTRING r;
        TSTRING temp(s);
        r.s = _T("'");
        HStrReplace(temp, _T("'"), _T("''"));
        r.s.append(temp);
        r.s.append(_T("'"));
        return r;
    }

    inline SQLSTRING ToSQL(PCTSTR s)
    {
        return ToSQL(TSTRING(s));
    }
}
