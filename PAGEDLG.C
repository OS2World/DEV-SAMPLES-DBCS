#pragma	title("DBCS Driver  --  Version 1  --  (PageDlg.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#pragma	info(noext)

#include <malloc.h>
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	the routines to	handle the page	setup dialogue	*/
/* where the header and	footer is specified along with the margins.	*/

/* Filename:   PageDlg.C						*/

/*  Version:   3							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   static BOOL fBuildPreview(HWND hWnd);			*/
/*	       MRESULT EXPENTRY	PageSetupDlgProc(HWND hWnd, ULONG msg,	*/
/*						 MPARAM	mp1, MPARAM mp2;*/

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
/*     Module Data Definitions						*/
/*									*/
/************************************************************************/

static HBITMAP	 hbmPreview;	   /* Preview Bitmap Handle		*/
static RECTL	 rclDest;	   /* Preview Bitmap Rectangle		*/
static RECTL	 rclPreview;	   /* Preview Group Box	Rectangle	*/
static POINTL	 aptlShade[5];	   /* Shadow Points			*/
static PAGESETUP pagsTemp;	   /* Temporary	Page Setup		*/
static POINTL	 aptlArea[8];	   /* Shadow Points			*/
static LONG	 lScale;	   /* Scaling Factor			*/

#pragma	subtitle("   Module Purpose - Preview Bitmap Build Function")
#pragma	page( )

/* --- fBuildPreview ----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	create the preview bitmap for the	*/
/*     page setup dialogue.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd; = Dialog Window Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     fBuildPreview =	TRUE : Preview Bitmap Created			*/
/*		     = FALSE : Preview Bitmap Creation Failed		*/
/*									*/
/* --------------------------------------------------------------------	*/

static BOOL fBuildPreview(HWND hWnd)

