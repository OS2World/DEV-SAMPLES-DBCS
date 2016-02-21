#pragma	title("DBCS Driver  --  Version 1  --  (DrawPage.C)")
#pragma	subtitle("   Application Window - Interface Definitions")

#pragma	info(noext)

#define	INCL_DEV		   /* Include OS/2 Device Interface	*/
#define	INCL_DOS		   /* Include OS/2 DOS Kernal		*/
#define	INCL_DOSERRORS		   /* Include OS/2 DOS Errors		*/
#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>
#include <string.h>

#include "appdefs.h"
#include "prnsetup.h"

/* This	module contains	the various drawing routines for the example	*/
/* printing.								*/

/* Filename:   DrawPage.C						*/

/*  Version:   1							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   VOID DrawCommon(HPS hPS,	HPS hpsPrinter,			*/
/*			       PFONTCACHE pfcache, LONG	lLeftMargin,	*/
/*			       LONG lBottomMargin, LONG	lRightMargin,	*/
/*			       LONG lTopMargin,	PSZ pszHeader,		*/
/*			       PSZ pszFooter, BOOL fDrawHeader);	*/
/*	       static VOID DrawFormattedText(HPS hpsTarget,		*/
/*					     HPS hpsPrinter,		*/
/*					     LONG cString,		*/
/*					     PSZ pszString, ULONG fl,	*/
/*					     PFONTCACHE	pfcache);	*/
/*	       VOID DrawText(HPS hpsTarget);				*/
/*	       VOID _System PrnDisplay(PPRNDATA	pprni);			*/

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

/* Copyright ¸ International Business Machines Corp., 1995.		*/
/* Copyright ¸ 1995  Prominare Inc.  All Rights	Reserved.		*/

/* --------------------------------------------------------------------	*/

/************************************************************************/
/*									*/
/*     Module Data Definitions						*/
/*									*/
/************************************************************************/

HPS hpsPrint;			   /* Print Window Presentation	Handle	*/

PSZ pszString1 = "This is an example of ";
#define	CSTRING1 22L
PSZ pszString2 = "italic";
#define	CSTRING2 6L
PSZ pszString3 = " and ";
#define	CSTRING3 5L
PSZ pszString4 = "bold";
#define	CSTRING4 4L
PSZ pszString5 = " text displayed on same line.";
#define	CSTRING5 29L

PSZ apszScale[6] = { "0", "1", "2", "3", "4", "5" };

/************************************************************************/
/*									*/
/*     Module Prototype	Definitions					*/
/*									*/
/************************************************************************/

static VOID DrawFormattedText(HPS hpsTarget, HPS hpsPrinter,
			      LONG cString, PSZ	pszString, ULONG fl, PFONTCACHE	pfcache);

#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- DrawCommon -------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	perform	the drawing of the graphics	*/
/*     and text	within the presentation	space.	It is assumed by the	*/
/*     function	that the units of the presentation space are TWIPS.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS hPS;	= Presentation Space Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID DrawCommon(HPS hPS, HPS hpsPrinter, PFONTCACHE pfcache,
		LONG lLeftMargin, LONG lBottomMargin,
		LONG lRightMargin, LONG	lTopMargin,
		PSZ pszHeader, PSZ pszFooter, BOOL fDrawHeader)

{
POINTL ptl;			   /* Display Point			*/
POINTL ptlTickStart;		   /* Tick Start Display Point		*/
POINTL ptlTickEnd;		   /* Tick End Display Point		*/
POINTL ptlScale;		   /* Tick End Display Point		*/
POINTL rgptl[TXTBOX_COUNT];	   /* Text Box Point Array		*/
union {
LONG   clr;			   /* RGB Colour			*/
RGB2   rgb2;			   /* RGB Colour			*/
 } lClr;
POINTL aptl[4];			   /* Gray Scale Display Points		*/
SIZEL  sizl;			   /* Page Size				*/
register INT i,	n;		   /* Loop Counter			*/

GpiQueryPS(hPS,	&sizl);
aptl[0].x = lLeftMargin;
aptl[0].y = lBottomMargin;
aptl[1].x = lLeftMargin;
aptl[1].y = sizl.cy - lTopMargin;
aptl[2].x = sizl.cx - lRightMargin;
aptl[2].y = sizl.cy - lTopMargin;
aptl[3].x = sizl.cx - lRightMargin;
aptl[3].y = lBottomMargin;

