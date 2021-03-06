if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasCreations_WinprogAliases]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasCreations] DROP CONSTRAINT FK_WinprogAliasCreations_WinprogAliases
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasUpdates_WinprogAliases]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasUpdates] DROP CONSTRAINT FK_WinprogAliasUpdates_WinprogAliases
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasUses_WinprogAliases]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasUses] DROP CONSTRAINT FK_WinprogAliasUses_WinprogAliases
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogTransactions_WinprogChannels]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogTransactions] DROP CONSTRAINT FK_WinprogTransactions_WinprogChannels
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliases_WinprogMessages]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliases] DROP CONSTRAINT FK_WinprogAliases_WinprogMessages
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageCreations_WinprogMessages]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageCreations] DROP CONSTRAINT FK_WinprogMessageCreations_WinprogMessages
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageUpdates_WinprogMessages]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageUpdates] DROP CONSTRAINT FK_WinprogMessageUpdates_WinprogMessages
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageUses_WinprogMessages]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageUses] DROP CONSTRAINT FK_WinprogMessageUses_WinprogMessages
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasCreations_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasCreations] DROP CONSTRAINT FK_WinprogAliasCreations_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasUpdates_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasUpdates] DROP CONSTRAINT FK_WinprogAliasUpdates_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogAliasUses_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogAliasUses] DROP CONSTRAINT FK_WinprogAliasUses_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageCreations_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageCreations] DROP CONSTRAINT FK_WinprogMessageCreations_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageUpdates_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageUpdates] DROP CONSTRAINT FK_WinprogMessageUpdates_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogMessageUses_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogMessageUses] DROP CONSTRAINT FK_WinprogMessageUses_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogQueries_WinprogTransactions]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogQueries] DROP CONSTRAINT FK_WinprogQueries_WinprogTransactions
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_WinprogTransactions_WinprogUsers]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[WinprogTransactions] DROP CONSTRAINT FK_WinprogTransactions_WinprogUsers
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[DateToYYYYMM]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[DateToYYYYMM]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FormatUserMask]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[FormatUserMask]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[GetUserID]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[GetUserID]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_CreateNewCookie]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_CreateNewCookie]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_QueryInternal]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_QueryInternal]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiredUploads]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[ExpiredUploads]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[NewUpload]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[NewUpload]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[RecentUploads]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[RecentUploads]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SetDeleted]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[SetDeleted]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SetUploaded]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[SetUploaded]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WhatNeedsCleaning]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WhatNeedsCleaning]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMsg]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogMsg]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMsgQuery]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogMsgQuery]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogQueryEntities]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogQueryEntities]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetMostUsedMessagesByEntity]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetMostUsedMessagesByEntity]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetRecentUpdates]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetRecentUpdates]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetTopHelpers]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetTopHelpers]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetTopMessageEntities]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetTopMessageEntities]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_SearchMessages]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_SearchMessages]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Winprog_FindEntity]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[Winprog_FindEntity]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogQueryUserEntities]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogQueryUserEntities]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogTopHelpers]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogTopHelpers]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetLatestUses]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetLatestUses]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetMessageHistory]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetMessageHistory]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetMessageUsageHistory]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetMessageUsageHistory]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetMostAliases]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetMostAliases]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogWeb_GetMostUsedMessages]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[WinprogWeb_GetMostUsedMessages]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Winprog_AllMessages]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[Winprog_AllMessages]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Winprog_AllTransactions]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[Winprog_AllTransactions]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Uploads]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Uploads]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Users]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Users]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogAliasCreations]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogAliasCreations]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogAliasUpdates]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogAliasUpdates]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogAliasUses]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogAliasUses]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogAliases]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogAliases]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogChannels]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogChannels]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogEntities]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogEntities]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMessageCreations]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogMessageCreations]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMessageUpdates]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogMessageUpdates]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMessageUses]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogMessageUses]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogMessages]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogMessages]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogQueries]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogQueries]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogTransactions]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogTransactions]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[WinprogUsers]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[WinprogUsers]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE function dbo.DateToYYYYMM(@d as datetime)
returns varchar(20)
as
begin
    declare @sYear varchar(20)
    declare @sMonth varchar(20)
    set @sYear = Year(@d)
    set @sMonth = Month(@d)
    return left('0000', 4-len(@sYear)) + @sYear + '-' + left('0000', 2 - len(@sMonth)) + @sMonth
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE  FUNCTION dbo.FormatUserMask (@nick varchar(30), @ident varchar(30), @host varchar(100))  
RETURNS varchar(162) AS  
BEGIN 
    return @nick + '!' + @ident + '@' + @host
