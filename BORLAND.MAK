
# MAKE file for Borland.MAK 

# Revised:  1996-03-07

# Macro definitions

C_SW=-Ox -v- -w -c 


Print.Exe: AppDefs.Obj DBCS.Obj DrawPage.Obj\
        FilePrn.Obj FontDlg.Obj Fonts.Obj\
        PageDlg.Obj PrevDlg.Obj Print.Obj\
        Print.Res PrnAPI.Obj PrnWnd.Obj\
        Support.Obj ToolBar.Obj
 TLink @Borland.Lnk
 rc Print.Res Print.Exe

AppDefs.Obj: AppDefs.C
 Bcc $(C_SW) -o$*.Obj AppDefs.C

DBCS.Obj: DBCS.C
 Bcc $(C_SW) -o$*.Obj DBCS.C

DrawPage.Obj: DrawPage.C
 Bcc $(C_SW) -o$*.Obj DrawPage.C

FilePrn.Obj: FilePrn.C
 Bcc $(C_SW) -o$*.Obj FilePrn.C

FontDlg.Obj: FontDlg.C
 Bcc $(C_SW) -o$*.Obj FontDlg.C

Fonts.Obj: Fonts.C
 Bcc $(C_SW) -o$*.Obj Fonts.C

PageDlg.Obj: PageDlg.C
 Bcc $(C_SW) -o$*.Obj PageDlg.C

PrevDlg.Obj: PrevDlg.C
 Bcc $(C_SW) -o$*.Obj PrevDlg.C

Print.Obj: Print.C
 Bcc $(C_SW) -o$*.Obj Print.C

Print.Res: Print.Rc
 Brc -r Print.Rc $*.Res

PrnAPI.Obj: PrnAPI.C
 Bcc $(C_SW) -o$*.Obj PrnAPI.C

PrnWnd.Obj: PrnWnd.C
 Bcc $(C_SW) -o$*.Obj PrnWnd.C

Support.Obj: Support.C
 Bcc $(C_SW) -o$*.Obj Support.C

ToolBar.Obj: ToolBar.C
 Bcc $(C_SW) -o$*.Obj ToolBar.C

