#pragma	title("DBCS Driver  --  Version 1  --  (PrnWnd.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#pragma	info(noext)

#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>
#include <string.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	application window procedure.			*/

/* Filename:   PrnWnd.C							*/

/*  Version:   3							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   MRESULT EXPENTRY	PrintDriverWndProc(HWND	hWnd,		*/
/*						   ULONG msg,		*/
/*						   MPARAM mp1,		*/
/*						   MPARAM mp2);		*/

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

/************************************************************************/
/*									*/
/*     External	Data Definitions					*/
/*									*/
/************************************************************************/

extern PSZ pszToolBarClassName;

/************************************************************************/
/*									*/
/*     Module Data Definitions						*/
/*									*/
/************************************************************************/

LONG cyToolBar;			   /* Tool Bar Height			*/

#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- PrintDriverWndProc ------------------------------ [ Public ] ---	*/
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
/*     PrintDriverWndProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY PrintDriverWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
HDC	  hdcPrinter;		   /* Printer Device Context Handle	*/
HPS	  hpsPrinter;		   /* Presentation Space Handle		*/
HPS	  hPS;			   /* Presentation Space Handle		*/
POINTL	  ptl;			   /* Display Point			*/
PPRNDATA  pprni;		   /* Print Data Pointer		*/
RECTL	  rcl;			   /* Window Rectangle			*/
SIZEL	  sizl;			   /* Size Structure			*/
SIZEL	  sizlPage;		   /* Page Size	Holder			*/
TID	  tid;			   /* Thread ID				*/
ULONG	  flCreateFlags;	   /* Tool Bar Create Flag		*/

