/*
    File:
        ichar.h
    Created:
        August 17, 2002
        Carl Corcoran
    Description:
        This class provides char_traits functionality except overrides
        comparison functionality.  ichar_traits<> will perform a tolower()
        comparison instead of the normal comparison.

        This class is used in stdafx.h to create the datatypes WISTRING,
        PWISTRING, CWISTRING, and PCWISTRING.
*/


#pragma once


#include <string>


using namespace std;


template<class _Elem>
struct ichar_traits : public char_traits<_Elem>
{
    static bool __cdecl eq(const _Elem& _Left, const _Elem& _Right)
    {
        // test for element equality
        return (tolower(_Left) == tolower(_Right));
    }

    static bool __cdecl lt(const _Elem& _Left, const _Elem& _Right)
    {
        // test if _Left precedes _Right
        return (tolower(_Left) < tolower(_Right));
    }

    static int __cdecl compare(const _Elem *_First1, const _Elem *_First2, size_t _Count)
    {
        // compare [_First1, _First1 + _Count) with [_First2, ...)
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
};


#ifndef WSTRING_DEFINED
#   define WSTRING_DEFINED

    typedef       std::basic_string<wchar_t> WSTRING,   *PWSTRING;
    typedef const std::basic_string<wchar_t> CWSTRING,  *PCWSTRING;
    typedef       std::basic_string<wchar_t, ichar_traits<wchar_t> > WISTRING,  *PWISTRING;
    typedef const std::basic_string<wchar_t, ichar_traits<wchar_t> > CWISTRING, *PCWISTRING;

    typedef       std::basic_string<char> ASTRING,   *PASTRING;
    typedef const std::basic_string<char> CASTRING,  *PCASTRING;
    typedef       std::basic_string<char, ichar_traits<char> > AISTRING,  *PAISTRING;
    typedef const std::basic_string<char, ichar_traits<char> > CAISTRING, *PCAISTRING;

    typedef       std::basic_string<TCHAR> TSTRING,   *PTSTRING;
    typedef const std::basic_string<TCHAR> CTSTRING,  *PCTSTRING;
    typedef       std::basic_string<TCHAR, ichar_traits<TCHAR> > TISTRING,  *PTISTRING;
    typedef const std::basic_string<TCHAR, ichar_traits<TCHAR> > CTISTRING, *PCTISTRING;

#endif




#ifdef _UNICODE
#   define _tstring                std::wstring
#   define _tcin                   std::wcin
#   define _tcout                  std::wcout
#   define _tcerr                  std::wcerr
#   define _tclog                  std::wclog
#   define _tios                   std::wios
#   define _tstreambuf             std::wstreambuf
#   define _tistream               std::wistream
#   define _tostream               std::wostream
#   define _tiostream              std::wiostream
#   define _tstringbuf             std::wstringbuf
#   define _tistringstream std::wistringstream
#   define _tostringstream std::wostringstream
#   define _tstringstream  std::wstringstream
#   define _tfilebuf               std::wfilebuf
#   define _tifstream              std::wifstream
#   define _tofstream              std::wofstream
#   define _tfstream               std::wfstream
#else
#   define _tstring                std::string
#   define _tcin                   std::cin
#   define _tcout                  std::cout
#   define _tcerr                  std::cerr
#   define _tclog                  std::clog
#   define _tios                   std::ios
#   define _tstreambuf             std::streambuf
#   define _tistream               std::istream
#   define _tostream               std::ostream
#   define _tiostream              std::iostream
#   define _tstringbuf             std::stringbuf
#   define _tistringstream std::istringstream
#   define _tostringstream std::ostringstream
#   define _tstringstream  std::stringstream
#   define _tfilebuf               std::filebuf
#   define _tifstream              std::ifstream
#   define _tofstream              std::ofstream
#   define _tfstream               std::fstream
#endif


#ifndef IsValidHandle
#   define IsValidHandle(a) (((a)!=0)&&((a)!=INVALID_HANDLE_VALUE))
#endif

#ifndef IsBadHandle
#   define IsBadHandle(a) (((a)==0)||((a)==INVALID_HANDLE_VALUE))
#endif

#ifndef SAFE_DELETEOBJECT
#   define SAFE_DELETEOBJECT(a) { if((a) != NULL) DeleteObject(a); (a) = 0;}
#endif

#ifndef SAFE_FREE
#   define SAFE_FREE(a) { if((a) != NULL) free(a); (a) = 0;}
#endif

#ifndef SAFE_DELETE
#   define SAFE_DELETE(a) { if((a) != NULL) delete (a); (a) = 0;}
#endif

#ifndef SAFE_RELEASE
#   define SAFE_RELEASE(a)             { if((a) != NULL) (a)->Release(); (a) = 0;}
#endif
