

#include "StdAfx.h"
#include "helpdb.h"
#include <cctype>


namespace Winprog
{

db::db()
{
}

db::~db()
{
}


// can only 
bool db::IsValidAliasText(CTSTRING& alias)
{
    TSTRING::const_iterator it;
    TCHAR ch;
    TSTRING validchars(_T("~`!@#$%^&*()_+=-[]\\{}|;\':\"<>?,./"));
    for(it=alias.begin();it!=alias.end();it++)
    {
        ch = *it;
        if(!std::isalnum(ch))
        {
            if(validchars.find(ch) == TSTRING::npos)
            {
                // no valid chars
                return false;
            }
        }
    }
    return true;
}

bool db::Connect(CTSTRING& connstr)
{
    return m_conn.Open(connstr);
}

bool db::Disconnect()
{
    return m_conn.Close();
}

bool db::NewMessage(const TransactInfo& ti, CTSTRING& _text, Message& m)
{
    Transaction nTrans;
    bool r = false;

    TSTRING text = _text;
    if(text.length() > MAX_MESSAGE_TEXT)
    {
        text.resize(MAX_MESSAGE_TEXT);
    }

    // create a new transaction
    if(_CreateNewTransaction(ti, nTrans))
    {
        // create the new message
        CCADO::Insertion i;
        i.SetTableName(TABLE_MESSAGES);
        /*
            We create a blank one first, so the transactions will line up correctly.
            there are two transactions for a "New Message" action:
            1) Create (with blank data)
            2) Update with new message text.
        */
        i.AddPair(FIELD_MESSAGETEXT, CCADO::ToSQL(_T("''")));
        if(m_conn.Execute(i.ToString()))
        {
            _GetLastIdentity(m);

            // create a new MessageCreation record
            CCADO::Insertion i;
            i.SetTableName(TABLE_MESSAGECREATIONS);
            i.AddPair(FIELD_MESSAGECREATIONTRANSACTIONID, CCADO::ToSQL(nTrans));
            i.AddPair(FIELD_MESSAGECREATIONMESSAGEID, CCADO::ToSQL(m));
            if(m_conn.Execute(i.ToString()))
            {
                // We need to update it again simply so we can 
                r = UpdateMessage(ti, m, text);
            }
        }
        // set success
        _SetTransactionSuccess(nTrans, r);
    }

    return r;
}


/*
    Create a new transaction
        Update the message
        Create new messageupdate record
    Set transaction success
*/
bool db::UpdateMessage(const TransactInfo& ti, const Message& m, CTSTRING& newtext)
{
    Transaction nTrans;
    bool r = false;

    // create a new transaction
    if(_CreateNewTransaction(ti, nTrans))
    {
        // update the message
        CCADO::Where w;
        w.SetCriteria(CCADO::WhereEq(FIELD_MESSAGEID, m));

        CCADO::Update u;
        u.SetTable(TABLE_MESSAGES);
        u.SetWhere(w);
        u.AddValue(FIELD_MESSAGETEXT, CCADO::ToSQL(newtext));
        if(m_conn.Execute(u.ToString()))
        {
            // create a new MessageUpdate record
            CCADO::Insertion i;
            i.SetTableName(TABLE_MESSAGEUPDATES);
            i.AddPair(FIELD_MESSAGEUPDATETRANSACTIONID, CCADO::ToSQL(nTrans));
            i.AddPair(FIELD_MESSAGEUPDATEMESSAGEID, CCADO::ToSQL(m));
            i.AddPair(FIELD_MESSAGEUPDATETOTEXT, CCADO::ToSQL(newtext));
            if(m_conn.Execute(i.ToString()))
            {
                r = true;
            }
        }
        // set success
        _SetTransactionSuccess(nTrans, r);
    }

    return r;
}

/*
    Create a new transaction
        Create the new alias
        Create new aliascreate record
        Enable the alias
    Set transaction success
*/
bool db::NewAlias(const TransactInfo& ti, const Message& m, CTSTRING& _alias, Alias& out)
{
    Transaction nTrans;
    bool r = false;

    TSTRING alias;

    alias = _alias;
    if(alias.length() > MAX_ALIAS_TEXT)
    {
        alias.resize(MAX_ALIAS_TEXT);
    }

    // create a new transaction
    if(IsValidAliasText(alias))
    {
        if(_CreateNewTransaction(ti, nTrans))
        {
            // determine if it already exists
            if(AliasEnabled(alias))
            {
                // it's already enabled!  failure.
            }
            else
            {
                // if it EXISTS but is DISABLED, re-enable it.
                Alias existing;
                if(_AliasExists(alias, existing))
                {
                    // ok; we know it exists but it's just disabled.
                    // in addition to changing the MEssageID, change the Enabled flag
                    // update TABLE_ALIASES set AliasEnabled=true, AliasMessageID=m where AliasID=a
                    CCADO::Update u;
                    u.SetTable(TABLE_ALIASES);
                    u.AddValue(FIELD_ALIASMESSAGEID, CCADO::ToSQL(m));
                    u.AddValue(FIELD_ALIASENABLED, CCADO::ToSQL(true));
                    CCADO::Where w;
                    w.SetCriteria(CCADO::WhereEq(FIELD_ALIASID, existing));
                    u.SetWhere(w);
                    if(m_conn.Execute(u.ToString()))
                    {
                        // create a new AliasUpdate record
                        CCADO::Insertion i;
                        i.SetTableName(TABLE_ALIASUPDATES);
                        i.AddPair(FIELD_ALIASUPDATETRANSACTIONID, CCADO::ToSQL(nTrans));
                        i.AddPair(FIELD_ALIASUPDATEALIASID, CCADO::ToSQL(existing));
                        i.AddPair(FIELD_ALIASUPDATETOMESSAGEID, CCADO::ToSQL(m));
                        i.AddPair(FIELD_ALIASUPDATETOENABLED, CCADO::ToSQL(true));
                        if(m_conn.Execute(i.ToString()))
                        {
                            r = true;
                        }
                    }
                }
                else
                {
                    // create the new alias
                    CCADO::Insertion i;
                    i.SetTableName(TABLE_ALIASES);
                    i.AddPair(FIELD_ALIASMESSAGEID, CCADO::ToSQL(m));
                    i.AddPair(FIELD_ALIASTEXT, CCADO::ToSQL(alias));
                    i.AddPair(FIELD_ALIASENABLED, CCADO::ToSQL(false));
                    if(m_conn.Execute(i.ToString()))
                    {
                        _GetLastIdentity(out);

                        // create a new AliasCreation record
                        CCADO::Insertion i;
                        i.SetTableName(TABLE_ALIASCREATIONS);
                        i.AddPair(FIELD_ALIASCREATIONTRANSACTIONID, CCADO::ToSQL(nTrans));
                        i.AddPair(FIELD_ALIASCREATIONALIASID, CCADO::ToSQL(out));
                        if(m_conn.Execute(i.ToString()))
                        {
                            // We need to update it again simply so we can 
                            r = EnableAlias(ti, out, true);
                        }
                    }
                }
            }

            // set success
            _SetTransactionSuccess(nTrans, r);
        }
    }

    return r;
}


bool db::EnableAlias(const TransactInfo& ti, const Alias& alias, bool b)
{
    Transaction nTrans;
    bool r = false;

    // create a new transaction
    if(_CreateNewTransaction(ti, nTrans))
    {
        // update the message
        CCADO::Where w;
        w.SetCriteria(CCADO::WhereEq(FIELD_ALIASID, alias));

        CCADO::Update u;
        u.SetTable(TABLE_ALIASES);
        u.SetWhere(w);
        u.AddValue(FIELD_ALIASENABLED, CCADO::ToSQL(b));
        if(m_conn.Execute(u.ToString()))
        {
            // create a new AliasUpdate record
            CCADO::Insertion i;
            i.SetTableName(TABLE_ALIASUPDATES);
            i.AddPair(FIELD_ALIASUPDATETRANSACTIONID, CCADO::ToSQL(nTrans));
            i.AddPair(FIELD_ALIASUPDATEALIASID, CCADO::ToSQL(alias));
            i.AddPair(FIELD_ALIASUPDATETOENABLED, CCADO::ToSQL(b));
            if(m_conn.Execute(i.ToString()))
            {
                r = true;
            }
        }
        // set success
        _SetTransactionSuccess(nTrans, r);
    }

    return r;
}

bool db::GetFirstAliasForMessage(const Message& m, Alias& out, TSTRING& sOut)
{
    bool r = false;

    // SELECT TOP 1 * FROM Aliases WHERE AliasMessageID=m
    CCADO::Where w;
    w.AddToken(CCADO::WhereEq(FIELD_ALIASMESSAGEID, m));
    w.AddToken(_T("AND"));
    w.AddToken(CCADO::WhereEq(FIELD_ALIASENABLED, true));

    CCADO::Selection s;
    s.SetTop(true, 1);
    s.SetWhere(w);
    s.AddField(FIELD_ALIASID);
    s.AddField(FIELD_ALIASTEXT);
    s.SetTableName(TABLE_ALIASES);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))// perform the select
    {
        if(!rst.IsEOF())// make sure we have a record
        {
            // found a match
            _variant_t var;
            if(rst.Field(FIELD_ALIASID, var))// get the field
            {
                if(var.vt == VT_I4)
                {
                    out = var.intVal;

                    r = rst.Field(FIELD_ALIASTEXT, sOut);
                }
            }
        }
    }

    return r;
}


