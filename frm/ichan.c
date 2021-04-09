/*
* ichan provides multichannel signal processing by demultiplexing its standard
* input, feeding each channel (through pipes) to one of several monophonic 
* processing pipelines which run as parallel processes. Unlike chan, ichan does
* not remultiplex and produce any output.
*
* cmusic stereo.sc | ichan 2 "peak"
*
* takes the output of cmusic, separates the 2 channels, applies 
* "peak" to each channel, which produces two reports in parallel.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MAXCHAN 4
#define ERR(msg) {fprintf(stderr,msg); exit(-1);}
#define MS(msg,val) {fprintf(stderr,msg,val);}

main(argc, argv) int argc; char *argv[]; {
    register i, nchan;
    int fdi[MAXCHAN][2], fdo[MAXCHAN][2];
    FILE *pipe_in[MAXCHAN], *pipe_out[MAXCHAN];
    float frame[MAXCHAN];
    int data, status;

    if(argc > 1) nchan = atoi(argv[1]);
    if (argc != 3 && argc != nchan + 2) 
	ERR("Usage: ichan N 'cmd' -or- chan N 'cmd1' ... 'cmdN'\n");
    if(nchan < 2 || nchan > MAXCHAN)
	ERR("ichan: illegal number of channels\n");

    for(i = 0; i < nchan; i++)
	if( pipe( &fdi[i][0] ) < 0) ERR("ichan: pipe overflow\n");

	if( !isatty(1) )while(1){
	    for(i = 0; i < nchan; i++)
	     data = fread( &frame[i], sizeof(float), 1, pipe_out[i] ); 
	    if (data) fwrite( &frame[0], sizeof(float), nchan, stdout );
		else exit(0);
	}
/*
   processing children read stdin, which are connected to process input
*/
    for(i = 0; i < nchan; i++)if( !fork() ){
    /* connect stdin to process input pipes */
	close( fdi[i][1] );
	dup2( fdi[i][0], 0 );
	close( fdi[i][0] );
    /* execute processes */
	if(argc == 3)
	    execlp( "/bin/csh", "csh", "-c", argv[2], 0);
	else
	    execlp( "/bin/csh", "csh", "-c", argv[i+2], 0);

	ERR("ichan: We NEVER come here!");
    }
/*
   parent process reads its stdin, demuxes, and writes on input pipes
   of each parallel process 
*/
    for(i = 0; i < nchan; i++){
	close( fdi[i][0] );
	if( (pipe_in[i] = fdopen( fdi[i][1], "w") ) == NULL)
	    ERR("ichan: fdopen failed\n");
    }

    if( !isatty(0) ) while( fread( &frame[0], sizeof(float), nchan, stdin ) ){
	for( i = 0; i < nchan; i++ )
	    if( !fwrite( &frame[i], sizeof(float), 1, pipe_in[i] ) )
		ERR("ichan: unable to write process input pipes\n");
    } else wait( &status );
}
