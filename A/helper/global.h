

#pragma once


#include "applog.h"
#include <comdef.h>


void Err_DbOpen(_com_error& e);
void Err_DbExecute(_com_error& e);
void Err_DbClose(_com_error& e);
void Err_DbProviderSpecific(CTSTRING& sDescription, long lNumber, CTSTRING& sSource);
void Err_DbMoveNext(_com_error& e);
void Err_DbFieldItem(CTSTRING& sFieldName, _com_error& e);

