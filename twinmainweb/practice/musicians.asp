<!-- #include File="global.asp" -->
<% WriteHeader "Musician Management"

Const STATE_VIEW = 0
Const STATE_NEW = 1
Const STATE_EDITENTRY = 2
Const STATE_EDITPERFORM = 3

Dim state, sql, rst
state = CInt(GetRequestField("state"))
Dim Name, ID

If state = STATE_VIEW Then
    '
ElseIf state = STATE_NEW Then
    Name = CStr(Request.Form("MName"))
    If Len(Name) > 1 Then
        sql = "insert into tMusicians (Name) values (" & Str2Sql(Name) & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The musician has been added.</font></p>"
    End If
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("MID"))
    Set rst = PerformSelect("select Name, ID from tMusicians where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Name"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" >    
            <input type=hidden name=MID value=<%=ID%> >    
            <table>
                <tr><th>Name</th><td><input type=text name=MName value="<%=dbf(rst("Name"))%>"></td></tr>
                <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
        </form>
        <img class=line>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    Name = CStr(Request.Form("MName"))
    ID = CInt(Request.Form("MID"))
    If Len(Name) > 1 Then
        sql = "update tMusicians set Name = " & Str2Sql(Name) & " where ID = " & ID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The musician has been updated.</font></p>"
    End If
End If

Set rst = PerformSelect("select Name, ID from tMusicians order by Name asc")

%>
<h1>Musician Management</h1>
<p>Manage the musicians on the website.</p>
<%

%><script>
    <!--
    function Postit(id){document.fedit.MID.value = id;  document.fedit.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value="<%=STATE_EDITENTRY%>" >    
    <input type=hidden name=MID value=-1 >    
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Name</th></tr><%
Do While Not rst.EOF
    %><tr><td><a href="#" onclick="Postit(<%=CInt(rst("ID"))%>);">[edit]</a></td><td><%=dbf(rst("ID"))%></td><td><%=dbf(rst("Name"))%></td></tr><%
    rst.MoveNext
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.MName"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new lesson</h3>
    <table>
        <tr><th>Name</th><td><input type=text name=MName value="" ></td></tr>
        <tr><th colspan=2><input type=submit value="New musician" ></th></tr>
    </table>
</form>

<% WriteFooter %>