/*========================================================
				Initialize the game
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include <ultra64.h>
#include <nualstl_n.h>
#include "configuration.h"
#include "stages.h"


/*=================================
             Prototypes
=================================*/

static void   vsyncCallback(int pendingTaskCount);
void initAudio(void);


/*=================================
         Global Variables
=================================*/

int ptr_buf[NU_AU_SAMPLE_SIZE];
int sfx_buf[NU_AU_SE_SIZE];

char mem_heap[1024*512]; 	 // Half a megabyte of heap memory

NUContData contdata[1];      // Player 1 Controller data
u8 contPattern;              // Controller Manager


/*=================================
              Main
=================================*/

void  mainproc(void)
{
    // Initialize and activate the graphics thread and Graphics Task Manager.
    nuGfxInit(); 
	
	// Initialize the heap for dynamic memory allocation
	if (InitHeap(mem_heap, sizeof(mem_heap)) == -1)
        return; // or stop the program if something horrible happens.
    
    // Initialize audio thread
    initAudio();
	
	// Initialize controller manager.
    contPattern = nuContInit();
    
    // Create a callback function
    nuGfxFuncSet((NUGfxFunc)vsyncCallback);
    
    // Turn on the screen
    nuGfxDisplayOn();

    // Initialize stage00
    stage00_init();

    // Loop forever to keep the idle thread busy
    while(1);
}


/*=================================
              initAudio
    Initialize the audio thread
=================================*/
void initAudio(void)
{
    musConfig c;

    c.control_flag	    = 0;                                // Set to MUSCONTROL_RAM if wbk file also placed in RAM
    c.channels		    = NU_AU_CHANNELS;                   // Maximum total number of channels
    c.sched		        = NULL;                             // The address of the Scheduler structure. NuSystem uses NULL
    c.thread_priority	= NU_AU_MGR_THREAD_PRI;             // thread priority (highest)
    c.heap		        = (unsigned char*)NU_AU_HEAP_ADDR;  // Heap address
    c.heap_length	    = NU_AU_HEAP_SIZE;                  // Heap size
    c.ptr		        = NULL;                             // Allows you to set a default ptr file
    c.wbk		        = NULL;                             // Allows you to set a default wbk file
    c.default_fxbank	= NULL;                             // Allows you to set a default bfx file
    c.fifo_length	    = NU_AU_FIFO_LENGTH;                // The size of the library's FIFO buffer
    c.syn_updates	    = NU_AU_SYN_UPDATE_MAX;             // Specifies the number of updates usable by the synthesizer.
    c.syn_output_rate	= 44100;                            // Audio output rate. The higher, the better quality
    c.syn_rsp_cmds	    = NU_AU_CLIST_LEN;                  // The maximum length of the audio command list.
    c.syn_retraceCount	= 1;                                // The number of frames per retrace message
    c.syn_num_dma_bufs	= NU_AU_DMA_BUFFER_NUM;             // Specifies number of buffers Audio Manager can use for DMA from ROM.
    c.syn_dma_buf_size	= NU_AU_DMA_BUFFER_SIZE;            // The length of each DMA buffer

    // Initialize the Audio Manager.
    nuAuStlMgrInit(&c);

    // Register the PRENMI function.
    nuAuPreNMIFuncSet(nuAuPreNMIProc);
}

/*=================================
          vsyncCallback
This is a callback functions which
execute instructions, and once
it has run out of instructions, it
activates the display process.
=================================*/
void  vsyncCallback(int pendingTaskCount)
{
    // Update stage00
    stage00_update();
    
    // If out of instructions, make the display
    if(pendingTaskCount < 1) 
        stage00_draw();
}
