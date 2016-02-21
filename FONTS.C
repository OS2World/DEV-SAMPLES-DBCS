#pragma	title("DBCS Driver  --  Version 1  --  (Fonts.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#define	INCL_DEV		   /* Include OS/2 Device Interface	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_DOSERRORS		   /* Include OS/2 DOS Errors		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#pragma	info(noext)

#include <os2.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	the font handling routines.			*/

/* Filename:   Fonts.C							*/

/*  Version:   3							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   LONG lSetFonts(HWND hwndFontNames);			*/
/*	       VOID SetFontSize(HWND hwndFont, HWND hwndFontSize);	*/
/*	       BOOL fQuerySelectedFont(HWND hwndFont,			*/
/*				       HWND hwndFontSize);		*/
/*	       LONG lSelectFont(HPS hPS, HDC hDC, PSZ pszFacename,	*/
/*				LONG lNominalPointSize,	ULONG fl);	*/


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

PFONTMETRICS pfm;		   /* Font Metrics Pointer		*/
LONG	     cFonts;		   /* Font Count			*/
FONTSEL	     fsel;		   /* Font Selection			*/

FONTSIZE afsiz[	] = { {	 "6",  6 },
		      {	 "8",  8 },
		      {	"10", 10 },
		      {	"12", 12 },
		      {	"14", 14 },
		      {	"16", 16 },
		      {	"18", 18 },
		      {	"20", 20 },
		      {	"24", 24 },
		      {	"32", 32 },
		      {	"40", 40 },
		      {	"48", 48 } };

BOOL   afLCID[255] = { TRUE };

#pragma	subtitle("   File Print - Font Selection Function")
#pragma	page( )

/* --- SetFonts	---------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	enumerate the fonts for	the selected printer	*/
/*     and fill	the fonts combo	box with the font names.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd;	   = Dialogue Window Handle			*/
/*     HWND hwndFontNames; = Font Names	Combo Box Handle		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

LONG lSetFonts(HWND hwndFontNames)

{
BOOL  fFonts = FALSE;		   /* Fonts Found Flag			*/
BOOL  fSelected;		   /* Font Selected Flag		*/
CHAR  szFacename[128];		   /* Font Facename Buffer		*/
HDC   hdcPrinter;		   /* Device Context Handle		*/
HPS   hPS;			   /* Presentation Space Handle		*/
LONG  lFonts = 0L;		   /* Fonts Count			*/
PSZ   pszFacename;		   /* Facename				*/
SIZEL sizlPage;			   /* Size Holder			*/
register INT i,	n;		   /* Loop Counter			*/

sizlPage.cx = sizlPage.cy = 0L;
if ( !(hdcPrinter = PrnOpenInfoDC(&prn,	"PM_Q_STD")) )
   return(1L);
else
   if (	!(hPS =	GpiCreatePS(hAB, hdcPrinter, &sizlPage,	PU_TWIPS |
			    GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC)) )
       {
       DevCloseDC(hdcPrinter);
       return(2L);
       }
		       /* Get the number of fonts for the face name	*/
		       /* provided					*/

cFonts = GpiQueryFonts(hPS, QF_PUBLIC, NULL, &lFonts,
		       sizeof(FONTMETRICS), (PFONTMETRICS)NULL);

		       /* Allocate space for the font metrics for the	*/
		       /* different font sizes and devices of the font	*/
if ( pfm )
   DosFreeMem((PVOID)pfm);
DosAllocMem((PVOID)&pfm, (ULONG)(sizeof(FONTMETRICS) * cFonts),	PAG_READ | PAG_WRITE | PAG_COMMIT);

		       /* Get the font metrics for the number of fonts	*/
		       /* for the face name provided			*/

GpiQueryFonts(hPS, QF_PUBLIC, NULL, &cFonts,
	      sizeof(FONTMETRICS), pfm);

WinSendMsg(hwndFontNames, LM_DELETEALL,	0L, 0L);

		       /* Loop through the font	metrics	returned to	*/
		       /* and place each of the	different font names	*/
		       /* within the fonts combo box			*/

