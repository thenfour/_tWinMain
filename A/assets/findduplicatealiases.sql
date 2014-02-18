
/*
    This script will find duplicate aliases in the table.
*/
use winprog

-- get the # of duplicates for each alias
declare @t1 table(AliasText varchar(50), AliasID int, C int)
declare @t2 table(AliasText varchar(50), AliasID int, C int)
declare @t3 table(AliasText varchar(50), AliasID int)

insert into @t1 select AliasText, AliasID, count(AliasID) from WinprogAliases
group by AliasText, AliasID with rollup

-- this is the aliases that have duplicates
insert into @t2 select * from @t1 where (C > 1) and (AliasText is not null)

-- 
-- -- this is the alias IDs that are duplicated.
insert into @t3 select R.AliasText, R.AliasID from @t2 L left join @t1 R on L.AliasText = R.AliasText where R.AliasID is not null

select * from @t3

-- update WinprogAliases set AliasText='COMPILER_old' where AliasID=287
-- update WinprogAliases set AliasText='iknow_old' where AliasID=129
-- update WinprogAliases set AliasText='nopaste_old' where AliasID=242
-- update WinprogAliases set AliasText='spy++_old' where AliasID=310
-- update WinprogAliases set AliasText='test_old' where AliasID=521
-- update WinprogAliases set AliasText='tinycrt_old' where AliasID=581
