#include "cdk.h"

#ifdef HAVE_XCURSES
char *XCursesProgramName="selection_ex";
#endif

int main (void)
{
   /* Declare variables. */
   CDKSCREEN *cdkscreen		= (CDKSCREEN *)NULL;
   CDKSELECTION	*selection	= (CDKSELECTION *)NULL;
   WINDOW *cursesWin		= (WINDOW *)NULL;
   char *title			= "<C></5>Pick one or more accounts.";
   char *choices[]		= {"   ", "***"};
   char *item[400], temp[256], *mesg[200];
   struct passwd *ent;
   int count, x, y;

   /* Set up CDK. */ 
   cursesWin = initscr();
   cdkscreen = initCDKScreen (cursesWin);

   /* Set up CDK Colors. */
   initCDKColor();

   /* Use the account names to create a list. */
   count = 0;
   while ((ent = getpwent ()) != NULL)
   {
      item[count++] = copyChar (ent->pw_name);
   }
   count--;

   /* Create the selection list. */
   selection = newCDKSelection (cdkscreen, CENTER, CENTER, RIGHT,
					10, 50, title, item, count, choices, 2,
					A_REVERSE, TRUE, FALSE);

   /* Is the selection list NULL? */
   if (selection == (CDKSELECTION *)NULL)
   {
      /* Exit CDK. */
      destroyCDKScreen (cdkscreen);
      endCDK();

      /* Print out a message and exit. */
      printf ("Oops. Can;t seem to create the selection list. Is the window too small?\n");
      exit (1);
   }

   /* Activate the selection list. */
   activateCDKSelection (selection, (chtype *)NULL);

   /* Check the exit status of the widget. */
   if (selection->exitType == vESCAPE_HIT)
   {
      mesg[0] = "<C>You hit escape. No items selected.";
      mesg[1] = "",
      mesg[2] = "<C>Press any key to continue.";
      popupLabel (cdkscreen, mesg, 3);
   }
   else if (selection->exitType == vNORMAL)
   {
      mesg[0] = "<C>Here are the accounts you selected.";
      y = 1;
      for (x=0; x < count; x++)
      {
         if (selection->selections[x] == 1)
         {
            sprintf (temp, "<C></5>%s", item[x]);
            mesg[y++] = copyChar (temp);
         }
      }
      popupLabel (cdkscreen, mesg, y);

      /* Clean up. */
      for (x=1; x < y; x++)
      {
          freeChar (mesg[x]);
      }
   }

   /* Clean up. */
   destroyCDKSelection (selection);
   destroyCDKScreen (cdkscreen);
   delwin (cursesWin);
   endCDK();
   exit (0);
}