for ( i	= 0, fSelected = fFonts	= FALSE; i < (INT)cFonts; i++ )
   if (	pfm[i].lMatch <	0 )
       {
       if ( pfm[i].fsType & FM_TYPE_FACETRUNC )
	   WinQueryAtomName(WinQuerySystemAtomTable( ),	(ULONG)pfm[i].FaceNameAtom, pszFacename	= szFacename, 128UL);
       else
	   pszFacename = pfm[i].szFacename;
       if ( (INT)LONGFROMMR(WinSendMsg(hwndFontNames, LM_SEARCHSTRING,
				       MPFROM2SHORT(LSS_CASESENSITIVE, LIT_FIRST),
				       MPFROMP(pszFacename))) == LIT_NONE )
	   {
	   fFonts = TRUE;

		       /* Place	the font within	the combo box and set	*/
		       /* the item handle as the font match number	*/

	   WinSendMsg(hwndFontNames, LM_SETITEMHANDLE,
		      MPFROMSHORT(n = (INT)LONGFROMMR(WinSendMsg(hwndFontNames,	LM_INSERTITEM,
								 MPFROMSHORT(LIT_SORTASCENDING),
								 MPFROMP(pfm[i].szFacename)))),
		      MPFROMLONG(pfm[i].lMatch));

		       /* Check	to see if the last font	used is	the	*/
		       /* font placed in the combo box and if so,	*/
		       /* select it					*/

	   if (	!strcmp(fsel.szFacename, pfm[i].szFacename) )
	       {
	       WinSendMsg(hwndFontNames, LM_SELECTITEM,
			  MPFROMSHORT(n), MPFROMSHORT(TRUE));
	       fSelected = TRUE;
	       }
	   }
       }
		       /* Check	to see if any fonts selected and if	*/
		       /* none were, get the default font for the	*/
		       /* printer					*/
if ( !fFonts )
    {
    GpiQueryFontMetrics(hPS, sizeof(FONTMETRICS), pfm);

		       /* Place	the font within	the combo box and set	*/
		       /* the item handle as the font match number	*/

    WinSendMsg(hwndFontNames, LM_SETITEMHANDLE,
	       MPFROMSHORT(n = (INT)LONGFROMMR(WinSendMsg(hwndFontNames, LM_INSERTITEM,
							  MPFROMSHORT(LIT_SORTASCENDING), MPFROMP(pfm[0].szFacename)))),
	       MPFROMLONG(pfm[0].lMatch));

    WinSendMsg(hwndFontNames, LM_SELECTITEM, MPFROMSHORT(n), MPFROMSHORT(TRUE));
    fSelected =	TRUE;
    cFonts = 1L;
    }

if ( !fSelected	)
    WinSendMsg(hwndFontNames, LM_SELECTITEM,
	       MPFROMSHORT(0), MPFROMSHORT(TRUE));

GpiAssociate(hPS, (HDC)NULL);
GpiDestroyPS(hPS);
DevCloseDC(hdcPrinter);

return(0L);
}
#pragma	subtitle("   File Print -  File Print Dialogue Procedure")
#pragma	page( )

/* --- SetFontSize ------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	set the	font sizes within the font	*/
/*     sizes combo box for a font selected.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hwndFont;	  = Font Combo Box Handle			*/
/*     HWND hwndFontSize; = Font Size Combo Box	Handle			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID SetFontSize(HWND hwndFont,	HWND hwndFontSize)

{
CHAR	 szStrBuf[CCHMAXPATH];	   /* String Buffer			*/
LONG	 lMatch;		   /* Font Match			*/
register INT i,	k, n;		   /* Indices				*/

		       /* Check	to see that a valid entry has been	*/
		       /* selected within the combo box			*/