END

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

-- returns -1 if there was no user found that matches the given credentials.
create function dbo.GetUserID(
    @User varchar(50),
    @Password varchar(50)
    )
returns int
as
begin
    declare @r int
    declare @n int
    select @n = count(*), @r = min(Users.ID) from Users where
        Nick = @User and
        Password = @Password
    if @n = 0
        set @r = -1
    return @r
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create function dbo.fn_CreateNewCookie(@guid as uniqueidentifier)
returns varchar(8)
as
begin
    declare @newid as varchar(50)
    declare @checksum as int
    declare @digits as varchar(50)
    declare @bin as binary(4)
    declare @res as varchar(40)

    set @checksum = CHECKSUM(@guid)
    set @bin = @checksum
    EXEC master..xp_varbintohexstr @bin, @res OUT

    select @res = lower(substring(@res, 3, 8))
    select @res = replace(@res, '0', 'm')
    select @res = replace(@res, '1', 'o')
    select @res = replace(@res, '2', 'w')
    select @res = replace(@res, '3', 'u')
    select @res = replace(@res, '4', 'i')
    select @res = replace(@res, '5', 'r')
    select @res = replace(@res, '6', 's')
    select @res = replace(@res, '7', 't')
    select @res = replace(@res, '8', 'l')
    select @res = replace(@res, '9', 'n')
    return @res
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

/*
    returns a table that looks like this (for @q='%mod%'):
    
    AliasID AliasText   MessageID   Usage   MessageText
    625     pastebawx   41          1       Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    854     iknow       41          1       Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    626     nopaste     41          3       Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    442     pastecode   41          3       Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    43      PASTE       41          1506    Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    102     +M          100         5       Setting the channel to +m will allow only those with a +v to post messages to the chan. We are moderated because devoicing is less harsh than kicking.
    197     DOGGIE      195         1       Great movie! http://modernhumorist.com/mh/0205/free/
    205     DEPENDS     203         12      Dependency walker shows you export information about 32-bit Windows modules. http://www.dependencywalker.com/ ... and don't forget about dumpbin.exe :)
    361     SARCASM     359         6       a mode of satirical wit depending for its effect on bitter, caustic, and often ironic language that is usually directed against an individual
    363     EFFORT      361         20      We're undertaking an effort to moderate the stupid. It's nothing personal, we're just people who are interested in productivity and coding, not your ebay ad or your sister. Return in 24 hours and try to be a little more mature, thanks. :)
    680     filename    671         5       GetModuleFileName() retrieves the full path and filename of your binary. If you just want the filename or path, stop being a pussy and write the 2 lines of code to parse it.
    Another function is used to clean it up to look like this:
    AliasID AliasText   MessageID   Usage   MessageText
    43      PASTE       41          1506    Don't interrupt the channel with your code. Paste it at http://rafb.net/paste/ NOTE: Paste the entire .c or .cpp file UNMODIFIED, unless otherwise instructed.
    102     +M          100         5       Setting the channel to +m will allow only those with a +v to post messages to the chan. We are moderated because devoicing is less harsh than kicking.
    197     DOGGIE      195         1       Great movie! http://modernhumorist.com/mh/0205/free/
    205     DEPENDS     203         12      Dependency walker shows you export information about 32-bit Windows modules. http://www.dependencywalker.com/ ... and don't forget about dumpbin.exe :)
    361     SARCASM     359         6       a mode of satirical wit depending for its effect on bitter, caustic, and often ironic language that is usually directed against an individual
    363     EFFORT      361         20      We're undertaking an effort to moderate the stupid. It's nothing personal, we're just people who are interested in productivity and coding, not your ebay ad or your sister. Return in 24 hours and try to be a little more mature, thanks. :)
    680     filename    671         5       GetModuleFileName() retrieves the full path and filename of your binary. If you just want the filename or path, stop being a pussy and write the 2 lines of code to parse it.
*/
CREATE  function fn_QueryInternal(@q varchar(50))
returns @r table(
    AliasID int,
    AliasText varchar(50),
    MessageID int,
    NumberOfAliasUses int,
    MessageText varchar(1000),
    AliasEnabled int
    )