{
HDC	hdcPrinter;		   /* Printer Device Context Handle	*/
HPS	hPS;			   /* Presentation Space Handle		*/
LONG	cxPrev;			   /* Preview Width			*/
LONG	cyPrev;			   /* Preview Height			*/
SIZEL	sizlPage;		   /* Page Size	Holder			*/
SWP	swp;			   /* Window Position			*/

if ( (hdcPrinter = PrnOpenInfoDC(&prn, "PM_Q_STD")) != (HDC)NULL )
   {	
		       /* Create a presentation	space into which the	*/
		       /* printing will	occur using TWIPS as the unit	*/
		       /* of measure					*/

   sizlPage.cx = sizlPage.cy = 0L;

		       /* Determine the	size of	the output area		*/

   GpiQueryPS(hPS = GpiCreatePS(hAB, hdcPrinter, &sizlPage, PU_TWIPS |
	      GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC),	&sizlPage);

		       /* Create the bitmap image			*/

   hbmPreview =	hbmCreatePreview(sizlPage.cx, sizlPage.cy, hPS,	hdcPrinter);

		       /* Convert the image size from twips to pels	*/

   GpiConvert(hPS, CVTC_DEFAULTPAGE, CVTC_DEVICE, 1L, (PPOINTL)(PVOID)&sizlPage);

		       /* Set the page boundaries points		*/

   aptlArea[0].x = (LONG)(1440.0 * pags.rdLeftMargin);
   aptlArea[0].y = (LONG)(1440.0 * pags.rdBottomMargin)	/ 2L;
   aptlArea[1].x = (LONG)(1440.0 * pags.rdLeftMargin);
   aptlArea[1].y = (LONG)(1440.0 * pags.rdTopMargin) / 2L;

   aptlArea[2].x = (LONG)(1440.0 * pags.rdRightMargin);
   aptlArea[2].y = (LONG)(1440.0 * pags.rdTopMargin) / 2L;
   aptlArea[3].x = (LONG)(1440.0 * pags.rdRightMargin);
   aptlArea[3].y = (LONG)(1440.0 * pags.rdBottomMargin)	/ 2L;

   aptlArea[4].x = (LONG)(1440.0 * pags.rdLeftMargin) /	2L;
   aptlArea[4].y = (LONG)(1440.0 * pags.rdBottomMargin);
   aptlArea[5].x = (LONG)(1440.0 * pags.rdRightMargin) / 2L;
   aptlArea[5].y = (LONG)(1440.0 * pags.rdBottomMargin);

   aptlArea[6].x = (LONG)(1440.0 * pags.rdLeftMargin) /	2L;
   aptlArea[6].y = (LONG)(1440.0 * pags.rdTopMargin);
   aptlArea[7].x = (LONG)(1440.0 * pags.rdRightMargin) / 2L;
   aptlArea[7].y = (LONG)(1440.0 * pags.rdTopMargin);

		       /* Convert the points from twips	to pels		*/

   GpiConvert(hPS, CVTC_DEFAULTPAGE, CVTC_DEVICE, 8L, aptlArea);

		       /* Release the presentation space and printer	*/
		       /* device context				*/

   GpiAssociate(hPS, (HDC)NULL);
   GpiDestroyPS(hPS);
   DevCloseDC(hdcPrinter);

		       /* Get the size and position of the preview	*/
		       /* group	box					*/

   WinQueryWindowPos(WinWindowFromID(hWnd, GB_PREVIEW),	(PSWP)&swp);

		       /* Form the rectangle for the preview image	*/

   rclPreview.xRight = (rclPreview.xLeft   = swp.x + 10L) + (cxPrev = swp.cx - 20L);
   rclPreview.yTop   = (rclPreview.yBottom = swp.y + 10L) + (cyPrev = swp.cy - 10L - fm.lMaxBaselineExt	* 2);

		       /* Resize the image to fit the group box	making	*/
		       /* sure that it is scaled retaining the aspect	*/
		       /* ratio						*/

   if (	((sizlPage.cx /	cxPrev)	+ 1) > ((sizlPage.cy / cyPrev) + 1) )
       cyPrev =	sizlPage.cy / (lScale =	(sizlPage.cx / cxPrev) + 1);
   else
       cxPrev =	sizlPage.cx / (lScale =	(sizlPage.cy / cyPrev) + 1);

		       /* Form the final bitmap	destination rectangle	*/

   rclDest.xRight = (rclDest.xLeft   = swp.x + ((swp.cx	- cxPrev) / 2L)) + cxPrev;
   rclDest.yTop	  = (rclDest.yBottom = swp.y + ((swp.cy	- cyPrev) / 2L)) + cyPrev;

		       /* Form the final boundary points		*/

   aptlArea[0].x = rclDest.xLeft   + aptlArea[0].x / lScale;
   aptlArea[0].y = rclDest.yBottom + aptlArea[0].y / lScale;
   aptlArea[1].x = rclDest.xLeft   + aptlArea[1].x / lScale;
   aptlArea[1].y = rclDest.yTop	   - aptlArea[1].y / lScale;

   aptlArea[2].x = rclDest.xRight  - aptlArea[2].x / lScale;
   aptlArea[2].y = rclDest.yTop	   - aptlArea[2].y / lScale;
   aptlArea[3].x = rclDest.xRight  - aptlArea[3].x / lScale;
   aptlArea[3].y = rclDest.yBottom + aptlArea[3].y / lScale;

   aptlArea[4].x = rclDest.xLeft   + aptlArea[4].x / lScale;
   aptlArea[4].y = rclDest.yBottom + aptlArea[4].y / lScale;
   aptlArea[5].x = rclDest.xRight  - aptlArea[5].x / lScale;
   aptlArea[5].y = rclDest.yBottom + aptlArea[5].y / lScale;

   aptlArea[6].x = rclDest.xLeft   + aptlArea[6].x / lScale;
   aptlArea[6].y = rclDest.yTop	   - aptlArea[6].y / lScale;
   aptlArea[7].x = rclDest.xRight  - aptlArea[7].x / lScale;
   aptlArea[7].y = rclDest.yTop	   - aptlArea[7].y / lScale;

		       /* Form the 3D edge points for the preview	*/

   aptlShade[0].x = rclDest.xLeft;
   aptlShade[0].y = rclDest.yTop;
   aptlShade[1].x = rclDest.xRight + 2L;
   aptlShade[1].y = rclDest.yTop;
   aptlShade[2].x = rclDest.xRight + 2L;
   aptlShade[2].y = rclDest.yBottom - 2L;
   aptlShade[3].x = rclDest.xLeft;
   aptlShade[3].y = rclDest.yBottom - 2L;
   return(TRUE);
   }

return(FALSE);
}
#pragma	subtitle("   Module Purpose - Page Setup Dialogue Procedure")
#pragma	page( )

