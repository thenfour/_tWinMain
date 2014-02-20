<!-- #include File = "global.asp"-->
<%
'   QUERYSTRINGS:
'               : a = the alias id

'On Error Resume Next

WriteHeader "_tWinMain Web"

Dim sidAlias' string ID of alias
Dim aliasid, messageid

sidAlias = Request.QueryString("a")

If Len(sidAlias) = 0 Then Response.Redirect("default.asp")

aliasid = sidAlias+0

'////////////////////////////////////////////////////////////////////////////////////// SUMMARY
' Created By
' Used # times
Dim AliasText
Dim MessageText
Dim Creator
Dim rstTrans
Dim rstUser
Dim rstHistory
Dim rstAliases
Dim sql

messageid = GetAliasMessageID(aliasid)
MessageText = GetMessageText(messageid)
AliasText = GetAliasText(aliasid)

Set rstTrans = PerformSelect(SqlGetCreationTransactionFromMessageID(messageid))
Set rstUser = PerformSelect(SqlGetUserFromTransactionID(rstTrans("TransactionID")))
Creator = UserRstToMask(rstUser)

%>

<h1><%=HTMLEncode(AliasText)%></h1>
<img class=line><h3>Summary</h3>
<p>Message Text:<br><%=HTMLEncode(MessageText)%></p>
<p>Created by <b><%=HTMLEncode(Creator)%></b> on <b><%=HTMLEncode(FormatDateTime(rstTrans("TransactionDate")))%></b></p>
<h4>All aliases for this message</h4>
<ul>
<%

Set rstAliases = PerformSelect(SqlGetAliasesFromMessageID(messageid))

While Not rstAliases.EOF
    %><li><%=rstAliases("AliasText")%> (<%
        If rstAliases("AliasEnabled") = True Then
            response.write "Enabled"
        Else
            response.write "Disabled"
        End If
    %>)</li><%
    rstAliases.MoveNext
Wend
%></ul><%

'////////////////////////////////////////////////////////////////////////////////////// STATISTICS
%><img class=line><h3>Statistics</h3><%

%>
Total uses:
<%
Dim nUsage
sql = "SELECT COUNT(*) FROM WinprogMessageUses WHERE MessageUseMessageID=" & (0+messageid)
nUsage = GetFirstValueOfQuery(sql)
%>
<%=nUsage%>
<h4>Top Users</h4>

<%
WhipUpATable "Top Users", "EXECUTE WinprogWeb_GetTopMessageEntities 20, " & CInt(messageid), "Uses"

'////////////////////////////////////////////////////////////////////////////////////// HISTORY
%><img class=line><h3>Change History</h3><%
sql = "EXECUTE WinprogWeb_GetMessageHistory " & (0+messageid)

Set rstHistory = PerformSelect(sql)

While Not rstHistory.EOF
    %><p><b><%=HTMLEncode(UserRstToMask(rstHistory))%></b><br><b>
<%=HTMLEncode(FormatDateTime(rstHistory("TransactionDate")))%></b><br>
<%=HTMLEncode(rstHistory("MessageUpdateToText"))%>
</p>
<%
    rstHistory.MoveNext
Wend

'////////////////////////////////////////////////////////////////////////////////////// HISTORY
%><img class=line><h3>Usage History</h3><%
sql = "EXECUTE WinprogWeb_GetMessageUsageHistory 10, " & (0+messageid)
Set rstHistory = PerformSelect(sql)

%>
<p>
<h4>Last 10 uses</h4>
<table class=stats><tr><th>Date</th><th>User</th><th>Hostmask</th></tr>
<%
While Not rstHistory.EOF
    %><tr><td><%=HTMLEncode(FormatDateTime(rstHistory("TransactionDate")))%></td>
<td><%=HTMLEncode(rstHistory("EntityName"))%></td><td><%=HTMLEncode(rstHistory("UserHostMask"))%></td></tr><%
    rstHistory.MoveNext
Wend
%></table></p><%

'//////////////////////////////////////////////////////////////////////////////////////
'//////////////////////////////////////////////////////////////////////////////////////
WriteFooter

%>
