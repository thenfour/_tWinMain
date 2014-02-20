<!-- #include File = "global.asp"-->
<%
'   FORM ITEMS
'       p = the passcode
'       q = the query to pass back to manageentities
'       CSV = the comma separated list of entities (for use in the UPDATE statement)
'       [#] = the primary entity


'On Error Resume Next

WriteHeader "_tWinMain Web"

HandleAuthentication Request.Form("p")

Dim sql
Dim sCSV
Dim sPrimary

sCSV = Request.Form("CSV")
sPrimary = Request.Form("PrimaryEntity")

' UPDATE WinprogUsers SET UserEntityID=@primaryid WHERE UserEntityID IN @List
sql = "UPDATE WinprogUsers SET UserEntityID=" & (0+sPrimary) & " WHERE UserEntityID IN " & sCSV
PerformSelect sql

Response.Redirect("manageentities.asp?q=" & Request.Form("q") & "&p="&Request.Form("p"))

WriteFooter
%>



