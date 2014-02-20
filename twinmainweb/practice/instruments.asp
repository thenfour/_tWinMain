<!-- #include File="global.asp" -->
<% WriteHeader "Instrument Management"

Const STATE_VIEW = 0
Const STATE_NEW = 1
Const STATE_EDITENTRY = 2
Const STATE_EDITPERFORM = 3

Dim state, sql, rst
state = CInt(GetRequestField("state"))
Dim Name, ID, ParentID, Root

If state = STATE_VIEW Then
    '
ElseIf state = STATE_NEW Then
    Name = CStr(Request.Form("IName"))
    ParentID = CInt(Request.Form("IParentID"))
    Root = CheckboxToBool(Request.Form("IRoot"))
    If Root = True Then ParentID = "Null"

    If Len(Name) > 1 Then
        sql = "insert into tInstruments (Name, ParentID) values (" & Str2Sql(Name) & ", " & ParentID & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The instrument has been added.</font></p>"
    End If
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("IID"))
    Set rst = PerformSelect("select Name, ParentID, ID from tInstruments where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Name"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" >    
            <input type=hidden name=IID value=<%=ID%> >    
            <table>
                <tr><th>Name</th><td><input type=text name=IName value="<%=dbf(rst("Name"))%>"></td></tr>
                <tr><th>Parent</th><td><% WriteInstrumentSelector "IParentID", rst("ParentID") %></td></tr>
                <tr><th>Root-Level?</th><td><input type=Checkbox name=IRoot <% if isnull(rst("ParentID")) then response.write("Checked") %> /></td></tr>
                <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
            <img class=line>
        </form>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    Name = CStr(Request.Form("IName"))
    ParentID = CInt(Request.Form("IParentID"))
    ID = CInt(Request.Form("IID"))
    Root = CheckboxToBool(Request.Form("IRoot"))
    If Root = True Then ParentID = "Null"

    If (Len(Name) > 1) and (InstrumentCircularReferenceCheck(ID, ParentID) = False) Then
        sql = "update tInstruments set Name = " & Str2Sql(Name) & ", ParentID = " & ParentID & " where ID = " & ID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The instrument has been updated.</font></p>"
    else
        Response.Write "<P><font color=red>The instrument could not be updated - check for circular references or missing information.</font></p>"
    End If
End If

Set rst = PerformSelect("select i.Name, i.ID, IndentString, p.Name Parent from dbo.fn_GetInstrumentTree(Null, '<img src=blank.gif>') i left join tInstruments p on p.ID = i.ParentID order by SortID asc")

%>
<h1>Instrument Management</h1>
<p>Manage the instruments used on the website.</p>
<%

%><script>
    <!--
    function Postit(id){document.fedit.iid.value = id;  document.fedit.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value="<%=STATE_EDITENTRY%>" >    
    <input type=hidden name=iid value=-1 >    
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Name</th><th>Parent</th></tr><%
Do While Not rst.EOF
    %><tr>
        <td><a href="#" onclick="Postit(<%=CInt(rst("ID"))%>);">[edit]</a></td>
        <td><%=dbf(rst("ID"))%></td>
        <td><%=rst("IndentString") & dbf(rst("Name"))%></td>
        <td><%=dbf(rst("Parent"))%></td>
        </tr><%
    rst.MoveNext
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.IName"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new instrument</h3>
    <table>
        <tr><th>Name</th><td><input type=text name=IName value="" ></td></tr>
        <tr><th>Parent</th><td><% WriteInstrumentSelector "IParentID", Empty %></td></tr>
        <tr><th>Root-Level?</th><td><input type=Checkbox name=IRoot Checked /></td></tr>
        <tr><th colspan=2><input type=submit value="New instrument" ></th></tr>
    </table>
</form>

<% WriteFooter %>