<!-- #include File = "global.asp"-->
<%
' FORM ITEMS
'   q = query to pass around
'   p = password
'   csv = comma separated values list of users
'   NewEntityID = the entity to assign them to
 

'On Error Resume Next

HandleAuthentication Request.Form("p")

WriteHeader "_tWinMain Web"

' Confirm that you want to move these hosts:
'
'    tenfour!DrDrew@evtaglkjaglkrjgrg
'    tenfear!DrDrew@evtaglkjaglkrjgrg
'    tenphool!DrDrew@evtaglkjaglkrjgrg
'
'
' To this entity:
'
'    tenfour
'      otherstuff!DrDrew@winprog.org
'
' [Next>>] [Cancel]

'WriteFormData

%><h3>Confirmation</h3><p><b>Confirm that you want to move these hosts</b></p>
<ul><%

Dim rstUsers
'SELECT * FROM WinprogUsers WHERE UserID IN (x,x,x,x)
Set rstUsers = PerformSelect("SELECT * FROM WinprogUsers WHERE UserID IN " & Request.Form("csv"))

While Not rstUsers.EOF
    %><li><%=HTMLEncode(UserRstToMask(rstUsers))%></li><%
    rstUsers.MoveNext
Wend

%></ul><p><b>To this entity</b></p><%

Dim sEntityName
'SELECT EntityName FROM WinprogEntities WHERE EntityID=eid
Dim NewEntityID
NewEntityID = 0+Request.Form("NewEntityID")

sEntityName = GetFirstValueOfQuery("SELECT EntityName FROM WinprogEntities WHERE EntityID=" & NewEntityID)

%><p><ul><li><%=HTMLEncode(sEntityName)%></li></ul></p><%

%>
<form name=f action="entitysplit_2.asp" method=post>
<input type=hidden name=q value="<%=HTMLEncode(Request.Form("q"))%>" ID="Hidden1">
<input type=hidden name=p value="<%=HTMLEncode(Request.Form("p"))%>" ID="Hidden2">
<input type=hidden name=csv value="<%=HTMLEncode(Request.Form("csv"))%>" ID="Hidden3">
<input type=hidden name=NewEntityID value="<%=HTMLEncode(Request.Form("NewEntityID"))%>" ID="Hidden4">
<input type=submit value="Finish">
</form>
<%

WriteFooter
%>



