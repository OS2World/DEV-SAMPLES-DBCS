#pragma	title("DBCS Driver  --  Version 1  --  (DBCS.C)")
#pragma	subtitle("   DBCS Support - Interface Definitions")

#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
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

/* Filename:   DBCS.C							*/

/*  Version:   1							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-20						*/

/* Routines:   VOID SetCountryCode(ULONG ulCountry, ULONG ulCP);	*/
/*	       LONG FindDBCSFont(CHAR *pszFacename, PLONG plPoints);	*/
/*	       BOOL fQueryDBCS(VOID);					*/
/*	       ULONG ulQueryCharDBCS(const PSZ pszString, INT n);	*/
/*	       PSZ  strrchrDBCS(const CHAR *p, INT c);			*/
/*	       PSZ  strchrDBCS(const CHAR *p, INT c);			*/
/*	       PSZ  struprDBCS(CHAR *p);				*/
/*	       PSZ  strstrDBCS(const CHAR *str1, const CHAR *str2);	*/
/*	       VOID CenterDlg(HWND hWnd);				*/

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

/* -- Standard Window Classes -----------------------------------------	*/

#define	WINCLASS_FRAME		   0x00000001L
#define	WINCLASS_COMBOBOX	   0x00000002L
#define	WINCLASS_BUTTON		   0x00000003L
#define	WINCLASS_MENU		   0x00000004L
#define	WINCLASS_STATIC		   0x00000005L
#define	WINCLASS_ENTRYFIELD	   0x00000006L
#define	WINCLASS_LISTBOX	   0x00000007L
#define	WINCLASS_SCROLLBAR	   0x00000008L
#define	WINCLASS_TITLEBAR	   0x00000009L
#define	WINCLASS_MLE		   0x0000000AL
/* 000B	to 000F	reserved */
#define	WINCLASS_APPSTAT	   0x00000010L
#define	WINCLASS_KBDSTAT	   0x00000011L
#define	WINCLASS_PECIC		   0x00000012L
#define	WINCLASS_DBE_KKPOPUP	   0x00000013L
/* 0014	to 001F	reserved */
#define	WINCLASS_SPINBUTTON	   0x00000020L
/* 0021	to 0024	reserved */
#define	WINCLASS_CONTAINER	   0x00000025L
#define	WINCLASS_SLIDER		   0x00000026L
#define	WINCLASS_VALUESET	   0x00000027L
#define	WINCLASS_NOTEBOOK	   0x00000028L
#define	WINCLASS_HWXENTRY	   0x00000029L
#define	WINCLASS_SKETCH		   0x0000002aL
/* 002b	to 0030	reserved */
/* 0030	to 003F	reserved */
#define	WINCLASS_GRAPHICBUTTON	   0x00000040L
#define	WINCLASS_CIRCULARSLIDER	   0x00000041L

#define	PMCLASS_FRAME		  "#1"
#define	PMCLASS_COMBOBOX	  "#2"
#define	PMCLASS_BUTTON		  "#3"
#define	PMCLASS_MENU		  "#4"
#define	PMCLASS_STATIC		  "#5"
#define	PMCLASS_ENTRYFIELD	  "#6"
#define	PMCLASS_LISTBOX		  "#7"
#define	PMCLASS_SCROLLBAR	  "#8"
#define	PMCLASS_TITLEBAR	  "#9"
#define	PMCLASS_MLE		  "#10"
/* 000B	to 000F	reserved */
#define	PMCLASS_APPSTAT		  "#16"
#define	PMCLASS_KBDSTAT		  "#17"
#define	PMCLASS_PECIC		  "#18"
#define	PMCLASS_DBE_KKPOPUP	  "#19"
/* 0014	to 001F	reserved */
#define	PMCLASS_SPINBUTTON	  "#32"
/* 0021	to 0024	reserved */
#define	PMCLASS_CONTAINER	  "#37"
#define	PMCLASS_SLIDER		  "#38"
#define	PMCLASS_VALUESET	  "#39"
#define	PMCLASS_NOTEBOOK	  "#40"
#define	PMCLASS_HWXENTRY	  "#41"
#define	PMCLASS_SKETCH		  "#42"
/* 002b	to 0030	reserved */
/* 0030	to 003F	reserved */
#define	PMCLASS_GRAPHICBUTTON	  "#64"
#define	PMCLASS_CIRCULARSLIDER	  "#65"

