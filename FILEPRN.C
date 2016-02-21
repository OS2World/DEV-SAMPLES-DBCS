#pragma	title("DBCS Driver  --  Version 1  --  (FilePrn.C)")
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

/* This	module contains	the routines to	handle the font	selection,	*/
/* file	selection and printing.						*/

/* Filename:   FilePrn.C						*/

/*  Version:   1							*/
/*  Created:   1996-01-19						*/
/*  Revised:   1996-01-19						*/

/* Routines:   MRESULT EXPENTRY	FilePrintDlgProc(HWND hWnd,		*/
/*						 ULONG msg,		*/
/*						 MPARAM	mp1,		*/
/*						 MPARAM	mp2);		*/
/*	       static VOID ShowDirInfo(HWND hWnd, ULONG	idPath,		*/
/*				       ULONG idVol, ULONG idRoom);	*/
/*	       static VOID DiskVolDrive(ULONG ulDrive,			*/
/*					CHAR *pszDiskName);		*/
/*	       static VOID QueryDriveList( );				*/
/*	       static ULONG ulGetDirList(HWND hwndDrives, HWND hwndDirs,*/
/*					 HWND hwndFiles);		*/
/*	       static BOOL fPrnLine(HPS	hpsPrinter, HDC	hdcPrinter,	*/
/*				    PSZ	pszStr,	PPOINTL	pptl,		*/
/*				    PPOINTL pptlFooter,			*/
/*				    PSIZEL psizlPage, LONG yShift,	*/
/*				    BOOL fDateFooter, PSZ pszFooter,	*/
/*				    PULONG pcPages);			*/
/*	       VOID _System PrnFile(PPRNDATA pprni);			*/

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
/*     Module Prototype	Definitions					*/
/*									*/
/************************************************************************/

static VOID  QueryDriveList(VOID);
static ULONG ulGetDirList(HWND hwndDrives, HWND	hwndDirs, HWND hwndFiles);
static VOID  DiskVolDrive(ULONG	ulDrive, CHAR *pszDiskName);
static VOID  ShowDirInfo(HWND hWnd, ULONG idPath, ULONG	idVol, ULONG idRoom);
static BOOL  fPrnLine(HPS hpsPrinter, HDC hdcPrinter, PSZ pszStr,
	       PPOINTL pptl, PPOINTL pptlFooter, PSIZEL	psizlPage, LONG	yShift,
	       BOOL fDateFooter, PSZ pszFooter,	PULONG pcPages);
VOID _System PrnFile(PPRNDATA pprni);

/************************************************************************/
/*									*/
/*     Module Data Definitions						*/
/*									*/
/************************************************************************/

BOOL  fLineNumbers = FALSE;	   /* Line Numbers Flag			*/
INT   iFileSel = LIT_NONE;	   /* File Selected			*/
BOOL  fDrivesFilled = FALSE;	   /* Drives Filled Flag		*/
ULONG ulDriveMap;		   /* Drive Bit	Map Holder		*/
LONG  cDrives;
CHAR  aszDrives[26][20];
BOOL  afRemoveable[26] = { FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE, FALSE,	FALSE, FALSE, FALSE, FALSE, FALSE,
			   FALSE, FALSE	};

CHAR szDayNames[ ]   = "SunMonTueWedThuFriSat";
CHAR szMonthNames[ ] = "JanFebMarAprMayJunJulAugSepOctNovDec";

#pragma	subtitle("   File Print -  File Print Dialogue Procedure")
#pragma	page( )

/* --- FilePrintDlgProc	-------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the file	*/
/*     print dialogue procedure.					*/
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
/*     FilePrintDlgProc	= Message Handling Result			*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY FilePrintDlgProc(HWND hWnd, ULONG msg,	MPARAM mp1, MPARAM mp2)

{
CHAR	 szFileName[CCHMAXPATH];   /* Filename Holder			*/
CHAR	 szStrBuf[CCHMAXPATH];	   /* String Buffer			*/
PPRNDATA pprni;			   /* Print Data Pointer		*/
TID	 tid;			   /* Thread ID				*/
register INT i;			   /* Loop Counters			*/

