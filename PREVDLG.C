#pragma	title("DBCS Driver  --  Version 1  --  (PrevDlg.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#define	INCL_DEV		   /* Include OS/2 Device Interface	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_DOSERRORS		   /* Include OS/2 DOS Errors		*/
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

/* This	module contains	the routines to	print preview dialogue		*/
/* including the construction of the preview image.			*/

/* Filename:   PrevDlg.C						*/

/*  Version:   3							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   HBITMAP hbmCreatePreview(LONG cx, LONG cy,		*/
/*					HPS hpsPrinter,	HDC hdcPrinter);*/
/*	       MRESULT EXPENTRY	PrintPreviewDlgProc(HWND hWnd,		*/
/*						    ULONG msg,		*/
/*						    MPARAM mp1,		*/
/*						    MPARAM mp2);	*/

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

static HBITMAP hbmPreview;	   /* Preview Bitmap Handle		*/
static RECTL   rclDest;		   /* Preview Bitmap Rectangle		*/
static POINTL  aptlShade[5];	   /* Shadow Points			*/
static POINTL  aptlArea[8];	   /* Print Boundary Points		*/
static LONG    lScale;		   /* Scaling Factor			*/

/************************************************************************/
/*									*/
/*     Module Prototype	Definitions					*/
/*									*/
/************************************************************************/

#pragma	subtitle("   Print Preview Dialogue - Preview Bitmap Create")
#pragma	page( )

/* --- hbmCreatePreview	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialogue	*/
/*     procedure.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     LONG cx;	= Bitmap Width						*/
/*     LONG cy;	= Bitmap Height						*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     hbmCreatePreview	= Bitmap Handle					*/
/*									*/
/* --------------------------------------------------------------------	*/

HBITMAP	hbmCreatePreview(LONG cx, LONG cy, HPS hpsPrinter, HDC hdcPrinter)

{
BITMAPINFOHEADER2 bminfo2;	   /* Bitmap Information Header		*/
FONTCACHE	  fc;		   /* Font Cache			*/
FONTSEL		  fselPreview;	   /* Preview Font Selections		*/
HBITMAP		  hbmConstruct;	   /* Bitmap Handle			*/
HDC		  hDC;		   /* Device Context Handle		*/
HPS		  hpsBitmap;	   /* Bitmap Presentation Space	Handle	*/
LONG		  cFormats;	   /* Formats Count			*/
PLONG		  plFormats;	   /* Formats Array			*/
POINTL		  ptl;		   /* Conversion Point			*/
SIZEL		  sizl;		   /* Sizing Structure			*/

		       /* Get bitmap device context handle for the main	*/
		       /* Client Window					*/

if ( !(hDC = DevOpenDC(hAB, OD_MEMORY, "*", 0L,	0L, 0L)) )
   return((HBITMAP)NULL);
		       /* Create bitmap	presentation space specifying	*/
		       /* entire map Client Window for size required	*/
sizl.cx	= cx;
sizl.cy	= cy;

if ( !(hpsBitmap = GpiCreatePS(hAB, hDC, &sizl,	PU_TWIPS | GPIT_NORMAL | GPIA_ASSOC)) )
   {
		       /* Error	occurred during	creation of		*/
		       /* presentation space, close device context	*/
   DevCloseDC(hDC);
   return((HBITMAP)NULL);
   }
		       /* Get the number of bitmap formats that	the	*/
		       /* display driver supports			*/

DevQueryCaps(hDC, CAPS_BITMAP_FORMATS, 1L, &cFormats);

		       /* Get the bitmap display formats.  The first	*/
		       /* set within the array will be the one that	*/
		       /* most closely matches the display device.	*/

GpiQueryDeviceBitmapFormats(hpsBitmap, cFormats	* 2L,
			    plFormats =	(PLONG)malloc(2UL * cFormats * sizeof(LONG)));

		       /* Create actual	bitmap storage for colour wheel	*/
		       /* having the default plane and bit count	*/
ptl.x =	cx;
ptl.y =	cy;
GpiConvert(hpsBitmap, CVTC_DEFAULTPAGE,	CVTC_DEVICE, 1L, &ptl);
memset(&bminfo2, 0, sizeof(BITMAPINFOHEADER2));
bminfo2.cbFix	  = sizeof(BITMAPINFOHEADER2);
bminfo2.cx	  = (ULONG)ptl.x;
bminfo2.cy	  = (ULONG)ptl.y;
bminfo2.cPlanes	  = (USHORT)plFormats[0];
bminfo2.cBitCount = (USHORT)plFormats[1];

free(plFormats);

if ( !(hbmConstruct = GpiCreateBitmap(hpsBitmap, &bminfo2, 0L, 0L, 0L))	)
   {
		       /* Error	occurred during	creation of bitmap	*/
		       /* storage, destroy presentation	space created	*/
		       /* and close device context opened		*/

   GpiDestroyPS(hpsBitmap);
   DevCloseDC(hDC);
   return((HBITMAP)NULL);
   }
		       /* Set bitmap as	current	bitmap to use		*/

GpiSetBitmap(hpsBitmap,	hbmConstruct);

		       /* Draw the page					*/
GpiErase(hpsBitmap);
if ( fQuerySelectedFont(WinWindowFromID(hwndToolBar, CBX_FONT),
			WinWindowFromID(hwndToolBar, CBX_FONTSIZE), &fselPreview) )
   {
   BuildFontCache(hpsBitmap, hpsPrinter, hdcPrinter,
		  fselPreview.szFacename, fselPreview.lNominalPointSize,
		  &fc);
   DrawCommon(hpsBitmap, hpsPrinter, &fc,
	      (LONG)(1440.0 * pags.rdLeftMargin),
	      (LONG)(1440.0 * pags.rdBottomMargin),
	      (LONG)(1440.0 * pags.rdRightMargin),
	      (LONG)(1440.0 * pags.rdTopMargin),
	      pags.szHeader, pags.szFooter,
	      TRUE);

   DeleteFontCache(hpsBitmap, &fc);
   }
		       /* Set the bitmap to allow completion of	bitmap	*/
		       /* in memory					*/

GpiSetBitmap(hpsBitmap,	(HDC)NULL);

		       /* Destroy the memory device context		*/

GpiAssociate(hpsBitmap,	(HDC)NULL);

		       /* Destroy the presentation spaces used		*/
GpiDestroyPS(hpsBitmap);
DevCloseDC(hDC);
		       /* Return the bitmap handle that	will be	used in	*/
		       /* painting the image on	the window		*/
return(hbmConstruct);
}
#pragma	subtitle("   Module Purpose - Dialogue Procedure")
#pragma	page( )