#define	PMCLASS_LEN_FRAME	      3
#define	PMCLASS_LEN_COMBOBOX	      3
#define	PMCLASS_LEN_BUTTON	      3
#define	PMCLASS_LEN_MENU	      3
#define	PMCLASS_LEN_STATIC	      3
#define	PMCLASS_LEN_ENTRYFIELD	      3
#define	PMCLASS_LEN_LISTBOX	      3
#define	PMCLASS_LEN_SCROLLBAR	      3
#define	PMCLASS_LEN_TITLEBAR	      3
#define	PMCLASS_LEN_MLE		      4
/* 000B	to 000F	reserved */
#define	PMCLASS_LEN_APPSTAT	      4
#define	PMCLASS_LEN_KBDSTAT	      4
#define	PMCLASS_LEN_PECIC	      4
#define	PMCLASS_LEN_DBE_KKPOPUP	      4
/* 0014	to 001F	reserved */
#define	PMCLASS_LEN_SPINBUTTON	      4
/* 0021	to 0024	reserved */
#define	PMCLASS_LEN_CONTAINER	      4
#define	PMCLASS_LEN_SLIDER	      4
#define	PMCLASS_LEN_VALUESET	      4
#define	PMCLASS_LEN_NOTEBOOK	      4
#define	PMCLASS_LEN_HWXENTRY	      4
#define	PMCLASS_LEN_SKETCH	      4
/* 002b	to 0030	reserved */
/* 0030	to 003F	reserved */
#define	PMCLASS_LEN_GRAPHICBUTTON     4
#define	PMCLASS_LEN_CIRCULARSLIDER    4


#define	TYPE_SBCS	     0x0000UL
#define	TYPE_DBCS_1ST	     0x0001UL
#define	TYPE_DBCS_2ND	     0x0002UL

#define	MAX_LEADBYTE	256

#pragma	pack(1)

typedef	struct _DBCSVECTOR
   {
   BYTE	bLow;
   BYTE	bHigh;
   } DBCSVECTOR;

#pragma	pack( )

/* --- DBCS Support ---------------------------------------------------	*/

BOOL	    afLeadByte[MAX_LEADBYTE];  /* Lead bytes Array		*/
BOOL	    fDBCS = FALSE;	   /* Double Byte System		*/
COUNTRYCODE cc = { 0, 0	};	   /* Country Code			*/
DBCSVECTOR  aDBCSVector[8];	   /* DBCS Vector			*/

CHAR	    szFontNameSize[FACESIZE * 2];
ULONG	    cFontNameSize;

/* --- Module Prototype	Definitions -----------------------------------	*/

BOOL fDBCSChar(UCHAR uch);
ULONG ulQueryCharDBCS(const CHAR *pszString, INT n);

#pragma	subtitle("   DBCS Support - Country Code Set Function")
#pragma	page( )

/* --- SetCountryCode ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	set the	country	code and code page for	*/
/*     DBCS activities.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     ULONG ulCountry;	= Country Code					*/
/*     ULONG ulCP;	= Code Page					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID SetCountryCode(ULONG ulCountry, ULONG ulCP)

{

cc.country  = ulCountry;
cc.codepage = ulCP;

}
#pragma	subtitle("   DBCS Support - DBCS Font Selection Function")
#pragma	page( )

/* --- FindDBCSFont ------------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	find a DBCS raster font	for a given	*/
/*     point size.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     CHAR  *pszFacename; = Font Face Name				*/
/*     PLONG plPoints;	   = Points Size Requested Pointer		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     FindDBCSFont = lMatch Number for	Requested Font			*/
/*									*/
/* --------------------------------------------------------------------	*/

LONG FindDBCSFont(CHAR *pszFacename, PLONG plPoints)