GpiBeginPath(hPS, 1L);
GpiMove(hPS, &aptl[3]);
GpiPolyLine(hPS, 4L, aptl);
GpiEndPath(hPS);
		       /* Circle complete, convert the normal path to a	*/
		       /* clip path to allow the determination of the	*/
		       /* mouse	button click within the	colour wheel or	*/
		       /* outside the edge of the wheel	but still	*/
		       /* within the limits of the control itself	*/

GpiSetClipPath(hPS, 1L,	SCP_ALTERNATE |	SCP_AND);

		       /* Set the colour table to RGB mode		*/

GpiCreateLogColorTable(hPS, 0UL, LCOLF_RGB, 0L,	0L, (PLONG)NULL);

ptl.x =	ptlTickStart.x = ptlTickEnd.x =	ptlScale.x = lLeftMargin;
ptl.y =	ptlTickStart.y = lBottomMargin + 720L;

GpiMove(hPS, &ptl);
ptl.x =	9360L +	lLeftMargin;
GpiLine(hPS, &ptl);

ptlScale.y = lBottomMargin + 540L;

for ( i	= 0; i < 6; i++, ptlScale.x += 1440L )
   for ( n = 0;	n < 16;	n++, ptlTickEnd.x = (ptlTickStart.x += 90L) )
       {
       switch (	n )
	   {
	   case	1 :	   /* 1/16 */
	   case	3 :	   /* 3/16 */
	   case	5 :	   /* 5/16  */
	   case	7 :	   /* 7/16 */
	   case	9 :	   /* 9/16 */
	   case	11 :	   /* 11/16 */
	   case	13 :	   /* 13/16 */
	   case	15 :	   /* 15/16 */
	       ptlTickEnd.y = lBottomMargin + 720L + 90L;
	       break;

	   case	2 :	   /* 1/8  */
	   case	6 :	   /* 3/8  */
	   case	10 :	   /* 5/8  */
	   case	14 :	   /* 7/8  */
	       ptlTickEnd.y = lBottomMargin + 720L + 180L;
	       break;

	   case	4 :	   /* 1/4  */
	   case	12 :	   /* 3/4  */
	       ptlTickEnd.y = lBottomMargin + 720L + 270L;
	       break;

	   case	0 :	   /* 0	 */
	       GpiMove(hPS, &ptlScale);
	       DrawFormattedText(hPS, hpsPrinter, 1, apszScale[i], 0UL,	&fcacheScale);

	   case	8 :	   /* 1/2  */
	       ptlTickEnd.y = lBottomMargin + 720L + 450L;
	       break;
	   }
       GpiMove(hPS, &ptlTickStart);
       GpiLine(hPS, &ptlTickEnd);
       }

lClr.rgb2.fcOptions = (BYTE)0;

aptl[0].x = lLeftMargin	  + 720L;
aptl[0].y = lBottomMargin + 360L;
aptl[1].x = lLeftMargin	  + 900L;
aptl[1].y = lBottomMargin + 360L;
aptl[2].x = lLeftMargin	  + 900L;
aptl[2].y = lBottomMargin + 180L;
aptl[3].x = lLeftMargin	  + 720L;
aptl[3].y = lBottomMargin + 180L;

for ( i	= 0; i < 100; i	+= 10 )
   {
   lClr.rgb2.bGreen =
   lClr.rgb2.bBlue  =
   lClr.rgb2.bRed   = (BYTE)((i	* 255L)	/ 100L);

   GpiSetColor(hPS, (LONG)lClr.clr);

   GpiBeginArea(hPS, BA_NOBOUNDARY | BA_ALTERNATE);

   GpiMove(hPS,	&aptl[3]);
   GpiPolyLine(hPS, 4L,	aptl);
   GpiEndArea(hPS);
   aptl[0].x = (aptl[3].x += 180L);
   aptl[1].x = (aptl[2].x += 180L);
   }

aptl[0].x = (aptl[3].x += 360L);
aptl[1].x = (aptl[2].x += 360L);