bool db::GetFirstAliasForMessage(const Message& m, Alias& out)
{
    bool r = false;

    // SELECT TOP 1 * FROM Aliases WHERE AliasMessageID=m
    CCADO::Where w;
    w.AddToken(CCADO::WhereEq(FIELD_ALIASMESSAGEID, m));
    w.AddToken(_T("AND"));
    w.AddToken(CCADO::WhereEq(FIELD_ALIASENABLED, true));

    CCADO::Selection s;
    s.SetTop(true, 1);
    s.SetWhere(w);
    s.AddField(FIELD_ALIASID);
    s.SetTableName(TABLE_ALIASES);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))// perform the select
    {
        if(!rst.IsEOF())// make sure we have a record
        {
            // found a match
            _variant_t var;
            if(rst.Field(FIELD_ALIASID, var))// get the field
            {
                if(var.vt == VT_I4)
                {
                    out = var.intVal;
                    r = true;
                }
            }
        }
    }

    return r;
}


/*
    Uses a query to select a list of aliases.
    We search both alias names and messages to construct the list.
*/
bool db::QueryMessages(const TransactInfo& ti, const CTSTRING& sQuery, AliasList& out, int nRows)
{
    Transaction nTrans;
    bool r = false;

    // create a new transaction
    if(_CreateNewTransaction(ti, nTrans))
    {
        // create the query transaction record
        CCADO::Insertion i;
        i.SetTableName(TABLE_QUERIES);
        i.AddPair(FIELD_QUERYTRANSACTIONID, CCADO::ToSQL(nTrans));
        i.AddPair(FIELD_QUERYTEXT, CCADO::ToSQL(sQuery));
        if(m_conn.Execute(i.ToString()))
        {
            // perform the query
            out._Clear();

            CCADO::Recordset rst;

            if(m_conn.Execute(_T("EXECUTE WinProgMsgQuery ") + CCADO::ToSQL(sQuery).s + _T(", ") + CCADO::ToSQL(nRows).s, rst))
            {
                _variant_t varID;
                bool rid, rtext;
                while(1)
                {
                    if(rst.IsEOF())
                    {
                        r = true;
                        break;
                    }
                    TSTRING sTemp;
                    rid = rst.Field(FIELD_ALIASID, varID);
                    rtext = rst.Field(FIELD_ALIASTEXT, sTemp);
                    if((!rid) || (!rtext))
                    {
                        break;// something failed.
                    }
                    else
                    {
                        if(varID.vt == VT_I4)
                        {
                            out._Insert(varID.intVal, sTemp);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(!rst.MoveNext())
                    {
                        break;
                    }
                }
            }
        }
        // set transaction success
        _SetTransactionSuccess(nTrans, r);
    }
    return r;
}


bool db::FetchMessage(const TransactInfo& ti, CTSTRING& sAlias, TSTRING& sMessage)
{
    Transaction nTransAlias;
    bool r = false;

    // create a new transaction
    if(_CreateNewTransaction(ti, nTransAlias))
    {
        // EXECUTE 
        CCADO::Recordset rst;
        if(m_conn.Execute(_T("EXECUTE WinprogMsg ") + CCADO::ToSQL(sAlias).s, rst))
        {
            if(!rst.IsEOF())
            {
                if(rst.Field(FIELD_MESSAGETEXT, sMessage))
                {
                    Winprog::Alias a;
                    Winprog::Message m;
                    _variant_t var;

                    if(rst.Field(FIELD_MESSAGEID, var))
                    {
                        m = var.intVal;
                        if(rst.Field(FIELD_ALIASID, var))
                        {
                            a = var.intVal;
                            // Create the alias usage record
                            CCADO::Insertion i;
                            i.SetTableName(TABLE_ALIASUSES);
                            i.AddPair(FIELD_ALIASUSETRANSACTIONID, CCADO::ToSQL(nTransAlias));
                            i.AddPair(FIELD_ALIASUSEALIASID, CCADO::ToSQL(a));
                            if(m_conn.Execute(i.ToString()))
                            {
                                // Now create the transaction for the message
                                Transaction nTransMsg;
                                if(_CreateNewTransaction(ti, nTransMsg))
                                {
                                    // Create the message usage record
                                    CCADO::Insertion i;
                                    i.SetTableName(TABLE_MESSAGEUSES);
                                    i.AddPair(FIELD_MESSAGEUSETRANSACTIONID, CCADO::ToSQL(nTransMsg));
                                    i.AddPair(FIELD_MESSAGEUSEMESSAGEID, CCADO::ToSQL(m));
                                    if(m_conn.Execute(i.ToString()))
                                    {
                                        r = true;
                                    }

                                    _SetTransactionSuccess(nTransMsg, r);
                                }// create message transaction
                            }
                        }
                    }// add new alias usage record
                } // field()
            } // if eof
        }// execute sql

        // set transaction success
        _SetTransactionSuccess(nTransAlias, r);
    }

    return r;
}

bool db::GetMessageText(const Message& idMsg, TSTRING& sMsg)
{
    bool r = false;
    CCADO::Where w;
    w.SetCriteria(CCADO::WhereEq(FIELD_MESSAGEID, idMsg));

    CCADO::Selection s;
    s.AddField(FIELD_MESSAGETEXT);
    s.SetTableName(TABLE_MESSAGES);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            r = rst.Field(FIELD_MESSAGETEXT, sMsg);
        }
    }

    return r;
}

