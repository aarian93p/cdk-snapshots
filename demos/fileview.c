#include "cdk.h"

#ifdef HAVE_XCURSES
char *XCursesProgramName="codeViewer";
#endif

#define	MAXINFOLINES	10000

/*
 * This program demonstrates the file selector and the viewer widget.
 */
int main (int argc, char **argv)
{
   /* Declare variables. */
   CDKSCREEN *cdkscreen	= (CDKSCREEN *)NULL;
   CDKVIEWER *example	= (CDKVIEWER *)NULL;
   CDKFSELECT *fSelect	= (CDKFSELECT *)NULL;
   WINDOW *cursesWin	= (WINDOW *)NULL;
   char *title		= "<C>Pick a file.";
   char *label		= "File: ";
   char *directory	= ".";
   char *filename	= (char *)NULL;
   char *info[MAXINFOLINES];
   char *button[5], vtitle[256], *mesg[4], temp[256];
   int selected, lines, ret;

   /* Parse up the command line. */
   while (1)
   {
      ret = getopt (argc, argv, "d:f:");
      if (ret == -1)
      {
         break;
      }
      switch (ret)
      {
         case 'd' :
              directory = strdup (optarg);
              break;

         case 'f' :
              filename = strdup (optarg);
              break;
      }
   }

   /* Create the viewer buttons. */
   button[0]	= "</5><OK><!5>";
   button[1]	= "</5><Cancel><!5>";

   /* Set up CDK. */
   cursesWin = initscr();
   cdkscreen = initCDKScreen (cursesWin);

   /* Start color. */
   initCDKColor();

   /* Get the filename. */
   if (filename == 0)
   {
      fSelect = newCDKFselect (cdkscreen, CENTER, CENTER, 20, 65,
				title, label, A_NORMAL, '_', A_REVERSE,
				"</5>", "</48>", "</N>", "</N>", TRUE, FALSE);

      /*
       * Set the starting directory. This is not neccessary because when
       * the file selector starts it uses the present directory as a default.
       */
      setCDKFselect (fSelect, directory, A_NORMAL, '.', A_REVERSE,
			"</5>", "</48>", "</N>", "</N>", ObjOf(fSelect)->box);

      /* Activate the file selector. */
      filename = copyChar (activateCDKFselect (fSelect, NULL));

      /* Check how the person exited from the widget. */
      if (fSelect->exitType == vESCAPE_HIT)
      {
         /* Pop up a message for the user. */
         mesg[0] = "<C>Escape hit. No file selected.";
         mesg[1] = "";
         mesg[2] = "<C>Press any key to continue.";
         popupLabel (cdkscreen, mesg, 3);

         /* Destroy the file selector. */
         destroyCDKFselect (fSelect);

         /* Exit CDK. */
         destroyCDKScreen (cdkscreen);
         endCDK();

         exit (0);
      }
   }

   /* Destroy the file selector. */
   destroyCDKFselect (fSelect);

   /* Create the file viewer to view the file selected.*/
   example = newCDKViewer (cdkscreen, CENTER, CENTER, 20, -2,
				button, 2, A_REVERSE, TRUE, FALSE);

   /* Could we create the viewer widget? */
   if (example == (CDKVIEWER *)NULL)
   {
      /* Exit CDK. */
      destroyCDKScreen (cdkscreen);
      endCDK();

      /* Print out a message and exit. */
      printf ("Oops. Can't seem to create viewer. Is the window too small?\n");
      exit (0);
   }

   /* Open the file and read the contents. */
   lines = readFile (filename, info, MAXINFOLINES);
   if (lines == -1)
   {
      printf ("Could not open %s\n", filename);
      exit (1);
   }

   /* Set up the viewer title, and the contents to the widget. */
   sprintf (vtitle, "<C></B/22>%20s<!22!B>", filename);
   setCDKViewer (example, vtitle, info, lines, A_REVERSE, TRUE, TRUE, TRUE);

   /* Activate the viewer widget. */
   selected = activateCDKViewer (example, (chtype *)NULL);

   /* Check how the person exited from the widget.*/
   if (example->exitType == vESCAPE_HIT)
   {
      mesg[0] = "<C>Escape hit. No Button selected.";
      mesg[1] = "";
      mesg[2] = "<C>Press any key to continue.";
      popupLabel (cdkscreen, mesg, 3);
   }
   else if (example->exitType == vNORMAL)
   {
      sprintf (temp, "<C>You selected button %d", selected);
      mesg[0] = copyChar (temp);
      mesg[1] = "";
      mesg[2] = "<C>Press any key to continue.";
      popupLabel (cdkscreen, mesg, 3);
   }

   /* Clean up. */
   destroyCDKViewer (example);
   destroyCDKScreen (cdkscreen);
   delwin (cursesWin);
   freeChar (filename);
   endCDK();
   exit (0);
}