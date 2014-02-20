<!-- #include File = "global.asp"-->
<%
'   QUERYSTRINGS: q = user query (to pass back to manageentities.asp later)
'                 p = the passcode

'On Error Resume Next

HandleAuthentication Request.QueryString("p")

WriteHeader "_tWinMain Web"

Sub BeginTable(sCSV)
    %><form name=f2 action="entitylink_2.asp" method=post ID="Form1">
    <input type=hidden name=q value="<%=HTMLEncode(Request.QueryString("q"))%>">
    <input type=hidden name=p value="<%=HTMLEncode(Request.QueryString("p"))%>">
    <input type=hidden name=CSV value="<%=HTMLEncode(sCSV)%>" ID="Hidden1">
    <p>Select the primary entity.  The other entities will be considered duplicates and joined up to the primary one.</p><input type=submit value="Link" ID="Submit1" NAME="Submit1"><table ID="Table1"><%
End Sub

Sub EndTable()
    %></table></form><%
End Sub

Sub NewEntityGroup(neID, seName)
    %><tr><td colspan=2><input type=radio name="PrimaryEntity" value=<%=neID%> ID="Checkbox1">&nbsp;<%=HTMLEncode(seName)%></td></tr><%
End Sub

Sub WriteUser(sMask)
    %><tr><td width=50px>&nbsp;</td><td colspan=1><%=HTMLEncode(sMask)%></td></tr><%
End Sub

Dim eID
Dim sCSV' comma separated values list of entity IDs

' Build a string of entities
sCSV = ""
Dim f
Dim nCount
For Each f In Request.Form
    nCount = nCount + 1
    sCSV = sCSV & ", " & CInt(f)
Next
If nCount < 2 Then
    %>You must select at least two items to link.<%
Else
    sCSV = "(" & Mid(sCSV, 3) & ")"

    Dim sql
    sql = "SELECT EntityName, UserNick, UserIdent, UserHost, EntityID, UserID "
    sql = sql & "FROM WinprogUsers INNER JOIN WinprogEntities  ON UserEntityID=EntityId "
    sql = sql & "WHERE EntityID IN " & sCSV
    sql = sql & " ORDER BY EntityName ASC"

    Dim rst
    Set rst = PerformSelect(sql)

    BeginTable sCSV
    Dim sCurrentEntityName' start with a blank string
    Dim sThisEntityName
    While Not rst.EOF
            sThisEntityName = rst("EntityName")
            If sThisEntityName <> sCurrentEntityName Then
                ' start a new entity group
                NewEntityGroup rst("EntityID"), sThisEntityName
                sCurrentEntityName = sThisEntityName
            End If

            WriteUser UserRstToMask(rst)
            
        rst.MoveNext
    Wend
    EndTable
End If
WriteFooter
%>