/* --- PageSetupDlgProc	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the Page	*/
/*     Setup dialogue procedure.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     PageSetupDlgProc	= Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY PageSetupDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2)

{
CHAR	szBuffer[32];		   /* String Buffer			*/
HPS	hPS;			   /* Presentation Space Handle		*/
MRESULT	mr;			   /* Message Result			*/

switch ( msg )
   {
		       /* Perform dialog initialization			*/
   case	WM_INITDLG :
		       /* Save the page	setup				*/
       pagsTemp	= pags;
		       /* Form the margin values and place within the	*/
		       /* entry	fields					*/

       sprintf(szBuffer, "%.2f\"", pags.rdTopMargin);
       WinSetDlgItemText(hWnd, EF_MARGINTOP, szBuffer);
       sprintf(szBuffer, "%.2f\"", pags.rdBottomMargin);
       WinSetDlgItemText(hWnd, EF_MARGINBOTTOM,	szBuffer);
       sprintf(szBuffer, "%.2f\"", pags.rdLeftMargin);
       WinSetDlgItemText(hWnd, EF_MARGINLEFT, szBuffer);
       sprintf(szBuffer, "%.2f\"", pags.rdRightMargin);
       WinSetDlgItemText(hWnd, EF_MARGINRIGHT, szBuffer);

		       /* Place	the header and footer within the	*/
		       /* entry	fields					*/

       WinSetDlgItemText(hWnd, EF_HEADER, pags.szHeader);
       WinSetDlgItemText(hWnd, EF_FOOTER, pags.szFooter);

		       /* Build	the preview bitmap			*/

       if ( fBuildPreview(hWnd)	)
	   CenterDlg(hWnd);
       else
	   {
	   WinMessageBox(HWND_DESKTOP, hWnd, "Preview bitmap creation failed.",
			 "Page Setup", 0UL, MB_OK);

		       /* Bitmap creation failed, dismiss the		*/
		       /* dialogue					*/

	   WinDismissDlg(hWnd, FALSE);
	   }
       break;
			/* Process control selections			*/
   case	WM_CONTROL :
       switch (	SHORT2FROMMP(mp1) )
	   {

/************************************************************************/
/* Entry field losing focus notification				*/
/************************************************************************/

	   case	EN_KILLFOCUS :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	EF_MARGINLEFT :
		   case	EF_MARGINRIGHT :
		   case	EF_MARGINTOP :
		   case	EF_MARGINBOTTOM	:
		   case	EF_HEADER :
		   case	EF_FOOTER :
		       GpiDeleteBitmap(hbmPreview);

		       /* Get the values the user has entered within	*/
		       /* the entry fields and save internally		*/

		       WinQueryDlgItemText(hWnd, EF_MARGINTOP,	  8L, szBuffer);
		       pags.rdTopMargin	= atof(szBuffer);
		       WinQueryDlgItemText(hWnd, EF_MARGINBOTTOM, 8L, szBuffer);
		       pags.rdBottomMargin = atof(szBuffer);
		       WinQueryDlgItemText(hWnd, EF_MARGINLEFT,	  8L, szBuffer);
		       pags.rdLeftMargin = atof(szBuffer);
		       WinQueryDlgItemText(hWnd, EF_MARGINRIGHT,  8L, szBuffer);
		       pags.rdRightMargin = atof(szBuffer);

		       /* Get the header and footer text from the	*/
		       /* entry	fields and save	internally for the	*/
		       /* print	routines				*/

		       WinQueryDlgItemText(hWnd, EF_HEADER, 256L, pags.szHeader);
		       WinQueryDlgItemText(hWnd, EF_FOOTER, 256L, pags.szFooter);

		       if ( fBuildPreview(hWnd)	)
			   WinInvalidateRect(hWnd, &rclPreview,	TRUE);
		       break;
		   }
	       break;
	   }
       break;
		       /* Process push button selections		*/
   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
		       /* OK push button selected			*/
	   case	DID_OK :
		       /* Get the values the user has entered within	*/
		       /* the entry fields and save internally		*/

	       WinQueryDlgItemText(hWnd, EF_MARGINTOP,	  8L, szBuffer);
	       pags.rdTopMargin	= atof(szBuffer);
	       WinQueryDlgItemText(hWnd, EF_MARGINBOTTOM, 8L, szBuffer);
	       pags.rdBottomMargin = atof(szBuffer);
	       WinQueryDlgItemText(hWnd, EF_MARGINLEFT,	  8L, szBuffer);
	       pags.rdLeftMargin = atof(szBuffer);
	       WinQueryDlgItemText(hWnd, EF_MARGINRIGHT,  8L, szBuffer);
	       pags.rdRightMargin = atof(szBuffer);

		       /* Get the header and footer text from the	*/
		       /* entry	fields and save	internally for the	*/
		       /* print	routines				*/

	       WinQueryDlgItemText(hWnd, EF_HEADER, 256L, pags.szHeader);
	       WinQueryDlgItemText(hWnd, EF_FOOTER, 256L, pags.szFooter);

		       /* Delete the preview bitmap			*/

	       GpiDeleteBitmap(hbmPreview);
	       WinDismissDlg(hWnd, TRUE);
	       break;
		       /* Cancel push button selected			*/
	   case	DID_CANCEL :
		       /* Restore the page setup and delete the	preview	*/
		       /* bitmap					*/

	       pags = pagsTemp;
	       GpiDeleteBitmap(hbmPreview);
	       WinDismissDlg(hWnd, FALSE);
	       break;
		       /* Setup	push button selected			*/

	   case	DID_SETUP :
		       /* Since	the form selection and usage is	not	*/
		       /* consistant, the printer setup	dialogue must	*/
		       /* used.	 Display the job properties dialogue to	*/
		       /* allow	the form to be selected.		*/

	       PrnQueryJobProperties(&prn, prn.iQueue);
	       if ( fBuildPreview(hWnd)	)
		   WinInvalidateRect(hWnd, &rclPreview,	TRUE);
	       break;
	   }
       break;
		       /* Close	requested, exit	dialogue		*/
   case	WM_CLOSE :
		       /* Restore the page setup and delete the	preview	*/
		       /* bitmap					*/
       pags = pagsTemp;
       GpiDeleteBitmap(hbmPreview);
       WinDismissDlg(hWnd, FALSE);
       break;
		       /* Paint	required				*/
   case	WM_PAINT :
       mr = WinDefDlgProc(hWnd,	msg, mp1, mp2);
       WinDrawBitmap(hPS = WinGetPS(hWnd), hbmPreview,
		     (PRECTL)NULL, (PPOINTL)(PVOID)&rclDest,
		     0L, 0L, DBM_NORMAL	| DBM_STRETCH);

       GpiSetColor(hPS,	RGBCLR_PALEGRAY);

		       /* Set up the starting position and start	*/
		       /* drawing the lighted edge of the window	*/

       GpiPolyLineDisjoint(hPS,	8L, aptlArea);

		       /* Get the presentation space and set the first	*/
		       /* outline edge colour to white			*/

       GpiCreateLogColorTable(hPS, 0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);
       GpiSetColor(hPS,	RGBCLR_SHADOW);

		       /* Set up the starting position and start	*/
		       /* drawing the lighted edge of the window	*/

       GpiMove(hPS, &aptlShade[3]);
       GpiPolyLine(hPS,	2L, aptlShade);

		       /* Set the shaded edge colour and start the	*/
		       /* drawing of the shaded	edge of	the window	*/

       GpiSetColor(hPS,	RGB_WHITE);
       GpiPolyLine(hPS,	2L, &aptlShade[2]);
       WinReleasePS(hPS);
       return(mr);
			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
