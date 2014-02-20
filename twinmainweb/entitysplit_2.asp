<!-- #include File = "global.asp"-->
<%
' FORM ITEMS
'   q = query to pass around
'   p = password
'   csv = comma separated values list of users
'   NewEntityID = the entity to assign them to

HandleAuthentication Request.Form("p")

WriteHeader "_tWinMain Web"

'WriteFormData
' UPDATE WinprogUsers SET UserEntityID=NewEntityID WHERE UserID IN csv
Dim sql
Dim csv
Dim NewEntityID

csv = Request.Form("csv")
NewEntityID = Request.Form("NewEntityID")

sql = "UPDATE WinprogUsers SET UserEntityID=" & (0+NewEntityID) & " WHERE UserID IN " & csv
Application("cnn").Execute sql
'Response.Write sql

%>
<h3>Success.</h3>
<form name=f action="manageentities.asp" method=get>
<input type=hidden name=q value="<%=HTMLEncode(Request.Form("q"))%>" ID="Hidden1">
<input type=hidden name=p value="<%=HTMLEncode(Request.Form("p"))%>" ID="Hidden2">
<input type=submit value="Return">
</form>
<%

WriteFooter
%>



