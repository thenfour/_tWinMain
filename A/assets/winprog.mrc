; winprog.dll
; Procedures which make use of the dll follow, defined as aliases
; 
; [aliases]
; LoadWinprog
; UnloadWinprog


alias LoadWinprog {
  $dll(winprog.dll, InitWinprog,)
}

alias UnloadWinprog {
  /dll -u winprog.dll
  /dll -u helper.dll
}