{
HDC	     hDC;		   /* Display Device Context Handle	*/
HPS	     hPS;		   /* Presentation Space Handle		*/
LONG	     cDBCSFonts;	   /* DBCS Font	Count			*/
LONG	     cFonts;		   /* Fonts Count			*/
LONG	     lFontsTotal = 0L;	   /* Fonts Total Count			*/
LONG	     lMatch = 0L;	   /* Font Match Number			*/
LONG	     lPoints;		   /* Point Size			*/
LONG	     lXDeviceRes;	   /* x	Device Resolution		*/
LONG	     lYDeviceRes;	   /* y	Device Resolution		*/
PFONTMETRICS pfm;		   /* Font Metrics Pointer		*/
PFONTMETRICS pfmDBCS;		   /* Font Metrics Pointer		*/
register INT i,	n;		   /* Loop Counter			*/

		       /* Get the height of the	screen in pels		*/

if ( (hPS = WinGetScreenPS(HWND_DESKTOP)) != (HPS)NULL )
   if (	(hDC = GpiQueryDevice(hPS)) != (HDC)NULL )
       {
       DevQueryCaps(hDC, CAPS_HORIZONTAL_FONT_RES, 1L, &lXDeviceRes);
       DevQueryCaps(hDC, CAPS_VERTICAL_FONT_RES,   1L, &lYDeviceRes);

		       /* Get the number of fonts for the face name	*/
		       /* provided					*/

       cFonts =	GpiQueryFonts(hPS, QF_PUBLIC, (PSZ)NULL, &lFontsTotal, sizeof(FONTMETRICS), (PFONTMETRICS)NULL);

		       /* Make a pointer for the memory	allocated for	*/
		       /* the font metrics and get the font metrics for	*/
		       /* the number of	fonts for the face name		*/
		       /* provided					*/

       DosAllocMem((PPVOID)(PVOID)&pfm,	(ULONG)(sizeof(FONTMETRICS) * cFonts), PAG_READ	| PAG_WRITE | PAG_COMMIT);
       GpiQueryFonts(hPS, QF_PUBLIC, (PSZ)NULL,	&cFonts, sizeof(FONTMETRICS), pfm);

		       /* Release the presentation space acquired to	*/
		       /* determine the	screen height and to get the	*/
		       /* font metrics					*/
       WinReleasePS(hPS);
		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

       for ( i = 0, cDBCSFonts = 0L; i < (INT)cFonts; i++ )
	   if (	(pfm[i].sXDeviceRes == (SHORT)lXDeviceRes) && (pfm[i].sYDeviceRes == (SHORT)lYDeviceRes) &&
		(pfm[i].fsType & FM_TYPE_MBCS) && !(pfm[i].fsType & FM_TYPE_FIXED) )
	       ++cDBCSFonts;

       DosAllocMem((PPVOID)(PVOID)&pfmDBCS, (ULONG)(sizeof(FONTMETRICS)	* cDBCSFonts), PAG_READ	| PAG_WRITE | PAG_COMMIT);

		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

       for ( i = n = 0;	i < (INT)cFonts; i++ )
	   if (	(pfm[i].sXDeviceRes == (SHORT)lXDeviceRes) && (pfm[i].sYDeviceRes == (SHORT)lYDeviceRes) &&
		(pfm[i].fsType & FM_TYPE_MBCS) && !(pfm[i].fsType & FM_TYPE_FIXED) )
	       pfmDBCS[n++] = pfm[i];

		       /* Adjust the point size	to correspond to the	*/
		       /* the nominal point size that is contained	*/
		       /* within the font metrics structure		*/

       lPoints = *plPoints * 10L;

		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

       for ( i = 0; i <	(INT)cDBCSFonts; i++ )
	   if (	pfmDBCS[i].sNominalPointSize ==	lPoints	)
	       {
		       /* Font found, get the match value to allow the	*/
		       /* exact	font to	be selected by the calling	*/
		       /* application					*/

	       strcpy(pszFacename, pfmDBCS[i].szFacename);
	       lMatch =	pfmDBCS[i].lMatch;
	       }
		       /* Font of specified size not found, check to	*/
		       /* see if any DBCS fonts	defined	in which case	*/
		       /* select the first font				*/
       if ( cDBCSFonts )
	   {
	   strcpy(pszFacename, pfmDBCS[0].szFacename);
	   *plPoints = pfmDBCS[0].sNominalPointSize / 10L;
	   lMatch = pfmDBCS[0].lMatch;
	   }
       else
		       /* No DBCS fonts	defined, select	the first	*/
		       /* raster font to use				*/
	   {
	   strcpy(pszFacename, pfm[0].szFacename);
	   *plPoints = pfm[0].sNominalPointSize	/ 10L;
	   lMatch = pfm[0].lMatch;
	   }
       }

DosFreeMem((PVOID)pfm);
DosFreeMem((PVOID)pfmDBCS);

return(lMatch);
}
#pragma	subtitle("   DBCS Support - DBCS Active Query Function")
#pragma	page( )

/* --- fQueryDBCS -------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	query the DBCS vector and determine	*/
/*     if DBCS is active.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     QueryDBCS =  TRUE : Double Byte System				*/
/*		 = FALSE : single Byte System				*/
/*									*/
/* --------------------------------------------------------------------	*/

BOOL fQueryDBCS(VOID)

