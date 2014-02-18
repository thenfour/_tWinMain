

#include "StdAfx.h"
#include "dbhelper.h"
namespace Winprog
{

dbHelper::dbHelper(db& x) :
    m_db(x)
{
}

dbHelper::~dbHelper()
{
}

/*
    When just trying to link to tables and stuff, this helps by returning a single
    ID from a query.
*/
bool dbHelper::GetFirstID(CTSTRING& sTable, CTSTRING& sField, CTSTRING& sCriteria, int& n)
{
    bool r = false;

    // SELECT [field] AS 'MyValue' FROM [table] WHERE [criteria]
    TSTRING sQuery;
    sprintf(sQuery, _T("SELECT %s AS 'MyValue' FROM %s WHERE %s"),
        sField.c_str(), sTable.c_str(), sCriteria.c_str());

    CCADO::Recordset rst;

    if(m_db.m_conn.Execute(sQuery, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            rst.Field(_T("MyValue"), var);
            n = var;
            r = true;
        }
    }
    return r;
}

bool dbHelper::GetAliasTotalUsage(const Alias& a, long& n)
{
    bool r = false;
    // SELECT Count(*) AS 'MyCount' FROM WinprogAliasUses WHERE AliasUseAliasId=5
    TSTRING sQuery;
    sprintf(sQuery, _T("SELECT Count(*) AS 'MyCount' FROM %s WHERE %s"),
        TABLE_ALIASUSES,
        CCADO::WhereEq(FIELD_ALIASUSEALIASID, a).c_str());

    CCADO::Recordset rst;

    if(m_db.m_conn.Execute(sQuery, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(_T("MyCount"), var))
            {
                n = var;
                r = true;
            }
        }
    }

    return r;
}

bool dbHelper::GetAliasCreatedBy(const Alias& a, TSTRING& mask)
{
    bool r = false;
    TSTRING sNick, sIdent, sHost;
    Transaction nTrans;

    // Get the transaction from the creation record
    if(GetFirstID(TABLE_ALIASCREATIONS, FIELD_ALIASCREATIONTRANSACTIONID,
        CCADO::WhereEq(FIELD_ALIASCREATIONALIASID, a), nTrans))
    {
        r = GetUserAssociatedWithTransaction(nTrans, mask);
    }

    return r;
}

bool dbHelper::GetAliasCreatedOn(const Alias& a, SYSTEMTIME& st)
{
    bool r = false;
    Transaction nTrans;

    // Get the transaction from the creation record
    if(GetFirstID(TABLE_ALIASCREATIONS, FIELD_ALIASCREATIONTRANSACTIONID,
        CCADO::WhereEq(FIELD_ALIASCREATIONALIASID, a), nTrans))
    {
        CCADO::Recordset rstTrans;
        CCADO::Selection s;
        CCADO::Where w;
        w.SetCriteria(CCADO::WhereEq(FIELD_TRANSACTIONID, nTrans));
        s.SetTableName(TABLE_TRANSACTIONS);
        s.SetWhere(w);

        _variant_t var;

        if(m_db.m_conn.Execute(s.ToString(), rstTrans))
        {
            if(!rstTrans.IsEOF())
            {
                // get the date.
                if(rstTrans.Field(FIELD_TRANSACTIONDATE, var))
                {
                    r = (TRUE == VariantTimeToSystemTime(var.date, &st));
                }
            }
        }
    }

    return r;
}

// converts a user recordset to a IRC-style hostmask
bool dbHelper::UserRstToMask(CCADO::Recordset& rstUser, TSTRING& mask)
{
    bool r = false;
    TSTRING sNick, sIdent, sHost;
    if(!rstUser.IsEOF())
    {
        if(rstUser.Field(FIELD_USERNICK, sNick))
        {
            if(rstUser.Field(FIELD_USERIDENT, sIdent))
            {
                if(rstUser.Field(FIELD_USERHOST, sHost))
                {
                    sprintf(mask, _T("%s!%s@%s"), sNick.c_str(), sIdent.c_str(), sHost.c_str());
                    r = true;
                }
            }
        }
    }
    return r;
}