switch ( msg )
   {
			/* Window being	created, perform window		*/
			/* initialization				*/
   case	WM_CREATE :

       hptrWait	 = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT,	FALSE);
       hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW,	FALSE);

		       /* Get a	temporary presentation space so	that	*/
		       /* the system's font metrics can be found and    */
		       /* the bitmap image used	within the created	*/
		       /* windows menus	can be used			*/

       sizl.cx = sizl.cy = 0L;
       hpsPrint	= GpiCreatePS(hAB, WinOpenWindowDC(hWnd), &sizl,
			      PU_TWIPS | GPIF_DEFAULT |	GPIT_MICRO | GPIA_ASSOC);

			/************************************************/
			/* PDSGetTemplate is used to allow controls in	*/
			/* windows.  Do	not remove this	function if you	*/
			/* intend to include controls within the window.*/
			/************************************************/

       PDSGetTemplate(hWnd, WIN_PRINTDRIVER);
       ptl.x = 0L;
       ptl.y = 16L;
       WinMapDlgPoints(HWND_DESKTOP, &ptl, 1UL,	TRUE);
       cyToolBar = ptl.y;
		       /* Create the toolbar window			*/

       flCreateFlags = FCF_BORDER | FCF_NOBYTEALIGN;
       if ( !(hwndToolBarFrame = WinCreateStdWindow(hWnd, WS_VISIBLE,
						    &flCreateFlags,
						    pszToolBarClassName, "",
						    0L,	(HMODULE)NULL, ID_TOOLBAR, &hwndToolBar)) )
	   return(MRFROMLONG(TRUE));
       break;

   case	WM_SIZE	:
		       /* Place	the new	legend window in the lower	*/
		       /* right	hand corner of the display before	*/
		       /* displaying it					*/

       WinQueryWindowRect(hWnd,	&rcl);

       WinSetWindowPos(hwndToolBarFrame, HWND_BOTTOM,
		       rcl.xLeft - 1L,
		       rcl.yTop	- cyToolBar,
		       rcl.xRight + 2L,
		       cyToolBar,
		       SWP_MOVE	| SWP_SIZE | SWP_SHOW);
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
			/* Process menu	and button selections		*/
   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	IDM_QUERYDISPLAYFONTS :
	       WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame, (PFNWP)FontMetricsDlgProc,
			 (HMODULE)NULL,	DLG_FONTMETRICS, NULL);
	       break;
	   case	IDM_PRINTPREVIEW :
	       if ( !WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame,	(PFNWP)PrintPreviewDlgProc,
			       (HMODULE)NULL, DLG_PRINTPREVIEW,	NULL) )
		   break;

	   case	IDM_PRINTDISPLAY :
	       if ( fQuerySelectedFont(WinWindowFromID(hwndToolBar, CBX_FONT),
				       WinWindowFromID(hwndToolBar, CBX_FONTSIZE),
				       &fsel) )
		   {
		       /* Allocate memory for the printer information	*/
		       /* and save the printer information from	the	*/
		       /* dialogue in the memory			*/

		   DosAllocMem((PPVOID)(PVOID)&pprni, PRNDATASIZE + CCHMAXPATH,
			       PAG_READ	| PAG_WRITE | PAG_COMMIT);

		   pprni->prn  = prn;
		   pprni->fsel = fsel;
		   pprni->pags = pags;
		   memcpy(pprni->szTitle, "Print Font Example - Display Example", 37);

		       /* Create a separate thread from	which the	*/
		       /* printing is performed				*/

		   DosCreateThread(&tid, (PFNTHREAD)PrnDisplay,	(ULONG)pprni,
				   STACK_COMMITTED, 16384UL);
		   }
	       break;

	  case IDM_PRINT :
	      WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame, (PFNWP)FilePrintDlgProc,
			(HMODULE)NULL, DLG_FILEPRN, NULL);
	      break;

	   case	IDM_PRINTERSETUP :
	       if ( WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame, (PFNWP)PrnSetupDlgProc,
			      (HMODULE)NULL, DLG_PRNSETUP, (PVOID)&prn)	)
		   switch ( lSetFonts(WinWindowFromID(hwndToolBar, CBX_FONT)) )
		       {
		       /* Error	return:	bad HDC	for printer		*/

		       case 1L :
			   WinMessageBox(HWND_DESKTOP, hWnd, "Printer Device Context could not be opened.",
					 "Print Font Example", 0UL, MB_OK | MB_ICONHAND	| MB_MOVEABLE);
			   return(MRFROMLONG(TRUE));

		       /* Error	return:	bad HPS	for printer		*/

		       case 2L :
			   WinMessageBox(HWND_DESKTOP, hWnd, "Printer Presentation Space could not be created.",
					 "Print Font Example", 0UL, MB_OK | MB_ICONHAND	| MB_MOVEABLE);
			   return(MRFROMLONG(TRUE));

		       /* Error	return:	no error, fonts	set in combo	*/

		       default :
			   break;
		       }
	       break;

	   case	IDM_PAGESETUP :
	       WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame, (PFNWP)PageSetupDlgProc,
			 (HMODULE)NULL,	DLG_PAGESETUP, NULL);
	       break;
	   }
       break;
			/* Erase window	background			*/

   case	WM_ERASEBACKGROUND :
       WinQueryWindowRect(hWnd,	&rcl);
       WinFillRect((HPS)LONGFROMMP(mp1), &rcl, SYSCLR_WINDOW);
       break;
			/* Paint client	window				*/
   case	WM_PAINT :
       GpiSetColor(hPS = WinBeginPaint(hWnd, hpsPrint, &rcl), CLR_BLACK);
       WinFillRect(hPS,	&rcl, SYSCLR_WINDOW);

       if ( ((hdcPrinter = PrnOpenInfoDC(&prn, "PM_Q_STD")) != (HDC)NULL) )
	   {
		       /* Create a presentation	space into which the	*/
		       /* printing will	occur using TWIPS as the unit	*/
		       /* of measure					*/

	   sizlPage.cx = sizlPage.cy = 0L;

	   DrawCommon(hPS, hpsPrinter =	GpiCreatePS(hAB, hdcPrinter, &sizlPage,	PU_TWIPS |
						    GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC),
		      &fcache, 90L, 90L, 0L, 0L, NULL, NULL, FALSE);

	   GpiAssociate(hpsPrinter, (HDC)NULL);
	   GpiDestroyPS(hpsPrinter);
	   DevCloseDC(hdcPrinter);

	   DrawText(hPS);
	   }

       WinEndPaint(hPS);
       break;
			/* Window being	destroyed, perform clean-up	*/
			/* operations					*/
   case	WM_DESTROY :
       break;

			/* Default message processing			*/
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
