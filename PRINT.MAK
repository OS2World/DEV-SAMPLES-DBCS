
# MAKE file for Print.MAK     Release 5

# Revised:  1996-01-19

# Copyright ¸ 1987-1996  Prominare Inc.

# MAKE file created by Prominare Builder  Release 5

Update: $(OBJDIR)AppDefs.Obj $(OBJDIR)DBCS.Obj $(OBJDIR)DrawPage.Obj\
        $(OBJDIR)FilePrn.Obj $(OBJDIR)FontDlg.Obj $(OBJDIR)Fonts.Obj\
        $(OBJDIR)PageDlg.Obj $(OBJDIR)PrevDlg.Obj $(OBJDIR)Print.Obj\
        $(OBJDIR)PrnAPI.Obj $(OBJDIR)PrnWnd.Obj $(OBJDIR)Support.Obj\
        $(OBJDIR)ToolBar.Obj

$(OBJDIR)AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW)-Fo$*.Obj AppDefs.C

$(OBJDIR)DBCS.Obj: DBCS.C
 $(CC) $(C_SW)-Fo$*.Obj DBCS.C

$(OBJDIR)DrawPage.Obj: DrawPage.C
 $(CC) $(C_SW)-Fo$*.Obj DrawPage.C

$(OBJDIR)FilePrn.Obj: FilePrn.C
 $(CC) $(C_SW)-Fo$*.Obj FilePrn.C

$(OBJDIR)FontDlg.Obj: FontDlg.C
 $(CC) $(C_SW)-Fo$*.Obj FontDlg.C

$(OBJDIR)Fonts.Obj: Fonts.C
 $(CC) $(C_SW)-Fo$*.Obj Fonts.C

$(OBJDIR)PageDlg.Obj: PageDlg.C
 $(CC) $(C_SW)-Fo$*.Obj PageDlg.C

$(OBJDIR)PrevDlg.Obj: PrevDlg.C
 $(CC) $(C_SW)-Fo$*.Obj PrevDlg.C

$(OBJDIR)Print.Obj: Print.C
 $(CC) $(C_SW)-Fo$*.Obj Print.C

$(OBJDIR)PrnAPI.Obj: PrnAPI.C
 $(CC) $(C_SW)-Fo$*.Obj PrnAPI.C

$(OBJDIR)PrnWnd.Obj: PrnWnd.C
 $(CC) $(C_SW)-Fo$*.Obj PrnWnd.C

$(OBJDIR)Support.Obj: Support.C
 $(CC) $(C_SW)-Fo$*.Obj Support.C

$(OBJDIR)ToolBar.Obj: ToolBar.C
 $(CC) $(C_SW)-Fo$*.Obj ToolBar.C