switch ( msg )
   {
		       /* Perform dialog initialization			*/
   case	WM_INITDLG :
       WinSetPointer(HWND_DESKTOP, hptrWait);

		       /* Clear	the drives filled flag			*/

       fDrivesFilled = FALSE;

		       /* Show the selected print name			*/

       WinSetDlgItemText(hWnd, IT_PRINTER, PrnQueryPrinterName(&prn));

		       /* Set the fonts	for the	printer	selected	*/

       switch (	lSetFonts(WinWindowFromID(hWnd,	CBX_FONT)) )
	   {
		       /* Error	return:	bad HDC	for printer		*/
	   case	1L :
	       WinMessageBox(HWND_DESKTOP, hWnd, "Printer Device Context could not be opened.",
			     "Print Font Example", 0UL,	MB_OK |	MB_ICONHAND | MB_MOVEABLE);
	       WinDismissDlg(hWnd, FALSE);
	       return(0L);

		       /* Error	return:	bad HPS	for printer		*/
	   case	2L :
	       WinMessageBox(HWND_DESKTOP, hWnd, "Printer Presentation Space could not be created.",
			     "Print Font Example", 0UL,	MB_OK |	MB_ICONHAND | MB_MOVEABLE);
	       WinDismissDlg(hWnd, FALSE);
	       return(0L);

		       /* Error	return:	no error, fonts	set in combo	*/
	   default :
	       break;
	   }
		       /* Set the check	box for	the inclusion of the	*/
		       /* line numbers					*/

       WinSendDlgItemMsg(hWnd, CB_INCLUDELINENUMS, BM_SETCHECK,
			 MPFROMSHORT(fLineNumbers), 0L);

		       /* Get directory/file list and display in list	*/
		       /* boxes						*/

       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);

		       /* Get a	list of	the drives, directories	and	*/
		       /* files	for the	list boxes and show the	number	*/
		       /* of files within the directory			*/

       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES), WinWindowFromID(hWnd, LB_DIRECTORIES),
					    WinWindowFromID(hWnd, LB_FILES)));
       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Select the last file printed			*/

       if ( iFileSel !=	LIT_NONE )
	   WinSendDlgItemMsg(hWnd, LB_DIRECTORIES, LM_SELECTITEM, MPFROMSHORT(iFileSel), MPFROMSHORT(TRUE));

       CenterDlg(hWnd);
       WinSetPointer(HWND_DESKTOP, hptrArrow);
       break;
			/* Process control selections			*/
   case	WM_CONTROL :
       switch (	SHORT2FROMMP(mp1) )
	   {
		       /* Enter	or selection made within error combo	*/
		       /* box						*/
	   case	CBN_ENTER :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	CBX_DRIVES :

		       /* Get item selected from the list box		*/

		       WinSendDlgItemMsg(hWnd, CBX_DRIVES, LM_QUERYITEMTEXT,
				  MPFROM2SHORT(LONGFROMMR(WinSendDlgItemMsg(hWnd, CBX_DRIVES, LM_QUERYSELECTION, 0L, 0L)),
					       CCHMAXPATH),
				  MPFROMP(szFileName));

		       /* Change drives					*/

		       DosSetDefaultDisk((ULONG)((szFileName[0]	& 223) - '@'));

		       /* Refresh the directory	files list and show the	*/
		       /* number of files within the directory		*/

		       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES),
				    WinWindowFromID(hWnd, LB_DIRECTORIES),
				    WinWindowFromID(hWnd, LB_FILES)));
		       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Display the directory	path, volume label and	*/
		       /* total	disk space free				*/

		       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);
		       break;

		   default :
		       break;
		   }
	       break;
		       /* Process message combo	box entry selections	*/

	   case	CBN_LBSELECT :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	CBX_FONT :
		       SetFontSize(WinWindowFromID(hWnd, CBX_FONT),
				   WinWindowFromID(hWnd, CBX_FONTSIZE));
		       break;
		   }
	       break;
		       /* Drive/directory selected, get	entry selected	*/
		       /* and change to	drive or directory selected	*/

	   case	LN_ENTER :
	       switch (	SHORT1FROMMP(mp1) )
		   {
		   case	LB_DIRECTORIES :

		       /* Get item selected from the directories list	*/
		       /* box						*/

		       WinSendDlgItemMsg(hWnd, LB_DIRECTORIES, LM_QUERYITEMTEXT,
				  MPFROM2SHORT(LONGFROMMR(WinSendDlgItemMsg(hWnd, LB_DIRECTORIES,
									    LM_QUERYSELECTION, 0L, 0L)),
					       sizeof(szFileName)),
				  MPFROMP(szFileName));

		       /* Change to the	selected directory		*/

		       DosSetCurrentDir(szFileName);

		       /* Refresh the directory	files list and show the	*/
		       /* number of files within the directory		*/

		       sprintf(szStrBuf, "%d", ulGetDirList(WinWindowFromID(hWnd, CBX_DRIVES),
				    WinWindowFromID(hWnd, LB_DIRECTORIES),
				    WinWindowFromID(hWnd, LB_FILES)));
		       WinSetDlgItemText(hWnd, IT_FILECOUNT, szStrBuf);

		       /* Display the directory	path, volume label and	*/
		       /* total	disk space free				*/

		       ShowDirInfo(hWnd, IT_DIRECTORY, IT_VOLUMELABEL, IT_DISKFREE);
		       break;

		   case	LB_FILES :

		       /* Item within the files	list box double	clicked	*/
		       /* on, user indicating that the file selected	*/
		       /* should be printed, therefore,	force the	*/
		       /* simulation of	the Print push button		*/

		       WinSendMsg(hWnd,	WM_COMMAND, MPFROMSHORT(DID_OK), 0L);
		       break;
		   }
	       break;
	   }
       break;
			/* Process push	button selections		*/
   case	WM_COMMAND :
	switch ( SHORT1FROMMP(mp1) )
	    {
		       /* Print	push button clicked			*/
	   case	DID_OK :
		       /* Get the file selected				*/

	       if ( (iFileSel =	(i = (INT)LONGFROMMR(WinSendDlgItemMsg(hWnd, LB_FILES,
								       LM_QUERYSELECTION,
								       0L, 0L)))) == LIT_NONE )
		   break;
	       else
		       /* Get item selected from the list box and	*/
		       /* determine which file selected			*/

		   WinSendDlgItemMsg(hWnd, LB_FILES, LM_QUERYITEMTEXT,
				     MPFROM2SHORT(i, sizeof(szFileName)),
				     MPFROMP(szFileName));

		       /* Get the font selected				*/

	       if ( !fQuerySelectedFont(WinWindowFromID(hWnd, CBX_FONT),
					WinWindowFromID(hWnd, CBX_FONTSIZE),
					&fsel) )
		   break;

		       /* Get the line number include option		*/

	       fLineNumbers = (BOOL)SHORT1FROMMR(WinSendDlgItemMsg(hWnd,
								   CB_INCLUDELINENUMS,
								   BM_QUERYCHECK,
								   0L, 0L));

		       /* Allocate memory for the printer information	*/
		       /* and save the printer information from	the	*/
		       /* dialogue in the memory			*/

	       DosAllocMem((PPVOID)(PVOID)&pprni, PRNDATASIZE +	CCHMAXPATH,
			   PAG_READ | PAG_WRITE	| PAG_COMMIT);

	       pprni->prn	   = prn;
	       pprni->fsel	   = fsel;
	       pprni->pags	   = pags;
	       pprni->cPrnFiles	   = 1;
	       pprni->fLineNumbers = fLineNumbers;
	       pprni->fDateFooter  = TRUE;
	       memcpy(pprni->szTitle, "Print Font Example - ", 22);
	       strcat(pprni->szTitle, strcpy(pprni->aszFiles[0], szFileName));

		       /* Create a separate thread from	which the	*/
		       /* printing is performed				*/

	       DosCreateThread(&tid, (PFNTHREAD)PrnFile, (ULONG)pprni,
			       STACK_COMMITTED,	16384UL);

	       WinDismissDlg(hWnd, TRUE);
	       break;
		       /* Cancel push button selected			*/
	   case	DID_CANCEL :
	       WinDismissDlg(hWnd, FALSE);
	       break;
		       /* Setup	push button selected			*/

	   case	DID_SETUP :
	       if ( WinDlgBox(HWND_DESKTOP, hwndPrnSetupFrame, (PFNWP)PrnSetupDlgProc,
			      (HMODULE)NULL, DLG_PRNSETUP, (PVOID)&prn)	)
		   {
		       /* Set the fonts	for the	printer	selected	*/

		   switch ( lSetFonts(WinWindowFromID(hWnd, CBX_FONT)) )
		       {
		       /* Error	return:	bad HDC	for printer		*/

		       case 1L :
			   WinMessageBox(HWND_DESKTOP, hWnd, "Printer Device Context could not be opened.",
					 "Print Font Example", 0UL, MB_OK | MB_ICONHAND	| MB_MOVEABLE);
			   WinDismissDlg(hWnd, FALSE);
			   return(0L);

		       /* Error	return:	bad HPS	for printer		*/

		       case 2L :
			   WinMessageBox(HWND_DESKTOP, hWnd, "Printer Presentation Space could not be created.",
					 "Print Font Example", 0UL, MB_OK | MB_ICONHAND	| MB_MOVEABLE);
			   WinDismissDlg(hWnd, FALSE);
			   return(0L);

		       /* Error	return:	no error, fonts	set in combo	*/

		       default :
			   WinSetDlgItemText(hWnd, IT_PRINTER, PrnQueryPrinterName(&prn));
			   break;
		       }

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
		   }
	       break;
	    }
	break;
			/* Close requested, exit dialogue		*/
   case	WM_CLOSE :
	WinDismissDlg(hWnd, FALSE);
	break;
			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
