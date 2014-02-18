/*
    assimilates old XML data into the new winprog server
*/

#pragma once


#include "helpdb.h"


namespace Winprog
{

bool Assimilate(CTSTRING& xml, db& sql);
bool PopulateEntities(CCADO::Connection&);

}