for ( i	= 0; i < 7; i++	)
   {
   switch ( i )
       {
       case 0 :
	   lClr.rgb2.bGreen = (BYTE)0;
	   lClr.rgb2.bBlue  = (BYTE)0;
	   lClr.rgb2.bRed   = (BYTE)0;
	   break;

       case 1 :
	   lClr.rgb2.bGreen = (BYTE)255;
	   lClr.rgb2.bBlue  = (BYTE)0;
	   lClr.rgb2.bRed   = (BYTE)0;
	   break;

       case 2 :
	   lClr.rgb2.bGreen = (BYTE)0;
	   lClr.rgb2.bBlue  = (BYTE)255;
	   lClr.rgb2.bRed   = (BYTE)0;
	   break;

       case 3 :
	   lClr.rgb2.bGreen = (BYTE)0;
	   lClr.rgb2.bBlue  = (BYTE)0;
	   lClr.rgb2.bRed   = (BYTE)255;
	   break;

       case 4 :
	   lClr.rgb2.bGreen = (BYTE)255;
	   lClr.rgb2.bBlue  = (BYTE)255;
	   lClr.rgb2.bRed   = (BYTE)0;
	   break;

       case 5 :
	   lClr.rgb2.bGreen = (BYTE)255;
	   lClr.rgb2.bBlue  = (BYTE)0;
	   lClr.rgb2.bRed   = (BYTE)255;
	   break;

       case 6 :
	   lClr.rgb2.bGreen = (BYTE)0;
	   lClr.rgb2.bBlue  = (BYTE)255;
	   lClr.rgb2.bRed   = (BYTE)255;
	   break;
       }
   GpiSetColor(hPS, (LONG)lClr.clr);

   GpiBeginArea(hPS, BA_NOBOUNDARY | BA_ALTERNATE);

   GpiMove(hPS,	&aptl[3]);
   GpiPolyLine(hPS, 4L,	aptl);
   GpiEndArea(hPS);
   aptl[0].x = (aptl[3].x += 180L);
   aptl[1].x = (aptl[2].x += 180L);
   }

ptl.x =	lLeftMargin   +	720L;
ptl.y =	lBottomMargin +	1440L;
GpiMove(hPS, &ptl);
GpiSetColor(hPS, RGB_BLACK);

DrawFormattedText(hPS, hpsPrinter, CSTRING1, pszString1, 0UL,		   pfcache);
DrawFormattedText(hPS, hpsPrinter, CSTRING2, pszString2, FATTR_SEL_ITALIC, pfcache);
DrawFormattedText(hPS, hpsPrinter, CSTRING3, pszString3, 0UL,		   pfcache);
DrawFormattedText(hPS, hpsPrinter, CSTRING4, pszString4, FATTR_SEL_BOLD,   pfcache);
DrawFormattedText(hPS, hpsPrinter, CSTRING5, pszString5, 0UL,		   pfcache);

		       /* Draw the header and footer if	necessary	*/
if ( fDrawHeader )
   {
		       /* Determine the	size of	the output area		*/

   if (	pszHeader && pszHeader[0] )
       {
       GpiSetCharSet(hPS, pfcache->lfontBold.lcid);
       GpiQueryTextBox(hPS, (LONG)strlen(pszHeader), pszHeader,	5L, rgptl);
       ptl.x = (sizl.cx	- lLeftMargin -	lRightMargin - rgptl[TXTBOX_CONCAT].x) / 2L +
	       lLeftMargin;
       ptl.y = sizl.cy - lTopMargin / 2L;
       GpiMove(hPS, &ptl);
       DrawFormattedText(hPS, hpsPrinter, (LONG)strlen(pszHeader), pszHeader, FATTR_SEL_BOLD, pfcache);
       }

   if (	pszFooter && pszFooter[0] )
       {
       GpiSetCharSet(hPS, pfcache->lfontBold.lcid);
       GpiQueryTextBox(hPS, (LONG)strlen(pszFooter), pszFooter,	5L, rgptl);
       ptl.x = (sizl.cx	- lLeftMargin -	lRightMargin - rgptl[TXTBOX_CONCAT].x) / 2L +
	       lLeftMargin;
       ptl.y = lBottomMargin / 2L;
       GpiMove(hPS, &ptl);
       DrawFormattedText(hPS, hpsPrinter, (LONG)strlen(pszFooter), pszFooter, FATTR_SEL_BOLD, pfcache);
       }
   }
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- DrawFormattedText ------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	perform	the drawing of the graphics	*/
/*     and text	within the presentation	space.	It is assumed by the	*/
/*     function	that the units of the presentation space are TWIPS.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS	  hpsTarget;  =	Target Presentation Space Handle	*/
/*     HPS	  hpsPrinter; =	Printer	Presentation Space Handle	*/
/*     LONG	  cString;    =	String Length				*/
/*     PSZ	  pszString;  =	String					*/
/*     ULONG	  fl;	      =	Drawing	Flags				*/
/*     PFONTCACHE pfcache;    =	Font Cache				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID DrawFormattedText(HPS hpsTarget, HPS hpsPrinter, LONG cString, PSZ pszString, ULONG	fl,
			      PFONTCACHE pfcache)

{
PLONG  alVector;		   /* Vector Array			*/
register INT i;			   /* Loop Counter			*/

