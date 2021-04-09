/*
* para provides parallel signal processing by feeding its stdin (through pipes) 
* to each of several monophonic processing pipelines which run in parallel,
* summing the outputs of these processes, and writing the resulting 
* summed data stream on the standard output. A typical command form:
* 
* cmusic score.sc | para 2 "reverb .6 1000" "reverb .7 1131" | sndout
* 
* applies both "reverb .6 1000" and "reverb .7 1131" to separate copies
* of the output of cmusic, then adds the parallel signals and pipes 
* the sum to sndout.  If only one process is specified for multiple channels, 
* it is applied in parallel to each channel.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXCHAN 4
#define ERR(msg) {fprintf(stderr,msg); exit(-1);}
#define MS(msg,val) {fprintf(stderr,msg,val);}

main(argc, argv) int argc; char *argv[]; {
    register i, nchan;
    int fdi[MAXCHAN][2], fdo[MAXCHAN][2];
    FILE *pipe_in[MAXCHAN], *pipe_out[MAXCHAN];
    float sample;
    int data, status;

    if(argc > 1) nchan = atoi(argv[1]);
    if (argc != 3 && argc != nchan + 2) 
	ERR("Usage: para N `cmd' -or- para N `cmd1' ... `cmdN'\n");
    if(nchan < 2 || nchan > MAXCHAN)
	ERR("para: illegal number of channels\n");

    for(i = 0; i < nchan; i++)
	if( pipe( &fdi[i][0] ) < 0 || pipe( &fdo[i][0] ) < 0)
	    ERR("para: pipe overflow\n");
/* 
   spouse process which sums output pipes, and writes on stdout
*/
    if( !fork() ){
	for(i = 0; i < nchan; i++){
	    close( fdi[i][0] );
	    close( fdi[i][1] );
	    close( fdo[i][1] );
	    pipe_out[i] = fdopen( fdo[i][0], "r");
	}

	if( !isatty(1)) while(1){ 
	 float sum;
	    for(sum = 0.0, i = 0; i < nchan; i++, sum += sample)
		data = fread( &sample, sizeof(float), 1, pipe_out[i] ); 
	    if (data) fwrite( &sum, sizeof(float), 1, stdout );
		else exit(0);
	} else while(1){ 
	 register float sum;
	    for(sum = 0.0, i = 0; i < nchan; i++, sum += sample)
		data = fread( &sample, sizeof(float), 1, pipe_out[i] ); 
	    if (data) printf("%f\n",sum);
		else exit(0);
	}
    }
/*
   processing children read stdin and write stdout, which are connected
   to process input and process output pipes, respectively
*/
    for(i = 0; i < nchan; i++)if( !fork() ){
    /* connect stdin to process input pipes */
	close( fdi[i][1] );
	dup2( fdi[i][0], 0 );
	close( fdi[i][0] );
    /* connect stdout to process output pipes */
	close( fdo[i][0] );
	dup2( fdo[i][1], 1 );
	close( fdo[i][1] );
    /* execute processes */
	if(argc == 3)
	    execlp( "/bin/csh", "csh", "-c", argv[2], 0);
	else
	    execlp( "/bin/csh", "csh", "-c", argv[i+2], 0);

	ERR("para: We NEVER come here!");
    }
/*
   parent reads its stdin, and writes on input pipes of each process 
*/
    for(i = 0; i < nchan; i++){
	close( fdo[i][0] );
	close( fdo[i][1] );
	close( fdi[i][0] );
	if( (pipe_in[i] = fdopen( fdi[i][1], "w") ) == NULL)
	    ERR("para: fdopen failed\n");
    }

    if( !isatty(0) ) while( fread( &sample, sizeof(float), 1, stdin ) ){
	for( i = 0; i < nchan; i++ )
	    if( !fwrite( &sample, sizeof(float), 1, pipe_in[i] ) )
		ERR("para: unable to write process input pipes\n");
    } else wait( &status );
}
