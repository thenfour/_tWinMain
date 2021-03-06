if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_GetSkillTree]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_GetSkillTree]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tBooks]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tBooks]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tDates]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tDates]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tInstruments]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tInstruments]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tLessons]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tLessons]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tMusicians]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tMusicians]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tSessions]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tSessions]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tSkills]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[tSkills]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE FUNCTION dbo.fn_GetSkillTree(@rootid int, @indenttoken varchar(100) = ' ')
RETURNS @ret table
    (
        ID int primary key,
        Name varchar(50),
        ParentID int,
        Level int,
	IndentString varchar(1000),
        SortID varchar(1000)
    )
AS  
BEGIN 
    declare @RowsAdded int
    declare @level int
    declare @r TABLE (
        ID int primary key,
        Name nvarchar(50),
        ParentID int,
        Level int,
        SortID varchar(1000),
        processed int default 0,
	IndentString varchar(1000)
        )
    
    insert @r
    select ID, Name, ParentID, 0, '.', 0, '' from tSkills where ParentID = @rootid
    select @RowsAdded = @@rowcount
    
    select @level = 1
    
    /*
        in order to sort this stuff correctly we will need to do it like this:
        1) we pulled in the root, ID 1.  Format it to be 00000001.  Thats the sort ID
        2) pull in all the 
    
        values for processed:
        0 == newly inserted
        1 == a node we're searching for children for
        2 == nodes that are completely finished
    */
    while @RowsAdded > 0
    begin
        UPDATE @r
            SET processed = 1
            WHERE processed = 0
    
        insert @r
        select
            s.ID, s.Name, s.ParentID, @level,
            cast(r.SortID as varchar(50)) + Left('00000000', 8 - Len(s.ID)) + cast(s.ID as varchar(50)),
            0,
            r.IndentString + @indenttoken
        from
            tSkills s inner join
            @r r on s.ParentID = r.ID and r.processed = 1
    
        select @RowsAdded = @@rowcount
        select @level = @level + 1
    
        UPDATE @r
            SET processed = 2
            WHERE processed = 1
    end
    
    insert @ret
    select ID, Name, ParentID, Level, IndentString, SortID from @r order by SortID

    return
END



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE TABLE [dbo].[tBooks] (
	[ID] [int] IDENTITY (1, 1) NOT NULL ,
	[Title] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Author] [varchar] (100) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[ISBN] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[Description] [text] COLLATE SQL_Latin1_General_CP1_CI_AS NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[tDates] (
	[ID] [float] NOT NULL ,
	[Year] [float] NULL ,
	[Quarter] [float] NULL ,
	[Month] [float] NULL ,
	[YearWeek] [float] NULL ,
	[MonthWeek] [float] NULL ,
	[YearDay] [float] NULL ,
	[MonthDay] [float] NULL ,
	[Weekday] [float] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tInstruments] (
	[Name] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[ID] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tLessons] (
	[ID] [int] IDENTITY (1, 1) NOT NULL ,
	[Name] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[BookID] [int] NOT NULL ,
	[Description] [text] COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[Pages] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[ParentID] [int] NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[tMusicians] (
	[Name] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[ID] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tSessions] (
	[ID] [int] IDENTITY (1, 1) NOT NULL ,
	[BeginTime] [int] NOT NULL ,
	[EndTime] [int] NOT NULL ,
	[Musician] [int] NOT NULL ,
	[Instrument] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tSkills] (
	[ID] [int] IDENTITY (1, 1) NOT NULL ,
	[Name] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[ParentID] [int] NULL 
) ON [PRIMARY]
GO

