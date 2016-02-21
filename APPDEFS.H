
/* appdefs.h	    Created:	1996-01-19  Revised:   1996-01-19	*/

/* Module Definition Header						*/

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



/************************************************************************/
/************************************************************************/
/*	Structure Definitions						*/
/************************************************************************/
/************************************************************************/


typedef	struct _QUEINFO
   {
   PSZ	       pszName;		   /* Device Name			*/
   PSZ	       pszDriverName;	   /* Driver Name			*/
   PSZ	       pszPrinters;	   /* Printers				*/
   PVOID       pDriverData;	   /* Device Data			*/
   } QUEINFO ;		   /* quei */

typedef	QUEINFO	*PQUEINFO; /* pquei */

typedef	struct _PRN
   {
   HAB		hAB;		   /* Anchor block for list		*/
   CHAR		szQueue[256];	   /* Default Queue			*/
   INT		iQueue;		   /* Selected Printer Index		*/
   INT		cQueues;	   /* Total Queue Count			*/
   PQUEINFO	pquei;		   /* Queue Information			*/
   } PRN ;		   /* prn */

typedef	PRN *PPRN;	   /* pprn */

/* --- Page Setup Structure Definitions	-------------------------------	*/

typedef	struct _PAGESETUP	       /* pags */
   {
   double rdLeftMargin;		       /* Left Margin			*/
   double rdRightMargin;	       /* Right	Margin			*/
   double rdTopMargin;		       /* Top Margin			*/
   double rdBottomMargin;	       /* Bottom Margin			*/
   CHAR	  szHeader[256];	       /* Header			*/
   CHAR	  szFooter[256];	       /* Footer			*/
   } PAGESETUP ;

typedef	PAGESETUP *PPAGESETUP;

/* --- Font Selection Structure	Definitions ---------------------------	*/

typedef	struct _FONTSEL		       /* fsel */
   {
   LONG	  lMatch;		       /* Font Match Number		*/
   BOOL	  fFixed;		       /* Fixed	Size Flag		*/
   LONG	  lPointSize;		       /* Font Point Size		*/
   LONG	  lNominalPointSize;	       /* Nominal Font Point Size	*/
   CHAR	  szFacename[32];	       /* Font Face Name		*/
   } FONTSEL ;

typedef	FONTSEL	*PFONTSEL;

typedef	struct _FONTSIZE	       /* fsiz */
   {
   PSZ	  pszSize;		       /* Font Match Number		*/
   LONG	  lPointSize;		       /* Font Point Size		*/
   } FONTSIZE ;

#define	TWIPS_POINT    20L

typedef	struct _LOGFONT		       /* lfont	*/
   {
   LONG	  lcid;			       /* Logical Font ID		*/
   LONG	  alWidths[256];	       /* Width	Table			*/
   BOOL	  fScalable;		       /* Scalable Font			*/
   SIZEF  sizfxBox;		       /* Scalable Character Box	*/
   } LOGFONT ;

typedef	struct _FONTCACHE	       /* fcache */
   {
   LOGFONT lfontNormal;		       /* Normal Font			*/
   LOGFONT lfontBold;		       /* Bold Font			*/
   LOGFONT lfontItalic;		       /* Italic Font			*/
   } FONTCACHE ;

typedef	FONTCACHE *PFONTCACHE;

/* --- Re-entrant Printer Support -------------------------------------	*/

typedef	struct _PRNDATA
   {
   PRN	     prn;		       /* Printer Information		*/
   FONTSEL   fsel;		       /* Font Selection		*/
   PAGESETUP pags;		       /* Page Setup			*/
   LONG	     cPrnFiles;		       /* Files	Count			*/
   BOOL	     fLineNumbers;	       /* Include Line Numbers Flag	*/
   BOOL	     fDateFooter;	       /* Date and Time	Footer Flag	*/
   CHAR	     szTitle[CCHMAXPATH];      /* Print	Title			*/
   CHAR	     aszFiles[1][CCHMAXPATH];  /* Files	Array Start		*/
   } PRNDATA ;		   /* prni */

#define	PRNDATASIZE (sizeof(PRNDATA) - CCHMAXPATH)

typedef	PRNDATA	*PPRNDATA;

#define	TAB 9			   /* Tab Stop Value			*/
#define	LF 10			   /* Line Feed	Value			*/
#define	CR 13			   /* Carriage Return Value		*/
#define	TOF 12			   /* Top of Form Value			*/
#define	EOF_MARK 26		   /* End of File Mark Value		*/
#define	ESCAPE 27		   /* Escape Value			*/
#define	TOP_MARGIN 4		   /* Printout Top Margin Count		*/

#define	RGBCLR_PALEGRAY		0x00c9c9c9L

#define	RGBCLR_SHADOW		0x00808080L
#define	RGBCLR_DIALOGBACKGROUND	0x00ccccccL

/************************************************************************/
/************************************************************************/
/*	Global Variable	Definitions					*/
/************************************************************************/
/************************************************************************/

extern FONTMETRICS fm;		   /* Font Metrics Info			*/
extern HSWITCH	   hSwitch;	   /* Task List	Entry Handle		*/
extern HAB	   hAB;		   /* Program Anchor Block Handle	*/

extern HPOINTER	   hptrWait;	   /* Wait Mouse Pointer Handle		*/
extern HPOINTER	   hptrArrow;	   /* Arrow Mouse Pointer Handle	*/

