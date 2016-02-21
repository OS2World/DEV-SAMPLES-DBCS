#pragma	title("DBCS Driver  --  Version 1  --  (FontDlg.C)")
#pragma	subtitle("   Font Metrics Display - Interface Definitions")

#define	INCL_DEV		   /* Include OS/2 Device Interface	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_DOSERRORS		   /* Include OS/2 DOS Errors		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#pragma	info(noext)

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	the routine to display selected	font metric	*/
/* information.								*/

/* Filename:   FilePrn.C						*/

/*  Version:   3							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   MRESULT EXPENTRY	FontMetricsDlgProc(HWND	hWnd,		*/
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

static PFONTMETRICS pfmDisplay;	   /* Font Metrics Pointer		*/


/************************************************************************/
/*									*/
/*     Module Prototype	Definitions					*/
/*									*/
/************************************************************************/

#pragma	subtitle("   Font Metrics Display - Dialog Procedure")
#pragma	page( )

/* --- FontMetricsDlgProc ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	display	selected font metric info.	*/
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
/*     FontMetricsDlgProc = Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY FontMetricsDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
CHAR  szFacename[128];		   /* Font Facename Buffer		*/
CHAR  szInfo[256];		   /* Info Buffer			*/
HDC   hDC;			   /* Device Context Handle		*/
HPS   hPS;			   /* Presentation Space Handle		*/
LONG  cFontsDisplay;		   /* Fonts Count			*/
LONG  lFontsTotal = 0L;		   /* Fonts Count			*/
LONG  lXDeviceRes;		   /* x	Device Resolution		*/
LONG  lYDeviceRes;		   /* y	Device Resolution		*/
PSZ   pszFacename;		   /* Facename				*/
PSZ   pszSep;			   /* Separator				*/
register INT i;			   /* Loop Counter			*/

