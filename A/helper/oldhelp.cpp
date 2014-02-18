/*
    <HELPMSGS>
        <MSG CREATOR="(n/a)" TIME=0 ALIAS="test" TEXT="(sample help message)" USAGE="1"/>
    </HELPMSGS>
*/
/*
[script]
n4=on *:TEXT:!help*:%HelpChan:{ msg $chan $dll(winprog.dll, Msg, $2-) }
n5=
n6=on tWinmain:TEXT:!msgquery*:%HelpChan:{  msg $chan $dll(winprog.dll, MsgQuery, $2-) }
n7=on tWinmain:TEXT:!msgqueer*:%HelpChan:{  msg $chan $dll(winprog.dll, MsgQuery, $2-) }
n8=on tWinmain:TEXT:!msgqueery*:%HelpChan:{  msg $chan $dll(winprog.dll, MsgQuery, $2-) }
n9=on tWinmain:TEXT:!msgfind*:%HelpChan:{  msg $chan $dll(winprog.dll, MsgQuery, $2-) }
n10=on tWinmain:TEXT:!msgseek*:%HelpChan:{  msg $chan $dll(winprog.dll, MsgQuery, $2-) }
n11=on tWinmain:TEXT:!msgchange*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgChange, $2-) }
n12=on tWinmain:TEXT:!msgset*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgSet, $nick $2-) }
n13=on tWinmain:TEXT:!msgadd*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgSet, $nick $2-) }
n14=on tWinmain:TEXT:!msgcommit*:%HelpChan:{ msg $chan $dll(winprog.dll, Commit, $2-) }
n15=on tWinmain:TEXT:!msgpending*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgPending, $2-) }
n16=on tWinmain:TEXT:!msgclr*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n17=on tWinmain:TEXT:!msgdel*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n18=on tWinmain:TEXT:!msgdelete*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n19=on tWinmain:TEXT:!msgclear*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n20=on tWinmain:TEXT:!msgrem*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n21=on tWinmain:TEXT:!msgremove*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgClr, $2-) }
n22=on tWinmain:TEXT:!msginfo*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgNfo, $2-) }
n23=on tWinmain:TEXT:!msgnfo*:%HelpChan:{ msg $chan $dll(winprog.dll, MsgNfo, $2-) }
n27=on *:TEXT:!msghelp*:%HelpChan:{
n28=  /msg $nick .!msghelp................This message.
n29=  /msg $nick .!help alias [nick]......Displays a message (optionally to a specific nick)
n30=  /msg $nick .!msgset alias text......Adds a new message.
n31=  /msg $nick .!msgchange alias text...Modifies an existing message.
n32=  /msg $nick .!msgclr alias...........Removes a message from the database.
n33=  /msg $nick .!msgnfo alias...........Display information about a message.
n34=  /msg $nick .!msgquery pattern.......Searches for messages.  Wildcards supported.
n35=  /msg $nick .!msgpending.............Returns the number of uncommited changes
n36=  /msg $nick .!msgcommit..............Commits the database to disk and http://0x68.com/winprog.xml
n37=}
*/
#include "stdafx.h"
#include "oldhelp.h"
#include "apputil.h"
#include "global.h"


#define TAG_HELPMSGS _T("HELPMSGS")
#define TAG_MSG _T("MSG")

#define TAG_ALIAS _T("ALIAS")
#define TAG_CREATOR _T("CREATOR")
#define TAG_TIME _T("DATE")
#define TAG_TEXT _T("TEXT")
#define TAG_USAGE _T("USAGE")


#import "msxml.dll" implementation_only


