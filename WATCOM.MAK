
# MAKE file for WATCOM.MAK

# Revised:  1996-03-07

# Macro definitions

C_SW=-ms -5s -s -ox -zp4 -w3 


Print.Exe: AppDefs DBCS DrawPage\
        FilePrn FontDlg Fonts\
        PageDlg PrevDlg Print\
        Print.Res PrnAPI PrnWnd\
        Support ToolBar
 WLink @WATCOM.Lnk
 rc Print.Res Print.Exe

AppDefs: AppDefs.C
 Wcl386 $(C_SW) -fo=$*.Obj AppDefs.C

DBCS: DBCS.C
 Wcl386 $(C_SW) -fo=$*.Obj DBCS.C

DrawPage: DrawPage.C
 Wcl386 $(C_SW) -fo=$*.Obj DrawPage.C

FilePrn: FilePrn.C
 Wcl386 $(C_SW) -fo=$*.Obj FilePrn.C

FontDlg: FontDlg.C
 Wcl386 $(C_SW) -fo=$*.Obj FontDlg.C

Fonts: Fonts.C
 Wcl386 $(C_SW) -fo=$*.Obj Fonts.C

PageDlg: PageDlg.C
 Wcl386 $(C_SW) -fo=$*.Obj PageDlg.C

PrevDlg: PrevDlg.C
 Wcl386 $(C_SW) -fo=$*.Obj PrevDlg.C

Print: Print.C
 Wcl386 $(C_SW) -fo=$*.Obj Print.C

Print.Res: Print.Rc
 Rc -r Print.Rc $*.Res

PrnAPI: PrnAPI.C
 Wcl386 $(C_SW) -fo=$*.Obj PrnAPI.C

PrnWnd: PrnWnd.C
 Wcl386 $(C_SW) -fo=$*.Obj PrnWnd.C

Support: Support.C
 Wcl386 $(C_SW) -fo=$*.Obj Support.C

ToolBar: ToolBar.C
 Wcl386 $(C_SW) -fo=$*.Obj ToolBar.C

