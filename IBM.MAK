
# MAKE file for IBM.MAK

# Revised:  1996-03-07

# Macro definitions

C_SW=-G5 -O+ -W3 -C

Print.Exe: AppDefs DBCS DrawPage FilePrn FontDlg Fonts\
        PageDlg PrevDlg Print Print.Res PrnAPI PrnWnd\
        Support ToolBar
 ILink /FREE /BASE:0x10000 /ALIGN:4 @IBM.Lnk
 rc Print.Res Print.Exe

AppDefs: AppDefs.C
 Icc $(C_SW) -Fo$*.Obj AppDefs.C

DBCS: DBCS.C
 Icc $(C_SW) -Fo$*.Obj DBCS.C

DrawPage: DrawPage.C
 Icc $(C_SW) -Fo$*.Obj DrawPage.C

FilePrn: FilePrn.C
 Icc $(C_SW) -Fo$*.Obj FilePrn.C

FontDlg: FontDlg.C
 Icc $(C_SW) -Fo$*.Obj FontDlg.C

Fonts: Fonts.C
 Icc $(C_SW) -Fo$*.Obj Fonts.C

PageDlg: PageDlg.C
 Icc $(C_SW) -Fo$*.Obj PageDlg.C

PrevDlg: PrevDlg.C
 Icc $(C_SW) -Fo$*.Obj PrevDlg.C

Print: Print.C
 Icc $(C_SW) -Fo$*.Obj Print.C

Print.Res: Print.Rc
 Rc -r Print.Rc $*.Res

PrnAPI: PrnAPI.C
 Icc $(C_SW) -Fo$*.Obj PrnAPI.C

PrnWnd: PrnWnd.C
 Icc $(C_SW) -Fo$*.Obj PrnWnd.C

Support: Support.C
 Icc $(C_SW) -Fo$*.Obj Support.C

ToolBar: ToolBar.C
 Icc $(C_SW) -Fo$*.Obj ToolBar.C