#pragma	subtitle("   File Print - Directory Information Display Function")
#pragma	page( )

/* --- ShowDirInfo ------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	display	information regarding the	*/
/*     path, volume label and room remaining.				*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND  hWnd;   = Dialog Dialogue Window Handle			*/
/*     ULONG idPath; = Path ID						*/
/*     ULONG idVol;  = Volume Label ID					*/
/*     ULONG idRoom; = Room Remaining ID				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID ShowDirInfo(HWND hWnd, ULONG idPath, ULONG idVol, ULONG idRoom)

{
CHAR	   szStrBuf[CCHMAXPATH];   /* String Buffer			*/
FSALLOCATE diskinfo;		   /* Disk Information Structure	*/
FSINFO	   fsinf;		   /* Disk Volume Information Structure	*/
ULONG	   cbBuf;		   /* Directory	Buffer Size Count	*/
ULONG	   ulCurDrive;		   /* Current Drive Designation		*/
ULONG	   ulDrives;		   /* Drive Map				*/
ULONG	   ulDrvMap;		   /* Drive Map				*/

		       /* Form the current drive/path and display	*/
		       /* within the Path display field			*/

DosQueryCurrentDisk(&ulCurDrive, &ulDrvMap);

		       /* Get the current drive	and path being used and	*/
		       /* display with Path display field		*/

DosQueryCurrentDisk(&ulCurDrive, &ulDrives);
memcpy(&szStrBuf[1], ":\\", 3);
szStrBuf[0] = (CHAR)(ulCurDrive	+ '@');
cbBuf =	CCHMAXPATH - 3;
		       /* Get information on the current directory	*/

if ( !DosQueryCurrentDir(ulCurDrive, &szStrBuf[3], &cbBuf) )
   {
		       /* Show the full	path information		*/

   WinSetDlgItemText(hWnd, idPath, szStrBuf);

		       /* Get the volume information and display it	*/

   if (	!DosQueryFSInfo(0UL, FSIL_VOLSER, (PBYTE)(&fsinf), sizeof(FSINFO)) )
       if ( fsinf.vol.szVolLabel[0] )
	   WinSetDlgItemText(hWnd, idVol, fsinf.vol.szVolLabel);
       else
	   WinSetDlgItemText(hWnd, idVol, "");

		       /* Get the remaining space of the drive and	*/
		       /* display					*/

   if (	!DosQueryFSInfo(ulCurDrive, FSIL_ALLOC,	(PVOID)&diskinfo,
			sizeof(FSALLOCATE)) )
       WinSetDlgItemText(hWnd, idRoom,
			 strcat(_ltoa((LONG)(diskinfo.cbSector * diskinfo.cSectorUnit *	diskinfo.cUnitAvail),
				szStrBuf, 10), " Bytes"));
   }
}
#pragma	subtitle("   File Print - Get Disk Volume Label Function")
#pragma	page( )

