<!-- #include File="global.asp" -->
<% WriteHeader "Ranking Management"

Const STATE_VIEW = 0
Const STATE_NEW = 1
Const STATE_EDITENTRY = 2
Const STATE_EDITPERFORM = 3
Const STATE_DELETE = 4

Dim state, sql, rst
state = CInt(GetRequestField("state"))
Dim Name, ID, Low, High, DefaultVal

If state = STATE_VIEW Then
    '
ElseIf state = STATE_NEW Then
    Name = CStr(Request.Form("RName"))
    Low = CInt(Request.Form("RLow"))
    High = CInt(Request.Form("RHigh"))
    DefaultVal = CInt(Request.Form("RDefaultVal"))
    If Len(Name) > 1 Then
        sql = "insert into tRankings (Name, High, Low, DefaultVal) values (" & Str2Sql(Name) & ", " & High  & ", " & Low & ", " & DefaultVal & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The ranking has been added.</font></p>"
    End If
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("RID"))
    Set rst = PerformSelect("select Name, Low, High, DefaultVal, ID from tRankings where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Name"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" >    
            <input type=hidden name=RID value=<%=ID%> >    
            <table>
                <tr><th>Name</th><td><input type=text name=RName value="<%=dbf(rst("Name"))%>"></td></tr>
                <tr><th>Low range</th><td><input type=text name=RLow value="<%=dbf(rst("Low"))%>"></td></tr>
                <tr><th>High range</th><td><input type=text name=RHigh value="<%=dbf(rst("High"))%>"></td></tr>
                <tr><th>Default</th><td><input type=text name=RDefaultVal value="<%=dbf(rst("DefaultVal"))%>"></td></tr>
                <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
        </form>
        <img class=line>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    Name = CStr(Request.Form("RName"))
    ID = CInt(Request.Form("RID"))
    Low = CInt(Request.Form("RLow"))
    High = CInt(Request.Form("RHigh"))
    DefaultVal = CInt(Request.Form("RDefaultVal"))
    If (Len(Name) > 1) Then
        sql = "update tRankings set Name = " & Str2Sql(Name) & ", Low = " & Low & ", High = " & High & ", DefaultVal = " & DefaultVal & " where ID = " & ID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The ranking has been updated.</font></p>"
    Else
        Response.Write "<P><font color=red>The ranking could not be updated - check for missing information.</font></p>"
    End If
ElseIf state = STATE_DELETE Then
    ID = CInt(Request.Form("RID"))
    ExecuteSQL "delete from tRankings where ID = " & ID
    Response.Write "<P><font color=green>Deletion complete.</font></p>"
End If

Set rst = PerformSelect("SELECT Name, ID, Low, High, DefaultVal from tRankings order by DefaultVal")

%>
<h1>Ranking Management</h1>
<p>A rank is used to rate the concentration of a skill with a lesson.
A long-tones exercise, for instance, might rank very highly in the "Tone" skill, and very weak in
the "Technique" skill.
<p>
A ranking is really just a name given to a range of values.
<p>
<%


%><script>
    <!--
    function Postit(id, state){document.fedit.RID.value = id; document.fedit.state.value = state; document.fedit.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value=-1 >    
    <input type=hidden name=RID value=-1 >    
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Name</th><th>Low</th><th>High</th><th>Default Val</th></tr><%
Do While Not rst.EOF
    %><tr>
        <td>
            <a href="#" onclick="Postit(<%=CInt(rst("ID"))%>, <%=STATE_EDITENTRY%>);">[edit]</a>
            <a href="#" onclick="Postit(<%=CInt(rst("ID"))%>, <%=STATE_DELETE%>);">[delete]</a>
        </td>
        <td><%=dbf(rst("ID"))%></td>
        <td><%=dbf(rst("Name"))%></td>
        <td><%=dbf(rst("Low"))%></td>
        <td><%=dbf(rst("High"))%></td>
        <td><%=dbf(rst("DefaultVal"))%></td>
    </tr><%
    rst.MoveNext
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.RName"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new ranking</h3>
    <table>
        <tr><th>Name</th><td><input type=text name=RName value="" ></td></tr>
        <tr><th>Low</th><td><input type=text name=RLow value="" ></td></tr>
        <tr><th>High</th><td><input type=text name=RHigh value="" ></td></tr>
        <tr><th>DefaultVal</th><td><input type=text name=RDefaultVal value="" ></td></tr>
        <tr><th colspan=2><input type=submit value="New ranking" ></tr>
    </table>
</form>

<% WriteFooter %>