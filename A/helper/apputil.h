

#pragma once


#include "ichar.h"
#include <string>
#include "buffer.h"


using namespace std;


/*/////////////////////////////////////////////////////////////////////////////
    MISCELLANEOUS DEFINES
*/

#ifndef OLESCRIPT_E_SYNTAX
#   define OLESCRIPT_E_SYNTAX     0x80020101
#endif


/*/////////////////////////////////////////////////////////////////////////////
    STRING CONVERSIONS
*/
PCTSTR BoolToString(bool b);
bool BoolFromString(CTSTRING& s);
TSTRING BoolToStringL(bool b);// localized
bool BoolFromStringL(CTSTRING& s);// localized

int Str2Bin(CTSTRING& s, PBYTE pData, int nBufferSize);
bool Bin2Str(TSTRING& s, PBYTE pData, int nSizeBytes);

void dtos(TSTRING& sDest, double d);// double to WSTRING
void btos(TSTRING& sDest, BYTE b);// byte to WSTRING (2 hex digits)
void ltos(TSTRING& sDest, long l, int nBase);
void ultos(TSTRING& sDest, unsigned long l, int nBase);

unsigned long stoul(CTSTRING& s, int nBase);
long stol(CTSTRING& s, int nBase);
double stod(CTSTRING& s);

bool HexToRGB(const TCHAR *pHex, COLORREF &clr);


/*/////////////////////////////////////////////////////////////////////////////
    STRING UNICODE/ANSI/ASCII CONVERSIONS
*/
bool WideCharToASCII(const WCHAR* wszIn, int cchW, char* szOut, int cchA);// returns false if wc could not be translated to a char - this is basically any character <= 127
bool ASCIIToWideChar(const char* szIn, int cchA, WCHAR* wszOut, int cchW);

void HA2W(WSTRING& sDest, PCSTR szIn);
void HW2A(std::string& sDest, PCWSTR szIn);
//size_t HA2WC(PWSTR wszDest, PCSTR szIn);// wszDest must already be the right size (or null) - returns number of elements required.
//size_t HW2AC(PSTR szDest, PCWSTR wszIn);// wszDest must already be the right size (or null) - returns number of elements required.
void HA2WB(CBuffer<WCHAR>& Dest, PCSTR szIn, size_t n);// if n is -1, szIn is zero-terminated
void HW2AB(CBuffer<char>& Dest, PCWSTR wszIn, size_t n);// if n is -1, szIn is zero-terminated


#ifdef _UNICODE
#   define HA2T HA2W
#   define HT2A HW2A
#   define HA2TC HA2WC
#   define HT2AC HW2AC
#   define HW2T(d, s) { d = s; }
#   define HT2W(d, s) { d = s; }
#   define HW2TC(d, s) { wcscpy(d, s); }
#   define HT2WC(d, s) { wcscpy(d, s); }
#else
#   define HA2T(sDest, szIn) { sDest = szIn; }
#   define HT2A(sDest, szIn) { sDest = szIn; }
#   define HA2TC(szDest, szIn) { strcpy(szDest, szIn); }
#   define HT2AC(szDest, szIn) { strcpy(szDest, szIn); }
#   define HT2W HA2W
#   define HW2T HW2A
#   define HT2WC HW2AC
#   define HW2TC HA2WC
#endif


/*/////////////////////////////////////////////////////////////////////////////
    STRING HIGH LEVEL UTILITIES
*/

struct __CTSTRING_REF
{
    // encapsulates a CTSTRING& - this is just so that sprintf()'s 'sFormat' argument can be passed on
    // the stack, so va_start works correctly.
    __CTSTRING_REF(CTSTRING& s)
        : m_r(s)
    {
    }
    CTSTRING& m_r;
};

void HStrToUpper(TSTRING &s);	// Converts to uppercase
void HStrToLower(TSTRING &s);	// Converts to lowercase

void HStrRemoveAll(TSTRING& s, TCHAR wc);// remove all occurances
void HStrReplace(TSTRING &s, CTSTRING& to_find, CTSTRING& repl_with);// find/replace a whole string

bool IsWhitespace(TCHAR);
void TrimWhitespace(TSTRING& s);// removes spaces and tabs from right and left of string
void TrimWhitespace(TISTRING& s);

