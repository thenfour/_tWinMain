<!-- #include File="global.asp" -->
<% WriteHeader "Lesson Management"

Const STATE_VIEW = 0
Const STATE_NEW = 1
Const STATE_EDITENTRY = 2
Const STATE_EDITPERFORM = 3
Const STATE_EDITSKILLS = 4

Dim state, sql, rst
state = CInt(GetRequestField("state"))
Dim ID, Name, BookID, Description, Pages, ParentID
Dim state2, SID, Ranking, Root

If state = STATE_VIEW Then
ElseIf state = STATE_NEW Then
    Name = CStr(Request.Form("LName"))
    BookID = CInt(Request.Form("LBookID"))
    ParentID = CInt(Request.Form("LParentID"))
    Description = CStr(Request.Form("LDescription"))
    Pages = CStr(Request.Form("LPages"))
    Root = CheckboxToBool(Request.Form("LRoot"))

    If Len(Name) > 0 Then
        If Root = True Then ParentID = "Null"
        sql = "insert into tLessons (Name, BookID, Description, ParentID, Pages) values ("
        sql = sql & Str2Sql(Name) & ", "
        sql = sql & BookID & ", "
        sql = sql & Str2Sql(Description) & ", "
        sql = sql & ParentID & ", "
        sql = sql & Str2Sql(Pages) & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The lesson has been added.</font></p>"
    End if
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("LID"))
    Set rst = PerformSelect("select ID, Name, BookID, Description, ParentID, Pages from tLessons where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Name"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" ><br/>
            <input type=hidden name=LID value=<%=ID%> ><br/>
            <table>
                <tr><th>Name</th><td><input type=text name=LName value="<%=dbf(rst("Name"))%>"></td></tr>
                <tr><th>Book</th><td><% WriteBookSelector "LBookID", rst("BookID") %></td></tr>
                <tr><th>Description</th><td><input type=text name=LDescription value="<%=dbf(rst("Description"))%>"></td></tr>
                <tr><th>Parent</th><td><% WriteLessonSelector "LParentID", rst("ParentID") %></td></tr>
                <tr><th>Root-Level?</th><td><input type=Checkbox name=LRoot <% if isnull(rst("ParentID")) then response.write("Checked") %> /></td></tr>
                <tr><th>Pages</th><td><input type=text name=LPages value="<%=dbf(rst("Pages"))%>"></td></tr>
                <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
        </form>
        <img class=line>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    ID = CInt(Request.Form("LID"))
    Name = CStr(Request.Form("LName"))
    BookID = CInt(Request.Form("LBookID"))
    Description = CStr(Request.Form("LDescription"))
    Pages = CStr(Request.Form("LPages"))
    ParentID = CInt(Request.Form("LParentID"))
    Root = CheckboxToBool(Request.Form("LRoot"))
    If Root = True Then ParentID = "Null"

    If (Len(Name) > 1) And (LessonCircularReferenceCheck(ID, ParentID) = False) Then
        sql = "update tLessons set "
        sql = sql & "Name = " & Str2Sql(Name)
        sql = sql & ", BookID = " & BookID
        sql = sql & ", Description = " & Str2Sql(Description)
        sql = sql & ", ParentID = " & ParentID
        sql = sql & ", Pages = " & Str2Sql(Pages)
        sql = sql  & " where ID = " & ID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The lesson has been updated.</font></p>"
    Else
        Response.Write "<P><font color=red>The lesson could not be updated - check for circular references or missing information.</font></p>"
    End If
ElseIf state = STATE_EDITSKILLS Then
    ' required vars: LName, LID, (state2 will default to 0)
    ' for add: SID, Ranking
    ' for rem: SID
    Name = CStr(Request.Form("LName"))
    ID = CInt(Request.Form("LID"))

    Const STATE_SKILL_VIEW = 0
    Const STATE_SKILL_ADD = 1
    Const STATE_SKILL_REMOVE = 2
    
    state2 = CInt(GetRequestField("state2"))

    if state2 = STATE_SKILL_ADD then
        SID = CInt(Request.Form("SID"))
        Ranking = CInt(Request.Form("Ranking"))
        sql = "insert into tLessonSkills (SkillID, LessonID, Ranking) values ("
        sql = sql & SID & ", "
        sql = sql & ID & ", "
        sql = sql & Ranking
        sql = sql & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The skill has been added to the lesson.</font></p>"
    elseif state2 = STATE_SKILL_REMOVE then
        SID = CInt(Request.Form("SID"))
        sql = "delete tLessonSkills where SkillID = " & SID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The skill has been removed from the lesson.</font></p>"
    end if

    %><h1>Skills management for <%=dbf(Name)%>.</h1>
    <form name=fskilladd method=post>
        <input type=hidden name=state value=<%=STATE_EDITSKILLS%> >
        <input type=hidden name=state2 value=<%=STATE_SKILL_ADD%> >
        <input type=hidden name=LName value="<%=Name%>">
        <input type=hidden name=LID value=<%=ID%> >
        <table>
            <tr><th>Skill</th><td><% WriteSkillSelector "SID", Empty %></td></tr>
            <tr><th>Ranking</th><td><% WriteRankingSelector "fskilladd", "Ranking", Empty %></td></tr>
            <tr><th colspan=2><input type=submit value="Add Skill"></th></tr>
        </table>
    </form>
    <script>
        <!--
        function PostRemoveSkill(id){document.fskillremove.SID.value = id;  document.fskillremove.submit();}
        //-->
    </script>
    <form name=fskillremove method=post>
        <input type=hidden name=state value=<%=STATE_EDITSKILLS%> >
        <input type=hidden name=state2 value=<%=STATE_SKILL_REMOVE%> >
        <input type=hidden name=LID value=<%=ID%> >
        <input type=hidden name=LName value="<%=Name%>">
        <input type=hidden name=SID value=-1 >
    </form>
    <%
    sql = "select l.Name Lesson, lnk.Ranking Rank, s.Name Skill, l.ID LessonID, s.ID SkillID from tLessons l inner join tLessonSkills lnk on (l.ID = lnk.LessonID) inner join tSkills s on (s.ID = lnk.SkillID) where l.ID = " & ID
    Set rst = PerformSelect(sql)
    %>
    <table>
        <tr><th>&nbsp;</th><th>Skill</th><th>Ranking</th></tr>
        <%
        Do While Not rst.EOF
            %><tr>
                <td><a href="#" onclick="PostRemoveSkill(<%=CInt(rst("SkillID"))%>);">[remove]</a></td>
                <td><%=dbf(rst("Skill"))%></td>
                <td><%=dbf(rst("Rank"))%></td>
            </tr>
            <%
            rst.MoveNext
            if err.number <> 0 then exit do
        Loop
        %></table><img class=line><%
    Set rst = Nothing
End If

sql = ""
sql = sql & "select "
sql = sql & "    l.ID ID, "
sql = sql & "    l.Name Name, "
sql = sql & "    b.Title + '(' + b.Author + ')' Book, "
sql = sql & "    l.Description Description, "
sql = sql & "    l.Pages, "
sql = sql & "    l.IndentString, "
sql = sql & "    p.Name Parent, "
sql = sql & "    p.ID ParentID, "
sql = sql & "    dbo.fn_GetSkillListForLesson(l.ID, ', ') Skills "
sql = sql & "from "
sql = sql & "    dbo.fn_GetLessonTree(Null, '<img src=""blank.gif"">') l left join "
sql = sql & "    tBooks b on b.ID = l.BookID left join "
sql = sql & "    tLessons p on p.ID = l.ParentID "
sql = sql & "order by "
sql = sql & "    SortID asc "

Set rst = PerformSelect(sql)


%>
<h1>Lesson Management</h1>
<p>A lesson is some specific thing to practice.  This is different from a SKILL.
A skill is something about your playing you need to improve.  A lesson is a part of
a book, or an etude - something to help work on a skill.  Indicate how specific the lesson is by using the heirarchy.</p>
<%


%><script>
    <!--
    function Postit(id){document.fedit.LID.value = id;  document.fedit.submit();}
    function PostManageSkills(id, name){document.feditskills.LID.value = id;  document.feditskills.LName.value = name;  document.feditskills.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value="<%=STATE_EDITENTRY%>" >    
    <input type=hidden name=LID value=-1 >    
</form>
<form name=feditskills method=post>
    <input type=hidden name=state value="<%=STATE_EDITSKILLS%>" >    
    <input type=hidden name=LID value=-1 >
    <input type=hidden name=LName value="" >
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Name</th><th>Book</th><th>Pages</th><th>Description</th><th>Parent</th><th>Skills</th></tr><%
Do While Not rst.EOF
    %><tr><td>
        <a href="#" onclick="Postit(<%=CInt(rst("ID"))%>);">[edit]</a>
        <a href="#" onclick="PostManageSkills(<%=CInt(rst("ID"))%>, '<%=Str2JScript(dbf(rst("Name")))%>');">[skills]</a>
        </td>
    <td><%=dbf(rst("ID"))%></td>
    <td><%=rst("IndentString") & dbf(rst("Name"))%></td>
    <td><%=dbf(rst("Book"))%></td>
    <td><%=dbf(rst("Pages"))%></td>
    <td><%=dbf(rst("Description"))%></td>
    <td><%=dbf(rst("Parent"))%></td>
    <td><%=dbf(rst("Skills"))%></td>
    </tr><%
    rst.MoveNext
    if err.number <> 0 then exit do
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.LName"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new lesson</h3>
    <table>
        <tr><th>Name</th><td><input type=text name=LName value="" ></td></tr>
        <tr><th>Book</th><td><% WriteBookSelector "LBookID", Empty %></td></tr>
        <tr><th>Pages</th><td><input type=text name=LPages value="" ></td></tr>
        <tr><th>Parent</th><td><% WriteLessonSelector "LParentID", Empty %></td></tr>
        <tr><th>Root-Level?</th><td><input type=Checkbox name=LRoot Checked /></td></tr>
        <tr><th>Description</th><td><input type=text name=LDescription value="" ></td></tr>
        <tr><th colspan=2><input type=submit value="New lesson" ></th></tr>
    </table>
</form>

<% WriteFooter %>