extern HMQ  hmqPrnSetup;	   /* Program Message Queue Handle	*/
extern HWND hwndPrnSetup;	   /* Client Window Handle		*/
extern HWND hwndPrnSetupFrame;	   /* Frame Window Handle		*/
extern HWND hmenuPrnSetup;	   /* Menu Handle			*/

extern HPS hpsPrint;		   /* Print Window Presentation	Handle	*/

extern HWND hwndToolBar;	   /* Client Window Handle		*/
extern HWND hwndToolBarFrame;	   /* Frame Window Handle		*/

extern HWND hwndHelp;		   /* Help Window Handle		*/
extern HELPINIT	helpinit;	   /* Help Initialization Table		*/

extern PRN  prn;		   /* Printer Control Holder		*/

extern FONTSEL	    fsel;	   /* Font Selection			*/

extern PAGESETUP pags;		   /* Page Setup			*/

extern FONTCACHE fcache;	   /* Font Cache			*/
extern FONTCACHE fcacheScale;	   /* Scale Font Cache			*/

/************************************************************************/
/************************************************************************/
/*	Function Prototype Definitions					*/
/************************************************************************/
/************************************************************************/

/* --- DBCS.C Prototype	Definitions -----------------------------------	*/

VOID SetCountryCode(ULONG ulCountry, ULONG ulCP);
BOOL fQueryDBCS(VOID);
PSZ  strrchrDBCS(const CHAR *p,	INT c);
PSZ  strchrDBCS(const CHAR *p, INT c);
PSZ  struprDBCS(CHAR *p);
PSZ  strstrDBCS(const CHAR *str1, const	CHAR *str2);
VOID SetDBCSFont(HWND hWnd);

/* --- Fonts.C Prototype Definitions ----------------------------------	*/

LONG lSetFonts(HWND hwndFontNames);
VOID SetFontSize(HWND hwndFont,	HWND hwndFontSize);
BOOL fQuerySelectedFont(HWND hwndFont, HWND hwndFontSize, PFONTSEL pfsel);
LONG lSelectFont(HPS hPS, HDC hDC, PSZ pszFacename, LONG lNominalPointSize, ULONG fl, PBOOL pfScalable,	PSIZEF psizfxBox);
VOID DeSelectFont(HPS hPS, LONG	lcid);
VOID BuildFontCache(HPS	hpsTarget, HPS hpsPrinter, HDC hdcPrinter, PSZ pszFacename, LONG lNominalPointSize,
		    PFONTCACHE pfcache);
VOID DeleteFontCache(HPS hpsTarget, PFONTCACHE pfcache);

/* --- PrnAPI.C	Prototype Definitions ---------------------------------	*/

BOOL	PrnCreatePrinterList(PPRN pprn);
BOOL	PrnDestroyPrinterList(PPRN pprn);
HDC	PrnOpenDC(PPRN pprn, PSZ pszDataType);
HDC	PrnOpenInfoDC(PPRN pprn, PSZ pszDataType);
BOOL	PrnQueryJobProperties(PPRN pprn, INT iQueue);
PSZ	PrnQueryPrinterName(PPRN pprn);

MRESULT	EXPENTRY PrnSetupDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY FilePrintDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2);

LONG SelectFont(HDC hDC, HPS hPS, CHAR *pszFacename, LONG lPoints);
LONG SelectScalableFont(HPS hPS, CHAR *pszFacename);
LONG ScaleFont(HPS hPS,	CHAR *pszFacename, LONG	lSize);

/* --- DrawPage.C Prototype Definitions	-------------------------------	*/

VOID DrawCommon(HPS hPS, HPS hpsPrinter, PFONTCACHE pfcache,
		LONG lLeftMargin, LONG lBottomMargin,
		LONG lRightMargin, LONG	lTopMargin,
		PSZ pszHeader, PSZ pszFooter, BOOL fDrawHeader);
VOID DrawText(HPS hpsTarget);
VOID _System PrnDisplay(PPRNDATA pprni);

/* --- PageDlg.C Prototype Definitions --------------------------------	*/

MRESULT	EXPENTRY PageSetupDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2);

/* --- PrnWnd.C	Prototype Definitions ---------------------------------	*/

MRESULT	EXPENTRY PrintDriverWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);

/* --- PrevDlg.C Prototype Definitions --------------------------------	*/

HBITMAP	hbmCreatePreview(LONG cx, LONG cy, HPS hpsPrinter, HDC hdcPrinter);
MRESULT	EXPENTRY PrintPreviewDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2);

/* --- Support.C Prototype Definitions --------------------------------	*/

VOID	InitApp(HWND hwndFrame,	HWND hwndClient, PSZ pszWindowListTitle);
HWND	CreateStdWindow(HWND hwndParent, ULONG flStyle,	ULONG flCreateFlags,
			PSZ pszClientClass, PSZ	pszTitle, ULONG	styleClient,
			HMODULE	hmod, ULONG idResources, PHWND phwndClient,
			LONG x,	LONG y,	LONG cx, LONG cy);
MRESULT	PDSKeyProc(HWND	hWnd, ULONG msg, MPARAM	mp1, MPARAM mp2);
VOID	PDSGetTemplate(HWND hWnd, ULONG	id);
VOID	CenterDlg(HWND hWnd);

/* --- ToolBar.C Prototype Definitions --------------------------------- */

MRESULT	EXPENTRY ToolBarWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT	EXPENTRY FontMetricsDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