namespace Winprog
{
namespace xml
{

CHelpDB::CHelpDB()
{
}


CHelpDB::~CHelpDB()
{
    Close();
}


bool CHelpDB::Open(TSTRING& sFileName)
{
    HRESULT hr = 0;
    bool r = false;
    Close();

    g_pLog->msg(_T("Attempting to open %s"), sFileName.c_str());

    try
    {
        if(SUCCEEDED(m_doc.CreateInstance("Msxml.DOMDocument")))
        {
            if(VARIANT_TRUE == m_doc->load(_variant_t(sFileName.c_str())))
            {
                m_sFileName = sFileName;

                // Get the top level node for later use.
                MSXML::IXMLDOMNodePtr Root;

                if(FAILED(hr = m_doc.QueryInterface(__uuidof(MSXML::IXMLDOMNode), &Root)))
                {
                    throw _com_error(hr);
                }

                try
                {
                    m_root = Root->selectSingleNode(_bstr_t(_T("/") TAG_HELPMSGS));

                    if(m_root != 0)
                    {
                        r = true;
                    }
                }
                catch(_com_error& e)
                {
                    sprintf(m_err, _T("COM Error: %08x"), e.Error());
                }
            }
            else
            {
                // Deal with an error from m_doc->load().
                MSXML::IXMLDOMParseErrorPtr pParse = m_doc->parseError;
                try
                {
                    if(pParse != 0)
                    {
                        TSTRING sReason, sText, sURL;
                        long lErrCode = pParse->errorCode;
                        long lFilePos = pParse->filepos;
                        long lLine = pParse->line;
                        long lCol = pParse->linepos;
                        if(pParse->reason.length()) HW2T(sReason, (BSTR)(pParse->reason));
                        if(pParse->srcText.length()) HW2T(sText, (BSTR)(pParse->srcText));
                        if(pParse->url.length()) HW2T(sURL, (BSTR)(pParse->url));

                        // Format the error.
                        sprintf(m_err, _T("Parsing Error #%d [%s] (line %d, col %d, filepos: %d, url %s, text: %s)"),
                            lErrCode, sReason.c_str(), lLine, lCol, lFilePos, sURL.c_str(), sText.c_str());
                        g_pLog->msg(_T("%s"), m_err.c_str());
                    }
                }
                catch(_com_error& e)
                {
                    TSTRING s = e.Description();
                    sprintf(m_err, _T("COM Error: %08x: %s"), e.Error(), s.c_str());
                    g_pLog->msg(_T("%s"), m_err.c_str());
                }
            }
        }
    }
    catch(_com_error& e)
    {
        sprintf(m_err, _T("COM Error: %08x"), e.Error());
        g_pLog->msg(_T("%s"), m_err.c_str());
    }

    if(!r)
    {
        // free up stuff.
        g_pLog->msg(_T("Failed to open %s"), sFileName.c_str());
        Close();
    }
    else
    {
        g_pLog->msg(_T("Successfully opened %s"), sFileName.c_str());
    }

    return r;
}


bool CHelpDB::Save()
{
    bool r = false;
    HRESULT hr = 0;
    if(m_doc == 0) return false;

    g_pLog->msg(_T("Attempting to save %s"), m_sFileName.c_str());

    if(_FormatXML())
    {
        try
        {
            if(SUCCEEDED(hr = m_doc->save(_variant_t(m_sFileName.c_str()))))
            {
                r = true;
                g_pLog->msg(_T("Successfully saved %s"), m_sFileName.c_str());
            }
            else
            {
                g_pLog->msg(_T("Error saving the document to disk. gle=%d, hr=%d"), GetLastError(), hr);
            }
        }
        catch(_com_error& e)
        {
            sprintf(m_err, _T("0x%08x"), e.Error());
            g_pLog->msg(_T("CHelpDB::Save COM ERROR"));
        }
    }

    if(!r)
    {
        g_pLog->msg(_T("Failed to save %s"), m_sFileName.c_str());
    }

    return r;
}


bool CHelpDB::Close()
{
    m_sFileName.clear();

    try
    {
        m_root.Release();
        m_doc.Release();
    }
    catch(...)
    {
    }

    return true;
}


bool CHelpDB::GetItemInfo(CTSTRING& sAlias, CHelpMessage& info, bool bIncUsage)
{
    MSXML::IXMLDOMElementPtr El;
    bool r = false;
    if(m_doc == 0) return false;

    if(GetItemElement(sAlias, El, false))
    {
        r = ElementToStruct(info, El, bIncUsage);
    }

    return r;
}


bool CHelpDB::DeleteItem(CTSTRING& sAlias)
{
    MSXML::IXMLDOMElementPtr El;
    MSXML::IXMLDOMNodePtr Node;
    MSXML::IXMLDOMNodePtr Parent;
    bool r = false;
    if(m_doc == 0) return false;

    if(GetItemElement(sAlias, El, false))
    {
        El.QueryInterface(__uuidof(MSXML::IXMLDOMNode), &Node);
        Parent = Node->GetparentNode();
        Parent->removeChild(Node);
        r = true;
    }

    return r;
}


bool CHelpDB::NewItem(CTSTRING& _sAlias, CTSTRING& sText, CTSTRING& sCreator)
{
    MSXML::IXMLDOMElementPtr El;
    bool r = false;
    if(m_doc == 0) return false;
    TSTRING sAlias;

    {
        TCHAR* tsz = _tcsdup(_sAlias.c_str());
        _tcsupr(tsz);
        sAlias = tsz;
        SAFE_FREE(tsz);
    }

    // Just determine if it exists.
    if(GetItemElement(sAlias, El, false))
    {
        // it already exists; fail.
        sprintf(m_err, _T("%s already exists.  If you want to change the existing entry, use !msgchange."), sAlias.c_str());
        g_pLog->msg(_T("%s"), m_err.c_str());
    }
    else
    {
        // doesn't already exist... we're golden like the shower.  This time
        // just call GetItemElement() with bCreate set to true.
        if(!GetItemElement(sAlias, El, true))
        {
            sprintf(m_err, _T("%s could not be added."), sAlias.c_str());
            g_pLog->msg(_T("%s"), m_err.c_str());
        }
        else
        {
            El->setAttribute(TAG_ALIAS, sAlias.c_str());
            El->setAttribute(TAG_CREATOR, sCreator.c_str());
            El->setAttribute(TAG_TEXT, sText.c_str());
            El->setAttribute(TAG_USAGE, _T("0"));
            SYSTEMTIME st;
            TSTRING sTime;
            GetLocalTime(&st);
            Bin2Str(sTime, (BYTE*)&st, sizeof(st));
            El->setAttribute(TAG_TIME, sTime.c_str());

            g_pLog->msg(_T("New db item: %s = %s"), _sAlias.c_str(), sText.c_str());

            r = true;
        }
    }

    return r;
}


bool CHelpDB::ChangeItem(CTSTRING& sAlias, CTSTRING& sText)
{
    MSXML::IXMLDOMElementPtr El;
    HRESULT hr = 0;
    bool r = false;
    if(m_doc == 0) return false;

    if(GetItemElement(sAlias, El, false))
    {
        if(SUCCEEDED(hr = El->setAttribute(TAG_TEXT, sText.c_str())))
        {
            g_pLog->msg(_T("Item changed: %s = %s"), sAlias.c_str(), sText.c_str());
            r = true;
        }
        else
        {
            g_pLog->msg(_T("SetAttribute(%s=%s) failed, hr=0x%08x"), TAG_TEXT, sText.c_str(), hr);
        }
    }
    else
    {
        sprintf(m_err, _T("%s was not found in the database."), sAlias.c_str());
        g_pLog->msg(_T("CHelpDB::ChangeItem(%s) - Item wasn't found."), sAlias.c_str());
    }

    return r;
}


bool CHelpDB::_FormatXML()
{
    bool r = false;

    TSTRING strIntermediate;
    BSTR strNewXML = 0;

    // get the XML code from the DOM
    try
    {
        {
            _bstr_t strOriginalXML;
            m_doc->replaceChild(m_root, m_doc->documentElement);
            strOriginalXML = m_doc->xml;
            strIntermediate = strOriginalXML;
        }

        HStrReplace(strIntermediate, _T("><"), _T(">\r\n<"));

        strNewXML = SysAllocString(strIntermediate.c_str());

        if(strNewXML)
        {
            //' load back into the DOM
            if(VARIANT_TRUE == m_doc->loadXML(strNewXML))
            {
                r = true;
            }

            SysFreeString(strNewXML);
            strNewXML = 0;
        }
    }
    catch(_com_error& e)
    {
        sprintf(m_err, _T("0x%08x"), e.Error());
        g_pLog->msg(_T("CHelpDB::_FormatXML COM ERROR"));
    }

    return r;
}


CTSTRING& CHelpDB::last_error()
{
    return m_err;
}


bool CHelpDB::GetItemElement(CTSTRING& _sAlias, MSXML::IXMLDOMElementPtr& Out, bool bCreate)
{
    bool r = false;
    TSTRING sXPath;
    MSXML::IXMLDOMNodePtr Node;
    TSTRING sAlias;

    {
        TCHAR* tsz = _tcsdup(_sAlias.c_str());
        _tcsupr(tsz);
        sAlias = tsz;
        SAFE_FREE(tsz);
    }

    // this will look something like MSG[@ALIAS="hithere"]
    sprintf(sXPath, TAG_MSG _T("[@") TAG_ALIAS _T("=\"%s\"]"), sAlias.c_str());

    try
    {
        Node = m_root->selectSingleNode(_bstr_t(sXPath.c_str()));

        if((Node == NULL) && (bCreate == true))
        {
            MSXML::IXMLDOMElementPtr NewEl;
            MSXML::IXMLDOMNodePtr NewNode;

            NewEl = m_doc->createElement("MSG");
            NewEl.QueryInterface(__uuidof(MSXML::IXMLDOMNode), &NewNode);
            Node = m_root->appendChild(NewNode);
        }

        if(Node != NULL)
        {
            Node.QueryInterface(__uuidof(MSXML::IXMLDOMElement), &Out);
            r = true;
        }
    }
    catch(_com_error& e)
    {
        sprintf(m_err, _T("COM Error: %08x"), e.Error());
        g_pLog->msg(_T("CHelpDB::GetItemElement(%s)"), _sAlias.c_str());
    }

    return r;
}


CMessageEnum::CMessageEnum()
{
    m_pOwner = 0;
}


CMessageEnum::~CMessageEnum()
{
    if(m_list) m_list.Release();
}


bool CMessageEnum::Reset()
{
    if(m_list) m_list.Release();
    if(m_pOwner)
    {
        try
        {
            m_list = m_pOwner->m_root->selectNodes(_bstr_t("MSG"));
        }
        catch(_com_error&)
        {
            g_pLog->msg(_T("CMessageEnum::Reset COM ERROR"));
        }
        catch(...)
        {
        }
    }
    return true;
}


bool CMessageEnum::Reset(CHelpDB* pOwner)
{
    if(m_list) m_list.Release();
    m_pOwner = pOwner;
    return Reset();
}


bool CMessageEnum::Next(CHelpMessage& nfo)
{
    bool r = false;
    MSXML::IXMLDOMNodePtr Node;
    MSXML::IXMLDOMElementPtr Element;

    try
    {
        Node = m_list->nextNode();
        Node.QueryInterface(__uuidof(MSXML::IXMLDOMElement), &Element);

        r = m_pOwner->ElementToStruct(nfo, Element);
    }
    catch(_com_error&)
    {
        g_pLog->msg(_T("CMessageEnum::Next COM ERROR"));
    }
    catch(...)
    {
    }

    return r;
}


CTSTRING& CHelpDB::file_name()
{
    return m_sFileName;
}


bool CHelpDB::ElementToStruct(CHelpMessage& info, MSXML::IXMLDOMElementPtr& El, bool bIncUsage)
{
    bool r = false;

    try
    {
        _variant_t vAlias = El->getAttribute(TAG_ALIAS);
        _variant_t vCreator = El->getAttribute(TAG_CREATOR);
        _variant_t vText = El->getAttribute(TAG_TEXT);
        _variant_t vCreated = El->getAttribute(TAG_TIME);
        _variant_t vUsage = El->getAttribute(TAG_USAGE);

        HW2T(info.Alias, vAlias.bstrVal);
        HW2T(info.Creator, vCreator.bstrVal);
        HW2T(info.Text, vText.bstrVal);

        TSTRING sTemp;

        HW2T(sTemp, vCreated.bstrVal);
        Str2Bin(sTemp, (PBYTE)&info.Created, sizeof(info.Created));

        HW2T(sTemp, vUsage.bstrVal);
        info.nUsage = _tcstoul(sTemp.c_str(), 0, 10);

        if(bIncUsage)
        {
            TCHAR tsz[500];
            info.nUsage++;
            _ultot(info.nUsage, tsz, 10);
            El->setAttribute(_bstr_t(TAG_USAGE), _variant_t(tsz));
        }

        r = true;
    }
    catch(_com_error& e)
    {
        sprintf(m_err, _T("COM Error: %08x"), e.Error(), (TCHAR*)e.Description());
        g_pLog->msg(_T("CHelpDB::ElementToStruct ERROR"));
    }

    return r;
}


}
}