if ( (i	= (INT)LONGFROMMR(WinSendMsg(hwndFont,
		    LM_QUERYSELECTION,
		    0L,	0L))) != LIT_NONE )
   {
   WinSetPointer(HWND_DESKTOP, hptrWait);

		       /* Get the font match number for	the font	*/
		       /* selected from	the selected item handle	*/

   lMatch = (LONG)LONGFROMMR(WinSendMsg(hwndFont,
					       LM_QUERYITEMHANDLE,
					       MPFROMSHORT(i), 0L));

		       /* Clear	the entries within the font sizes box	*/

   WinSendMsg(hwndFontSize, LM_DELETEALL, 0L, 0L);

		       /* Find the font	metric for the selected	font	*/

   for ( i = 0;	i < (INT)cFonts; i++ )
       if ( pfm[i].lMatch == lMatch )
	   break;
		       /* Check	to see if the font is an outline font	*/
		       /* and if the case, place the outline font sizes	*/
		       /* within the font sizes	combo box		*/

   if (	pfm[i].fsDefn &	FM_DEFN_OUTLINE	)
       for ( n = 0; n <	12; n++	)
	   WinSendMsg(hwndFontSize,
		      LM_INSERTITEM,
		      MPFROMSHORT(LIT_END),
		      MPFROMP(afsiz[n].pszSize));
   else

		       /* Device font selected,	locate all the fonts	*/
		       /* that match the selected font within the	*/
		       /* font metrics array for each of the different	*/
		       /* sizes	supported by the printer and place each	*/
		       /* size within the combo	box			*/

       for ( n = k = 0;	n < (INT)cFonts; n++ )
	   if (	(pfm[n].lMatch < 0) &&
		!strcmp(pfm[i].szFacename,
			pfm[n].szFacename) &&
		!(pfm[n].fsDefn	& FM_DEFN_OUTLINE) )
	       {

		       /* Check	to see if the device font is the HP	*/
		       /* line printer font size which is a half size	*/

	       if ( pfm[n].sNominalPointSize ==	85 )
		   memcpy(szStrBuf, "8.5", 4);
	       else
		   _ltoa(pfm[n].sNominalPointSize / 10,
			 szStrBuf, 10);

		       /* Place	the font size within the combo box	*/

	       if ( (INT)LONGFROMMR(WinSendMsg(hwndFontSize,
			      LM_SEARCHSTRING,
			      MPFROM2SHORT(LSS_CASESENSITIVE, LIT_FIRST),
			      MPFROMP(szStrBuf))) ==
			      LIT_NONE )
		   {
		   WinSendMsg(hwndFontSize,
			      LM_INSERTITEM,
			      MPFROMSHORT(LIT_END),
			      MPFROMP(szStrBuf));
		   WinSendMsg(hwndFontSize,
			      LM_SETITEMHANDLE,
			      MPFROMSHORT(k++),
			      MPFROMLONG(pfm[n].lMatch));
		   }
	       }
		       /* Search the list of font sizes	for the	font	*/
		       /* size last selected and select	the size	*/

   if (	(i = (INT)LONGFROMMR(WinSendMsg(hwndFontSize,
				  LM_SEARCHSTRING,
				  MPFROM2SHORT(LSS_CASESENSITIVE, LIT_FIRST),
				  MPFROMP(_ltoa(fsel.lPointSize, szStrBuf, 10))))) ==
				  LIT_NONE )
       i = 0;

   WinSendMsg(hwndFontSize, LM_SELECTITEM,
	      MPFROMSHORT(i),
	      MPFROMSHORT(TRUE));
   WinSetPointer(HWND_DESKTOP, hptrArrow);
   }
}
#pragma	subtitle("   Fonts -  Font Selection Query Procedure")
#pragma	page( )

/* --- fQuerySelectedFont ------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	retrieve the necessary information for	*/
/*     the font	selected within	the font combo boxes.			*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND	hwndFont;     =	Font Combo Box Handle			*/
/*     HWND	hwndFontSize; =	Font Size Combo	Box Handle		*/
/*     PFONTSEL	pfsel;	      =	Font Selected Pointer			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     fQuerySelectedFont =  TRUE : Font Selected			*/
/*			  = FALSE : No font Selected			*/
/*									*/
/* --------------------------------------------------------------------	*/

BOOL fQuerySelectedFont(HWND hwndFont, HWND hwndFontSize, PFONTSEL pfsel)

