<!-- #include File="global.asp" -->
<% WriteHeader "Skill Management"

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
    Name = CStr(Request.Form("SName"))
    ParentID = CStr(Request.Form("SParentID"))
    Root = CheckboxToBool(Request.Form("SRoot"))
    If Len(Name) > 1 Then
        If Root = True Then
            sql = "insert into tSkills (Name, ParentID) values (" & Str2Sql(Name) & ", Null)"
        Else
            sql = "insert into tSkills (Name, ParentID) values (" & Str2Sql(Name) & ", " & ParentID & ")"
        End If
        ExecuteSQL sql
        Response.Write "<P><font color=green>The skill has been added.</font></p>"
    End If
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("SID"))
    Set rst = PerformSelect("select Name, ParentID, ID from tSkills where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Name"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" >    
            <input type=hidden name=SID value=<%=ID%> >
            <table>
                <tr><th>Name</th><td><input type=text name=SName value="<%=dbf(rst("Name"))%>"></td></tr>
                <tr><th>Parent</th><td><% WriteSkillSelector "SParentID", rst("ParentID") %></td></tr>
                <tr><th>Root-Level?</th><td><input type=Checkbox name=SRoot <% if isnull(rst("ParentID")) then response.write("Checked") %> /></td></tr>
                <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
        </form>
        <img class=line>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    Name = CStr(Request.Form("SName"))
    ID = CInt(Request.Form("SID"))
    ParentID = CInt(Request.Form("SParentID"))
    Root = CheckboxToBool(Request.Form("SRoot"))
    If (Len(Name) > 1) And (SkillCircularReferenceCheck(ID, ParentID) = False) Then
        If Root = True Then
            sql = "update tSkills set Name = " & Str2Sql(Name) & ", ParentID = Null where ID = " & ID
        Else
            sql = "update tSkills set Name = " & Str2Sql(Name) & ", ParentID = " & ParentID & " where ID = " & ID
        End If
        ExecuteSQL sql
        Response.Write "<P><font color=green>The skill has been updated.</font></p>"
    Else
        Response.Write "<P><font color=red>The skill could not be updated - check for circular references or missing information.</font></p>"
    End If
End If

Set rst = PerformSelect("SELECT e.ID, e.Name, e.IndentString, s.Name Parent from dbo.fn_GetSkillTree(Null, '<img src=""blank.gif"">') e left join tSkills s on e.ParentID = s.ID order by SortID")

%>
<h1>Skill Management</h1>
<p>A skill is some aspect of playing an instrument that requires practice, like articulation.
Skills are stored in a hierarchial format so you can drill down as specific as you need.</p>
<%

%><script>
    <!--
    function Postit(id){document.fedit.SID.value = id;  document.fedit.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value="<%=STATE_EDITENTRY%>" >    
    <input type=hidden name=SID value=-1 >    
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Name</th><th>Parent</th></tr><%
Do While Not rst.EOF
    %><tr><td><a href="#" onclick="Postit(<%=CInt(rst("ID"))%>);">[edit]</a></td><td><%=dbf(rst("ID"))%></td><td><%=rst("IndentString") & dbf(rst("Name"))%></td><td><%=dbf(rst("Parent"))%></td></tr><%
    rst.MoveNext
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.SName"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new skill</h3>
    <table>
        <tr><th>Name</th><td><input type=text name=SName value="" ></td></tr>
        <tr><th>Parent</th><td><% WriteSkillSelector "SParentID", Empty %></td></tr>
        <tr><th>Root-Level?</th><td><input type=Checkbox name=SRoot /></td></tr>
        <tr><th colspan=2><input type=submit value="New skill" ></tr>
    </table>
</form>

<% WriteFooter %>