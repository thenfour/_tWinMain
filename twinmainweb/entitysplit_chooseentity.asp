<!-- #include File = "global.asp"-->
<%
'   FORM ITEMS
'       q = user query (to pass back to manageentities.asp)
'       eq = entity query (to use in THIS search)
'       p = the passcode
'       e = the entity ID to split up
'       csv = the comma separated list of users.
'       user_[#] = multiple user IDs to reassign
'
' EITHER csv or the user_ form data will be present.

'On Error Resume Next

HandleAuthentication Request.Form("p")

RegisterFocusItem "f.eq"

WriteHeader "_tWinMain Web"

'WriteFormData

' Build a string of entities
Dim sCSV' comma separated values list of user IDs
sCSV = Request.Form("csv")
If Len(""&sCSV) = 0 Then
    Dim f
    Dim nCount
    For Each f In Request.Form
        If Len(f) > 4 Then
            If Left(f, 5) = "user_" Then
                nCount = nCount + 1
                sCSV = sCSV & ", " & Mid(f, 6)
            End If
        End If
    Next
    sCSV = "(" & Mid(sCSV, 3) & ")"
End If


'Response.Write sCSV

'
'    Select an entity to assign these hosts:
'    [ ]    tenfour
'    [ ]    tenfear
'    [ ]    tenphool
'    [ ]    theForger
'    [ ]    [ new entity ]___CrankHank_______
'    
'    [ NEXT -> ]

Dim rstUsers
Set rstUsers = PerformSelect("SELECT * FROM WinprogUsers WHERE UserID IN " & sCSV)

%><p><b>These hosts will be transferred</b></p><ul><%

While Not rstUsers.EOF
    %><li><%=UserRstToMask(rstUsers)%></li><%
    rstUsers.MoveNext
Wend

%>
</ul>
<p><h3>Search for an entity to assign these hosts</h3></p>
<form name=f action="entitysplit_chooseentity.asp" method=post>
<input type=hidden name=p value=<%=HTMLEncode(Request.Form("p"))%>>
<input type=hidden name=e value=<%=HTMLEncode(Request.Form("e"))%>>
<input type=hidden name=q value=<%=HTMLEncode(Request.Form("q"))%> ID="Hidden1">
<input type=hidden name=csv value="<%=HTMLEncode(sCSV)%>" ID="Hidden3">
<input type=text name=eq value=<%=HTMLEncode(Request.Form("eq"))%>>
<input type=submit name=s value="Search">
</form>

<%

Dim sQuery
sQuery = "%" & Request.Form("eq") & "%"

Sub BeginTable()
    %><form name=f2 action="entitysplit_confirm.asp" method=post ID="Form1">
    <input type=submit value="Next »" NAME="Submit1"><table ID="Table1">
    <input type=hidden name=q value=<%=HTMLEncode(Request.Form("q"))%>>
    <input type=hidden name=p value=<%=HTMLEncode(Request.Form("p"))%>>
    <input type=hidden name=csv value="<%=HTMLEncode(sCSV)%>"><%
End Sub

Sub EndTable()
    %></table></form><%
End Sub

Sub NewEntityGroup(neID, seName)
    %><tr><td colspan=2><input type=radio name=NewEntityID value=<%=HTMLEncode(neID)%>>&nbsp;<%=HTMLEncode(seName)%></td></tr><%
End Sub

Sub WriteUser(sMask)
    %><tr><td width=50px>&nbsp;</td><td colspan=1><%=HTMLEncode(sMask)%></td></tr><%
End Sub

If Len(Request.Form("eq")) > 0 Then
    Dim rst
    Set rst = PerformSelect("EXECUTE WinprogQueryEntities " & Str2Sql(sQuery))
    Dim sCurrentEntityName' start with a blank string
    Dim sThisEntityName
    %><h3>Select the entity to assign these hosts to.</h3><%
    BeginTable
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

%><h3>Create New Entity</h3>
<form name=f3 action="entitysplit_new.asp" method=post>
<input type=hidden name=p value="<%=HTMLEncode(Request.Form("p"))%>" ID="Hidden2">
<input type=hidden name=csv value="<%=HTMLEncode(sCSV)%>" ID="Hidden4">
<input type=hidden name=q value="<%=HTMLEncode(Request.Form("q"))%>" ID="Hidden6">
New entity nickname <input type=text name=ne ID="Hidden5">
<input type=submit name=s value="Create" ID="Submit1">
</form>
<%


WriteFooter
%>



