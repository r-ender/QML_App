#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>


void listen_msg(void)
{
    puts("Aufruf von listen_msg()!\n");
    long loops;
    int rc, openfd, size, dir;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    snd_pcm_uframes_t frames;
    char *buffer;

    //read in file-descriptor of audiofile
    if( fopen("/tmp/jabra_capture.wav", "r") == NULL) {
        printf("Error opening/creating audio sample file!\n");
        exit(1);
    }
    openfd = open("/tmp/jabra_capture.wav", O_RDONLY);
    if(openfd < 0) printf("Error opening audio sample file!\n");

    //opens the default PCM device and set the access mode to PLAYBACK.
    //This function returns a handle in the first function argument that is used in subsequent calls to manipulate the PCM stream
    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
      fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
      exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */
    /* Interleaved mode = left and right channel information is stored alternately within a frame; this is called interleaved mode */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    /* One channel (mono) */
    snd_pcm_hw_params_set_channels(handle, params, 1);

    /* 44100 bits/second sampling rate (CD quality)
        note: sound hardware is not always able to support every sampling rate exactly.
        We use the function snd_pcm_hw_params_set_rate_near to request the nearest supported sampling rate. */
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params,&val, &dir);

    //The buffer can be quite large, and transferring it at once could cause latency. Hence ALSA splits the buffer up into a series of periods and transfers the data in units of a period.
    //A period stores frames, each of which contains the samples captured at one point in time. For a stereo device, the frame would contain samples for two channels.
    /* Set period size to 4410 frames. */
    frames = 4410; // = 1 period-size

    /* Params not active until we call the function snd_pcm_hw_params!! Write the parameters to the driver! */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
      fprintf(stderr,"unable to set hw parameters: %s\n", snd_strerror(rc));
      exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    //size = frames * 2 --> 2 bytes/sample, 1 channel
    size = frames * 2;
    buffer = (char *) malloc(size);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    /* 5 seconds in microseconds divided by period time */
    loops = 5000000 / val;

    while (loops > 0) {
      loops--;
      //rc = read(0, buffer, size);	//hier fd=0 ??ndern um Fie direkt im Programm zu lesen anstatt per terminal einzulesen, fd=0 bedeutet "read from keyboard"
      rc = read(openfd, buffer, size);
      if (rc == 0) {
        fprintf(stderr, "end of file on input\n");
        break;
      } else if (rc != size) {
        fprintf(stderr, "short read: read %d bytes\n", rc);
      }
      rc = snd_pcm_writei(handle, buffer, frames);
      if (rc == -EPIPE) {
        /* EPIPE means underrun */
        fprintf(stderr, "underrun occurred\n");
        snd_pcm_prepare(handle);
      } else if (rc < 0) {
        fprintf(stderr,"error from writei: %s\n", snd_strerror(rc));
      }  else if (rc != (int)frames) {
        fprintf(stderr, "short write, write %d frames\n", rc);
      }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
    close(openfd);
    /*
    if( remove( "/tmp/jabra_capture.wav" ) != 0 )
        perror( "Error deleting file" );
      else
        puts( "File successfully deleted");
	*/
    puts("Ende von listen_msg()!\n");
}


int main(int argc, char *argv[])
{
    listen_msg();
    return 0;
}


