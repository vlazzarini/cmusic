/*********************************************************/
/* todac.c                                           
/* reads float samples from stdin and puts them
/* to an audio device via portaudio
/* (c) V Lazzarini, 2009
/*
/* This program is licensed under the GNU Public Licence
/*   See License.txt for details
/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <portaudio.h>

char crack (int argc, char **argv, char *flags, int ign, char **arg_option);
void usage(int code);

int main(int argc, char** argv){

  PaError err;
  const PaDeviceInfo *info;
  PaStreamParameters outparam;
  PaStream *handle;
  int i, chn=1, bufsize=4096, dev=0, sr=44100;
  float *buf, out = 0.f;
  char ch, *arg_option;

  while ((ch = crack(argc, argv, "c|r|b|d|h", 0, &arg_option)) != 0) {
		switch (ch) {
		case 'b': bufsize = atoi(arg_option); 
                   fprintf(stderr,"buffersize: %s samples\n", arg_option); break;
		case 'c': chn = atoi(arg_option); 
		   fprintf(stderr,"channels: %s\n", arg_option); break;
		case 'd': dev = atoi(arg_option);  
		  fprintf(stderr, "device: %s\n",arg_option); break;
		case 'r': sr = atoi(arg_option);  
                  fprintf(stderr, "sr: %s\n", arg_option); break;
		case 'h': usage(0);
		default: usage(1);
		}
		} 

  err = Pa_Initialize();
  if( err == paNoError){
    buf =  (float *)malloc(sizeof(float)*bufsize);   
    memset(&outparam, 0, sizeof(PaStreamParameters));
    outparam.device = (PaDeviceIndex) dev;
    chn = outparam.channelCount = chn;
    outparam.sampleFormat = paFloat32;

    err = Pa_OpenStream(&handle,NULL,&outparam,sr,bufsize,paNoFlag, 
			NULL, NULL);
    if(err == paNoError){
      err = Pa_StartStream(handle);
      if(err == paNoError){
        long cnt; 
	while((cnt = fread(buf, sizeof(float), bufsize, stdin)) == bufsize)
	  {
	    err = (int) Pa_WriteStream(handle, buf, cnt/chn);
	    if(err != paNoError)  printf("write error: %s \n", Pa_GetErrorText(err));
	  }
       Pa_StopStream(handle);
      } else printf("%s \n", Pa_GetErrorText(err));
      Pa_CloseStream(handle);
    } else printf("%s \n", Pa_GetErrorText(err));
    Pa_Terminate();
  }  else  printf("%s \n", Pa_GetErrorText(err));

  return 0;
}


char crack (int argc, char **argv, char *flags, 
int ign, char **arg_option)
{
  static arg_index =0;
  static char *pvcon=NULL;
  char   *pv,*flgp;
	while ((arg_index) < argc) {
		if (pvcon != NULL)
			pv = pvcon;
		else {
			if (++arg_index >= argc)
				return 0;
			pv = argv[arg_index];
			if (*pv != '-')
				return 0;
		}
		pv++;		/* skip '-' or prev. flag */
		if (*pv != 0) {
			if ((flgp = index (flags, *pv)) != NULL) {
				pvcon = pv;
				if (*(flgp + 1) == '|') {
					*arg_option = pv + 1;
					pvcon = NULL;
				}
				return (*pv);
			}
			else
				if (!ign) {
					fprintf (stderr, 
						"%s: no such flag: %s\n", 
						argv[0], pv);
					return (EOF);
				}
				else
					pvcon = NULL;
		}
		pvcon = NULL;
	}
	return 0;
}


void usage(int code)
{
fprintf (
stderr,
"usage: todac [flags] [filename] < input\n"
"input format: 32-bit floats"
"-bN\t set buffer size to N\n"
"-dN\t select device number N\n"
"-cN\t set number of channels to n\n"
"-rN\t set sampling rate to N\n"
"default usage: todac -b4096 -d0 -c1 -r44100\n"
);
exit(code);
}