bool db::GetAliasText(const Alias& idAlias, TSTRING& sAlias)
{
    bool r = false;
    CCADO::Where w;
    w.AddToken(CCADO::WhereEq(FIELD_ALIASID, idAlias));
    w.AddToken(_T("AND"));
    w.AddToken(CCADO::WhereEq(FIELD_ALIASENABLED, true));

    CCADO::Selection s;
    s.AddField(FIELD_ALIASTEXT);
    s.SetTableName(TABLE_ALIASES);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            r = rst.Field(FIELD_ALIASTEXT, sAlias);
        }
    }

    return r;
}

// Given an alias, return the ID for it.
bool db::GetAliasID(CTSTRING& sAlias, Alias& idAlias, bool bMustBeEnabled)
{
    bool r = false;
    CCADO::Where w;
    w.AddToken(CCADO::WhereLike(FIELD_ALIASTEXT, sAlias));

    if(bMustBeEnabled)
    {
        w.AddToken(_T("AND"));
        w.AddToken(CCADO::WhereEq(FIELD_ALIASENABLED, true));
    }

    CCADO::Selection s;
    s.AddField(FIELD_ALIASID);
    s.SetTableName(TABLE_ALIASES);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            rst.Field(FIELD_ALIASID, var);
            if(var.vt == VT_I4)
            {
                idAlias = var.intVal;
                r = true;
            }
        }
    }

    return r;
}

