#pragma	title("DBCS Driver  --  Version 1  --  (ToolBar.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#pragma	info(noext)

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	tool bar window	procedure.			*/

/* Filename:   ToolBar.C						*/

/*  Version:   1							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   static VOID CalcSize(PRECTL prcl);			*/
/*	       MRESULT EXPENTRY	ToolBarWndProc(HWND hWnd, ULONG	msg,	*/
/*					       MPARAM mp1, MPARAM mp2);	*/

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* DISCLAIMER OF WARRANTIES:						*/
/* -------------------------						*/
/* The following [enclosed] code is sample code	created	by IBM		*/
/* Corporation and Prominare Inc.  This	sample code is not part	of any	*/
/* standard IBM	product	and is provided	to you solely for the purpose	*/
/* of assisting	you in the development of your applications.  The code	*/
/* is provided "AS IS",	without	warranty of any	kind.  Neither IBM nor	*/
/* Prominare shall be liable for any damages arising out of your	*/
/* use of the sample code, even	if they	have been advised of the	*/
/* possibility of such damages.						*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*		       D I S C L A I M E R				*/
/* This	code is	provided on an as is basis with	no implied support.	*/
/* It should be	considered freeware that cannot	be rebundled as		*/
/* part	of a larger "*ware" offering without our consent.		*/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Copyright ¸ International Business Machines Corp., 1996.		*/
/* Copyright ¸ 1996  Prominare Inc.  All Rights	Reserved.		*/

/* --------------------------------------------------------------------	*/

static SWP aswp[4];		   /* Size/Window Position Array	*/
static POINTL aptlShade[5];	   /* Shadow Points			*/

#define	ASWP_FONTLABEL	   0
#define	ASWP_FONTNAMES	   1
#define	ASWP_SIZELABEL	   2
#define	ASWP_FONTSIZES	   3

static VOID CalcSize(PRECTL prcl);

#pragma	subtitle("   Shadowed Frame DLL - Control Window Procedure")
#pragma	page ( )

/* --- CalcSize	---------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	calculate the sizes and	positions	*/
/*     of the various elements that are	used to	make up	the font	*/
/*     selection combo boxes within the	tool bar.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PRECTL	 prcl; = Control Rectangle Pointer			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID CalcSize(PRECTL prcl)

{

WinQueryWindowPos(aswp[ASWP_FONTSIZES].hwnd, &aswp[ASWP_FONTSIZES]);

aswp[ASWP_FONTSIZES].x = (prcl->xRight - prcl->xLeft) -	aswp[ASWP_FONTSIZES].cx	- 10L;
aswp[ASWP_SIZELABEL].x = aswp[ASWP_FONTSIZES].x	- aswp[ASWP_SIZELABEL].cx - 5L;

WinQueryWindowPos(aswp[ASWP_FONTLABEL].hwnd, &aswp[ASWP_FONTLABEL]);
aswp[ASWP_FONTNAMES].x = aswp[ASWP_FONTLABEL].x	+ aswp[ASWP_FONTLABEL].cx + 5L;
aswp[ASWP_FONTNAMES].cx	= (aswp[ASWP_SIZELABEL].x - 5L)	- aswp[ASWP_FONTNAMES].x;

WinSetMultWindowPos(hAB, aswp, 4UL);

}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- ToolBarWndProc ---------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	process	the messages sent to the	*/
/*     applications client window.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Window Handle					*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ToolBarWndProc =	Message	Handling Result				*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ToolBarWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
HDC	       hdcPrinter;	   /* Printer Device Context Handle	*/
HPS	       hpsPrinter;	   /* Presentation Space Handle		*/
RECTL	       rcl;		   /* Window Rectangle			*/
HPS	       hPS;		   /* Presentation Space Handle		*/
PCREATESTRUCT  pcrst;		   /* Create Structure Pointer		*/
POINTL	       ptl;		   /* Display Point			*/
SIZEL	       sizlPage;	   /* Page Size	Holder			*/
FONTSEL	       fselToolBar;	   /* Tool Bar Font Selected		*/
register INT i;			   /* Loop Counter			*/

