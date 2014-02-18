

#pragma once


#include "ichar.h"
#include "ccado.h"
#include <vector>
#include "message.h"
#include "alias.h"
#include <map>


namespace Winprog
{
    /////////////////////////////////////////////////////////
    /*
        This class is used to create a transaction record.
        Stores info about the user requesting the transaction
    */
    class TransactInfo
    {
    public:
        TransactInfo()
        {
            // not time-zone dependent here folks
            GetSystemTime(&DateTime);
        }
        TransactInfo(CTSTRING& sChannel, CTSTRING& sNick, CTSTRING& sIdent, CTSTRING& sHost)
        {
            GetSystemTime(&DateTime);
            Nick = sNick;
            Ident = sIdent;
            Host = sHost;
            Channel = sChannel;
        }
        TransactInfo(CTSTRING& sChannel, CTSTRING& sHostMask)
        {
            Channel = sChannel;
            FromHostMask(sHostMask);
        }
        bool FromHostMask(CTSTRING& hm)
        {
            // nick!ident@host
            // if there is anything but a single exl. point and a single at sign, this fails.
            bool r = false;
            TSTRING::size_type nExcl;
            TSTRING::size_type nAt;
            nExcl = hm.find(_T('!'));
            if(nExcl != TSTRING::npos)
            {
                nAt = hm.find(_T('@'), nExcl);
                if(nAt != TSTRING::npos)
                {
                    Nick = hm.substr(0, nExcl);
                    Ident = hm.substr(nExcl+1, (nAt-nExcl)-1);
                    Host = hm.substr(nAt+1);
                    r = true;
                }
            }
            return r;
        }
        void SetDateTime(const SYSTEMTIME& st)
        {
            CopyMemory(&DateTime, &st, sizeof(SYSTEMTIME));
        }

        SYSTEMTIME DateTime;
        TSTRING Nick;
        TSTRING Ident;
        TSTRING Host;
        TSTRING Channel;
    };

    // These are the identity values of the tables, but since
    // they represent a single unique record they can be used like this
    typedef int Message;
    typedef int Transaction;
    typedef int User;
    typedef int Channel;
    typedef int Entity;

    /////////////////////////////////////////////////////////
    /*
        Represents one winprog session/connection.
        The API here should reflect exactly the transaction scheme
    */
    class db
    {
        friend class dbHelper;
    public:
        db();
        ~db();

        bool Connect(CTSTRING& connstr);
        bool Disconnect();

        bool NewMessage(const TransactInfo& ti, CTSTRING& text, Message& m);
        bool UpdateMessage(const TransactInfo& ti, const Message& m, CTSTRING& newtext);

        bool NewAlias(const TransactInfo& ti, const Message& m, CTSTRING& alias, Alias& out);
        bool EnableAlias(const TransactInfo& ti, const Alias& alias, bool b);

        // NOTE: These often will return false when there is no alias associated.
        bool GetFirstAliasForMessage(const Message& m, Alias& out);// returns the alias of the first
        bool GetFirstAliasForMessage(const Message& m, Alias& out, TSTRING& s);// returns the alias of the first

        bool AliasEnabled(CTSTRING& s);
        bool AliasEnabled(CTSTRING& s, Alias& a);

        bool QueryMessages(const TransactInfo& ti, const CTSTRING& sQuery, AliasList& out, int nRows);
        // creates a use transaction for both message and alias
        //bool FetchMessage(const TransactInfo& ti, const Alias& alias, Message& m);
        bool FetchMessage(const TransactInfo& ti, CTSTRING& sAlias, TSTRING& sMessage);

        bool GetMessageText(const Message& m, TSTRING& s);
        bool GetAliasText(const Alias& m, TSTRING& s);
        bool GetAliasID(CTSTRING& s, Alias& m, bool bMustBeEnabled = true);

        static bool IsValidAliasText(CTSTRING& alias);

        bool CreateNewTransaction(const TransactInfo& ti, Transaction& n);
        bool SetTransactionSuccess(const Transaction& n, bool b);

    private:
        // does the same thing as teh two variations above, all in 1 function to reduce copy/paste code
        //bool _FetchMessage(const TransactInfo& ti, const Alias& alias, CTSTRING& sText, bool bText, Message& m);

        // Internal functions
        bool _CreateNewTransaction(const TransactInfo& ti, Transaction& n);
        bool _SetTransactionSuccess(const Transaction& n, bool b);

        bool _FindEntity(const TransactInfo& ti, Entity& eid, bool bCreate);
        bool _NewEntity(CTSTRING& sNick, Entity& eid);

        bool _FindUser(const TransactInfo& ti, User& n, bool bCreate);
        bool _NewUser(const TransactInfo& ti, User& n);

