/*========================================================
				   Stage 00 of the game
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include <nualstl_n.h>
#include "configuration.h"
#include "stages.h"


/*=================================
             Prototypes
=================================*/

static void paint_screen(char r, char g, char b);


/*=================================
          Global Variables
=================================*/

static int sndnum = 0;
static int sndtotal = 0;
static musHandle sndHandle = 0;
static int *sndcoarse;

/*=================================
           stage00_init
       Initialize the level
=================================*/

void stage00_init(void)
{
    int i=0;
    
    // Read and register the sample bank and its pointers.
    Rom2Ram((void *)PBANK_START, (void *)ptr_buf, PBANK_END-PBANK_START);
    MusPtrBankInitialize(ptr_buf, WBANK_START);

    // Read and register the sound effects.
    Rom2Ram((void *)SFX_START, (void *)sfx_buf, SFX_END-SFX_START);
	MusFxBankInitialize(sfx_buf);
	
	// Get the total number of sounds in the bank and allocate memory
    sndtotal = MusFxBankNumberOfEffects(sfx_buf);
	sndcoarse = (int*) malloc(sizeof(int)*sndtotal);
	
	// Set all the coarse values to 0
	for (i=0;i<sndtotal;i++)
	    sndcoarse[i] = 0;
}


/*=================================
          stage00_update
    Update the level every clock
=================================*/

void stage00_update(void)
{
    // Read data from the player 1 controller
    nuContDataGetExAll(contdata);
	
	// Change the sound index with left and right C buttons
	if(contdata[0].trigger & R_CBUTTONS)
	{
	    sndnum++;
		if (sndnum == sndtotal)
		    sndnum = 0;
	}
	else if(contdata[0].trigger & L_CBUTTONS)
	{
	    sndnum--;
		if (sndnum == -1)
		    sndnum = sndtotal-1;
	}
	
	// Change the sound coarse with up and down C buttons
	if(contdata[0].trigger & U_CBUTTONS)
	{
	    sndcoarse[sndnum]++;
	}
	else if(contdata[0].trigger & D_CBUTTONS)
	{
	    sndcoarse[sndnum]--;
	}
	
	// Play sound when you press A
	if (contdata[0].trigger & A_BUTTON)
	{
	    sndHandle = MusStartEffect(sndnum);
		MusHandleSetFreqOffset(sndHandle, sndcoarse[sndnum]);
    }
}


/*=================================
          stage00_draw
        Make our display
=================================*/

void stage00_draw(void)
{
    int i=0;
	char conbuff[8];
	
    // Assign our glist pointer to the glist array to make our life easier
    glistp = glist;

    // Initialze the RCP
    RCPInit();
    
    // Fill the screen with a color
    paint_screen(0, 0, 0);

    // Syncronize the CPU and GPU
    gDPFullSync(glistp++);
    
    // State that we finished our display list
    gSPEndDisplayList(glistp++);

    // Start the display list task
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);    

	// Start drawing console text
    nuDebConClear(NU_DEB_CON_WINDOW0);
	
	// Draw the border
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_RED);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "+------Buu342's Sound Tester V1------+");
	for (i=1;i<27;i++)
	{
	    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1+i);
        nuDebConCPuts(NU_DEB_CON_WINDOW0, "|");
	    nuDebConTextPos(NU_DEB_CON_WINDOW0, 38, 1+i);
        nuDebConCPuts(NU_DEB_CON_WINDOW0, "|");
	}
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 28);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "+------------------------------------+");
	
	// First line
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 4, 4);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Sounds loaded: ");
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 19, 4);
	sprintf(conbuff, "%d",sndtotal);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuff);
	
	// Second line
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 4, 6);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Index: ");
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 6);
	sprintf(conbuff, "%d",sndnum);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuff);
	
	// Third line
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 4, 8);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Coarse: ");
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 12, 8);
	sprintf(conbuff, "%d",sndcoarse[sndnum]);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuff);
	
	// Controls block
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 17, 22);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Controls");
	
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 17, 23);
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_BLUE);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "A:");
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 24, 23);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "Play sound");
	
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 17, 24);
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_YELLOW);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "C L/R:");
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 24, 24);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "Change sound");
	
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 17, 25);
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_YELLOW);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "C U/D:");
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 24, 25);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "Tune sample");	
	
	// Finalize
	nuDebConDisp(NU_SC_SWAPBUFFER);
}


/*=================================
            paint_screen
    Wipe the screen with a color
=================================*/

static void paint_screen(char r, char g, char b)
{
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
}