bool dbHelper::GetUserAssociatedWithTransaction(const Transaction& t, TSTRING& mask)
{
    bool r = false;

    User nUser;
    // Get the user from the transaction record
    if(GetFirstID(TABLE_TRANSACTIONS, FIELD_TRANSACTIONUSERID,
        CCADO::WhereEq(FIELD_TRANSACTIONID, t), nUser))
    {
        // Get the user information
        CCADO::Recordset rstUser;
        CCADO::Selection s;
        CCADO::Where w;
        w.SetCriteria(CCADO::WhereEq(FIELD_USERID, nUser));
        s.SetTableName(TABLE_USERS);
        s.SetWhere(w);
        //SELECT * FROM users WHERE User
        if(m_db.m_conn.Execute(s.ToString(), rstUser))
        {
            r = UserRstToMask(rstUser, mask);
        }
    }

    return r;
}

bool dbHelper::GetAliasMessageID(const Alias& a, Message& m)
{
    return GetFirstID(TABLE_ALIASES, FIELD_ALIASMESSAGEID, CCADO::WhereEq(FIELD_ALIASID, a), m);
}

bool dbHelper::GetMessageTotalUsage(const Message& a, long& n)
{
    bool r = false;
    // SELECT Count(*) AS 'MyCount' FROM WinprogMessageUses WHERE MessageUseMessageId=5
    TSTRING sQuery;
    sprintf(sQuery, _T("SELECT Count(*) AS 'MyCount' FROM %s WHERE %s"),
        TABLE_MESSAGEUSES,
        CCADO::WhereEq(FIELD_MESSAGEUSEMESSAGEID, a).c_str());

    CCADO::Recordset rst;

    if(m_db.m_conn.Execute(sQuery, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(_T("MyCount"), var))
            {
                n = var;
                r = true;
            }
        }
    }

    return r;
}

bool dbHelper::GetMessageCreatedBy(const Message& a, TSTRING& mask)
{
    bool r = false;
    TSTRING s;
    CCADO::Recordset rst;
    _tostringstream ss;

    s = _T("select top 1 ")
        _T("WinprogUsers.* ")
        _T("from ")
        _T("WinprogMessages left join ")
        _T("WinprogMessageCreations on MessageCreationMessageID = MessageID left join ")
        _T("WinprogTransactions on MessageCreationTransactionID = TransactionID left join ")
        _T("WinprogUsers on TransactionUserID = UserID ")
        _T("where ")
        _T("MessageID = ");

    ss << s << a;

    if(m_db.m_conn.Execute(ss.str(), rst))
    {
        r = UserRstToMask(rst, mask);
    }

    return r;
}

bool dbHelper::GetMessageCreatedOn(const Message& a, SYSTEMTIME& st)
{
    bool r = false;
    //Transaction nTrans;
    TSTRING s;
    CCADO::Recordset rst;
    _tostringstream ss;
    _variant_t var;

    s = _T("select top 1 ")
        _T("    WinprogTransactions.* ")
        _T("from ")
        _T("    WinprogMessages left join ")
        _T("    WinprogMessageCreations on MessageCreationMessageID = MessageID left join ")
        _T("    WinprogTransactions on MessageCreationTransactionID = TransactionID ")
        _T("where ")
        _T("    MessageID = ");
    ss << s << a;

    if(m_db.m_conn.Execute(ss.str(), rst))
    {
        if(!rst.IsEOF())
        {
            if(rst.Field(FIELD_TRANSACTIONDATE, var))
            {
                r = (TRUE == VariantTimeToSystemTime(var.date, &st));
            }
        }
    }

    return r;
}

bool dbHelper::GetUserRst(const User& u, CCADO::Recordset& rst)
{
    bool r = false;
    CCADO::Selection s;
    CCADO::Where w;
    w.SetCriteria(CCADO::WhereEq(FIELD_USERID, u));
    s.SetTableName(TABLE_USERS);
    s.SetWhere(w);
    r = m_db.m_conn.Execute(s.ToString(), rst);
    return r;
}

bool dbHelper::GetTransactionRst(const Transaction& t, CCADO::Recordset& rst)
{
    bool r = false;
    CCADO::Selection s;
    CCADO::Where w;
    w.SetCriteria(CCADO::WhereEq(FIELD_TRANSACTIONID, t));
    s.SetTableName(TABLE_TRANSACTIONS);
    s.SetWhere(w);
    r = m_db.m_conn.Execute(s.ToString(), rst);
    return r;
}