if ( hpsPrinter	)
   {
   DosAllocMem((PPVOID)(PVOID)&alVector, (ULONG)(cString * sizeof(LONG)), PAG_READ | PAG_WRITE | PAG_COMMIT);
   if (	fl == 0UL )
       {
       GpiSetCharSet(hpsTarget,	pfcache->lfontNormal.lcid);
       if ( pfcache->lfontNormal.fScalable )
	   GpiSetCharBox(hpsTarget, &pfcache->lfontNormal.sizfxBox);

       for ( i = 0; i <	cString; i++ )
	   alVector[i] = pfcache->lfontNormal.alWidths[pszString[i]];
       }
   else
       if ( fl == FATTR_SEL_ITALIC )
	   {
	   GpiSetCharSet(hpsTarget, pfcache->lfontItalic.lcid);
	   if (	pfcache->lfontItalic.fScalable )
	       GpiSetCharBox(hpsTarget,	&pfcache->lfontItalic.sizfxBox);

	   for ( i = 0;	i < cString; i++ )
	       alVector[i] = pfcache->lfontItalic.alWidths[pszString[i]];
	   }
       else
	   {
	   GpiSetCharSet(hpsTarget, pfcache->lfontBold.lcid);
	   if (	pfcache->lfontBold.fScalable )
	       GpiSetCharBox(hpsTarget,	&pfcache->lfontBold.sizfxBox);

	   for ( i = 0;	i < cString; i++ )
	       alVector[i] = pfcache->lfontBold.alWidths[pszString[i]];
	   }

   GpiCharStringPos(hpsTarget, (PRECTL)NULL, CHS_VECTOR, cString, pszString, alVector);
   DosFreeMem((PVOID)alVector);
   }
else
   {
   if (	fl == 0UL )
       {
       GpiSetCharSet(hpsTarget,	pfcache->lfontNormal.lcid);
       if ( pfcache->lfontNormal.fScalable )
	   GpiSetCharBox(hpsTarget, &pfcache->lfontNormal.sizfxBox);
       }
   else
       if ( fl == FATTR_SEL_ITALIC )
	   {
	   GpiSetCharSet(hpsTarget, pfcache->lfontItalic.lcid);
	   if (	pfcache->lfontItalic.fScalable )
	       GpiSetCharBox(hpsTarget,	&pfcache->lfontItalic.sizfxBox);

	   }
       else
	   {
	   GpiSetCharSet(hpsTarget, pfcache->lfontBold.lcid);
	   if (	pfcache->lfontNormal.fScalable )
	       GpiSetCharBox(hpsTarget,	&pfcache->lfontBold.sizfxBox);
	   }

   GpiCharStringPos(hpsTarget, (PRECTL)NULL, 0UL, cString, pszString, (PLONG)NULL);
   }
}
#pragma	subtitle("   Client Window - Client Window Procedure")
#pragma	page( )

/* --- DrawText	---------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	draw the control text within the main	*/
/*     application window to show the spacing differences.  This text	*/
/*     does not	use the	vector spacing.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS hpsTarget; =	Presentation Space Handle			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID DrawText(HPS hpsTarget)

{
POINTL ptl;			   /* Display Point			*/

ptl.x =	810L;
ptl.y =	2160L;
GpiMove(hpsTarget, &ptl);
GpiSetColor(hpsTarget, RGB_BLACK);