switch ( msg )
   {
			/* Window being	created, perform window		*/
			/* initialization				*/
   case	WM_CREATE :
			/************************************************/
			/* PDSGetTemplate is used to allow controls in	*/
			/* windows.  Do	not remove this	function if you	*/
			/* intend to include controls within the window.*/
			/************************************************/

       PDSGetTemplate(hWnd, ID_TOOLBAR);

       for ( i = 0; i <	4; i++ )
	   WinQueryWindowPos(WinWindowFromID(hWnd, (ULONG)(IT_FONTLABEL	+ i)), &aswp[i]);

       ptl.x = 0L;
       ptl.y = 80L;
       WinMapDlgPoints(HWND_DESKTOP, &ptl, 1UL,	TRUE);

		       /* Adjust the size of the combo box and cause it	*/
		       /* to be	resized	to its new size			*/

       aswp[ASWP_FONTSIZES].y  = (aswp[ASWP_FONTNAMES].y  -= ptl.y);
       aswp[ASWP_FONTSIZES].cy = (aswp[ASWP_FONTNAMES].cy += ptl.y);

       aswp[ASWP_FONTLABEL].fl = aswp[ASWP_FONTLABEL].fl = SWP_MOVE;
       aswp[ASWP_FONTNAMES].fl = aswp[ASWP_FONTSIZES].fl = SWP_SIZE | SWP_MOVE;

       WinSetMultWindowPos(WinQueryAnchorBlock(hWnd), aswp, 4UL);

       pcrst = (PCREATESTRUCT)PVOIDFROMMP(mp2);
       rcl.xLeft   =
       rcl.yBottom = 0L;
       rcl.xRight  = pcrst->cx;
       rcl.yTop	   = pcrst->cy;
       if ( (pcrst->cx > 0L) &&	(pcrst->cy > 0L) )
	   CalcSize(&rcl);

       switch (	lSetFonts(WinWindowFromID(hWnd,	CBX_FONT)) )
	   {
		       /* Error	return:	bad HDC	for printer		*/
	   case	1L :
	       WinMessageBox(HWND_DESKTOP, hWnd, "Printer Device Context could not be opened.",
			     "Print Font Example", 0UL,	MB_OK |	MB_ICONHAND | MB_MOVEABLE);
	       return(MRFROMLONG(TRUE));

		       /* Error	return:	bad HPS	for printer		*/
	   case	2L :
	       WinMessageBox(HWND_DESKTOP, hWnd, "Printer Presentation Space could not be created.",
			     "Print Font Example", 0UL,	MB_OK |	MB_ICONHAND | MB_MOVEABLE);
	       return(MRFROMLONG(TRUE));

		       /* Error	return:	no error, fonts	set in combo	*/
	   default :
	       break;
	   }

       break;
		       /* Size of control changing, reset the display	*/
		       /* points of the	line				*/
   case	WM_SIZE	:
       if ( ((LONG)(SHORT)SHORT1FROMMP(mp2) > 0L) &&
	    ((LONG)(SHORT)SHORT2FROMMP(mp2) > 0L) )
	   {
	   WinQueryWindowRect(hWnd, &rcl);
	   CalcSize(&rcl);

		       /* Set the new width and	height of the window	*/

	   aptlShade[1].y = aptlShade[2].y = SHORT2FROMMP(mp2) - 1L;
	   aptlShade[2].x = aptlShade[3].x = SHORT1FROMMP(mp2) - 1L;
	   }
       break;
			/* Process key press from keyboard		*/
   case	WM_CHAR	:
			/************************************************/
			/* PDSKeyProc is used to allow controls	in	*/
			/* windows.  Do	not remove this	function if you	*/
			/* intend to include controls within the window.*/
			/************************************************/

	return(PDSKeyProc(hWnd,	msg, mp1, mp2));

			/* Process control selections			*/
   case	WM_CONTROL :
       switch (	SHORT2FROMMP(mp1) )
	   {
		       /* Process message combo	box entry selections	*/

	   case	CBN_LBSELECT :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	CBX_FONT :
		       SetFontSize(WinWindowFromID(hWnd, CBX_FONT), WinWindowFromID(hWnd, CBX_FONTSIZE));
		       if ( fQuerySelectedFont(WinWindowFromID(hWnd, CBX_FONT),
					       WinWindowFromID(hWnd, CBX_FONTSIZE), &fselToolBar) &&
			    ((hdcPrinter = PrnOpenInfoDC(&prn, "PM_Q_STD")) != (HDC)NULL) &&
			    hpsPrint )
			   {
			   sizlPage.cx = sizlPage.cy = 0L;
			   DeleteFontCache(hpsPrint, &fcache);
			   BuildFontCache(hpsPrint,
					  hpsPrinter = GpiCreatePS(hAB,	hdcPrinter, &sizlPage,
								   PU_TWIPS | GPIF_DEFAULT | GPIT_MICRO	| GPIA_ASSOC),
					  hdcPrinter,
					  fselToolBar.szFacename, fselToolBar.lNominalPointSize,
					  &fcache);
			   BuildFontCache(hpsPrint, hpsPrinter,	hdcPrinter,
					  fselToolBar.szFacename, 100L,
					  &fcacheScale);
			   GpiAssociate(hpsPrinter, (HDC)NULL);
			   GpiDestroyPS(hpsPrinter);
			   DevCloseDC(hdcPrinter);

			   WinInvalidateRect(hwndPrnSetup, (PRECTL)NULL, FALSE);
			   }
		       break;

		   case	CBX_FONTSIZE :
		       if ( fQuerySelectedFont(WinWindowFromID(hWnd, CBX_FONT),	WinWindowFromID(hWnd, CBX_FONTSIZE), &fselToolBar) &&
			    ((hdcPrinter = PrnOpenInfoDC(&prn, "PM_Q_STD")) != (HDC)NULL) && hpsPrint )
			   {
			   sizlPage.cx = sizlPage.cy = 0L;
			   DeleteFontCache(hpsPrint, &fcache);
			   BuildFontCache(hpsPrint,
					  hpsPrinter = GpiCreatePS(hAB,	hdcPrinter, &sizlPage,
								   PU_TWIPS | GPIF_DEFAULT | GPIT_MICRO	| GPIA_ASSOC),
					  hdcPrinter,
					  fselToolBar.szFacename, fselToolBar.lNominalPointSize,
					  &fcache);
			   GpiAssociate(hpsPrinter, (HDC)NULL);
			   GpiDestroyPS(hpsPrinter);
			   DevCloseDC(hdcPrinter);

			   WinInvalidateRect(hwndPrnSetup, (PRECTL)NULL, FALSE);
			   }
		       break;
		   }
	       break;
	   }
       break;
			/* Erase window	background			*/

   case	WM_ERASEBACKGROUND :
       WinQueryWindowRect(hWnd,	&rcl);
       WinMapWindowPoints(hWnd,	hwndToolBarFrame, (PPOINTL)(PVOID)&rcl,	2L);
       GpiCreateLogColorTable((HPS)mp1,	0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);
       WinFillRect((HPS)mp1, &rcl, RGBCLR_DIALOGBACKGROUND);
       break;
			/* Paint client	window				*/
   case	WM_PAINT :
		       /* Get the presentation space and set the first	*/
		       /* outline edge colour to white			*/

       GpiCreateLogColorTable(hPS = WinBeginPaint(hWnd,	(HPS)NULL, (PRECTL)NULL),
			      0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);
       GpiSetColor(hPS,	RGB_WHITE);

		       /* Set up the starting position and start	*/
		       /* drawing the lighted edge of the window	*/

       GpiMove(hPS, aptlShade);
       GpiPolyLine(hPS,	2L, &aptlShade[1]);

		       /* Set the shaded edge colour and start the	*/
		       /* drawing of the shaded	edge of	the window	*/

       GpiSetColor(hPS,	RGBCLR_SHADOW);
       GpiPolyLine(hPS,	2L, &aptlShade[3]);

		       /* Release the presentation space		*/
       WinEndPaint(hPS);
       break;

			/* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