/* --- DiskVolDrive -----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	retrieve the volume label of the	*/
/*     default disk drive.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     ULONG ulDrive;	   = Drive Number				*/
/*     CHAR  *pszDiskName; = Holder for	Volume Label			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID DiskVolDrive(ULONG ulDrive,	CHAR *pszDiskName)

{
FSINFO fsinf;			  /* Disk Volume Information Structure */

		       /* Get the disk volume label			*/

if ( !DosQueryFSInfo(ulDrive, FSIL_VOLSER, (PBYTE)(&fsinf), sizeof(FSINFO)) )
   if (	fsinf.vol.szVolLabel[0]	)
       strcat(strcat(memcpy(pszDiskName, "[ ", 3), fsinf.vol.szVolLabel), " ]");
   else
       pszDiskName[0] =	(CHAR)0;
}
#pragma	subtitle("   File Print - Drive List Query Procedure")
#pragma	page( )

/* --- QueryDriveList ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	build a	list of	valid drives.		*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

static VOID QueryDriveList(VOID)

{
CHAR	    szDrive[4];		   /* Drive Designation	Holder		*/
PFSQBUFFER2 psqbf;		   /* File System Query	Buffer Pointer	*/
ULONG	    cbDrvi;		   /* Buffer Count			*/
ULONG	    ulError;		   /* OS/2 Error			*/
ULONG	    ulCurDrive;		   /* Current Drive Designation		*/
ULONG	    ulDriveMask	= 1;	   /* Drive Bit	Map Mask		*/
register INT i;			   /* Loop Counter			*/

		       /* Perform general initialization		*/

DosQueryCurrentDisk(&ulCurDrive, &ulDriveMap);

		       /* Allocate space for the file system query	*/
		       /* buffer					*/

psqbf =	(PFSQBUFFER2)malloc(1024UL);
memcpy(&szDrive[1], ":", 2);
DosError(FERR_DISABLEHARDERR);

for ( i	= cDrives = 0; i < 26; i++ )
   {
		       /* Determine if a disk drive present by masking	*/
		       /* out the drive	present	bit			*/

   if (	ulDriveMap & ulDriveMask )
       {
		       /* Save the drive designation for the drive	*/
		       /* that is present				*/

       aszDrives[cDrives][0] = szDrive[0] = (CHAR)(i + 'A');
       memcpy(&aszDrives[cDrives][1], ":\\ ", 4);

		       /* Start	with hard disk drives, check to	see if	*/
		       /* the drive is local or	remote.	 Remote	drives	*/
		       /* are generally	networked drives.  Prominare	*/
		       /* Builder denotes diskette, local and remote	*/
		       /* drives within	the Drives/Directories window.	*/
       if ( i >	1 )
	   {
		       /* Initialize the buffer	count before the file	*/
		       /* system is queried otherwise the call may fail	*/

	   cbDrvi = 1024UL;

	   if (	(ulError = DosQueryFSAttach(szDrive, 1UL, FSAIL_QUERYNAME,
					    (PFSQBUFFER2)psqbf,
					    &cbDrvi)) == ERROR_NOT_READY )
	       afRemoveable[cDrives++] = TRUE;
	   else
	       if ( !ulError )
		   if (	!memcmp(&psqbf->rgFSAData[1], "CDFS", 5) )
		       afRemoveable[cDrives++] = TRUE;
		   else
		       ++cDrives;
	   }
       else
	   afRemoveable[cDrives++] = TRUE;
       }
		       /* Shift	the drive designation bitmap mask	*/
   ulDriveMask <<= 1;
   }
DosError(FERR_ENABLEHARDERR);

		       /* Release the memory allocated for the query	*/
		       /* buffer					*/
free(psqbf);
}
#pragma	subtitle("   File Print - Directory Retrieval/Display Function")
#pragma	page( )

/* --- ulGetDirList -----------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	search for sub-directories and files	*/
/*     within the current sub-directory/drive and place	the entries	*/
/*     within two list boxes.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hwndDrives;	= Drives Combo Box Handle			*/
/*     HWND hwndDirs;	= Directories List Box Handle			*/
/*     HWND hwndFiles;	= Files	List Box Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ulGetDirList = Number of	Files in Files List Box			*/
/*									*/
/* --------------------------------------------------------------------	*/

static ULONG ulGetDirList(HWND hwndDrives, HWND	hwndDirs, HWND hwndFiles)

{
HDIR	      hDir = (HDIR)HDIR_CREATE;/* Directory Handle		*/
HPOINTER      hptr;		   /* Mouse Pointer Handle		*/
PFILEFINDBUF3 pfindbuf3;	   /* File Find	Buffer			*/
PFILEFINDBUF3 pfindbuf;		   /* File Find	Buffer			*/
ULONG	      cFiles = 0UL;	   /* Files Count			*/
ULONG	      ulCurDrive;	   /* Current Drive Designation		*/
ULONG	      ulDrives;		   /* Drive Bit	Map Holder		*/
ULONG	      ulErr;		   /* OS/2 Error Value			*/
ULONG	      ulFileCnt	= 71UL;	   /* File Counter Holder		*/
register INT i,	n;		   /* Loop Counters			*/

hptr = WinQueryPointer(HWND_DESKTOP);

		       /* Set the mouse	pointer	to hourglass shape	*/
		       /* while	directory search taking	place		*/

WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));

		       /* Allocate memory for a	large directory	search	*/
		       /* buffer					*/

