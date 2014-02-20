<!-- #include File = "global.asp"-->
<%

WriteHeader "_tWinMain Web"

%>

<table class=stats>
    <tr><th colspan=2>Misc. stats</th></tr>
    <tr><td>Number of transactions</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogTransactions")%></td></tr>
    <tr><td>Number of !help calls</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogAliasUses")%></td></tr>
    <tr><td>Number of !msgset calls</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogMessageCreations")%></td></tr>
    <tr><td>Number of !msgchange calls</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogMessageUpdates")%></td></tr>
    <tr><td>Number of !msgquery calls</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogQueries")%></td></tr>
    <tr><td>Number of users</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogUsers")%></td></tr>
    <tr><td>Total number of aliases</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogAliases")%></td></tr>
    <tr><td>Number of active aliases</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogAliases where AliasEnabled = 1")%></td></tr>
    <tr><td>Number of messages</td><td><%=GetFirstValueOfQuery("select count(*) from WinprogMessages")%></td></tr>
</table>

<%
WhipUpATable "Top 20 Helpers", "EXECUTE WinprogWeb_GetTopHelpers 20", "Uses"
WhipUpATable "Latest 20 message uses", "EXECUTE WinprogWeb_GetLatestUses 20", ""
WhipUpATable "Latest 20 message updates", "EXECUTE WinprogWeb_GetRecentUpdates 20", ""
WhipUpATable "Most used messages", "EXECUTE WinprogWeb_GetMostUsedMessages 40", "Uses"
WhipUpATable "Most aliases for a message", "EXECUTE WinprogWeb_GetMostAliases 20", "AliasCount"
%>

<a href=bymonth.asp>[ Monthly Stats (query takes a while) ]</a>

<%
WriteFooter
%>