// Creates a new transaction record with unspecified success bit.
// This also creates new user and channel records if they aren't already registered.
bool db::_CreateNewTransaction(const TransactInfo& ti, Transaction& n)
{
    bool r = false;
    User nUser;

    // determine/create the user
    if(_FindUser(ti, nUser, true))
    {
        Channel nChannel;
        // determine/create the channel
        if(_FindChannel(ti, nChannel, true))
        {
            // create a new transaction
            CCADO::Insertion i;
            i.SetTableName(TABLE_TRANSACTIONS);
            i.AddPair(FIELD_TRANSACTIONDATE, CCADO::ToSQL(ti.DateTime));
            i.AddPair(FIELD_TRANSACTIONUSERID, CCADO::ToSQL(nUser));
            i.AddPair(FIELD_TRANSACTIONCHANNELID, CCADO::ToSQL(nChannel));

            if(m_conn.Execute(i.ToString()))
            {
                _GetLastIdentity(n);
                r = true;
            }
        }

    }
    return r;
}

bool db::_SetTransactionSuccess(const Transaction& n, bool b)
{
    bool r = false;
    CCADO::Where w;
    w.SetCriteria(CCADO::WhereEq(FIELD_TRANSACTIONID, n));

    CCADO::Update u;
    u.SetTable(TABLE_TRANSACTIONS);
    u.AddValue(FIELD_TRANSACTIONSUCCESS, CCADO::ToSQL(b));
    u.SetWhere(w);

    if(m_conn.Execute(u.ToString()))
    {
        r = true;
    }

    return r;
}