pfindbuf3 = (PFILEFINDBUF3)malloc(71UL * sizeof(FILEFINDBUF3));

		       /* Get the handles of the list boxes and	disable	*/
		       /* updating of them while inserting the data	*/
		       /* into them and	delete the contents within them	*/

WinEnableWindowUpdate(hwndDirs,	 FALSE);
WinEnableWindowUpdate(hwndFiles, FALSE);
WinSendMsg(hwndDirs,  LM_DELETEALL, 0L,	0L);
WinSendMsg(hwndFiles, LM_DELETEALL, 0L,	0L);

		       /* Start	directory search			*/

if ( !(ulErr = DosFindFirst("*.*", &hDir, MUST_HAVE_DIRECTORY |	FILE_DIRECTORY,
			    (PVOID)pfindbuf3, 71UL * sizeof(FILEFINDBUF3),
			    &ulFileCnt,	FIL_STANDARD)) )
   {
   do
       for ( i = 0, pfindbuf = pfindbuf3; i < ulFileCnt; i++ )
	   {
		       /* If file found	a sub-directory, place into the	*/
		       /* Directories list box				*/

	   if (	(pfindbuf->attrFile & FILE_DIRECTORY) && memcmp(pfindbuf->achName, ".",	2) )
	       WinSendMsg(hwndDirs, LM_INSERTITEM,
			  MPFROMSHORT(LIT_SORTASCENDING),
			  MPFROMP(pfindbuf->achName));
	   pfindbuf = (PFILEFINDBUF3)((PBYTE)pfindbuf +	pfindbuf->oNextEntryOffset);
	   }
       while ( (ulFileCnt == 71UL) &&
	       !DosFindNext(hDir, pfindbuf3, 71UL * sizeof(FILEFINDBUF3), &ulFileCnt) );

		       /* Close	directory search handle			*/
   DosFindClose(hDir);
   }
else
   if (	ulErr != ERROR_NO_MORE_FILES )
       {
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/

       free((PVOID)pfindbuf3);
       return(0);
       }

if ( !fDrivesFilled )
   {
		       /* Form the current drive/path and display	*/
		       /* within the Path display field			*/

   DosQueryCurrentDisk(&ulCurDrive, &ulDrives);
   if (	ulDriveMap != ulDrives )
       QueryDriveList( );
   for ( i = 0;	i < cDrives; i++ )
       {
       if ( !afRemoveable[i] )
	   DiskVolDrive((ULONG)(i + 1),	&aszDrives[i][4]);
       else
	   aszDrives[i][4] = (CHAR)0;
       n = (INT)LONGFROMMR(WinSendMsg(hwndDrives, LM_INSERTITEM, MPFROMSHORT(LIT_END),
				MPFROMP(aszDrives[i])));
       if ( i == (ulCurDrive - 1) )
	   WinSendMsg(hwndDrives, LM_SELECTITEM, MPFROMLONG(n),	MPFROMLONG(TRUE));
       }
   fDrivesFilled = TRUE;
   }
		       /* Reset	directory handle and file counter	*/
hDir = (HDIR)HDIR_CREATE;
ulFileCnt = 71UL;
		       /* Start	file search				*/

if ( !(ulErr = DosFindFirst("*.*", &hDir, FILE_NORMAL, (PVOID)pfindbuf3, 71UL *	sizeof(FILEFINDBUF3), &ulFileCnt, FIL_STANDARD)) )
   {
   do
       for ( i = 0, pfindbuf = pfindbuf3; i < ulFileCnt; i++ )
	   {
		       /* If file found	not a sub-directory, place	*/
		       /* filename in list box				*/

	   if (	(pfindbuf->attrFile & FILE_DIRECTORY) != FILE_DIRECTORY	)
	       {
	       ++cFiles;
	       WinSendMsg(hwndFiles, LM_INSERTITEM, MPFROMSHORT(LIT_SORTASCENDING), MPFROMP(pfindbuf->achName));
	       }
	   pfindbuf = (PFILEFINDBUF3)((PBYTE)pfindbuf +	pfindbuf->oNextEntryOffset);
	   }
		       /* Search for remaining entries and place valid	*/
		       /* entries in list box				*/

       while ( (ulFileCnt == 71UL) && !DosFindNext(hDir, pfindbuf3, 71UL * sizeof(FILEFINDBUF3), &ulFileCnt) );

		       /* Close	directory search handle			*/
   DosFindClose(hDir);
   }
else
   if (	ulErr != ERROR_NO_MORE_FILES )
       {
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/

       free((PVOID)pfindbuf3);
       return(0);
       }
		       /* Release the memory allocated for the search	*/
		       /* buffer					*/
free((PVOID)pfindbuf3);
		       /* Re-enable updating to	the list boxes and	*/
		       /* force	the displaying of the new contents	*/

WinShowWindow(hwndDirs,	TRUE);
WinShowWindow(hwndFiles, TRUE);

		       /* Reset	edit field text	in case	invalid	search	*/
		       /* spec entered and reset mouse pointer back to	*/
		       /* arrow	shape					*/

WinSetPointer(HWND_DESKTOP, hptr);
return(cFiles);
}
#pragma	subtitle("   File Print - File Line Printing Routine")
#pragma	page( )

