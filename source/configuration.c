/*========================================================
				Configuration of the game
				       for porting
========================================================*/
/*=================================
            Libraries
=================================*/

#include  <nusys.h>
#include "configuration.h"


/*=================================
           Global Variables
=================================*/

Gfx glist[DISPLAY_LIST_LEN]; // Display list array
Gfx *glistp;                 // Display list buffer


// Viewport
static Vp viewport = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	// Size of the viewport
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	// Position of the viewport
};


// RSP
Gfx setup_rsp[] = {
    // Set the viewport
    gsSPViewport(&viewport),
    
    // Clear the geometry pipeline modes
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
                            G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                            G_TEXTURE_GEN_LINEAR | G_LOD),
                            
    // Disable textures
    gsSPTexture(0, 0, 0, 0, G_OFF),
    
    // Specify the end of this display list
    gsSPEndDisplayList(), 
};


// RDP
Gfx setup_rdp[] = {
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipelineMode(G_PM_1PRIMITIVE),
	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
	gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetTextureFilter(G_TF_POINT),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};


/*=================================
	    	   RCP
     Initialize the RCP Chip.
	  RDP - Graphics creator
	   RSP - Sound and math
=================================*/

void RCPInit(void)
{
	// Set the RSP segment register
	gSPSegment(glistp++, 0, 0x0);

	// Setup the RSP
	gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rsp));

	// Setup the RDP
	gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rdp));
}


/*=================================
	    	gfxClearCfb
   Clear the Z and Frame Buffer
=================================*/
void gfxClearCfb(void)
{
	// Clear the Z-buffer 
	gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
	gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	gDPPipeSync(glistp++);

	// Clear the frame buffer 
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	gDPPipeSync(glistp++);
}


/*=================================
	    	Rom2Ram
Read data from the ROM and transfer 
        it to the RAM.
=================================*/
void Rom2Ram(void *from_addr, void *to_addr, s32 seq_size)
{
    // Cannot transfer if size is an odd number, so make it even
    if(seq_size & 0x00000001) 
        seq_size++;

    nuPiReadRom((u32)from_addr, to_addr, seq_size);
}