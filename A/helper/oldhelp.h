

#pragma once


//#include "XMLUtils.h"
#include "stdafx.h"
#include "apputil.h"

#import "msxml.dll" no_implementation


namespace Winprog
{
namespace xml
{

class CHelpMessage
{
public:
    void assign(CHelpMessage& x)
    {
        Alias = x.Alias;
        Text = x.Text;
        Creator = x.Creator;
        memcpy(&Created, &x.Created, sizeof(SYSTEMTIME));
        nUsage = x.nUsage;
    }

    TSTRING Alias;
    TSTRING Text;
    TSTRING Creator;
    SYSTEMTIME Created;
    int nUsage;
};



class CHelpDB
{
    friend class CMessageEnum;
public:
    CHelpDB();
    ~CHelpDB();

    bool Open(TSTRING& sFileName);
    bool Save();
    bool Close();

    bool GetItemInfo(CTSTRING& sAlias, CHelpMessage& info, bool bIncUsage = false);
    bool DeleteItem(CTSTRING& sAlias);
    bool NewItem(CTSTRING& sAlias, CTSTRING& sText, CTSTRING& sCreator);
    bool ChangeItem(CTSTRING& sAlias, CTSTRING& sText);

    CTSTRING& last_error();
    CTSTRING& file_name();

private:
    bool _FormatXML();
    bool GetItemElement(CTSTRING& sAlias, MSXML::IXMLDOMElementPtr& Out, bool bCreate);

    bool ElementToStruct(CHelpMessage& info, MSXML::IXMLDOMElementPtr& el, bool bIncUsage = false);

    MSXML::IXMLDOMDocumentPtr m_doc;
    MSXML::IXMLDOMNodePtr m_root;
    TSTRING m_sFileName;

    TSTRING m_err;
};


class CMessageEnum
{
public:
    CMessageEnum();
    ~CMessageEnum();

    bool Reset();
    bool Reset(CHelpDB* pOwner);

    bool Next(CHelpMessage& nfo);

private:
    CHelpDB* m_pOwner;
    MSXML::IXMLDOMNodeListPtr m_list;
};
}
}