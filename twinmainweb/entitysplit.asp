<!-- #include File = "global.asp"-->
<%
'   QUERYSTRINGS: q = user query (to pass back to manageentities.asp)
'                 p = the passcode
'               : e = the entity ID to split up
'

'On Error Resume Next

HandleAuthentication Request.QueryString("p")

WriteHeader "_tWinMain Web"

'4) Let's do a Split
'
'    Split up tenfour
'    Select the users that should not be part of this entity
'     X  CrankHank!cranker@scubagear.com
'        tenfour!carl@ript.net
'        tenfour!DrDrew@evrtwa1.verizon-dsl.net
'        tenfear!DrDrew@evrtwa1.verizon-dsl.net
'        tenphool!DrDrew@evrtwa1.verizon-dsl.net
Dim sql
Dim rstUsers
Dim rstEntity
Dim nEntity' entity id
Dim sEntity' entity name
Dim sMask

nEntity = (0+Request.QueryString("e"))

sql = "SELECT TOP 1 EntityName FROM WinprogEntities WHERE EntityID=" & nEntity
sEntity = GetFirstValueOfQuery(sql)

sql = "SELECT * FROM WinprogUsers WHERE UserEntityID=" & nEntity
Set rstUsers = PerformSelect(sql)


%><h3>Choose the hosts to remove from entity "<%=HTMLEncode(sEntity)%>"</h3>
<form name=f action="entitysplit_chooseentity.asp" method=post>
<input type=hidden name=p value=<%=Server.URLEncode(Request.QueryString("p"))%>>
<input type=hidden name=q value=<%=Server.URLEncode(Request.QueryString("q"))%>>
<input type=hidden name=eq value=<%=Server.URLEncode(Request.QueryString("q"))%>>
<input type=hidden name=e value=<%=Server.URLEncode(Request.QueryString("e"))%>>
<table><tr><td colspan=2><input type=submit value="Next »"></td></tr><%

While Not rstUsers.EOF
    sMask = UserRstToMask(rstUsers)
    %><tr><td><input type=checkbox name="user_<%=HTMLEncode(rstUsers("UserID"))%>"></td><td><%=sMask%></td></tr><%
    rstUsers.MoveNext
Wend
%></table></form><%

WriteFooter
%>



