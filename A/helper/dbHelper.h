

#pragma once


#include "helpdb.h"


namespace Winprog
{

class dbHelper
{
public:
    dbHelper(db& x);
    ~dbHelper();

    // HELPERS (but required because this is impossible otherwise)
    bool GetAliasTotalUsage(const Alias& a, long& n);
    bool GetAliasCreatedBy(const Alias& a, TSTRING& mask);
    bool GetAliasCreatedOn(const Alias& a, SYSTEMTIME& st);
    bool GetAliasLastModifiedOn(const Alias& a, SYSTEMTIME& st);
    bool GetAliasLastModifiedBy(const Alias& a, TSTRING& mask);
    bool GetAliasLastModifiedTransactionRst(const Alias& a, CCADO::Recordset& rstTrans);
    bool GetAliasList(const Alias& a, std::vector<TSTRING>& items);

    bool GetMessageTotalUsage(const Message& a, long& n);
    bool GetMessageCreatedBy(const Message& a, TSTRING& mask);
    bool GetMessageCreatedOn(const Message& a, SYSTEMTIME& st);
    bool GetMessageLastModifiedOn(const Message& a, SYSTEMTIME& st);
    bool GetMessageLastModifiedBy(const Message& a, TSTRING& mask);
    bool GetMessageLastModifiedTransactionRst(const Message& a, CCADO::Recordset& rstTrans);

    bool GetUserAssociatedWithTransaction(const Transaction& t, TSTRING& mask);
    bool GetAliasMessageID(const Alias& a, Message& m);
    bool GetTransactionRst(const Transaction& t, CCADO::Recordset& rst);
    bool GetUserRst(const User& u, CCADO::Recordset& rst);

    bool UserRstToMask(CCADO::Recordset& rstUser, TSTRING& mask);

private:
    db& m_db;
    bool GetFirstID(CTSTRING& sTable, CTSTRING& sField, CTSTRING& sCriteria, int& n);
};

}