DrawFormattedText(hpsTarget, (HPS)NULL,	CSTRING1, pszString1, 0UL,		&fcache);
DrawFormattedText(hpsTarget, (HPS)NULL,	CSTRING2, pszString2, FATTR_SEL_ITALIC,	&fcache);
DrawFormattedText(hpsTarget, (HPS)NULL,	CSTRING3, pszString3, 0UL,		&fcache);
DrawFormattedText(hpsTarget, (HPS)NULL,	CSTRING4, pszString4, FATTR_SEL_BOLD,	&fcache);
DrawFormattedText(hpsTarget, (HPS)NULL,	CSTRING5, pszString5, 0UL,		&fcache);

}
#pragma	subtitle("   File Print - File Printing Routine")
#pragma	page( )

/* --- PrnDisplay -------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	print the constructed display		*/
/*     image within a separate thread.	The routine uses the common	*/
/*     display function.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     PPRNDATA	pprni; = Print Information Pointer			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID _System PrnDisplay(PPRNDATA pprni)

{
HAB	    habThread;		   /* Thread Anchor Block Handle	*/
HDC	    hdcPrinter;		   /* Printer Device Context Handle	*/
HPS	    hpsPrinter;		   /* Printer Presentation Space Handle	*/
LONG	    lOut;		   /* Output Count			*/
SIZEL	    sizlPage;		   /* Page Size	Holder			*/
USHORT	    usJobID;		   /* Spooler Job ID			*/
FONTCACHE   fcachePrn;		   /* Font Cache			*/

		       /* Get an anchor	block handle to	allow proper	*/
		       /* thread initialization	of stack space and	*/
		       /* usage	of some	PM calls.  Create a message	*/
		       /* queue	as well	to make	sure that everything	*/
		       /* works	properly in terms of error message	*/
		       /* display.					*/

habThread = WinInitialize(0UL);

		       /* Open a device	context	for the	printer		*/
		       /* selected					*/

if ( (hdcPrinter = PrnOpenDC(&pprni->prn, "PM_Q_STD")) != (HDC)NULL )
   {
		       /* Create a presentation	space into which the	*/
		       /* printing will	occur using TWIPS as the unit	*/
		       /* of measure					*/

   sizlPage.cx = sizlPage.cy = 0L;
   hpsPrinter =	GpiCreatePS(habThread, hdcPrinter, &sizlPage, PU_TWIPS |
			    GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC);

		       /* Start	the printing of	the selected file by	*/
		       /* indicating the start of the document within	*/
		       /* device context				*/

   if (	DevEscape(hdcPrinter, DEVESC_STARTDOC, (LONG)strlen(pprni->szTitle),
		  pprni->szTitle, NULL,	NULL) != DEVESC_ERROR )
       {
       BuildFontCache((HPS)NULL, hpsPrinter, hdcPrinter,
		      pprni->fsel.szFacename, pprni->fsel.lNominalPointSize,
		      &fcachePrn);

       DrawCommon(hpsPrinter, (HPS)NULL, &fcachePrn,
		  (LONG)(1440 *	pprni->pags.rdLeftMargin),
		  (LONG)(1440 *	pprni->pags.rdBottomMargin),
		  (LONG)(1440.0	* pprni->pags.rdRightMargin),
		  (LONG)(1440.0	* pprni->pags.rdTopMargin),
		  pprni->pags.szHeader,	pprni->pags.szFooter,
		  TRUE);

		       /* Inform the device context printing complete	*/
		       /* to allow the correct spooling	and final	*/
		       /* printing to occur				*/
       lOut = 2L;
       DevEscape(hdcPrinter, DEVESC_ENDDOC, 0L,	NULL, &lOut, (PBYTE)&usJobID);

		       /* Release and destroy both the printing		*/
		       /* presentation space and device	context	handles	*/

       GpiAssociate(hpsPrinter,	(HDC)NULL);
       GpiDestroyPS(hpsPrinter);
       DevCloseDC(hdcPrinter);
       }
   DeleteFontCache(hpsPrinter, &fcachePrn);
   }
DosFreeMem((PVOID)pprni);

		       /* Destroy the message queue used by the	thread	*/
		       /* and destroy the anchor block before exiting	*/
		       /* the thread					*/
WinTerminate(habThread);

DosExit(EXIT_THREAD, 0UL);
}
