<!-- #include File="global.asp" -->
<% WriteHeader "Session management"

Const STATE_NEW = 0                 ' *
Const STATE_NEW2 = 1                ' * used when session_init has given us musician and instrument info so we can actually begin.
Const STATE_COMMIT = 2              ' *
Const STATE_CANCEL = 3              ' *
Const STATE_ADDEXISTINGLESSON = 4   ' *
Const STATE_ADDNEWLESSON = 5        ' *
Const STATE_REMOVELESSON = 6        ' *
Const STATE_ADDEXISTINGSKILL = 7    '
Const STATE_ADDNEWSKILL = 8         '
Const STATE_REMOVESKILL = 9         '
Const STATE_APPLY = 10              ' *

Const VAR_OpenSession = "session_open"
Const VAR_SessionID = "session_id"

' adding a skill is session-level.  So we persist the SessionID through IIS session variables.
' that way if the user cancels out the bad-bear way, there wont be stale records left on the
' db.  We have to do it this way for a couple reasons:
' 1) When the user returns to the page after they accidentally navigated away, the info will
'    still be there.
' 2) Since I have multiple form submissions before the record is committed, i need a way to
'    persist all this data across page refreshes.  Yes i could just keep it all in form variables,
'    but its really dirty to keep lists of things in form vars.  So I have to persist it to the db.

Dim state
state = CInt(GetRequestField("state"))

Dim sql, rst
Dim SessionID
Dim MusicianID, InstrumentID, EndTime, BeginTime, SDate, OverallRanking, Description
Dim SL_Description, SL_BeginTime, SL_EndTime, EL_LessonID
Dim slid, LessonID
Dim NL_BookID, NL_Pages, NL_Name, NL_Description, NL_ParentID, NL_Root

If state = STATE_NEW Then
    ' here we need to fill rst with a valid session record.
    '
    ' check to see if we have an "open" session.
    If IsEmpty(Session(VAR_OpenSession)) = False and Session(VAR_OpenSession) = True Then
        ' extract the session record
        SessionID = Session(VAR_SessionID)
        Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & SessionID)
        ShowDataEntryForm(rst)
    Else
        ' create a new default session record with Enabled = 0.
        ' to do that we need to get the musician and instrument first.
        RegisterFocusItem "f.MusicianID"
        %>
        <form method=post name=f>
            <input type=hidden name=state value=<%=STATE_NEW2%> >
            <h1>Practice session entry</h1>
            <table>
                <tr><th>Musician</th><td><% WriteMusicianSelector "MusicianID", Empty %></td></tr>
                <tr><th>Instrument</th><td><% WriteInstrumentSelector "InstrumentID", Empty %></td></tr>
                <tr><th colspan=2><input type=submit value="Continue"/></th></tr>
            </table>
        </form>
        <%
    End If
ElseIf state = STATE_NEW2 Then
    MusicianID = CLng(GetRequestField("MusicianID"))
    InstrumentID = CLng(GetRequestField("InstrumentID"))
    sql = "exec dbo.pNewDefaultSession @musicianid = " & MusicianID & ", @instrumentid = " & InstrumentID
    SessionID = GetFirstValueOfQuery(sql)
    Session(VAR_OpenSession) = True
    Session(VAR_SessionID) = SessionID
    Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & SessionID)
    ShowDataEntryForm(rst)
ElseIf state = STATE_CANCEL Then
    ' abandon the 'temp' session
    If Session(VAR_OpenSession) = True Then
        ExecuteSQL "delete tSessionLessonSkills where SessionLessonID in (select ID from tSessionLessons where SessionID = " & Session(VAR_SessionID) & ")"
        ExecuteSQL "delete tSessionLessons where SessionID = " & Session(VAR_SessionID)
        ExecuteSQL "delete tSessions where ID = " & Session(VAR_SessionID)
        Session(VAR_OpenSession) = False
        Session(VAR_SessionID) = Empty
        %><p><font color=green>The practice session has been canceled.</font></p><%
    End If
ElseIf state = STATE_APPLY Then
    ' update our temp session to the new values.  why do i allow this?  just for adding that final bit of detail in the UI to make it easy to use
    If Session(VAR_OpenSession) = True Then
        ApplyFormChanges 0
        Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & SessionID)
        ShowDataEntryForm(rst)
    End If