{
CHAR szFacename[FACESIZE];	   /* Font Facename Size		*/
LONG lPts = 8L;			   /* Font Point Size			*/
register INT i,	n;		   /* Loop Counters			*/

if ( DosQueryDBCSEnv(8UL * sizeof(DBCSVECTOR), &cc, (PCHAR)aDBCSVector)	)
   return(fDBCS	= FALSE);
else
   {
   for ( i = 0;	i < 8; i++ )
       if ( aDBCSVector[i].bLow	&& aDBCSVector[i].bHigh	)
	   {
	   for ( n = (INT)aDBCSVector[i].bLow; n <= (INT)aDBCSVector[i].bHigh; n++ )
	       afLeadByte[n] = TRUE;
	   fDBCS = TRUE;
	   }
       else
	   break;

   FindDBCSFont(szFacename, &lPts);
   cFontNameSize = strlen(strcat(strcat(_ltoa(lPts, szFontNameSize, 10), "."), szFacename)) + 1;
   return(fDBCS);
   }
}
#pragma	subtitle("   DBCS Support - DBCS Lead Byte Query Function")
#pragma	page( )

/* --- ulQueryCharDBCS --------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	determine the nth byte type within a	*/
/*     DBCS string.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PSZ pszString; =	String to Scan					*/
/*     INT n;	      =	Character Index					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ulQueryCharDBCS = TYPE_SBCS     : Single	Byte Character		*/
/*		       = TYPE_DBCS_1ST : Double	Byte Lead Character	*/
/*		       = TYPE_DBCS_2ND : Double	Byte Trail Character	*/
/*									*/
/* --------------------------------------------------------------------	*/

ULONG ulQueryCharDBCS(const CHAR *pszString, INT n)

{
ULONG ulDBCSType = TYPE_SBCS;	   /* DBCS Type				*/
register INT i;			   /* Loop Counter			*/

for ( i	= 0; i <= n; i++ )
   switch ( ulDBCSType )
       {
       case TYPE_SBCS :
       case TYPE_DBCS_2ND :
	   ulDBCSType =	afLeadByte[pszString[i]];
	   break;

       case TYPE_DBCS_1ST :
	   ulDBCSType =	TYPE_DBCS_2ND;
	   break;
       }

return(ulDBCSType);
}
#pragma	subtitle("   DBCS Support - DBCS strrchr Function")
#pragma	page( )

/* --- strrchrDBCS ------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	replace	the strrchr( ) C function to	*/
/*     allow for DBCS character	string searches.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     const CHAR *p; =	String String Scan				*/
/*     INT	   c; =	Character to Search For				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     strrchrDBCS =  NULL : Character Not Found			*/
/*		   =	   : Address Where Character Located		*/
/*									*/
/* --------------------------------------------------------------------	*/

PSZ strrchrDBCS(const CHAR *p, INT c)

{
CHAR *pch;			   /* Character	String Pointer		*/
ULONG ulDBCS;			   /* DBCS Type	Holder			*/

if ( !fDBCS )
   return(strrchr(p, c));
else
   {
   for ( pch = (CHAR *)(p + strlen(p));	pch != p; pch--	)
       if ( *pch == (CHAR)c )
	   if (	(ulDBCS	= ulQueryCharDBCS(p, (INT)(pch - (CHAR *)p))) == TYPE_SBCS )
	       return(pch);
	   else
	       if ( ulDBCS == TYPE_DBCS_2ND )
		   --pch;

   return(NULL);
   }
}
#pragma	subtitle("   DBCS Support - DBCS strchr Function")
#pragma	page( )

/* --- strchrDBCS -----	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	replace	the strchr( ) C	function to	*/
/*     allow for DBCS character	string searches.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     const CHAR *p; =	String String Scan				*/
/*     INT	   c; =	Character to Search For				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     strchrDBCS =  NULL : Character Not Found				*/
/*		  =	  : Address Where Character Located		*/
/*									*/
/* --------------------------------------------------------------------	*/

PSZ strchrDBCS(const CHAR *p, INT c)

{
CHAR  *pch;			   /* Character	String Pointer		*/
ULONG ulDBCS;			   /* DBCS Type	Holder			*/

if ( !fDBCS )
   return(strchr(p, c));
else
   {
   for ( pch = (CHAR *)p; *pch;	pch++ )
       if ( *pch == (CHAR)c )
	   if (	(ulDBCS	= ulQueryCharDBCS(p, (INT)(pch - (CHAR *)p))) == TYPE_SBCS )
	       return(pch);
	   else
	       if ( ulDBCS == TYPE_DBCS_1ST )
		   ++pch;

   return(NULL);
   }
}
#pragma	subtitle("   DBCS Support - DBCS strchr Function")
#pragma	page( )

/* --- struprDBCS -----	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	replace	the strupr( ) C	function to	*/
/*     allow for DBCS character	string searches.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     CHAR *p;	= String To convert to Upper Case			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     struprDBCS = String Address					*/
/*									*/
/* --------------------------------------------------------------------	*/