{
register INT i,	n;		   /* Index				*/

		       /* Get the font selected				*/

if ( (i	= (INT)LONGFROMMR(WinSendMsg(hwndFont, LM_QUERYSELECTION, 0L, 0L))) != LIT_NONE	)
   {
		       /* Get the font match number of the font		*/
		       /* selected					*/

   pfsel->lMatch = (LONG)LONGFROMMR(WinSendMsg(hwndFont, LM_QUERYITEMHANDLE, MPFROMSHORT(i), 0L));

		       /* Locate the font metrics for the font selected	*/

   for ( i = 0;	i < (INT)cFonts; i++ )
       if ( pfm[i].lMatch == pfsel->lMatch )
	   break;
		       /* Check	to see if the font selected is a	*/
		       /* scalable font					*/

   if (	pfm[i].fsDefn &	FM_DEFN_OUTLINE	)
       {
		       /* Scalable font, get the index of the font size	*/
		       /* selected					*/

       n = (INT)LONGFROMMR(WinSendMsg(hwndFontSize,
				      LM_QUERYSELECTION,
				      0L, 0L));
       pfsel->fFixed = FALSE;

		       /* Save the point size and facename		*/

       pfsel->lPointSize = afsiz[n].lPointSize;
       pfsel->lNominalPointSize	= afsiz[n].lPointSize *	10;
       strcpy(pfsel->szFacename, pfm[i].szFacename);
       }
   else
       {
		       /* Get the font match number for	the size	*/
		       /* selected					*/

       pfsel->lMatch = (LONG)LONGFROMMR(WinSendMsg(hwndFontSize,
						   LM_QUERYITEMHANDLE,
						   MPFROMSHORT(WinSendMsg(hwndFontSize,
									  LM_QUERYSELECTION,
									  0L, 0L)),
						   0L));
       pfsel->fFixed = TRUE;

		       /* Locate the font metrics for the font selected	*/

       for ( i = 0; i <	(INT)cFonts; i++ )
	   if (	pfm[i].lMatch == pfsel->lMatch )
	       break;
		       /* Save the point size and facename		*/

       pfsel->lNominalPointSize	= pfm[i].sNominalPointSize;
       strcpy(pfsel->szFacename, pfm[i].szFacename);
       }
   return(TRUE);
   }
else
   return(FALSE);
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- lSelectFont ------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used select the	requested font.	 An image font	*/
/*     is first	searched for and if one	is not found that matches the	*/
/*     the font	face and size, a scalable font is then searched	for	*/
/*     and selected.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS hPS;		       = Presentation Space Handle		*/
/*     PSZ pszFacename;	       = Font Facename				*/
/*     LONG lNominalPointSize; = Nominal Font Size (Decipoints)		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     lSelectFont = Logical Font ID					*/
/*									*/
/* --------------------------------------------------------------------	*/

LONG lSelectFont(HPS hPS, HDC hDC, PSZ pszFacename, LONG lNominalPointSize,
		 ULONG fl, PBOOL pfScalable, PSIZEF psizfxBox)

{
LONG	     cFont;		   /* Fonts Count			*/
LONG	     lFontsTotal = 0L;	   /* Fonts Total Count			*/
LONG	     lXDeviceRes;	   /* x	Device Resolution		*/
LONG	     lYDeviceRes;	   /* y	Device Resolution		*/
PFONTMETRICS pfmSelect;		   /* Font Metrics Pointer		*/
FATTRS	     fat;		   /* Font Attributes			*/
register INT i;			   /* Loop Counter			*/

		       /* Get the height of the	screen in pels		*/

DevQueryCaps(hDC, CAPS_HORIZONTAL_FONT_RES, 1L,	&lXDeviceRes);
DevQueryCaps(hDC, CAPS_VERTICAL_FONT_RES,   1L,	&lYDeviceRes);

		       /* Get the number of fonts for the face name	*/
		       /* provided					*/

if ( (cFont = GpiQueryFonts(hPS, QF_PUBLIC, pszFacename, &lFontsTotal,
			   sizeof(FONTMETRICS),	(PFONTMETRICS)NULL)) !=	0L )
   {
		       /* Allocate space for the font metrics for the	*/
		       /* different font sizes and devices of the font	*/

   DosAllocMem((PPVOID)(PVOID)&pfmSelect, (ULONG)(sizeof(FONTMETRICS) *	cFont),
	       PAG_READ	| PAG_WRITE | PAG_COMMIT);

		       /* Make a pointer for the memory	allocated for	*/
		       /* the font metrics and get the font metrics for	*/
		       /* the number of	fonts for the face name		*/
		       /* provided					*/

   GpiQueryFonts(hPS, QF_PUBLIC, pszFacename, &cFont,
		 sizeof(FONTMETRICS), pfmSelect);

		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

   for ( i = 0;	i < (INT)cFont;	i++ )
       if ( (pfmSelect[i].sXDeviceRes == (SHORT)lXDeviceRes) &&
	    (pfmSelect[i].sYDeviceRes == (SHORT)lYDeviceRes) &&
	    ((LONG)pfmSelect[i].sNominalPointSize == lNominalPointSize)	)
	   {
		       /* Font found, get the match value to allow the	*/
		       /* exact	font to	be selected by the calling	*/
		       /* application					*/

	   memset(&fat,	0, sizeof(FATTRS));
	   fat.usRecordLength  = (USHORT)sizeof(FATTRS);
	   strcpy(fat.szFacename, pszFacename);
	   fat.lMatch	       = pfmSelect[i].lMatch;
	   fat.fsSelection     = (USHORT)fl;

	   DosFreeMem((PVOID)pfmSelect);
	   for ( i = 1;	i < 255; i++ )
	       if ( !afLCID[i] )
		   {
		   *pfScalable = FALSE;
		   afLCID[i] = TRUE;
		   GpiCreateLogFont(hPS, (PSTR8)NULL, (LONG)i, &fat);
		   GpiSetCharSet(hPS, (LONG)i);
		   return((LONG)i);
		   }
	   return(0);
	   }
		       /* Loop through the font	metrics	returned to	*/
		       /* locate the desired font by matching the x and	*/
		       /* y device resolution of the font and the point	*/
		       /* size						*/

   for ( i = 0;	i < (INT)cFont;	i++ )
       if ( (pfmSelect[i].sXDeviceRes == 1000) && (pfmSelect[i].sYDeviceRes == 1000) )
	   {
		       /* Font found, get the match value to allow the	*/
		       /* exact	font to	be selected by the calling	*/
		       /* application					*/

	   memset(&fat,	0, sizeof(FATTRS));
	   fat.usRecordLength  = (USHORT)sizeof(FATTRS);
	   strcpy(fat.szFacename, pszFacename);
	   fat.lMatch	       = pfmSelect[i].lMatch;
	   fat.fsFontUse       = (USHORT)(FATTR_FONTUSE_OUTLINE	| FATTR_FONTUSE_TRANSFORMABLE);
	   fat.fsSelection     = (USHORT)fl;

	   DosFreeMem((PVOID)pfmSelect);

	   for ( i = 1;	i < 255; i++ )
	       if ( !afLCID[i] )
		   {
		   *pfScalable = TRUE;
		   afLCID[i] = TRUE;
		   GpiCreateLogFont(hPS, (PSTR8)NULL, (LONG)i, &fat);
		   GpiSetCharSet(hPS, (LONG)i);

		   psizfxBox->cx = psizfxBox->cy = MAKEFIXED(lNominalPointSize * 2, 0);

		   GpiSetCharBox(hPS, psizfxBox);
		   return((LONG)i);
		   }
	   return(0);
	   }
		       /* Release the memory allocated for the font	*/
		       /* metrics array					*/

   DosFreeMem((PVOID)pfmSelect);
   }
		       /* Return the match value to the	calling		*/
		       /* application					*/
return(0);
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- DeSelectFont ------------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used deselect the specified font.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS hPS;	  = Presentation Space Handle				*/
/*     LONG lcid; = Logical Font ID					*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID DeSelectFont(HPS hPS, LONG	lcid)

{
if ( lcid )
   {
		       /* Reset	the character set back to the default	*/
   GpiSetCharSet(hPS, 0L);
   GpiDeleteSetId(hPS, lcid);

		       /* Clear	the usage flag within the logical ID	*/
		       /* table						*/
   afLCID[lcid]	= FALSE;
   }
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- BuildFontCache ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used build the font cache for the selected	*/
/*     font.								*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS  hpsTarget;	       = Target	Presentation Space Handle	*/
/*     HPS  hpsPrinter;	       = Printer Presentation Space Handle	*/
/*     HDC  hdcPrinter;	       = Print Device Context Handle		*/
/*     PSZ  pszFacename;       = Font Facename				*/
/*     LONG lNominalPointSize; = Font Nominal Size			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID BuildFontCache(HPS	hpsTarget, HPS hpsPrinter, HDC hdcPrinter,
		    PSZ	pszFacename, LONG lNominalPointSize,
		    PFONTCACHE pfcache)

{
HDC  hDC;			   /* Display Device Context Handle	*/
LONG lcid;			   /* Logical Font ID			*/

lcid = lSelectFont(hpsPrinter, hdcPrinter, pszFacename,	lNominalPointSize,
		   0UL,	&pfcache->lfontNormal.fScalable,
		   &pfcache->lfontNormal.sizfxBox);
GpiQueryWidthTable(hpsPrinter, 0L, 255L, pfcache->lfontNormal.alWidths);

if ( hpsTarget )
   {
   DeSelectFont(hpsPrinter, lcid);

   pfcache->lfontNormal.lcid = lSelectFont(hpsTarget, hDC = GpiQueryDevice(hpsTarget),
					    pszFacename, lNominalPointSize, 0UL,
					    &pfcache->lfontNormal.fScalable,
					    &pfcache->lfontNormal.sizfxBox);
   }
else
   pfcache->lfontNormal.lcid = lcid;

lcid = lSelectFont(hpsPrinter, hdcPrinter, pszFacename,	lNominalPointSize,
		   FATTR_SEL_BOLD, &pfcache->lfontBold.fScalable,
		   &pfcache->lfontBold.sizfxBox);
GpiQueryWidthTable(hpsPrinter, 0L, 255L, pfcache->lfontBold.alWidths);

if ( hpsTarget )
   {
   DeSelectFont(hpsPrinter, lcid);

   pfcache->lfontBold.lcid = lSelectFont(hpsTarget, hDC,
					 pszFacename, lNominalPointSize, FATTR_SEL_BOLD,
					 &pfcache->lfontBold.fScalable,
					 &pfcache->lfontBold.sizfxBox);
   }
else
   pfcache->lfontBold.lcid = lcid;

lcid = lSelectFont(hpsPrinter, hdcPrinter, pszFacename,	lNominalPointSize,
		   FATTR_SEL_ITALIC, &pfcache->lfontItalic.fScalable,
		   &pfcache->lfontItalic.sizfxBox);
GpiQueryWidthTable(hpsPrinter, 0L, 255L, pfcache->lfontItalic.alWidths);

if ( hpsTarget )
   {
   DeSelectFont(hpsPrinter, lcid);

   pfcache->lfontItalic.lcid = lSelectFont(hpsTarget, hDC,
					   pszFacename,	lNominalPointSize, FATTR_SEL_ITALIC,
					   &pfcache->lfontItalic.fScalable,
					   &pfcache->lfontItalic.sizfxBox);
   }
else
   pfcache->lfontItalic.lcid = lcid;
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- DeleteFontCache --------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used delete the	font current cache.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS  hpsTarget;	= Target Presentation Space Handle		*/
/*     HPS  hpsPrinter;	= Printer Presentation Space Handle		*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID DeleteFontCache(HPS hpsTarget, PFONTCACHE pfcache)

{
DeSelectFont(hpsTarget,	pfcache->lfontNormal.lcid);
DeSelectFont(hpsTarget,	pfcache->lfontBold.lcid);
DeSelectFont(hpsTarget,	pfcache->lfontItalic.lcid);
}