/* --- PrnLine ----------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	print a	multiple line entry such that	*/
/*     when the	string is wider	than the printable area	it is properly	*/
/*     truncated and the remainder printed on the next line or lines.	*/
/*									*/
/*     This routine is recursive.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HPS     hpsPrinter;  = Presentation Space Handle			*/
/*     HDC     hdcPrinter;  = Device Context Handle			*/
/*     PSZ     pszStr;	    = String to	Print				*/
/*     PPOINTL pptl;	    = Display Point				*/
/*     PPOINTL pptlFooter;  = Footer Display Point			*/
/*     PSIZEL  psizlPage;   = Page Size					*/
/*     LONG    yShift;	    = Vertical Line Shift			*/
/*     BOOL    fDateFooter; = Print Footer Flag				*/
/*     PSZ     pszFooter;   = Footer String				*/
/*     PULONG  pcPages;	    = Page Counter Pointer			*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     fPrnLine	=  TRUE	: New Page Started				*/
/*		= FALSE	: Normal Line Printed				*/
/*									*/
/* --------------------------------------------------------------------	*/

static BOOL fPrnLine(HPS hpsPrinter, HDC hdcPrinter, PSZ pszStr,
		     PPOINTL pptl, PPOINTL pptlFooter, PSIZEL psizlPage, LONG yShift,
		     BOOL fDateFooter, PSZ pszFooter, PULONG pcPages)

{
PPOINTL	aptlLimits;		   /* Limits Point Array Pointer	*/
POINTL	aptl[TXTBOX_COUNT];	   /* Character	Text Box Array		*/
register INT i,	n;		   /* Index				*/

		       /* Check	to see if the length of	the string is	*/
		       /* greater than 512 characters.	Present	GPI	*/
		       /* functions cannot handle greater than 512	*/
		       /* characters.					*/

if ( (n	= (INT)strlen(pszStr)) > 512 )
   n = 512;
		       /* Determine the	width of the line		*/

GpiQueryTextBox(hpsPrinter, n, pszStr, TXTBOX_COUNT, aptl);

		       /* Check	to see if the width is less than the	*/
		       /* width	of the page				*/

if ( aptl[TXTBOX_CONCAT].x < (psizlPage->cx - 720L) )
   GpiCharStringAt(hpsPrinter, pptl, n,	pszStr);
else
   {
		       /* Allocate memory for the point	array to allow	*/
		       /* the proper break to be found			*/

   DosAllocMem((PPVOID)(PVOID)&aptlLimits, (ULONG)(n * sizeof(POINTL)),
	       PAG_READ	| PAG_WRITE | PAG_COMMIT);

		       /* Get the point	position of each character	*/
		       /* within the string				*/

   GpiQueryCharStringPosAt(hpsPrinter, pptl, 0UL, n, pszStr, (PLONG)NULL, aptlLimits);

		       /* Search for a break character			*/

   if (	strchr(pszStr, ' ') )
       {
       for ( i = n - 1;	i > 0; i-- )
	   if (	aptlLimits[i].x	< (pptl->x + (psizlPage->cx - 720L)) )
	       {
	       while ( pszStr[i] && (pszStr[i] != ' ') )
		   --i;
	       ++i;
	       break;
	       }
       }
   else
		       /* No break character found, look for a clean	*/
		       /* location where the forced break can occur	*/

       for ( i = 0; i <	n; i++ )
	   if (	aptlLimits[i].x	> (pptl->x + (psizlPage->cx - 720L)) )
	       break;
		       /* Release the memory allocated for the point	*/
		       /* array						*/

   DosFreeMem((PVOID)aptlLimits);

		       /* Display the string				*/

   GpiCharStringAt(hpsPrinter, pptl, i,	pszStr);

		       /* Update the string pointer and	check to see	*/
		       /* if any more characters should	be displayed	*/
   pszStr += i;
   if (	*pszStr	)
       {
		       /* Update the position on the page		*/

       if ( (pptl->y -=	yShift)	< 720L )
	   {
		       /* Have reached the bottom of the page, display	*/
		       /* the footer for the page			*/

	   if (	fDateFooter )
	       GpiCharStringAt(hpsPrinter, pptlFooter, 37L, pszFooter);

		       /* Force	the ejection of	the page		*/

	   DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, NULL, NULL, NULL);

		       /* Reset	the pointer to the top of the page	*/

	   pptl->y = psizlPage->cy - yShift * TOP_MARGIN;
	   ++(*pcPages);
	   }
		       /* Print	the remaining portion of the string	*/

       return(fPrnLine(hpsPrinter, hdcPrinter, pszStr,
		       pptl, pptlFooter, psizlPage, yShift,
		       fDateFooter, pszFooter, pcPages));
       }
   }
		       /* Update the position on the page		*/

if ( (pptl->y -= yShift) < 720L	)
   {
		       /* Have reached the bottom of the page, display	*/
		       /* the footer for the page			*/

   if (	fDateFooter )
       GpiCharStringAt(hpsPrinter, pptlFooter, 37L, pszFooter);

		       /* Force	the ejection of	the page		*/

   DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, NULL, NULL, NULL);

		       /* Reset	the pointer to the top of the page	*/

   pptl->y = psizlPage->cy - yShift * TOP_MARGIN;
   ++(*pcPages);
   return(TRUE);
   }
else
   return(FALSE);
}
#pragma	subtitle("   File Print - File Printing Routine")
#pragma	page( )

/* --- PrnFile ----------------------------------------	[ Private ] ---	*/
/*									*/
/*     This function is	used to	read in	the file selected and		*/
/*     provide the appropriate decoding	for printing the file.		*/
/*     The function constructs a line from the file and	then		*/
/*     determines if further decoding is required or to	print		*/
/*     constructed line.						*/
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