bool sprintf(TSTRING& s, PCTSTR sFormat, ...);

void HStrEncode(TSTRING& s, TCHAR chKey, PCTSTR wszChars);// URL-encode
void HStrDecode(TSTRING& s, TCHAR cKey);

TSTRING HFormatFileSize(unsigned long sizebytes);

/*/////////////////////////////////////////////////////////////////////////////
    STRING PATH FUNCTIONS (mostly TSTRING renditions of the shlwapi functions)
/*/////////////////////////////////////////////////////////////////////////////

bool TFileNameFromPath(CTSTRING &tsPath, TSTRING &tsFile);// Returns 'foo.dat' from path e.g. C:\foo\bar\foo.dat
TSTRING::size_type HPathFindNextComponent(CTSTRING& s, TSTRING::size_type nStart = 0);
void HPathRemoveFileSpec(TSTRING& s);// removes the filename off of a path.
bool HPathMatchSpec(CTSTRING& sFile, CTSTRING& sFilter);
TSTRING::size_type HPathFindExtension(CTSTRING& s);// Returns npos if no extension was found.
TSTRING::size_type HPathFindFileName(CTSTRING& s);// Just like PathFindFileName().  Will never return npos.
bool HPathJoin(TSTRING& sDest, CTSTRING& sExtra);

/*
    This function will test if the string is a full absolute
    path.  If it is relative, then it is assumed to be relative
    to wszRoot and it's fixed up.

    If wszRoot is 0, then the current directory is used.

    Returns true on success, false on failure.
*/
bool HEnsureRelativePath(TSTRING& Path, CTSTRING& sRoot);
bool HEnsureAbsolutePath(TSTRING& Out, CTSTRING& sFile, CTSTRING& sRoot);
bool HEnsureAbsolutePathCD(TSTRING& Out, CTSTRING& sFile);// current directory.


/*/////////////////////////////////////////////////////////////////////////////
    WIN32 WRAPPERS
*/

bool HExpandEnvironmentVariables(CTSTRING& sIn, TSTRING& out);
bool HGetWindowText(HWND hWnd, TSTRING& s);
bool HGetCurrentDirectory(TSTRING& s);
bool HCreateDirectory(CTSTRING& wsz);// Creates a directory and any child directories it needs.
bool HCleanDirectory(CTSTRING& wszDir, CTSTRING& wszFilter);// Cleans a directory recursively using the specified filter.
bool HCopyFile(CTSTRING& sDest, CTSTRING& sSrc);
bool HCLSIDFromProgID(CTSTRING& sProgID, CLSID& x);
void *OLAcquireResource(HMODULE hMod, PCTSTR pszName, PCTSTR pszType, DWORD *pdwSize);


/*/////////////////////////////////////////////////////////////////////////////
    CONSOLE FUNCTIONS
*/

bool ConsoleGetLine(TSTRING& sOut);
bool Pause();



/*/////////////////////////////////////////////////////////////////////////////
    BYTE-ORDER FUNCTIONS
*/
enum ByteOrder
{
    BO_LE,
    BO_BE,
    BO_UNKNOWN,
    BO_MACHINE
};

ByteOrder GetMachineByteOrder();


/*/////////////////////////////////////////////////////////////////////////////
    TEMPLATED STRING FUNCTIONS
*/

/*
    This weird function just scans an array of whatever-sized units and
    copies from one type of c-style array to another.

    nElements is the number of chars to copy (INCLUDING the null terminator, if applicable)

    If nElements is -1, then it will just go until a NULL is found in 
    the source arrray.
*/
template<typename TSrc, typename TDest>
inline void ConvertBuffer(TDest* pOut, const TSrc* pIn, int nElements)
{
    while(nElements)
    {
        *pOut = (TDest)*pIn;
        pOut++;
        pIn++;
        nElements--;
    }
}

template<typename TSrc, typename TDest>
inline void ConvertBuffer(TDest* pOut, const TSrc* pIn)
{
    do
    {
        *pOut = (TDest)*pIn;
        pOut++;
        pIn++;
    }
    while(*pIn);
}