ElseIf state = STATE_COMMIT Then
    ' update the temp session to the new values, and enabled = 1
    If Session(VAR_OpenSession) = True Then
        ApplyFormChanges 1
        %>Successfully saved.<%
    End If
ElseIf state = STATE_ADDEXISTINGLESSON Then
    If Session(VAR_OpenSession) = True Then
        ' apply changes to other fields
        ApplyFormChanges 0

        ' now add the new record
        SL_Description = CStr(Request.Form("SL_Description"))
        SL_BeginTime = CDate(Request.Form("SL_BeginTime"))
        SL_EndTime = CDate(Request.Form("SL_EndTime"))
        EL_LessonID = CInt(Request.Form("EL_LessonID"))
        
        sql = "insert tSessionLessons (Description, BeginTime, EndTime, LessonID, SessionID) values ("
        sql = sql & Str2Sql(SL_Description)
        sql = sql & ", " & Date2Sql(SL_BeginTime)
        sql = sql & ", " & Date2Sql(SL_EndTime)
        sql = sql & ", " & EL_LessonID
        sql = sql & ", " & Session(VAR_SessionID)
        sql = sql & ")"
        
        ExecuteSQL sql
        
        ' display the data entry screen
        Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & Session(VAR_SessionID))
        ShowDataEntryForm(rst)
        End If
ElseIf state = STATE_REMOVELESSON Then
    If Session(VAR_OpenSession) = True Then
        ' apply changes to other fields
        ApplyFormChanges 0

        slid = CInt(Request.Form("removelessonslid"))

        sql = "delete tSessionLessonSkills where SessionLessonID = " & slid
        ExecuteSQL sql
        sql = "delete tSessionLessons where ID = " & slid
        ExecuteSQL sql
        
        ' display the data entry screen
        Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & Session(VAR_SessionID))
        ShowDataEntryForm(rst)
        End If
ElseIf state = STATE_ADDNEWLESSON Then
    If Session(VAR_OpenSession) = True Then
        ' apply changes to other fields
        ApplyFormChanges 0

        ' add the new lesson
        NL_BookID = CInt(Request.Form("NL_BookID"))
        NL_Pages =  CStr(Request.Form("NL_Pages"))
        NL_Name =  CStr(Request.Form("NL_Name"))
        NL_Description = CStr(Request.Form("NL_Description"))
        NL_ParentID = CInt(Request.Form("NL_ParentID"))

        If CheckboxToBool(Request.Form("NL_Root")) = True Then NL_ParentID = "Null"

        sql = "exec dbo.pNewLesson @name = " & Str2Sql(NL_Name) & _
            ", @BookID = " & NL_BookID & _
            ", @Description = " & Str2Sql(NL_Description) & _
            ", @Pages = " & Str2Sql(NL_Pages) & _
            ", @ParentID = " & NL_ParentID
        
        LessonID = GetFirstValueOfQuery(sql)

        ' now add the new session lesson record
        SL_Description = CStr(Request.Form("SL_Description"))
        SL_BeginTime = CDate(Request.Form("SL_BeginTime"))
        SL_EndTime = CDate(Request.Form("SL_EndTime"))
        
        sql = "insert tSessionLessons (Description, BeginTime, EndTime, LessonID, SessionID) values ("
        sql = sql & Str2Sql(SL_Description)
        sql = sql & ", " & Date2Sql(SL_BeginTime)
        sql = sql & ", " & Date2Sql(SL_EndTime)
        sql = sql & ", " & LessonID
        sql = sql & ", " & Session(VAR_SessionID)
        sql = sql & ")"
        
        ExecuteSQL sql
        
        ' display the data entry screen
        Set rst = PerformSelect("select top 1 * from tSessions s inner join tDates d on d.ID = s.DateID where s.ID = " & Session(VAR_SessionID))
        ShowDataEntryForm(rst)
        End If

End If

' applies changes to the temp session
Sub Apply(SessionID, MusicianID, InstrumentID, BeginTime, EndTime, SDate, OverallRanking, Description, Enabled)
    sql = "update tSessions set "
    sql = sql & " MusicianID = " & MusicianID
    sql = sql & ", InstrumentID = " & InstrumentID
    sql = sql & ", EndTime = " & Date2Sql(EndTime)
    sql = sql & ", BeginTime = " & Date2Sql(BeginTime)
    sql = sql & ", DateID = dbo.fn_GetDateID(" & Date2Sql(SDate) & ")"
    sql = sql & ", OverallRanking = " & OverallRanking
    sql = sql & ", Description = " & Str2Sql(Description)
    sql = sql & ", Enabled = " & Enabled
    sql = sql & " where ID = " & SessionID
    
    ExecuteSQL sql