as
begin
    insert @r
    select
        AliasID,
        min(AliasText) AliasText,
        MessageID,
        count(AliasUseTransactionID) NumberOfAliasUses,
        min(MessageText) MessageText,
        min(cast(AliasEnabled as int)) AliasEnabled
    from
        WinprogAliasUses right join
        WinprogAliases on AliasID = AliasUseAliasID inner join
        WinprogMessages on MessageID = AliasMessageID
    where
        MessageText like @q or
        AliasText like @q and
        AliasEnabled = 1
    group by
        MessageID,
        AliasID
    having min(cast(AliasEnabled as int)) = 1
    return
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE TABLE [dbo].[Uploads] (
	[Cookie] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Nick] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Requested] [smalldatetime] NOT NULL ,
	[Expires] [smalldatetime] NOT NULL ,
	[Bytes] [int] NOT NULL ,
	[UserID] [int] NOT NULL ,
	[Uploaded] [bit] NOT NULL ,
	[Deleted] [bit] NOT NULL ,
	[Filename] [varchar] (260) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[TransactionID] [int] NOT NULL ,
	[RealSize] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Users] (
	[ID] [int] IDENTITY (1, 1) NOT NULL ,
	[Nick] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Password] [varchar] (50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogAliasCreations] (
	[AliasCreationTransactionID] [int] NOT NULL ,
	[AliasCreationAliasID] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogAliasUpdates] (
	[AliasUpdateTransactionID] [int] NOT NULL ,
	[AliasUpdateAliasID] [int] NOT NULL ,
	[AliasUpdateToEnabled] [bit] NULL ,
	[AliasUpdateToMessageID] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogAliasUses] (
	[AliasUseTransactionID] [int] NOT NULL ,
	[AliasUseAliasID] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogAliases] (
	[AliasID] [int] IDENTITY (1, 1) NOT NULL ,
	[AliasMessageID] [int] NOT NULL ,
	[AliasText] [varchar] (50) COLLATE Latin1_General_CI_AI NOT NULL ,
	[AliasEnabled] [bit] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogChannels] (
	[ChannelID] [int] IDENTITY (1, 1) NOT NULL ,
	[ChannelName] [char] (300) COLLATE Latin1_General_CI_AI NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogEntities] (
	[EntityID] [int] IDENTITY (1, 1) NOT NULL ,
	[EntityName] [char] (50) COLLATE Latin1_General_CI_AI NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogMessageCreations] (
	[MessageCreationTransactionID] [int] NOT NULL ,
	[MessageCreationMessageID] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogMessageUpdates] (
	[MessageUpdateTransactionID] [int] NOT NULL ,
	[MessageUpdateMessageID] [int] NOT NULL ,
	[MessageUpdateToText] [varchar] (1000) COLLATE Latin1_General_CI_AI NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogMessageUses] (
	[MessageUseTransactionID] [int] NOT NULL ,
	[MessageUseMessageID] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogMessages] (
	[MessageID] [int] IDENTITY (1, 1) NOT NULL ,
	[MessageText] [varchar] (1000) COLLATE Latin1_General_CI_AI NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogQueries] (
	[QueryTransactionID] [int] NOT NULL ,
	[QueryText] [varchar] (1000) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogTransactions] (
	[TransactionID] [int] IDENTITY (1, 1) NOT NULL ,
	[TransactionDate] [datetime] NOT NULL ,
	[TransactionUserID] [int] NOT NULL ,
	[TransactionChannelID] [int] NOT NULL ,
	[TransactionSuccess] [bit] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[WinprogUsers] (
	[UserID] [int] IDENTITY (1, 1) NOT NULL ,
	[UserNick] [char] (30) COLLATE Latin1_General_CI_AI NOT NULL ,
	[UserIdent] [char] (30) COLLATE Latin1_General_CI_AI NULL ,
	[UserHost] [varchar] (100) COLLATE Latin1_General_CI_AI NULL ,
	[UserEntityID] [int] NULL 
) ON [PRIMARY]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

-- A table of all messages with a single ALIAS.  So if there are multiple aliases
-- for 1 message, only one will show up here.
CREATE VIEW dbo.Winprog_AllMessages
AS
SELECT     MAX(dbo.WinprogAliases.AliasText) AS AliasText, MAX(dbo.WinprogAliases.AliasID) AS AliasID, dbo.WinprogMessages.MessageText, 
                      dbo.WinprogMessages.MessageID
FROM         dbo.WinprogMessages RIGHT OUTER JOIN
                      dbo.WinprogAliases ON dbo.WinprogAliases.AliasMessageID = dbo.WinprogMessages.MessageID
GROUP BY dbo.WinprogMessages.MessageText, dbo.WinprogMessages.MessageID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE VIEW dbo.Winprog_AllTransactions
AS
SELECT     dbo.WinprogUsers.UserID, dbo.WinprogUsers.UserNick, dbo.WinprogUsers.UserIdent, dbo.WinprogUsers.UserHost, dbo.WinprogEntities.EntityName, 
                      dbo.WinprogTransactions.TransactionDate, dbo.WinprogTransactions.TransactionID, dbo.WinprogEntities.EntityID, 
                      dbo.WinprogTransactions.TransactionChannelID
FROM         dbo.WinprogTransactions LEFT OUTER JOIN
                      dbo.WinprogUsers ON dbo.WinprogTransactions.TransactionUserID = dbo.WinprogUsers.UserID LEFT OUTER JOIN
                      dbo.WinprogEntities ON dbo.WinprogEntities.EntityID = dbo.WinprogUsers.UserEntityID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.ExpiredUploads AS
select top 50 * from Uploads where Deleted=1 order by Requested desc
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

-- returns a "cookie" that can be used to identify this upload, or NULL if it could not be done.
CREATE procedure dbo.NewUpload
    @sCookie varchar(50) OUTPUT,
    @User varchar(50),
    @Password varchar(50),
    @Nick varchar(50),
    @Bytes int,
    @TransactionID int,
    @Minutes int = 60
AS
BEGIN
    -- validate user.
    Declare @Cookie varchar(50)
    Declare @uid int
--    Declare @newcookie uniqueidentifier
    set @Cookie = dbo.fn_CreateNewCookie(NEWID())
    set @uid = dbo.GetUserID(@User, @Password)
    if @uid = -1
    begin
        set @Cookie = null
    end
    else
    begin
        insert dbo.Uploads
            (Nick, Cookie, Requested, Expires, Bytes, UserID, TransactionID, Filename, RealSize)
        values
            (@Nick,
            @Cookie,
            GetDate(),
            DateAdd(mi, @Minutes, GetDate()),
            @Bytes,
            @uid,
            @TransactionID,
            '',
            0
            )
    end

    set @sCookie = @Cookie
END
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.RecentUploads AS
select
    *,
    datediff(ss, GetDate(), Expires) as SecondsLeft
from
    Uploads
where
    Uploaded=1 and
    Deleted=0
order by
    Requested desc
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.SetDeleted
    @c varchar(50)
AS
    update Uploads set Deleted = 1 where Cookie = @c
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.SetUploaded
    @c varchar(50),
    @filename varchar(260),
    @size int
AS
    update Uploads set Uploaded = 1, Filename=@filename, RealSize=@size where Cookie = @c
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.WhatNeedsCleaning
AS
    select * from Uploads where Expires < GetDate() and Uploaded=1 and Deleted = 0
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogMsg
    @Alias varchar(50)
AS
    SELECT TOP 1 MessageText, AliasID, MessageID FROM WinprogMessages
        INNER JOIN WinprogAliases ON AliasMessageID=MessageID
        WHERE AliasText = @Alias AND AliasEnabled=1

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE    PROCEDURE WinprogMsgQuery
    @sQuery varchar(50),
    @nRows  int = 50
AS
SET ROWCOUNT @nRows
-- modified 1/16/2004 by carl corcoran.
-- fixed a bug where !msgquery was returning every alias that matched a message.
-- that's ugly.  this one returns the most popular.
-- SELECT AliasID, AliasText FROM
--     WinprogMessages INNER JOIN WinprogAliases ON AliasMessageID=MessageID
--     WHERE MessageText LIKE @sQuery OR AliasText LIKE @sQuery AND AliasEnabled=1 ORDER BY AliasText ASC
select
    AliasID,
    AliasText
from
    fn_QueryInternal(@sQuery) a
where
    NumberOfAliasUses in
        (select
            max(NumberOfAliasUses)
        from
            fn_QueryInternal(@sQuery) b
        where
            b.MessageID = a.MessageID
        group by
            MessageID)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

-- Use this for getting a list of all ENTITIES, and for convenience, every user
-- associated with it.
--
-- This will return a list of many users and many entities, where it returns ALL entities.
-- If an entity does not have an associated user, the columns will be NULL.
CREATE PROCEDURE WinprogQueryEntities @sQuery varchar(150) AS
SELECT EntityName, UserNick, UserIdent, UserHost, EntityID, UserID
    FROM WinprogUsers RIGHT JOIN WinprogEntities  ON UserEntityID=EntityId
    WHERE EntityName LIKE @sQuery
    ORDER BY EntityName ASC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE  PROCEDURE dbo.WinprogWeb_GetMostUsedMessagesByEntity
    @eid int,
    @nRows int = 20
AS
SET ROWCOUNT @nRows
select
    AliasID,
    AliasText,
    min(AliasMessageID) MessageID,
    count(TransactionID) Uses
from
    WinprogAliasUses inner join
    WinprogAliases on AliasID = AliasUseAliasID inner join
    WinprogTransactions on AliasUseTransactionID = TransactionID inner join
    WinprogUsers on UserID = TransactionUserID inner join
    WinprogEntities on UserEntityID = EntityID
where EntityID = @eid
group by AliasID, AliasText
order by Uses desc

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetRecentUpdates
    @nRows int=20
AS
SET ROWCOUNT @nRows
SELECT RTRIM(AliasText) AS Alias, RTRIM(EntityName) AS EntityName, RTRIM(UserNick)+'!'+RTRIM(UserIdent)+'@'+RTRIM(UserHost) AS UserHostMask, TransactionDate, RTRIM(MessageUpdateToText)
FROM
    Winprog_AllTransactions INNER JOIN
    WinprogMessageUpdates ON MessageUpdateTransactionID=TransactionID LEFT JOIN
    Winprog_AllMessages ON MessageID=MessageUpdateMessageID
    ORDER BY TransactionDate DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetTopHelpers
    @nRows int = 20
AS
SET ROWCOUNT @nRows
SELECT EntityName, COUNT(*) AS 'Uses', RTRIM(MAX(UserNick))+'!'+RTRIM(MAX(UserIdent))+'@'+RTRIM(MAX(UserHost)) AS 'UserHostMask'
FROM
    Winprog_AllTransactions RIGHT JOIN
    WinprogMessageUses ON MessageUseTransactionID=TransactionID
    WHERE UserHost <> 'ted' AND UserIdent <> 'assimil'
    GROUP BY EntityName
    ORDER BY 'Uses' DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE  PROCEDURE WinprogWeb_GetTopMessageEntities
    @nRows int,
    @MessageID int
AS
    SET ROWCOUNT @nRows
    SELECT
        MAX(EntityName) AS EntityName,
        dbo.FormatUserMask(rtrim(MAX(UserNick)), rtrim(MAX(UserIdent)), rtrim(MAX(UserHost))) Hostmask,
        COUNT(*) AS Uses
    FROM WinprogMessageUses
        INNER JOIN WinprogTransactions ON MessageUseTransactionID=TransactionID
        INNER JOIN WinprogUsers ON UserID=TransactionUserID
        INNER JOIN WinprogEntities ON UserEntityID=EntityID
    WHERE
        MessageUseMessageID=@MessageID
        and UserHost != 'ted'
    GROUP BY EntityName
    ORDER BY Uses DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_SearchMessages
    @nRows int,
    @sQuery varchar(50)
AS
SET ROWCOUNT @nRows
SELECT AliasID, MessageText, AliasText FROM WinprogMessages
    INNER JOIN WinprogAliases ON AliasMessageID=MessageID
    WHERE AliasEnabled=1 AND MessageText LIKE @sQuery OR AliasText LIKE @sQuery
    ORDER BY AliasText ASC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE Winprog_FindEntity
    @sNick varchar(50),
    @sIdent varchar(50),
    @sHost varchar(150)
AS
SELECT TOP 1 EntityID FROM
    WinprogEntities
    RIGHT OUTER JOIN WinprogUsers ON UserEntityID=EntityID
    WHERE
        (UserNick=@sNick OR UserHost=@sHost)
        AND
        (UserHost <> 'ted' AND UserIdent <> 'assimil')

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

-- Use this for getting a list of all USERS, and the entityname that it's associated with.
CREATE PROCEDURE WinprogQueryUserEntities
    @sQuery varchar(150),
    @nRows int=0
AS
SET ROWCOUNT @nRows
SELECT EntityName, UserNick, UserIdent, UserHost, EntityID, UserID
    FROM WinprogUsers INNER JOIN WinprogEntities  ON UserEntityID=EntityId
    WHERE EntityID IN (SELECT UserEntityID FROM WinprogUsers WHERE UserNick+UserHost+UserIdent LIKE @sQuery)
    ORDER BY EntityName ASC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogTopHelpers
@nRows INT
AS
SET ROWCOUNT @nRows
SELECT MAX(UserNick) AS UserNick, COUNT(*) AS 'MyCount' FROM WinprogMessageUses
    INNER JOIN WinprogTransactions ON MessageUseTransactionID=TransactionID
    INNER JOIN WinprogUsers ON UserID=TransactionUserID
    WHERE UserIdent <> 'assimil' AND UserHost <> 'ted'
    GROUP BY TransactionUserID, UserNick ORDER BY MyCount DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetLatestUses
    @nRows int=20
AS
SET ROWCOUNT @nRows
SELECT EntityName, AliasText AS 'Message', TransactionDate AS 'Time', RTRIM(UserNick)+'!'+RTRIM(UserIdent)+'@'+RTRIM(UserHost) AS 'UserHostMask'
FROM
    Winprog_AllTransactions INNER JOIN
    WinprogMessageUses ON MessageUseTransactionID=TransactionID LEFT JOIN
    Winprog_AllMessages ON MessageID=MessageUseMessageID
    ORDER BY TransactionDate DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetMessageHistory
    @MessageID int
AS
SELECT UserNick, UserHost, UserIdent, TransactionDate, MessageUpdateToText
    FROM WinprogTransactions
    INNER JOIN WinprogMessageUpdates ON MessageUpdateTransactionID=TransactionID
    INNER JOIN WinprogUsers ON UserID=TransactionUserID
    WHERE MessageUpdateMessageID=@MessageID ORDER BY TransactionDate DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetMessageUsageHistory
    @nRows int,
    @nMessageID int
AS
SET ROWCOUNT @nRows
SELECT TOP 10 TransactionDate, EntityName, RTRIM(UserNick)+'!'+RTRIM(UserIdent)+'@'+RTRIM(UserHost) AS 'UserHostMask' FROM
    WinprogMessageUses
    LEFT JOIN Winprog_AllTransactions ON TransactionID=MessageUseTransactionID
    WHERE MessageUseMessageID=@nMessageID
    ORDER BY TransactionDate DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.WinprogWeb_GetMostAliases
    @nRows int=20
AS
SET ROWCOUNT @nRows
select
    count(AliasID) AliasCount,
    min(AliasText) AliasText,
    min(MessageText) Message
from
    WinprogAliases left join
    WinprogMessages on AliasMessageID = MessageID
where
    AliasEnabled = 1
group by
    MessageID
order by AliasCount desc

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE WinprogWeb_GetMostUsedMessages
    @nRows int=20
AS
SET ROWCOUNT @nRows
SELECT COUNT(*) AS Uses, MAX(AliasText) AS Alias
FROM
    WinprogMessageUses LEFT JOIN Winprog_AllMessages ON MessageID=MessageUseMessageID
    GROUP BY MessageID
    ORDER BY Uses DESC

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