bool dbHelper::GetAliasLastModifiedTransactionRst(const Alias& a, CCADO::Recordset& rstTrans)
{
    bool r = false;
    _tostringstream ss;

    ss << _T("select ")
        _T("    top 1 ")
        _T("    * ")
        _T(" from ")
        _T("    WinprogAliasUpdates left join ")
        _T("    WinprogAliases on AliasUpdateAliasID = AliasID left join ")
        _T("    WinprogTransactions on AliasUpdateTransactionID = TransactionID ")
        _T(" where ")
        _T("    AliasID = ") << a << 
        _T(" order by TransactionDate desc");

    CCADO::Recordset rst;
    r = m_db.m_conn.Execute(ss.str(), rstTrans);

    return r;
}

bool dbHelper::GetAliasLastModifiedOn(const Alias& a, SYSTEMTIME& st)
{
    bool r = false;
    CCADO::Recordset rst;
    if(GetAliasLastModifiedTransactionRst(a, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_TRANSACTIONDATE, var))
            {
                r = (TRUE == VariantTimeToSystemTime(var.date, &st));
            }
        }
    }

    return r;
}

bool dbHelper::GetAliasLastModifiedBy(const Alias& a, TSTRING& mask)
{
    bool r = false;
    CCADO::Recordset rst;
    if(GetAliasLastModifiedTransactionRst(a, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_TRANSACTIONUSERID, var))
            {
                User u = var;
                CCADO::Recordset rstUser;
                if(GetUserRst(u, rstUser))
                {
                    r = UserRstToMask(rstUser, mask);
                }
            }
        }
    }

    return r;
}


bool dbHelper::GetMessageLastModifiedTransactionRst(const Message& a, CCADO::Recordset& rstTrans)
{
    bool r = false;
    _tostringstream ss;

    ss << _T("select ")
        _T("    top 1 ")
        _T("    * ")
        _T(" from ")
        _T("    WinprogMessageUpdates left join ")
        _T("    WinprogMessages on MessageUpdateMessageID = MessageID left join ")
        _T("    WinprogTransactions on MessageUpdateTransactionID = TransactionID ")
        _T(" where ")
        _T("    MessageID = ") << a << 
        _T(" order by TransactionDate desc");

    CCADO::Recordset rst;
    r = m_db.m_conn.Execute(ss.str(), rstTrans);

    return r;
}

bool dbHelper::GetMessageLastModifiedOn(const Message& a, SYSTEMTIME& st)
{
    bool r = false;
    CCADO::Recordset rst;
    if(GetMessageLastModifiedTransactionRst(a, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_TRANSACTIONDATE, var))
            {
                r = (TRUE == VariantTimeToSystemTime(var.date, &st));
            }
        }
    }

    return r;
}

bool dbHelper::GetMessageLastModifiedBy(const Message& a, TSTRING& mask)
{
    bool r = false;
    CCADO::Recordset rst;
    if(GetMessageLastModifiedTransactionRst(a, rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_TRANSACTIONUSERID, var))
            {
                User u = var;
                CCADO::Recordset rstUser;
                if(GetUserRst(u, rstUser))
                {
                    r = UserRstToMask(rstUser, mask);
                }
            }
        }
    }

    return r;
}

bool dbHelper::GetAliasList(const Alias& a, std::vector<TSTRING>& items)
{
    bool r = false;

    items.clear();
    _tostringstream ss;
    CCADO::Recordset rst;
    Message m;

    if(GetAliasMessageID(a, m))
    {
        ss << _T("select ")
            _T("    AliasText ")
            _T("from ")
            _T("    WinprogAliases left join ")
            _T("    WinprogMessages on AliasMessageID = MessageID ")
            _T("where ")
            _T("    AliasEnabled = 1 and ")
            _T("    MessageID = ") << m;

        TSTRING sTemp;
        if(m_db.m_conn.Execute(ss.str(), rst))
        {
            while(1)
            {
                if(rst.IsEOF())
                {
                    r = true; 
                    break;
                }

                if(!rst.Field(FIELD_ALIASTEXT, sTemp))
                {
                    break;
                }

                items.push_back(sTemp);

                if(!rst.MoveNext())
                {
                    break;
                }
            }
        }
    }
    return r;
}


}