End Sub

' like Apply above, but automatically gets form info
Sub ApplyFormChanges(Enabled)
    SessionID = Session(VAR_SessionID)
    MusicianID = CLng(GetRequestField("CS_MusicianID"))
    InstrumentID = CLng(GetRequestField("CS_InstrumentID"))
    EndTime = CDate(GetRequestField("CS_EndTime"))
    BeginTime = CDate(GetRequestField("CS_BeginTime"))
    SDate = CDate(GetRequestField("CS_Date"))
    OverallRanking = CLng(GetRequestField("CS_OverallRanking"))
    Description = CStr(GetRequestField("CS_Description"))
    Apply SessionID, MusicianID, InstrumentID, BeginTime, EndTime, SDate, OverallRanking, Description, 0
End Sub

Sub ShowDataEntryForm(rst)
    ' form item naming scheme:
    ' Main session items:
    '   prefix with CS_ (current session)
    
    ' Session Lesson info
    '   prefix with SL_ (New lesson)
    ' New Lesson:
    '   prefix with NL_ (New lesson)
    ' Existing Lesson:
    '   prefix with EL_ (Existing lesson)
    
    ' SessionLesson Skill:
    '   prefix with SS_
    ' NewLesson Skill:
    '   prefix with NS_
    ' Existing Lesson Skill:
    '   prefix with ES_
    %>
    
    <SCRIPT language=VBScript>
    <!--
    Sub SubmitNewSessionForm(state)
        document.f.state.value = state
        document.f.submit()
    End Sub

    Function Validate_IsDate(o)
        Validate_IsDate = False
        if IsDate(o.value) then
            Validate_IsDate = True
        Else
            o.style.backgroundcolor = "yellow"
            o.Focus
            MsgBox o.Name & " is an invalid date."
        End If
    End Function

    Function Validate_NumberBetween(o, min, max)
        Validate_NumberBetween = False
        if IsNumeric(o.value) then
            if CLng(o.value) >= min then
                if CLng(o.value) <= max then
                    Validate_NumberBetween = True
                else
                    o.style.backgroundcolor = "yellow"
                    o.Focus
                    MsgBox o.Name & " is too large of a value."
                end if
            else
                o.style.backgroundcolor = "yellow"
                o.Focus
                MsgBox o.Name & " is too small of a value."
            end if
        Else
            o.style.backgroundcolor = "yellow"
            o.Focus
            MsgBox o.Name & " is an invalid number."
        End If
    End Function

    Function IsValidSessionInfo()
        IsValidSessionInfo = False
        if Validate_IsDate(document.f.CS_BeginTime) = True Then
        if Validate_IsDate(document.f.CS_EndTime) = True Then
        if Validate_IsDate(document.f.CS_Date) = True Then
        if Validate_NumberBetween(document.f.CS_OverallRanking, 0, 100) = True Then
            IsValidSessionInfo = True
        End If
        End If
        End If
        End If
    End Function

    Sub OnApply()
        If IsValidSessionInfo() = True Then
            SubmitNewSessionForm(<%=STATE_APPLY%>)
        End If
    End Sub

    Sub OnAddExistingLesson()
        If IsValidSessionInfo() = True Then
            SubmitNewSessionForm(<%=STATE_ADDEXISTINGLESSON%>)
        End If
    End Sub

    Sub OnRemoveLesson(slid)
        document.f.removelessonslid.value = slid
        SubmitNewSessionForm(<%=STATE_REMOVELESSON%>)
    End Sub
    
    Sub OnAddNewSkill(slid, oDesc, oRank, oName, oParentID, oRoot)
        document.f.SS_SLID.value = slid
        document.f.SS_Description.value = oDesc.value
        document.f.SS_Ranking.value = oDesc.value
        document.f.NS_Name.value = oDesc.value
        document.f.NS_ParentID.value = oDesc.value
        document.f.NS_Root.value = oDesc.value
        SubmitNewSessionForm(<%=STATE_ADDNEWSKILL%>)
    End Sub
    
    Sub OnAddExistingSkill(slid, oDesc, oRank, oSkillID)
        document.f.SS_SLID.value = slid
        document.f.SS_Description.value = oDesc.value
        document.f.SS_Ranking.value = oRank.value
        document.f.ES_SkillID.value = oSkillID.value
        SubmitNewSessionForm(<%=STATE_ADDEXISTINGSKILL%>)
    End Sub

    //-->
    </SCRIPT>

    <form method=post name=f>
        <input type=hidden name=state value=-1 >
        <input type=hidden name=removelessonslid value=-1 ><% ' for deleting a sessionlesson, this is the SessionLessonID %>
        <input type=hidden name=SS_Description value="" >
        <input type=hidden name=SS_Ranking value=-1 >
        <input type=hidden name=SS_SLID value=-1 >
        <input type=hidden name=ES_SkillID value=-1 >
        <input type=hidden name=NS_Name value=-1 >
        <input type=hidden name=NS_ParentID value=-1 >
        <input type=hidden name=NS_Root value=-1 >
        <table>
            <tr><th colspan=2><h4>Practice Session Information</h4>
            <tr><th>Musician<td><% WriteMusicianSelector "CS_MusicianID", rst("MusicianID") %>
            <tr><th>Instrument<td><% WriteInstrumentSelector "CS_InstrumentID", rst("InstrumentID") %>
            <tr><th>BeginTime<td><% WriteBegTimeField 1, "f", "CS_BeginTime", "CS_EndTime", dbf(rst("BeginTime")) %>
            <tr><th>EndTime<td><% WriteEndTimeField 1, "f", "CS_BeginTime", "CS_EndTime", dbf(rst("EndTime")) %>
            <tr><th>Date<td><input type=edit name=CS_Date value="<%=DateSerial(rst("Year"), rst("Month"), rst("MonthDay"))%>"><input type=button value="Today" onclick="document.f.CS_Date.value = '<%=Date()%>';">
            <tr><th>Overall Ranking<td><% WriteRankingSelector "f", "CS_OverallRanking", rst("OverallRanking") %>
            <tr><th>Description/Notes<td><textarea name="CS_Description" cols=40 rows=10 ><%=dbf(rst("Description"))%></textarea>
            <tr><th colspan=2><h4>Lessons</h4>
            <%
                Dim rstl'ne</wheel of fortune> for lessons
                Dim rsts' for skills
                Dim SessionLen, TotalLessonLen, LessonLen
                
                SessionLen = DateDiff("n", rst("BeginTime"), rst("EndTime"))
                TotalLessonLen = 0
                
                sql = ""
                sql = sql & " select "
                sql = sql & "     case when b.Title is NULL then l.Name else l.Name + ' (''' + b.Title + ''', ' + b.Author + ', ' + l.Pages + ')' end Name,"
                sql = sql & "     BeginTime, "
                sql = sql & "     EndTime, "
                sql = sql & "     sl.Description, "
                sql = sql & "     sl.ID SLID "
                sql = sql & " from "
                sql = sql & "     tLessons l inner join "
                sql = sql & "     tSessionLessons sl on sl.LessonID = l.ID left join "
                sql = sql & "     tBooks b on b.ID = l.BookID "
                sql = sql & " where sl.SessionID = " & Session(VAR_SessionID)
                sql = sql & " order by BeginTime, EndTime"
                Set rstl = PerformSelect(sql)
                
                do while not rstl.EOF
                    LessonLen = DateDiff("n", rstl("BeginTime"), rstl("EndTime"))
                    TotalLessonLen = TotalLessonLen + LessonLen
                    %><tr><th><input type=button onclick="OnRemoveLesson(<%=rstl("SLID")%>);" value="Delete">
                    <td><b><%=dbf(rstl("Name"))%></b><br>
                    <%=Server.HTMLEncode(rstl("BeginTime"))%> - <%=Server.HTMLEncode(rstl("EndTime"))%>
                    (<%=LessonLen%> minutes)<br>
                    <%=dbf(rstl("Description"))%><br>
                    <%
                    sql = "select " & _
                        " SkillID, SessionLessonID, Ranking, sls.Description, s.Name " & _
                        " from tSessionLessonSkills sls inner join tSkills s on s.ID = sls.SkillID where SessionLessonID = " & rstl("SLID") & " order by sls.Ranking"
                    Set rsts = PerformSelect(sql)
                    %>
                    <table>
                        <%
                        if not rsts.EOF then
                            %><tr><th>&nbsp;<th>Ranking (0-100)<th>Name<th>Description<%
                        end if

                        Do while not rsts.EOF
                            %><tr><td>[delete]<td><%=Server.HTMLEncode(rsts("Ranking"))%><td><%=Server.HTMLEncode(rsts("Name"))%><td><%=Server.HTMLEncode(rsts("Description"))%>
                            <%
                            rsts.MoveNext
                        loop

                        rsts.Close
                        set rsts = Nothing
                        %>
                        <tr><th colspan=4><% WriteCollapseButton "skills", False %> Add Skills
                        <tr><td colspan=4>
                        <% BeginCollapsableSection "skills", False %>
                            <table>
                                <tr><th>Ranking</th><td><% WriteRankingSelector "f", "XSS_Ranking", 50 %></td></tr>
                                <tr><th>Description/Notes<td><textarea name=XSS_Description cols=40 rows=4 ></textarea>
                                <tr><th colspan=2>Add Existing Skill
                                <tr><th>Skill</th><td><% WriteSkillSelector "XSS_Skill", Empty %></td></tr>
                                <tr><th colspan=2>Add NEW Skill
                                <tr><th>Name</th><td><input type=text name=XNS_Name value="" ></td></tr>
                                <tr><th>Parent</th><td><% WriteSkillSelector "XNS_ParentID", Empty %></td></tr>
                                <tr><th>Root-Level?</th><td><input type=Checkbox name=XNS_Root /></td></tr>
                                <tr><th colspan=2><input type=button onclick="OnAddNewSkill(<%=rstl("SLID")%>);" value="Add new skill"></th></tr>
                            </table>
                        <% EndCollapsableSection "skills" %>
                        </td></tr>
                    </table>
                    
                    <%
                    rstl.MoveNext
                loop
                rstl.Close

                %><tr><th colspan=2><p><h4>You have <%=SessionLen - TotalLessonLen%> unaccounted minutes in the session.</h4></p><%
                
                Set rstl = Nothing
            %><tr><th><% WriteCollapseButton "lessons", False %>Add lessons<td>
                <% BeginCollapsableSection "lessons", False %><h4>Lesson info</h4>
                <table>
                    <tr><th>BeginTime<td><% WriteBegTimeField 2, "f", "SL_BeginTime", "SL_EndTime", dbf(rst("BeginTime")) %>
                    <tr><th>EndTime<td><% WriteEndTimeField 2, "f", "SL_BeginTime", "SL_EndTime", dbf(rst("EndTime")) %>
                    <tr><th>Description/Notes<td><textarea name=SL_Description cols=40 rows=4 ></textarea>
                </table>
                <h4>Add existing lesson</h4>
                <table>
                    <tr><th>Lesson<td><% WriteLessonSelector "EL_LessonID", Empty %>
                    <tr><th colspan=2><input type=button onclick="OnAddExistingLesson();" value="Add Lesson">
                </table>
                <h4><% WriteCollapseButton "newlesson", False %>Add new lesson</h4>
                <% BeginCollapsableSection "newlesson", False %>
                <table>
                    <tr><th>Name</th><td><input type=text name=NL_Name value="" ></td></tr>
                    <tr><th>Book</th><td><% WriteBookSelector "NL_BookID", Empty %></td></tr>
                    <tr><th>Pages</th><td><input type=text name=NL_Pages value="" ></td></tr>
                    <tr><th>Parent</th><td><% WriteLessonSelector "NL_ParentID", Empty %></td></tr>
                    <tr><th>Root-Level?</th><td><input type=Checkbox name=NL_Root Checked /></td></tr>
                    <tr><th>Description</th><td><input type=text name=NL_Description value="" ></td></tr>
                    <tr><th colspan=2><input type=button onclick="SubmitNewSessionForm(<%=STATE_ADDNEWLESSON%>);" value="Add New Lesson">
                </table>
                <% EndCollapsableSection "newlesson" %>
                <% EndCollapsableSection "lessons" %>
            </tr>
            <tr><th colspan=2><h4>Finished?</h4>
                              <input type=button onclick="SubmitNewSessionForm(<%=STATE_COMMIT%>);" value="Finish">
                              <input type=button onclick="SubmitNewSessionForm(<%=STATE_CANCEL%>);" value="Cancel">
                              <input type=button onclick="OnApply();" value="Apply">
        </table>
    </form>
    <%
End Sub

WriteFooter %>