        bool _FindChannel(const TransactInfo& ti, Channel& n, bool bCreate);
        bool _NewChannel(const TransactInfo& ti, Channel& n);

        bool _AliasExists(CTSTRING& s);// an alias will exist even if it's been disabled.
        bool _AliasExists(CTSTRING& s, Alias& a);

        bool _GetLastIdentity(int&);
        CCADO::Connection m_conn;

        TSTRING m_sentinel;// just so we can use it as a reference
    };
}// namespace Winprog


/*
    TABLE NAMES
*/
#define TABLE_USERS                   _T("WinprogUsers")
#   define FIELD_USERID               _T("UserID")
#   define FIELD_USERNICK             _T("UserNick")
#   define FIELD_USERIDENT            _T("UserIdent")
#   define FIELD_USERHOST             _T("UserHost")
#   define FIELD_USERENTITYID         _T("UserEntityID")

#define TABLE_CHANNELS                _T("WinprogChannels")
#   define FIELD_CHANNELID            _T("ChannelID")
#   define FIELD_CHANNELNAME          _T("ChannelName")

#define TABLE_TRANSACTIONS            _T("WinprogTransactions")
#   define FIELD_TRANSACTIONID        _T("TransactionID")
#   define FIELD_TRANSACTIONDATE      _T("TransactionDate")
#   define FIELD_TRANSACTIONUSERID    _T("TransactionUserID")
#   define FIELD_TRANSACTIONCHANNELID _T("TransactionChannelID")
#   define FIELD_TRANSACTIONSUCCESS   _T("TransactionSuccess")

#define TABLE_ENTITIES                          _T("WinprogEntities")
#   define FIELD_ENTITYID                       _T("EntityID")
#   define FIELD_ENTITYNAME                     _T("EntityName")

////////////////////////
#define TABLE_QUERIES                 _T("WinprogQueries")
#   define FIELD_QUERYTRANSACTIONID   _T("QueryTransactionID")
#   define FIELD_QUERYTEXT            _T("QueryText")

////////////////////////
#define TABLE_MESSAGES                _T("WinprogMessages")
#   define FIELD_MESSAGEID            _T("MessageID")
#   define FIELD_MESSAGETEXT          _T("MessageText")

#define TABLE_MESSAGEUSES                       _T("WinprogMessageUses")
#   define FIELD_MESSAGEUSETRANSACTIONID        _T("MessageUseTransactionID")
#   define FIELD_MESSAGEUSEMESSAGEID            _T("MessageUseMessageID")

#define TABLE_MESSAGECREATIONS                  _T("WinprogMessageCreations")
#   define FIELD_MESSAGECREATIONTRANSACTIONID   _T("MessageCreationTransactionID")
#   define FIELD_MESSAGECREATIONMESSAGEID       _T("MessageCreationMessageID")

#define TABLE_MESSAGEUPDATES                    _T("WinprogMessageUpdates")
#   define FIELD_MESSAGEUPDATETRANSACTIONID     _T("MessageUpdateTransactionID")
#   define FIELD_MESSAGEUPDATEMESSAGEID         _T("MessageUpdateMessageID")
#   define FIELD_MESSAGEUPDATETOTEXT            _T("MessageUpdateToText")

////////////////////////
#define TABLE_ALIASES                 _T("WinprogAliases")
#   define FIELD_ALIASID              _T("AliasID")
#   define FIELD_ALIASMESSAGEID       _T("AliasMessageID")
#   define FIELD_ALIASTEXT            _T("AliasText")
#   define FIELD_ALIASENABLED         _T("AliasEnabled")

#define TABLE_ALIASCREATIONS                    _T("WinprogAliasCreations")
#   define FIELD_ALIASCREATIONTRANSACTIONID     _T("AliasCreationTransactionID")
#   define FIELD_ALIASCREATIONALIASID           _T("AliasCreationAliasID")

#define TABLE_ALIASUPDATES                      _T("WinprogAliasUpdates")
#   define FIELD_ALIASUPDATETRANSACTIONID       _T("AliasUpdateTransactionID")
#   define FIELD_ALIASUPDATEALIASID             _T("AliasUpdateAliasID")
#   define FIELD_ALIASUPDATETOENABLED           _T("AliasUpdateToEnabled")
#   define FIELD_ALIASUPDATETOMESSAGEID         _T("AliasUpdateToMessageID")

#define TABLE_ALIASUSES                         _T("WinprogAliasUses")
#   define FIELD_ALIASUSETRANSACTIONID          _T("AliasUseTransactionID")
#   define FIELD_ALIASUSEALIASID                _T("AliasUseAliasID")


#define MAX_ALIAS_TEXT 40
#define MAX_MESSAGE_TEXT 500