switch ( msg )
   {

/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       if ( (hPS = WinGetScreenPS(HWND_DESKTOP)) != (HPS)NULL )
	   if (	(hDC = GpiQueryDevice(hPS)) != (HDC)NULL )
	       {
	       DevQueryCaps(hDC, CAPS_HORIZONTAL_FONT_RES, 1L, &lXDeviceRes);
	       DevQueryCaps(hDC, CAPS_VERTICAL_FONT_RES,   1L, &lYDeviceRes);

		       /* Get the number of fonts for the face name	*/
		       /* provided					*/


		       /* Get the number of fonts for the face name	*/
		       /* provided					*/

	       cFontsDisplay = GpiQueryFonts(hPS, QF_PUBLIC, (PSZ)NULL,	&lFontsTotal, sizeof(FONTMETRICS), (PFONTMETRICS)NULL);

		       /* Allocate space for the font metrics for the	*/
		       /* different font sizes and devices of the font	*/

	       DosAllocMem((PVOID)&pfmDisplay, (ULONG)(sizeof(FONTMETRICS) * cFontsDisplay), PAG_READ |	PAG_WRITE | PAG_COMMIT);

		       /* Make a pointer for the memory	allocated for	*/
		       /* the font metrics and get the font metrics for	*/
		       /* the number of	fonts for the face name		*/
		       /* provided					*/

	       GpiQueryFonts(hPS, QF_PUBLIC, (PSZ)NULL,	&cFontsDisplay,	sizeof(FONTMETRICS), pfmDisplay);

		       /* Release the presentation space acquired to	*/
		       /* determine the	screen height and to get the	*/
		       /* font metrics					*/

	       WinReleasePS(hPS);

	       WinSendDlgItemMsg(hWnd, CBX_FONT, LM_DELETEALL, 0L, 0L);

		       /* Loop through the font	metrics	returned to	*/
		       /* and place each of the	different font names	*/
		       /* within the fonts combo box			*/

	       for ( i = 0; i <	(INT)cFontsDisplay; i++	)
		   if (	((pfmDisplay[i].sXDeviceRes == (SHORT)lXDeviceRes) && (pfmDisplay[i].sYDeviceRes == (SHORT)lYDeviceRes)) ||
			((pfmDisplay[i].sXDeviceRes == 1000) &&	(pfmDisplay[i].sYDeviceRes == 1000)) )
		       {
		       if ( pfmDisplay[i].fsType & (FM_TYPE_FACETRUNC |	FM_TYPE_ATOMS) )
			   WinQueryAtomName(WinQuerySystemAtomTable( ),	(ULONG)pfmDisplay[i].FaceNameAtom, pszFacename = szFacename, 128UL);
		       else
			   pszFacename = pfmDisplay[i].szFacename;
		       WinSendDlgItemMsg(hWnd, CBX_FONT, LM_SETITEMHANDLE,
				  MPFROMSHORT(WinSendDlgItemMsg(hWnd, CBX_FONT,	LM_INSERTITEM, MPFROMSHORT(LIT_END),
								MPFROMP(pfmDisplay[i].szFacename))), MPFROMLONG(i));
		       }
	       }
       CenterDlg(hWnd);
       break;

/************************************************************************/
/* Process control selections						*/
/************************************************************************/

   case	WM_CONTROL :
       switch (	SHORT2FROMMP(mp1) )
	   {
		       /* Process message combo	box entry selections	*/

	   case	CBN_LBSELECT :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	CBX_FONT :
		       if ( (i = (INT)LONGFROMMR(WinSendDlgItemMsg(hWnd, CBX_FONT, LM_QUERYSELECTION, 0L, 0L)))	!= LIT_NONE )
			   {
			   i = (INT)LONGFROMMR(WinSendDlgItemMsg(hWnd, CBX_FONT, LM_QUERYITEMHANDLE, MPFROMSHORT(i), 0L));
			   if (	pfmDisplay[i].fsType & (FM_TYPE_FACETRUNC | FM_TYPE_ATOMS) )
			       WinQueryAtomName(WinQuerySystemAtomTable( ), (ULONG)pfmDisplay[i].FamilyNameAtom, pszFacename = szFacename, 128UL);
			   else
			       pszFacename = pfmDisplay[i].szFamilyname;

			   WinSetDlgItemText(hWnd, ST_FAMILYNAME, pszFacename);

			   if (	pfmDisplay[i].fsType & (FM_TYPE_FAMTRUNC | FM_TYPE_ATOMS) )
			       WinQueryAtomName(WinQuerySystemAtomTable( ), (ULONG)pfmDisplay[i].FaceNameAtom, pszFacename = szFacename, 128UL);
			   else
			       pszFacename = pfmDisplay[i].szFacename;
			   WinSetDlgItemText(hWnd, ST_FACENAME,	pszFacename);
			   WinSetDlgItemText(hWnd, ST_SIZE, _ltoa(pfmDisplay[i].sNominalPointSize / 10,	szInfo,	10));
			   szInfo[0] = (CHAR)0;
			   pszSep = "";
			   if (	pfmDisplay[i].fsType & FM_TYPE_FIXED )
			       {
			       strcat(szInfo, "Fixed");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_LICENSED	)
			       {
			       strcat(strcat(szInfo, pszSep), "Licensed");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_KERNING )
			       {
			       strcat(strcat(szInfo, pszSep), "Kerning");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_64K )
			       {
			       strcat(strcat(szInfo, pszSep), "64K");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_DBCS )
			       {
			       strcat(strcat(szInfo, pszSep), "DBCS");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_MBCS )
			       {
			       strcat(strcat(szInfo, pszSep), "MBCS");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_FACETRUNC )
			       {
			       strcat(strcat(szInfo, pszSep), "Facename truncated");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_FAMTRUNC	)
			       {
			       strcat(strcat(szInfo, pszSep), "Family name truncated");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsType & FM_TYPE_ATOMS )
			       strcat(strcat(szInfo, pszSep), "Atoms");
			   WinSetDlgItemText(hWnd, ST_TYPE, szInfo);
			   szInfo[0] = (CHAR)0;
			   pszSep = "";
			   if (	pfmDisplay[i].fsDefn & FM_DEFN_OUTLINE )
			       {
			       strcat(szInfo, "Outline");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsDefn & FM_DEFN_GENERIC )
			       strcat(strcat(szInfo, pszSep), "Generic");
			   WinSetDlgItemText(hWnd, ST_DEFINITION, szInfo);
			   szInfo[0] = (CHAR)0;
			   pszSep = "";
			   if (	pfmDisplay[i].fsSelection & FM_SEL_ITALIC )
			       {
			       strcat(szInfo, "Italic");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_UNDERSCORE )
			       {
			       strcat(szInfo, "Underscore");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_NEGATIVE )
			       {
			       strcat(szInfo, "Negative");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_OUTLINE )
			       {
			       strcat(szInfo, "Outline");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_STRIKEOUT )
			       {
			       strcat(szInfo, "Strikeout");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_BOLD	)
			       {
			       strcat(szInfo, "Bold");
			       pszSep =	", ";
			       }
			   if (	pfmDisplay[i].fsSelection & FM_SEL_ISO9241_TESTED )
			       strcat(strcat(szInfo, pszSep), "ISO9241 tested");
			   WinSetDlgItemText(hWnd, ST_SELECTION, szInfo);
			   WinSetDlgItemShort(hWnd, ST_LMATCH, pfmDisplay[i].lMatch, FALSE);
			   }
		       break;
		   }
	   }
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_OK :
	       if ( pfmDisplay )
		   DosFreeMem((PVOID)pfmDisplay);
	       WinDismissDlg(hWnd, TRUE);
	       break;

	   case	DID_CANCEL :
	       if ( pfmDisplay )
		   DosFreeMem((PVOID)pfmDisplay);
	       WinDismissDlg(hWnd, FALSE);
	       break;
	   }
       break;

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       if ( pfmDisplay )
	   DosFreeMem((PVOID)pfmDisplay);
       WinDismissDlg(hWnd, FALSE);
       break;

			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