bool db::_GetLastIdentity(int& n)
{
    _variant_t v;
    bool r = false;
    if(m_conn.GetLastIdentity(v))
    {
        v.ChangeType(VT_I4);
        n = v.intVal;
        r = true;
    }
    return r;
}

/*
    Finds a user that matches all three criterium.  If it's not found,
    optionally creates it.
*/
bool db::_FindUser(const TransactInfo& ti, User& n, bool bCreate)
{
    bool r = false;

    // Construct the WHERE clause
    CCADO::Where w;
    TSTRING strWhere;
    sprintf(strWhere, _T("%s=%s AND %s=%s AND %s=%s"),
        FIELD_USERNICK, (CCADO::ToSQL(ti.Nick)).s.c_str(),
        FIELD_USERIDENT, (CCADO::ToSQL(ti.Ident)).s.c_str(),
        FIELD_USERHOST, (CCADO::ToSQL(ti.Host)).s.c_str());
    w.SetCriteria(strWhere);

    // Construct the SELECT statement
    CCADO::Selection s;
    s.SetTableName(TABLE_USERS);
    s.AddField(FIELD_USERID);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            // We have a match.
            _variant_t v;
            if(rst.Field(FIELD_USERID, v))
            {
                if(v.vt == VT_I4)
                {
                    n = v.intVal;
                    r = true;
                }
            }
        }
        else
        {
            if(bCreate)
            {
                r = _NewUser(ti, n);
            }
        }
    }
    else
    {
        if(bCreate)
        {
            r = _NewUser(ti, n);
        }
    }

    return r;
}

bool db::_NewUser(const TransactInfo& ti, User& n)
{
    bool r = false;
    Entity nEntityID;

    if(_FindEntity(ti, nEntityID, true))
    {
        CCADO::Insertion i;
        i.SetTableName(TABLE_USERS);
        i.AddPair(FIELD_USERNICK, CCADO::ToSQL(ti.Nick));
        i.AddPair(FIELD_USERIDENT, CCADO::ToSQL(ti.Ident));
        i.AddPair(FIELD_USERHOST, CCADO::ToSQL(ti.Host));
        i.AddPair(FIELD_USERENTITYID, CCADO::ToSQL(nEntityID));

        if(m_conn.Execute(i.ToString()))
        {
            r = _GetLastIdentity(n);
        }
    }

    return r;
}


bool db::_FindEntity(const TransactInfo& ti, Entity& eid, bool bCreate)
{
    bool r = false;
    TSTRING s(_T("EXECUTE Winprog_FindEntity "));
    s.append(CCADO::ToSQL(ti.Nick).s);
    s.append(_T(", "));
    s.append(CCADO::ToSQL(ti.Ident).s);
    s.append(_T(", "));
    s.append(CCADO::ToSQL(ti.Host).s);

    _variant_t var;

    if(m_conn.ExecuteAndReturnFirstValue(s, FIELD_ENTITYID, var))
    {
        eid = var.intVal;
        r = true;
    }
    else
    {
        if(bCreate)
        {
            r = _NewEntity(ti.Nick, eid);
        }
    }

    return r;
}

bool db::_NewEntity(CTSTRING& sNick, Entity& eid)
{
    bool r = false;
    CCADO::Insertion i;
    i.SetTableName(TABLE_ENTITIES);
    i.AddPair(FIELD_ENTITYNAME, CCADO::ToSQL(sNick));

    if(m_conn.Execute(i.ToString()))
    {
        if(_GetLastIdentity(eid))
        {
            r = true;
        }
    }

    return r;
}