VOID _System PrnFile(PPRNDATA pprni)

{
BOOL	    fEndOfFile;		   /* End of File Flag			*/
BOOL	    fNewPage;		   /* New Page Flag			*/
BYTE	    bCh;		   /* Input Character			*/
BYTE	    szLineBuf[8];	   /* Line Number Buffer		*/
BYTE	    szFooter[48];	   /* Footer Buffer			*/
DATETIME    dt;			   /* Date and Time Holder		*/
FILESTATUS3 fs;			   /* File Status			*/
FONTMETRICS fmPrn;		   /* Font Metrics Info			*/
HAB	    habThread;		   /* Thread Anchor Block Handle	*/
HCINFO	    hci;		   /* Hard Copy	Information Structure	*/
HDC	    hdcPrinter;		   /* Printer Device Context Handle	*/
HFILE	    hFile;		   /* Input File Handle			*/
HPS	    hpsPrinter;		   /* Printer Presentation Space Handle	*/
INT	    cLineNum;		   /* Line Number Character Counter	*/
INT	    cLines;		   /* Line Number Counter		*/
LONG	    lOut;		   /* Output Count			*/
LONG	    lcid;		   /* Logical Font ID			*/
LONG	    sPos;		   /* Current Buffer Position		*/
PBYTE	    pbInputBuffer;	   /* File Input Buffer			*/
PBYTE	    pszOutPutLine;	   /* Formatted	Output Buffer		*/
POINTL	    ptl;		   /* Display Point			*/
POINTL	    ptlFooter;		   /* Display Point			*/
SIZEL	    sizlPage;		   /* Page Size	Holder			*/
ULONG	    cPages;		   /* Page Count			*/
ULONG	    cbRead;		   /* Bytes Read			*/
ULONG	    ulAction;		   /* Action Taken			*/
USHORT	    usJobID;		   /* Spooler Job ID			*/
register INT i;			   /* Loop Counter			*/
register UINT sPointer;		   /* Input Buffer Pointer		*/

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
		       /* Determine the	size of	the output area		*/

       GpiQueryPS(hpsPrinter, &sizlPage);

		       /* Select and scale the font for	the printer	*/

       lcid = ScaleFont(hpsPrinter, pprni->fsel.szFacename,
			pprni->fsel.lNominalPointSize);

		       /* Get the new font metrics for the scaled font	*/
		       /* and determine	the number of lines that can be	*/
		       /* contained within the page			*/

       GpiQueryFontMetrics(hpsPrinter, sizeof(FONTMETRICS), &fmPrn);

		       /* Determine the	hard copy capabilities for the	*/
		       /* form selected.  Note that all	values returned	*/
		       /* are in millimetres except for	the xPels and	*/
		       /* yPels	elements.				*/

       DevQueryHardcopyCaps(hdcPrinter,	0L, 1L,	&hci);

       DosAllocMem((PPVOID)(PVOID)&pszOutPutLine, 4096UL, PAG_READ | PAG_WRITE | PAG_COMMIT);

		       /* Provide a 1/2	inch left margin including any	*/
		       /* device provided margin.  There are 57	twips	*/
		       /* per millimetre.				*/

       ptl.x = 1080L - hci.xLeftClip * 57L;

       if ( pprni->fDateFooter )
	   {
	   DosGetDateTime(&dt);

		       /* Copy the masking string into the buffer and	*/
		       /* copy the day and month strings into the	*/
		       /* buffer					*/

	   memcpy(szFooter, "Printed:  Tue Jan 01 00:00:00.00 1970", 38);
	   memcpy(&szFooter[10], (PCHAR)(szDayNames + (dt.weekday * 3)), 3);
	   memcpy(&szFooter[14], (PCHAR)(szMonthNames +	((dt.month - 1)	* 3)), 3);

		       /* Point	to the day of the month	within the	*/
		       /* buffer and store the day of the month	within	*/
		       /* the buffer (1	- 31)				*/

	   szFooter[18]	= (CHAR)('0' + (dt.day / 10));
	   szFooter[19]	= (CHAR)('0' + (dt.day % 10));

		       /* Store	the hours within the buffer (0 - 23)	*/

	   szFooter[21]	= (CHAR)('0' + (dt.hours / 10));
	   szFooter[22]	= (CHAR)('0' + (dt.hours % 10));

		       /* Store	the minutes within the buffer (0 - 59)	*/
		
	   szFooter[24]	= (CHAR)('0' + (dt.minutes / 10));
	   szFooter[25]	= (CHAR)('0' + (dt.minutes % 10));

		       /* Store	the seconds within the buffer (0 - 59)	*/

	   szFooter[27]	= (CHAR)('0' + (dt.seconds / 10));
	   szFooter[28]	= (CHAR)('0' + (dt.seconds % 10));

		       /* Store	the hundredths within the buffer	*/
		       /* (0 - 100)					*/

	   szFooter[30]	= (CHAR)('0' + (dt.hundredths /	10));
	   szFooter[31]	= (CHAR)('0' + (dt.hundredths %	10));

		       /* Store	the year within	the buffer		*/

	   szFooter[33]	= (CHAR)('0' + ((dt.year / 100)	/ 10));
	   szFooter[34]	= (CHAR)('0' + ((dt.year / 100)	% 10));
	   szFooter[35]	= (CHAR)('0' + ((dt.year % 100)	/ 10));
	   szFooter[36]	= (CHAR)('0' + ((dt.year % 100)	% 10));
	   ptlFooter.x = 1080L - hci.xLeftClip * 57L;
	   ptlFooter.y = TOP_MARGIN / 2L;
	   }

		       /* Open the requested file for printing		*/

       for ( i = 0; i <	pprni->cPrnFiles; i++ )
	   {
	   if (	DosOpen(pprni->aszFiles[i], &hFile, &ulAction, 0UL, FILE_NORMAL,
			OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
			OPEN_ACCESS_READONLY | OPEN_SHARE_DENYWRITE, NULL) )
	       continue;
	   else
	       {
		       /* Get the file information for the file	which	*/
		       /* will allow the size of the file to be		*/
		       /* determined and allow the entire file to be	*/
		       /* read in in one read				*/

	       DosQueryFileInfo(hFile, FIL_STANDARD, (PVOID)&fs, sizeof(FILESTATUS3));
	       DosAllocMem((PPVOID)(PVOID)&pbInputBuffer, fs.cbFile + 1UL, PAG_READ | PAG_WRITE	| PAG_COMMIT);
	       DosRead(hFile, (PVOID)pbInputBuffer, fs.cbFile, &cbRead);

		       /* Close	the file opened	for printing and clear	*/
		       /* the printing active flag			*/

	       DosClose(hFile);

	       cLines	 = 1;
	       cPages	 = 1;
	       fNewPage	 = TRUE;
	       sPointer	= 0;
	       fEndOfFile = FALSE;

		       /* Check	to see if line numbers requested and if	*/
		       /* required form	the start of the line		*/

	       if ( pprni->fLineNumbers	)
		   {
		   memcpy(pszOutPutLine, " 00000: ", 9);
		   sPos	= 8;
		   }
	       else
		   {
		   pszOutPutLine[0] = (CHAR)0;
		   sPos	= 0;
		   }
		       /* Enter	the loop that reads in the requested	*/
		       /* file,	and sends it to	the printing context	*/
	       do
		   {
		   if (	sPointer >= cbRead )

		       /* Logical End-of-File encountered, set		*/
		       /* character to End-of-File value		*/

		       bCh = (CHAR)26;
		   else
		       /* Get a	character from the file	input buffer	*/

		       bCh = pbInputBuffer[sPointer++];

		       /* Decode character to line demarcation values	*/
		   switch ( bCh	)
		       {
		       case CR :
			   continue;

		       case TAB	:
			   do pszOutPutLine[sPos] = (CHAR)' ';
			       while ( ++sPos %	8 );
			   break;

		       case TOF	:

		       /* Complete the printing	of the page and	set	*/
		       /* new page flag	before returning		*/

			   if (	pprni->fDateFooter )
			       GpiCharStringAt(hpsPrinter, &ptlFooter, 37L, szFooter);
			   DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, NULL, NULL, NULL);
			   fNewPage = TRUE;
			   break;

		       case LF :

		       /* Line feed signifying end-of-line encountered	*/
		       /* complete the line and	send for processing	*/
		       /* and printing					*/

			   pszOutPutLine[sPos] = (CHAR)0;
			   if (	fNewPage )
			       {
		       /* Form top margin co-ordinates			*/

			       ptl.y = sizlPage.cy - fmPrn.lMaxBaselineExt * TOP_MARGIN;

		       /* Reset	new page flag				*/

			       fNewPage	= FALSE;
			       }
			   if (	pprni->fLineNumbers )
			       {
			       cLineNum	= (INT)strlen(itoa(cLines++, szLineBuf,	10));

			       memcpy(&pszOutPutLine[6 - cLineNum], szLineBuf, (UINT)cLineNum);
			       }

		       /* Print	out formatted line			*/

			   fNewPage = fPrnLine(hpsPrinter, hdcPrinter, pszOutPutLine,
					       &ptl, &ptlFooter, &sizlPage, fmPrn.lMaxBaselineExt,
					       pprni->fDateFooter, szFooter, &cPages);
			   if (	pprni->fLineNumbers )
			       sPos = 8;
			   else
			       sPos = 0;
			   break;

		       case 26 :

		       /* End-of-File mark encounted, set end-of-file	*/
		       /* file to stop input processing			*/

			   fEndOfFile =	TRUE;
			   break;

		       /* Normal character, save in output buffer	*/

		       default :
			   pszOutPutLine[sPos++] = bCh;
		       }
		   } while ( !fEndOfFile );

	       if ( !fNewPage && ((i + 1) != pprni->cPrnFiles) )
		   {
		   if (	pprni->fDateFooter )
		       GpiCharStringAt(hpsPrinter, &ptlFooter, 37L, szFooter);
		   DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, NULL, NULL, NULL);
		   if (	cPages % 2 )
		       {
		       if ( pprni->fDateFooter )
			   GpiCharStringAt(hpsPrinter, &ptlFooter, 37L,	szFooter);
		       DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, NULL,	NULL, NULL);
		       }
		   }
	       }
	   DosFreeMem((PVOID)pbInputBuffer);
	   }
       if ( pprni->fDateFooter )
	   GpiCharStringAt(hpsPrinter, &ptlFooter, 37L,	szFooter);
       DosFreeMem((PVOID)pszOutPutLine);

		       /* Reset	the character set back to the default	*/

       GpiSetCharSet(hpsPrinter, 0L);
       GpiDeleteSetId(hpsPrinter, lcid);

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
   }
DosFreeMem((PVOID)pprni);

		       /* Destroy the message queue used by the	thread	*/
		       /* and destroy the anchor block before exiting	*/
		       /* the thread					*/
WinTerminate(habThread);

DosExit(EXIT_THREAD, 0UL);
}
