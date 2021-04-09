/*
* chan provides multichannel signal processing by demultiplexing its standard
* input, feeding each channel (through pipes) to one of several monophonic 
* processing pipelines which run as parallel processes, remultiplexing the 
* outputs of these processes, and writing the resulting multiplexed data stream
* on the standard output. A typical command form:
*
* cmusic stereo.sc | chan 2 "reverb .6 1000" "reverb .7 1131" | sndout -c 2
*
* takes the output of cmusic, separates the 2 channels, applies 
* "reverb .6 1000" to the first channel, and "reverb .7 1131" to the second, 
* then recombines the signal and pipes it to sndout.  If only one process 
* is specified for multiple channels, it is applied in parallel to each channel.
*/

#include <stdio.h>
#include <carl/carl.h>

#define MAXCHAN 4
#define ERR(msg) {fprintf(stderr,msg); exit(-1);}
#define MS(msg,val) {fprintf(stderr,msg,val);}

main(argc, argv) int argc; char *argv[]; 
{
    register i, nchan;
    int fdi[MAXCHAN][2], fdo[MAXCHAN][2];
    FILE *pipe_in[MAXCHAN], *pipe_out[MAXCHAN];
    float frame[MAXCHAN];
    int data, status;

    if (argc > 1) 
	nchan = atoi(argv[1]);
    if (argc != 3 && argc != nchan + 2) 
	ERR("Usage: chan N `cmd' -or- chan N `cmd1' ... `cmdN'\n");
    if (nchan < 2)
	ERR("chan: 2 channels minimum\n");
    if (nchan > MAXCHAN)
	ERR("chan: 4 channels maximum\n");

    for (i = 0; i < nchan; i++)
	if ( pipe( &fdi[i][0] ) < 0 || pipe( &fdo[i][0] ) < 0)
	    ERR("chan: pipe overflow\n");
/* 
 *   spouse process which reads output pipes, remuxes, and writes on stdout
 */
    if ( !fork() ) {
	for (i = 0; i < nchan; i++) {
	    close( fdi[i][0] );
	    close( fdi[i][1] );
	    close( fdo[i][1] );
	    pipe_out[i] = fdopen( fdo[i][0], "r");
	}

	if ( !isatty(1) )
		while(1) {
		    for (i = 0; i < nchan; i++)
			data = fread(&frame[i], sizeof(float), 1, pipe_out[i]);
		    if (data > 0) 
			fwrite( &frame[0], sizeof(float), nchan, stdout );
		    else 
			exit(0);
		} 
	else 
		while(1) {
		    for (i = 0; i < nchan; i++)
		        data = fread(&frame[i], sizeof(float), 1, pipe_out[i]);
		    if (data) for (i = 0; i < nchan; i++) 
			printf("%f\n", frame[i]);
		    else 
			exit(0);
		}
    }
/*
 *   processing children read stdin and write stdout, which are connected
 *  to process input and process output pipes, respectively
 */
    for (i = 0; i < nchan; i++)if ( !fork() ) {
    /* connect stdin to process input pipes */
	close( fdi[i][1] );
	dup2( fdi[i][0], 0 );
	close( fdi[i][0] );
    /* connect stdout to process output pipes */
	close( fdo[i][0] );
	dup2( fdo[i][1], 1 );
	close( fdo[i][1] );
    /* execute processes */
	if (argc == 3)
	    execlp( "/bin/csh", "csh", "-c", argv[2], 0);
	else
	    execlp( "/bin/csh", "csh", "-c", argv[i+2], 0);

	ERR("chan: We NEVER come here!");
    }
/*
 *   parent process reads its stdin, demuxes, and writes on input pipes
 *   of each parallel process 
 */
    for (i = 0; i < nchan; i++) {
	close( fdo[i][0] );
	close( fdo[i][1] );
	close( fdi[i][0] );
	if ( (pipe_in[i] = fdopen( fdi[i][1], "w") ) == NULL)
	    ERR("chan: fdopen failed\n");
    }

    if ( !isatty(0) ) 
	while( fread( &frame[0], sizeof(float), nchan, stdin ) ) { 
	for ( i = 0; i < nchan; i++ )
	    if ( !fwrite( &frame[i], sizeof(float), 1, pipe_in[i] ) )
		ERR("chan: unable to write process input pipes\n");
    } else 
	wait( &status );
}