// templated strlen()
template<typename TEl>
inline size_t xszlen(const TEl* sz)
{
    const TEl* sz2 = sz;

    while(*sz2)
    {
        sz2++;
    }

    return sz2-sz;
}

// Returns number of bytes required to hold the specified string, including the null terminator
template<typename TEl>
inline void xszbytes(const TEl* sz)
{
    return (xszlen<TEl>(sz)+1) * sizeof(TEl);
}


template<typename TEl>
inline void xszcpy(TEl* szDest, const TEl* szSrc)
{
    memcpy(szDest, szSrc, xszbytes(szSrc));
    return;
}


template<typename TEl>
inline bool eqi(const TEl& _Left, const TEl& _Right)
{
    // test for element equality
    return (tolower(_Left) == tolower(_Right));
}


template<typename TEl>
inline bool lti(const TEl& _Left, const TEl& _Right)
{
    // test if _Left precedes _Right
    return (tolower(_Left) < tolower(_Right));
}


template<typename TEl>
inline bool eq(const TEl& _Left, const TEl& _Right)
{
    // test for element equality
    return (_Left) == (_Right);
}


template<typename TEl>
inline bool lt(const TEl& _Left, const TEl& _Right)
{
    // test if _Left precedes _Right
    return _Left < _Right;
}


template<typename TEl>
inline int xsznicmp(const TEl *_First1, const TEl *_First2, size_t _Count)
{
    // compare [_First1, _First1 + _Count) with [_First2, ...)
    while(_Count)
    {
        if(!eqi(*_First1, *_First2))
        {
            return (lti(*_First1, *_First2) ? -1 : +1);
        }

        _Count--;
        _First1++;
        _First2++;
    }

    return (0);
}


template<typename TEl>
inline int xszncmp(const TEl *_First1, const TEl *_First2, size_t _Count)
{
    while(_Count)
    {
        if(!eq(*_First1, *_First2))
        {
            return (lt(*_First1, *_First2) ? -1 : +1);
        }

        _Count--;
        _First1++;
        _First2++;
    }

    return (0);
}


template<typename TEl>
inline int xszicmp(const TEl *left, const TEl *right)
{
    while(*left && *right)
    {
        if(!eqi(*left, *right))
        {
            return (lti(*left, *right) ? -1 : +1);
        }

        _First1++;
        _First2++;
    }

    return (0);
}


template<typename TEl>
inline int xszcmp(const TEl *left, const TEl *right)
{
    while(*left && *right)
    {
        if(!eq(*left, *right))
        {
            return (lt(*left, *right) ? -1 : +1);
        }

        left++;
        right++;
    }

    return (0);
}


struct EXCEPINFO_T
{
    EXCEPINFO_T(const EXCEPINFO& x)
    {
        if(x.bstrDescription) HW2T(Description, x.bstrDescription);
        if(x.bstrHelpFile) HW2T(HelpFile, x.bstrHelpFile);
        if(x.bstrSource) HW2T(Source, x.bstrSource);
        dwHelpContext = x.dwHelpContext;
        scode = x.scode;
        wcode = x.wCode;
    }

    TSTRING Description;
    TSTRING HelpFile;
    TSTRING Source;
    DWORD dwHelpContext;
    SCODE scode;
    WORD wcode;
};


class CExcepInfo : public EXCEPINFO
{
public:
    CExcepInfo()
    {
        wCode = 0;
        wReserved = 0;
        bstrSource = 0;
        bstrDescription = 0;
        bstrHelpFile = 0;
        dwHelpContext = 0;
        pvReserved = 0;
        pfnDeferredFillIn = 0;
        scode = 0;
    }
    ~CExcepInfo()
    {
        if ( bstrSource ) :: SysFreeString(bstrSource);
        if ( bstrDescription ) ::SysFreeString(bstrDescription);
        if ( bstrHelpFile ) ::SysFreeString(bstrHelpFile);
    }
};


// these functions assume that the type will automatically eliminate decimal places
// so that it will round correctly.

// GRANULARITY IS IN SHIFTS, not units.
template<typename T>
inline T GranularizeUp(const T& val, const T& granularity)
{
    return (1 + (val >> granularity)) << granularity;
}

template<typename T>
inline T GranularizeDown(const T& val, const T& granularity)
{
    return (val >> granularity) << granularity;
}

