//capture_voice()

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <bluetooth.h>
#include <rfcomm.h>
#include <sys/stat.h>



#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>


int main(int argc, char *argv[])
{
     int rc, openfd, size, dir;
      long loops;
      snd_pcm_t *handle;
      snd_pcm_hw_params_t *params;
      unsigned int val;
      snd_pcm_uframes_t frames;
      char *buffer;
	bool voice_flag = false;

      if( fopen("/tmp/jabra_cap.wav", "w") == NULL) printf("Error opening/creating audio sample file!\n");
      openfd = open("/tmp/jabra_cap.wav", O_WRONLY);
      if(openfd < 0) printf("Error opening audio sample file!\n");

      rc = snd_pcm_open(&handle, "hw:2,0", SND_PCM_STREAM_CAPTURE, 0);
        if (rc < 0) {
          fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
          exit(1);
        }

        /* Allocate a hardware parameters object. */
        snd_pcm_hw_params_alloca(&params);

        /* Fill it in with default values. */
        snd_pcm_hw_params_any(handle, params);

        /* Set the desired hardware parameters. */
        /* Interleaved mode */
        snd_pcm_hw_params_set_access(handle, params,SND_PCM_ACCESS_RW_INTERLEAVED);

        /* Signed 16-bit little-endian format */
        snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

        /* Two channels (stereo) */
        //snd_pcm_hw_params_set_channels(handle, params, 2);
        //test mono, 1 channel:
        snd_pcm_hw_params_set_channels(handle, params, 1);

        /* 44100 bits/second sampling rate (CD quality) */
        val = 44100;
        //val = 22100;
        snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

        /* Set period size to 32 frames. */
        //frames = 32;
        frames = 4410; // = 1 * period-size
        //frames = 44400; // = 1 period-size
        
        snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

        /* Write the parameters to the driver */
        rc = snd_pcm_hw_params(handle, params);
        if (rc < 0) {
           fprintf(stderr,"unable to set hw parameters: %s\n",
                   snd_strerror(rc));
           exit(1);
        }

        /* Use a buffer large enough to hold one period */
        snd_pcm_hw_params_get_period_size(params, &frames, &dir);
        //size = frames * 4; /* 2 bytes/sample, 2 channels */
        size = frames * 2;
        buffer = (char *) malloc(size);
        //qDebug() << "buffer-size: " << size << "\n";        //8820

        /* We want to loop for 5 seconds */
        snd_pcm_hw_params_get_period_time(params, &val, &dir);
        loops = 5000000 / val; //5.000.000 / 44100 = ~120 loops

        while (loops > 0  && voice_flag == false) {
          loops--;
          rc = snd_pcm_readi(handle, buffer, frames);
          if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
          } else if (rc < 0) {
            fprintf(stderr,"error from read: %s\n",snd_strerror(rc));
          } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
          }

          rc = write(openfd, buffer, size);
          if (rc != size)
            fprintf(stderr,"short write: wrote %d bytes\n", rc);
        }

        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(buffer);
        close(openfd);
        close(fopen);
        
      puts("chapter 1\n");
      
        
        //hier variablen zum parsen eintragen
        //char *voice_buffer;
        long numbytes;
        FILE * fjab;
        
        //parse voicemsg.
        //read in file-descriptor of audiofile
        /*
	    if( fjab = fopen("/tmp/jabra_cap.wav", "r") == NULL) {
		printf("Error opening/creating audio sample file!\n");
		exit(1);
	    }
	    //openfd = open("/tmp/Phone_Ringing.wav", O_RDONLY);
	    openfd = open("/tmp/jabra_cap.wav", O_RDONLY);
	    if(openfd < 0) printf("Error opening audio sample file!\n");
	*/
	
	//file-size ermitteln
	off_t file_size;
	char *voice_buffer;
	struct stat stbuf;
	int fd2;
	  
	fd2 = open("/tmp/jabra_cap.wav", O_RDONLY);
	if (fd2 == -1) {
	  /* Handle error */
	}
	  
	if ((fstat(fd2, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) {
	  /* Handle error */
	}
	  
	file_size = stbuf.st_size;
	  
	voice_buffer = (char*)malloc(file_size);
	if (voice_buffer == NULL) {
	  /* Handle error */
	}  
	    
	    
	    //puts("chapter 1a\n");
	    //if (numbytes = ftell(fjab) == 0) puts("error reading ftell\n");
      
      puts("chapter 1b\n");
      numbytes = (long)file_size;
        
      //size_t fread(void * buffer, size_t size, size_t count, FILE * stream)  
      //fread(voice_buffer,sizeof(char), numbytes, fopen);
      rc = read(fd2, voice_buffer, (int)numbytes);
        
  puts("chapter 2\n");
       
    //hier abschicken 
    struct sockaddr_rc addr2 = { 0 };
    int s, status;

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);	////domain: bluetooth, type:data-stream, BT-Protocol: RFCOMM
	printf("socket set up - return value: %d\n", s);

    addr2.rc_family = AF_BLUETOOTH;
    addr2.rc_channel = (uint8_t) 1;
    //str2ba( addr, &addr2.rc_bdaddr );	//convert string to bt-adress
    str2ba("A8:6D:AA:B0:2D:49", &addr2.rc_bdaddr );

    status = connect(s, (struct sockaddr *)&addr2, sizeof(addr2));	//connect to server
    puts("connect\n");

    // send  message
    if( status == 0 ) {
        //status = write(s, "hello!", 6);
        status = write(s, voice_buffer, numbytes);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    close(openfd);
    close(fopen);
    
    puts("chapter 3\n");

	return 0;
}