/* --- PrintPreviewDlgProc ----------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialogue	*/
/*     procedure.							*/
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
/*     PrintPreviewDlgProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY PrintPreviewDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
HDC	hdcPrinter;		   /* Printer Device Context Handle	*/
HPS	hPS;			   /* Presentation Space Handle		*/
LONG	cxDlg;			   /* Dialogue Width			*/
LONG	cyDlg;			   /* Dialogue Height			*/
MRESULT	mr;			   /* Message Result			*/
SIZEL	sizlPage;		   /* Page Size	Holder			*/
SWP	swp;			   /* Window Position			*/

switch ( msg )
   {
			/* Perform dialog initialization		*/
   case	WM_INITDLG :
       if ( (hdcPrinter	= PrnOpenInfoDC(&prn, "PM_Q_STD")) != (HDC)NULL	)
	   {
		       /* Get the size and position of the dialogue	*/

	   WinQueryWindowPos(hWnd, (PSWP)&swp);
	   cxDlg = swp.cx;
	   cyDlg = swp.cy;

		       /* Create a presentation	space into which the	*/
		       /* printing will	occur using TWIPS as the unit	*/
		       /* of measure					*/

	   sizlPage.cx = sizlPage.cy = 0L;

		       /* Determine the	size of	the output area		*/

	   GpiQueryPS(hPS = GpiCreatePS(hAB, hdcPrinter, &sizlPage, PU_TWIPS |
					GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC),
		      &sizlPage);

		       /* Create the preview bitmap			*/

	   hbmPreview =	hbmCreatePreview(sizlPage.cx, sizlPage.cy, hPS,	hdcPrinter);

		       /* Convert the image size from twips to pels	*/

	   GpiConvert(hPS, CVTC_DEFAULTPAGE, CVTC_DEVICE, 1L, (PPOINTL)(PVOID)&sizlPage);

		       /* Set the starting position of the bitmap	*/
		       /* image	for the	lower left corner of the	*/
		       /* dialogue at position 5, 5			*/

	   rclDest.xLeft = rclDest.yBottom = 5L;
	   WinMapDlgPoints(HWND_DESKTOP, (PPOINTL)(PVOID)&rclDest, 1UL,	TRUE);

		       /* Get the size and location of the Print push	*/
		       /* button					*/

	   WinQueryWindowPos(WinWindowFromID(hWnd, DID_OK), &swp);

		       /* Form the right and top edge of the preview	*/
		       /* image						*/

	   rclDest.xRight = swp.x - rclDest.xLeft;
	   rclDest.yTop	= swp.y	+ swp.cy;

		       /* Determine the	size of	the final image	along	*/
		       /* with the positions of	the push buttons	*/

	   if (	(sizlPage.cx / (rclDest.xRight - rclDest.xLeft)) >
		(sizlPage.cy / (rclDest.yTop - rclDest.yBottom)) )
	       {
		       /* Landscape mode, resize and psoition things	*/

	       rclDest.yTop = sizlPage.cy / (lScale = (sizlPage.cx / (rclDest.xRight - rclDest.xLeft)) + 1L);
	       cyDlg = rclDest.yTop + rclDest.yBottom +	WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
	       WinSetWindowPos(WinWindowFromID(hWnd, DID_OK), HWND_TOP,
			       swp.x, rclDest.yTop - swp.cy,
			       0L, 0L, SWP_MOVE);
	       WinSetWindowPos(WinWindowFromID(hWnd, DID_CANCEL), HWND_TOP,
			       swp.x, rclDest.yTop - swp.cy - 10L - swp.cy,
			       0L, 0L, SWP_MOVE);
	       }
	   else
	       {
		       /* Portrait mode, resize	and psoition things	*/

	       rclDest.xRight =	sizlPage.cx / (lScale =	(sizlPage.cy / (rclDest.yTop - rclDest.yBottom)) + 1L);
	       cxDlg = rclDest.xRight +	(rclDest.xLeft * 2L) + swp.cx;
	       WinSetWindowPos(WinWindowFromID(hWnd, DID_OK), HWND_TOP,
			       rclDest.xRight +	rclDest.xLeft, swp.y,
			       0L, 0L, SWP_MOVE);
	       WinSetWindowPos(WinWindowFromID(hWnd, DID_CANCEL), HWND_TOP,
			       rclDest.xRight +	rclDest.xLeft, swp.y - 10L - swp.cy,
			       0L, 0L, SWP_MOVE);
	       }
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

		       /* Convert the boundary points to pels from	*/
		       /* twips						*/

	   GpiConvert(hPS, CVTC_DEFAULTPAGE, CVTC_DEVICE, 8L, aptlArea);

		       /* Release the presentation space and printer	*/
		       /* device context				*/

	   GpiAssociate(hPS, (HDC)NULL);
	   GpiDestroyPS(hPS);
	   DevCloseDC(hdcPrinter);

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

	   aptlShade[0].x = rclDest.xLeft;
	   aptlShade[0].y = rclDest.yTop;
	   aptlShade[1].x = rclDest.xRight + 2L;
	   aptlShade[1].y = rclDest.yTop;
	   aptlShade[2].x = rclDest.xRight + 2L;
	   aptlShade[2].y = rclDest.yBottom - 2L;
	   aptlShade[3].x = rclDest.xLeft;
	   aptlShade[3].y = rclDest.yBottom - 2L;

		       /* Center the dialogue within the display	*/

	   WinSetWindowPos(hWnd, HWND_TOP, 0L, 0L, cxDlg, cyDlg, SWP_SIZE);
	   CenterDlg(hWnd);
	   }
       else
		       /* Printer device open failed			*/

	   WinDismissDlg(hWnd, FALSE);
       break;
			/* Process push	button selections		*/
   case	WM_COMMAND :
	switch ( SHORT1FROMMP(mp1) )
	    {
		       /* Print	push button selected			*/
	   case	DID_OK :
		       /* Delete the preview bitmap			*/

	       GpiDeleteBitmap(hbmPreview);
	       WinDismissDlg(hWnd, TRUE);
	       break;
		       /* Cancel push button selected			*/

	   case	DID_CANCEL :
		       /* Delete the preview bitmap			*/

	       GpiDeleteBitmap(hbmPreview);
	       WinDismissDlg(hWnd, FALSE);
	       break;
	    }
	break;
		       /* Close	requested, exit	dialogue		*/
   case	WM_CLOSE :
		       /* Delete the preview bitmap			*/

       GpiDeleteBitmap(hbmPreview);
       WinDismissDlg(hWnd, FALSE);
       break;
		       /* Paint	required				*/
   case	WM_PAINT :
       mr = WinDefDlgProc(hWnd,	msg, mp1, mp2);
       WinDrawBitmap(hPS = WinGetPS(hWnd), hbmPreview, (PRECTL)NULL, (PPOINTL)(PVOID)&rclDest, 0L, 0L, DBM_NORMAL | DBM_STRETCH);

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