PSZ struprDBCS(CHAR *p)

{

if ( !fDBCS )
   return(strupr(p));
else
   {
   DosMapCase((ULONG)strlen(p),	&cc, p);
   return(p);
   }
}
#pragma	subtitle("   DBCS Support - DBCS strchr Function")
#pragma	page( )

/* --- strchrDBCS -------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	replace	the strstr( ) C	function to	*/
/*     allow for DBCS character	string searches.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     CHAR *str1; = String To Search					*/
/*     CHAR *str2; = String to Search For				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     strstrDBCS =  NULL : String Address				*/
/*									*/
/* --------------------------------------------------------------------	*/

PSZ strstrDBCS(const CHAR *str1, const CHAR *str2)

{
INT   len1;			   /* String 1 Length			*/
INT   len2;			   /* String 2 Length			*/
ULONG ulDBCS;			   /* DBCS Type	Holder			*/
register INT i,	j, k;		   /* Index/Loop Counters		*/

if ( !fDBCS )
   return(strstr(str1, str2));
else
   {
   if (	!(len2 = strlen(str2)) )
       return((char *)str1);	/* return str1 if str2 empty */
   if (	!(len1 = strlen(str1)) )
       return(0);		/* return NULL if str1 empty */
   i = 0;
   for(	; ; )
       {
       while ( (i < len1) && (str1[i] != str2[0]) )
	   if (	(ulDBCS	= ulQueryCharDBCS(str1,	i)) == TYPE_SBCS )
	       ++i;
	   else
	       if ( ulDBCS == TYPE_DBCS_1ST )
		   i +=	2;
       if ( i >= len1 )
	   return(0);
       j = 0;
       k = i;
       while ( (i < len1) && (j	< len2)	&& (str1[i] == str2[j])	)
	   {
	   if (	(ulDBCS	= ulQueryCharDBCS(str1,	i)) == TYPE_SBCS )
	       ++i;
	   else
	       if ( ulDBCS == TYPE_DBCS_1ST )
		   i +=	2;

	   if (	(ulDBCS	= ulQueryCharDBCS(str2,	j)) == TYPE_SBCS )
	       ++j;
	   else
	       if ( ulDBCS == TYPE_DBCS_1ST )
		   j +=	2;
	   }
       if ( j == len2 )
	   return((PSZ)(str1 + k));
       if ( i == len1 )
	   return(0);
       }
   }
}
#pragma	subtitle("   Support - Center Dialog Box Function")
#pragma	page( )

/* --- CenterDlg --------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	centre a dialogue box about to be	*/
/*     displayed and select a particular entry field or	MLE.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd;     =	Dialogue Window	Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID CenterDlg(HWND hWnd)

{
CHAR   szClass[64];		   /* Class Name Holder			*/
HWND   hwndChild = hWnd;	   /* Child Window Handle		*/
SWP    swp;			   /* Screen Window Position Holder	*/
register LONG sCmd = QW_TOP;	   /* Query Window Command		*/

if ( fDBCS )
   {
   while ( (hwndChild =	WinQueryWindow(hwndChild, sCmd)) != (HWND)NULL )
       {
       WinQueryClassName(hwndChild, 64L, szClass);

		       /* Check	to see if the class of the control is a	*/
		       /* spin button in which case, the search	much	*/
		       /* broaden to the childen of the	spin button,	*/
		       /* namely the entry field			*/

       if ( !memcmp(szClass, PMCLASS_SPINBUTTON, PMCLASS_LEN_SPINBUTTON) ||
	    !memcmp(szClass, PMCLASS_MLE,	 PMCLASS_LEN_MLE)	 ||
	    !memcmp(szClass, PMCLASS_COMBOBOX,	 PMCLASS_LEN_COMBOBOX)	 ||
	    !memcmp(szClass, PMCLASS_ENTRYFIELD, PMCLASS_LEN_ENTRYFIELD) ||
	    !memcmp(szClass, PMCLASS_LISTBOX,	 PMCLASS_LEN_LISTBOX) )
	   WinSetPresParam(hwndChild, PP_FONTNAMESIZE, cFontNameSize, (PVOID)szFontNameSize);

       sCmd = QW_NEXT;
       }
   }

WinQueryWindowPos(hWnd,	(PSWP)&swp);
WinSetWindowPos(hWnd, HWND_TOP,	(WinQuerySysValue(HWND_DESKTOP,	SV_CXSCREEN) - swp.cx) / 2L,
		(WinQuerySysValue(HWND_DESKTOP,	SV_CYSCREEN) - swp.cy) / 2L, 0L, 0L, SWP_MOVE);
}

