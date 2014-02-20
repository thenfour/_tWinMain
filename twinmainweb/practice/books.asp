<!-- #include File="global.asp" -->
<% WriteHeader "Book Management"

Const STATE_VIEW = 0
Const STATE_NEW = 1
Const STATE_EDITENTRY = 2
Const STATE_EDITPERFORM = 3

Dim state, sql, rst
state = CInt(GetRequestField("state"))
Dim Title, Author, ISBN, Description, ID

If state = STATE_VIEW Then
    '
ElseIf state = STATE_NEW Then
    Title = CStr(Request.Form("BTitle"))
    Author = CStr(Request.Form("BAuthor"))
    ISBN = CStr(Request.Form("BISBN"))
    Description = CStr(Request.Form("BDescription"))
    
    If Len(Title) > 1 Then
        sql = "insert into tBooks (Title, Author, ISBN, Description) values ("
        sql = sql & Str2Sql(Title) & ", "
        sql = sql & Str2Sql(Author) & ", "
        sql = sql & Str2Sql(ISBN) & ", "
        sql = sql & Str2Sql(Description) & ")"
        ExecuteSQL sql
        Response.Write "<P><font color=green>The book has been added.</font></p>"
    End If
ElseIf state = STATE_EDITENTRY Then
    ID = CInt(Request.Form("BID"))
    Set rst = PerformSelect("select Title, Author, ISBN, Description, ID from tBooks where ID = " & ID)
    if not rst.EOF then
        %><h1>Edit <%=dbf(rst("Title"))%></h1><form name=fEditPerform method=post>
            <input type=hidden name=state value="<%=STATE_EDITPERFORM%>" ><br/>
            <input type=hidden name=BID value=<%=ID%> ><br/>
            <table>
            <tr><th>Title</th><td><input type=text name=BTitle value="<%=dbf(rst("Title"))%>"></td></tr>
            <tr><th>Author</th><td><input type=text name=BAuthor value="<%=dbf(rst("Author"))%>"></td></tr>
            <tr><th>ISBN</th><td><input type=text name=BISBN value="<%=dbf(rst("ISBN"))%>"></td></tr>
            <tr><th>Description</th><td><input type=text name=BDescription value="<%=dbf(rst("Description"))%>"></td></tr>
            <tr><th colspan=2><input type=submit value="Make The Change" ></th></tr>
            </table>
            <img class=line>
        </form>
        <%
    end if
    rst.Close
    Set rst = Nothing
ElseIf state = STATE_EDITPERFORM Then
    ID = CInt(Request.Form("BID"))
    Title = CStr(Request.Form("BTitle"))
    Author = CStr(Request.Form("BAuthor"))
    ISBN = CStr(Request.Form("BISBN"))
    Description = CStr(Request.Form("BDescription"))
    If Len(Title) > 1 Then
        sql = "update tBooks set "
        sql = sql & "Title = " & Str2Sql(Title)
        sql = sql & ", Author = " & Str2Sql(Author)
        sql = sql & ", ISBN = " & Str2Sql(ISBN)
        sql = sql & ", Description = " & Str2Sql(Description)
        sql = sql  & " where ID = " & ID
        ExecuteSQL sql
        Response.Write "<P><font color=green>The book has been updated.</font></p>"
    End If
End If

Set rst = PerformSelect("select ID, Title, Author, ISBN, Description from tBooks order by Title asc")

%>
<h1>Book Management</h1>
<p>Manage the books on the website.</p>
<%

%><script>
    <!--
    function Postit(id){document.fedit.BID.value = id;  document.fedit.submit();}
    //-->
</script>
<form name=fedit method=post>
    <input type=hidden name=state value="<%=STATE_EDITENTRY%>" >    
    <input type=hidden name=BID value=-1 >    
</form>
<table><tr><th>&nbsp;</th><th>ID</th><th>Title</th><th>Author</th><th>ISBN</th><th>Description</th></tr><%
Do While Not rst.EOF
    %><tr><td><a href="#" onclick="Postit(<%=CInt(rst("ID"))%>);">[edit]</a></td>
    <td><%=dbf(rst("ID"))%></td>
    <td><%=dbf(rst("Title"))%></td>
    <td><%=dbf(rst("Author"))%></td>
    <td><%=dbf(rst("ISBN"))%></td>
    <td><%=dbf(rst("Description"))%></td>
    </tr><%
    rst.MoveNext
Loop
%></table><%

rst.Close
Set rst = Nothing

RegisterFocusItem "f.BTitle"
%>
<form name=f method=post>
    <input type=hidden name=state value="<%=STATE_NEW%>" >    
    <h3>Add a new lesson</h3>
    <table>
        <tr><th>Title</th><td><input type=text name=BTitle value="" ></td></tr>
        <tr><th>Author</th><td><input type=text name=BAuthor value="" ></td></tr>
        <tr><th>ISBN</th><td><input type=text name=BISBN value="" ></td></tr>
        <tr><th>Description</th><td><input type=text name=BDescription value="" ></td></tr>
        <tr><th colspan=2><input type=submit value="New book" ></th></tr>
    </table>
</form>

<% WriteFooter %>