bool db::_FindChannel(const TransactInfo& ti, Channel& n, bool bCreate)
{
    bool r = false;

    // Construct the WHERE clause
    CCADO::Where w;
    TSTRING strWhere;
    sprintf(strWhere, _T("%s=%s"), FIELD_CHANNELNAME, (CCADO::ToSQL(ti.Channel)).s.c_str());
    w.SetCriteria(strWhere);

    // Construct the SELECT statement
    CCADO::Selection s;
    s.SetTableName(TABLE_CHANNELS);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            // We have a match.
            _variant_t v;
            if(rst.Field(FIELD_CHANNELID, v))
            {
                if(v.vt == VT_I4)
                {
                    n = v.intVal;
                    r = true;
                }
            }
        }
        else
        {
            if(bCreate)
            {
                r = _NewChannel(ti, n);
            }
        }
    }
    else
    {
        if(bCreate)
        {
            r = _NewChannel(ti, n);
        }
    }

    return r;
}

bool db::_NewChannel(const TransactInfo& ti, Channel& n)
{
    bool r = false;

    // Form the INSERT INTO statement
    CCADO::Insertion i;
    i.SetTableName(TABLE_CHANNELS);
    i.AddPair(FIELD_CHANNELNAME, CCADO::ToSQL(ti.Channel));

    if(m_conn.Execute(i.ToString()))
    {
        _GetLastIdentity(n);
        r = true;
    }

    return r;
}


bool db::AliasEnabled(CTSTRING& sAlias)
{
    Alias a;
    return AliasEnabled(sAlias, a);
}

bool db::AliasEnabled(CTSTRING& sAlias, Alias& a)
{
    // select top 1 * from WinprogAliases where (AliasText like 'blah') and (AliasEnabled = true)
    CCADO::Where w;
    w.SetCriteria(CCADO::WhereLike(FIELD_ALIASTEXT, sAlias));
    w.SetCriteria(
        _T("(") +
        CCADO::WhereLike(FIELD_ALIASTEXT, sAlias) +
        _T(") and (") +
        CCADO::WhereEq(FIELD_ALIASENABLED, true) +
        _T(")")
        );
    CCADO::Selection sel;
    sel.SetTableName(TABLE_ALIASES);
    sel.SetTop(true, 1);
    sel.AddField(FIELD_ALIASID);
    sel.SetWhere(w);

    bool r = false;
    CCADO::Recordset rst;

    if(m_conn.Execute(sel.ToString(), rst))
    {
        // did we get any results?
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_ALIASID, var))
            {
                a = var.intVal;
                r = true;
            }
        }
    }

    return r;
}

//bool db::AliasExists(CTSTRING& sAlias)
//{
//    Alias a;
//    return AliasExists(sAlias, a);
//}
//
//
//bool db::AliasExists(CTSTRING& sAlias, Alias& a)
//{
//    bool r = false;
//    CCADO::Where w;
//    w.SetCriteria(CCADO::WhereLike(FIELD_ALIASTEXT, sAlias));
//
//    CCADO::Selection s;
//    s.AddField(FIELD_ALIASID);
//    s.SetTableName(TABLE_ALIASES);
//    s.SetWhere(w);
//
//    CCADO::Recordset rst;
//
//    if(m_conn.Execute(s.ToString(), rst))
//    {
//        _variant_t var;
//        // get the first value.
//        if(rst.Field(FIELD_ALIASID, var))
//        {
//            a = var.intVal;
//            r = true;
//        }
//    }
//
//    return r;
//}

bool db::_AliasExists(CTSTRING& sAlias)
{
    Alias a;
    return _AliasExists(sAlias, a);
}

bool db::_AliasExists(CTSTRING& sAlias, Alias& a)
{
    bool r = false;
    CCADO::Where w;
    w.AddToken(CCADO::WhereLike(FIELD_ALIASTEXT, sAlias));
    //w.AddToken(_T("AND"));
    //w.AddToken(CCADO::WhereEq(FIELD_ALIASENABLED, true));

    CCADO::Selection s;
    s.SetTableName(TABLE_ALIASES);
    s.AddField(FIELD_ALIASID);
    s.SetWhere(w);

    CCADO::Recordset rst;

    if(m_conn.Execute(s.ToString(), rst))
    {
        if(!rst.IsEOF())
        {
            _variant_t var;
            if(rst.Field(FIELD_ALIASID, var))
            {
                a = var.intVal;
                r = true;
            }
        }
    }

    return r;
}

bool db::CreateNewTransaction(const TransactInfo& ti, Transaction& n)
{
  return _CreateNewTransaction(ti, n);
}
bool db::SetTransactionSuccess(const Transaction& n, bool b)
{
  return _SetTransactionSuccess(n, b);
}


}// namespace Winprog


