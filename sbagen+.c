//
//	SBaGen+ - Sequenced Brainwave Generator
//
//	Original version (c) 1999-2011 Jim Peters <jim@uazu.net>
//	This fork maintained by Ruan <https://ruan.sh/>
//
//	For latest version see http://sbagen.sf.net/ or
//	http://uazu.net/sbagen/. Released under the GNU GPL version 2.
//	Use at your own risk.
//
//	" This program is free software; you can redistribute it and/or modify
//	  it under the terms of the GNU General Public License as published by
//	  the Free Software Foundation, version 2.
//	  
//	  This program is distributed in the hope that it will be useful,
//	  but WITHOUT ANY WARRANTY; without even the implied warranty of
//	  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	  GNU General Public License for more details. "
//
//	See the file COPYING for details of this license.
//	
//	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//	Some code fragments in the Win32 audio handling are based on
//	code from PLIB (c) 2001 by Steve Baker, originally released
//	under the LGPL (slDSP.cxx and sl.h).  For the original source,
//	see the PLIB project: http://plib.sf.net
//
//	The code for the Mac audio output was based on code from the
//	FINK project's patches to ESounD, by Shawn Hsiao and Masanori
//	Sekino.  See: http://fink.sf.net

#define VERSION "1.5.2"

// This should be built with one of the following target macros
// defined, which selects options for that platform, or else with some
// of the individual named flags #defined as listed later.
//
//  T_LINUX	To build the LINUX version with ALSA support
//  T_MINGW	To build for Windows using MinGW and Win32 calls
//  T_MSVC	To build for Windows using MSVC and Win32 calls
//  T_MACOSX	To build for MacOSX using CoreAudio
//  T_POSIX	To build for simple file output on any Posix-compliant OS
//
// Ogg and MP3 support is handled separately from the T_* macros.

// Define ALSA_AUDIO to use ALSA for audio output
// Define WIN_AUDIO to use Win32 calls
// Define MAC_AUDIO to use Mac CoreAudio calls
// Define NO_AUDIO if no audio output device is usable
// Define UNIX_TIME to use UNIX calls for getting time
// Define WIN_TIME to use Win32 calls for getting time
// Define ANSI_TTY to use ANSI sequences to clear/redraw lines
// Define UNIX_MISC to use UNIX calls for various miscellaneous things
// Define WIN_MISC to use Windows calls for various miscellaneous things
// Define EXIT_KEY to require the user to hit RETURN before exiting after error

// Define OGG_DECODE to include OGG support code
// Define MP3_DECODE to include MP3 support code

#ifdef T_LINUX
#define ALSA_AUDIO
#define UNIX_TIME
#define UNIX_MISC
#define ANSI_TTY
#endif

#ifdef T_MINGW
#define WIN_AUDIO
#define WIN_TIME
#define WIN_MISC
#define EXIT_KEY
#endif

#ifdef T_MSVC
#define WIN_AUDIO
#define WIN_TIME
#define WIN_MISC
#define EXIT_KEY
#endif

#ifdef T_MACOSX
#define MAC_AUDIO
#define UNIX_TIME
#define UNIX_MISC
#define ANSI_TTY
#endif

#ifdef T_POSIX
#define NO_AUDIO
#define UNIX_TIME
#define UNIX_MISC
#endif

// Make sure NO_AUDIO is set if necessary
#ifndef MAC_AUDIO
#ifndef WIN_AUDIO
#ifndef ALSA_AUDIO
#define NO_AUDIO
#endif
#endif
#endif

// Make sure one of the _TIME macros is set
#ifndef UNIX_TIME
#ifndef WIN_TIME
#error UNIX_TIME or WIN_TIME not defined.  Maybe you did not define one of T_LINUX/T_MINGW/T_MACOSX/etc ?
#endif
#endif

// Make sure one of the _MISC macros is set
#ifndef UNIX_MISC
#ifndef WIN_MISC
#error UNIX_MISC or WIN_MISC not defined.  Maybe you did not define one of T_LINUX/T_MINGW/T_MACOSX/etc ?
#endif
#endif

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#ifdef T_MSVC
 #include <io.h>
 #define write _write
 #define vsnprintf _vsnprintf
 typedef long long S64;		// I have no idea if this is correct for MSVC
#else
 #include <unistd.h>
 #include <sys/time.h>
 typedef long long S64;
#endif

#ifdef T_MINGW
 #define vsnprintf _vsnprintf
#endif

#ifdef ALSA_AUDIO
 #include <alsa/asoundlib.h>
#endif
#ifdef WIN_AUDIO
 #include <windows.h>
 #include <mmsystem.h>
#endif
#ifdef MAC_AUDIO
#include <CoreAudio/CoreAudio.h>
#endif
#ifdef UNIX_TIME
 #include <sys/ioctl.h>
 #include <sys/times.h>
#endif
#ifdef UNIX_MISC
 #include <pthread.h>
#endif

typedef struct Channel Channel;
typedef struct Voice Voice;
typedef struct Period Period;
typedef struct NameDef NameDef;
typedef struct BlockDef BlockDef;
typedef unsigned char uchar;

int inbuf_loop(void *vp) ;
int inbuf_read(int *dst, int dlen) ;
void inbuf_start(int(*rout)(int*,int), int len) ;
int t_per24(int t0, int t1) ;
int t_per0(int t0, int t1) ;
int t_mid(int t0, int t1) ;
int main(int argc, char **argv) ;
void status(char *) ;
void dispCurrPer( FILE* ) ;
void init_sin_table() ;
void debug(char *fmt, ...) ;
void warn(char *fmt, ...) ;
void * Alloc(size_t len) ;
char * StrDup(char *str) ;
int calcNow() ;
void loop() ;
void outChunk() ;
void corrVal(int ) ;
int readLine() ;
char * getWord() ;
void badSeq() ;
void readSeqImm(int ac, char **av) ;
void readSeq(int ac, char **av) ;
void readPreProg(int ac, char **av) ;
void correctPeriods();
void setup_device(void) ;
void readNameDef();
void readTimeLine();
int voicesEq(Voice *, Voice *);
void error(char *fmt, ...) ;
int sprintTime(char *, int);
int sprintVoice(char *, Voice *, Voice *);
int readTime(char *, int *);
void writeWAV();
void writeOut(char *, int);
void sinc_interpolate(double *, int, int *);
inline int userTime();
void find_wav_data_start(FILE *in);
int raw_mix_in(int *dst, int dlen);
int scanOptions(int *acp, char ***avp);
void handleOptions(char *p);
void setupOptC(char *spec) ;
extern int out_rate, out_rate_def;
void create_drop(int ac, char **av);
void create_slide(int ac, char **av);
void validateTotalAmplitude(Voice *voices, int numChannels, const char *line, int lineNum);
void printSequenceDuration();
void checkMixInSequence(); // Check if mix/<amp> is specified

#define ALLOC_ARR(cnt, type) ((type*)Alloc((cnt) * sizeof(type)))
#define uint unsigned int

#ifdef OGG_DECODE
#include "oggdec.c"
#endif
#ifdef MP3_DECODE
#include "mp3dec.c"
#endif

#ifdef WIN_AUDIO
void CALLBACK win32_audio_callback(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
#endif
#ifdef MAC_AUDIO
OSStatus mac_callback(AudioDeviceID, const AudioTimeStamp *, const AudioBufferList *, 
		      const AudioTimeStamp *, AudioBufferList *, const AudioTimeStamp *, 
		      void *inClientData);
#endif

#define NL "\n"

void 
help() {
   printf("SBaGen+ - Sequenced Brainwave Generator, version " VERSION
     NL "Original version (c) 1999-2011 Jim Peters, http://uazu.net/"
     NL "This fork maintained by Ruan, https://ruan.sh/"
	  NL "Released under the GNU GPL v2. See file COPYING."
	  NL 
	  NL "Usage: sbagen+ [options] seq-file ..."
	  NL "       sbagen+ [options] -i tone-specs ..."
	  NL "       sbagen+ [options] -p pre-programmed-sequence-specs ..."
	  NL
	  NL "Options:  -h        Display this help-text"
	  NL "          -Q        Quiet - don't display running status"
	  NL "          -D        Display the full interpreted sequence instead of playing it"
	  NL "          -i        Immediate.  Take the remainder of the command line to be"
	  NL "                     tone-specifications, and play them continuously"
	  NL "          -p        Pre-programmed sequence.  Take the remainder of the command"
	  NL "                     line to be a type and arguments, e.g. \"drop 00ds+\""
	  NL "          -q mult   Quick.  Run through quickly (real time x 'mult') from the"
	  NL "                     start time, rather than wait for real time to pass"
	  NL
	  NL "          -r rate   Select the output rate (default is 44100 Hz, or from -m)"
#ifndef MAC_AUDIO
	  NL "          -b bits   Select the number bits for output (8 or 16, default 16)"
#else
	  NL "          -B size   Force buffer size (in samples) for audio output."
    NL "                     (e.g. 1024, 2048, 4096, etc.)"
#endif
	  NL "          -L time   Select the length of time (hh:mm or hh:mm:ss) to output"
	  NL "                     for.  Default is to output forever."
	  NL "          -S        Output from the first tone-set in the sequence (Start),"
	  NL "                     instead of working in real-time.  Equivalent to '-q 1'."
	  NL "          -E        Output until the last tone-set in the sequence (End),"
	  NL "                     instead of outputting forever."
	  NL "          -T time   Start at the given clock-time (hh:mm)"
	  NL
	  NL "          -o file   Output raw data to the given file instead of default device"
	  NL "          -O        Output raw data to the standard output"
	  NL "          -W        Output a WAV-format file instead of raw data"
	  NL "          -m file   Read audio data from the given file and mix it with the"
	  NL "                      generated binaural beats; may be "
#ifdef OGG_DECODE
	  "ogg/"
#endif
#ifdef MP3_DECODE
	  "mp3/"
#endif
	  "wav/raw format"
	  NL "          -M        Read raw audio data from the standard input and mix it"
	  NL "                      with the generated binaural beats (raw only)"
	  NL
	  NL "          -R rate   Select rate in Hz that frequency changes are recalculated"
	  NL "                     (for file/pipe output only, default is 10Hz)"
	  NL "          -F fms    Fade in/out time in ms (default 60000ms, or 1min)"
#ifdef ALSA_AUDIO
	  NL "          -d dev    Select a different ALSA device instead of 'default'"
#endif
	  NL "          -c spec   Compensate for low-frequency headphone roll-off; see docs"
	  NL
	  );
   exit(0);
}

void 
usage() {
  error("SBaGen+ - Sequenced Brainwave Generator, version " VERSION 
	NL "Original version (c) 1999-2011 Jim Peters, http://uazu.net/"
	NL "This fork maintained by Ruan, https://ruan.sh/"
	NL "Released under the GNU GPL v2. See file COPYING."
	NL 
	NL "Usage: sbagen+ [options] seq-file ..."
	NL "       sbagen+ [options] -i tone-specs ..."
	NL "       sbagen+ [options] -p pre-programmed-sequence-specs ..."
	NL
	NL "SBaGen+ is a fork of the original SBaGen with added features."
	NL "For full usage help, type 'sbagen+ -h'."
#ifdef EXIT_KEY
	NL
	NL "Windows users please note that this utility is designed to be run as the"
	NL "associated application for SBG files.  This should have been set up for you by"
	NL "the installer.  You can run all the SBG files directly from the desktop by"
	NL "double-clicking on them, and edit them using NotePad from the right-click menu."
	NL "Alternatively, SBaGen+ may be run from the command line, or from"
	NL "BAT/PS1 files.  SBaGen+ is powerful software -- it is worth the effort of figuring"
	NL "all this out.  See SBAGEN+.TXT for the full documentation."
#endif
	NL);
}


#define DEBUG_CHK_UTIME 0	// Check how much user time is being consumed
#define DEBUG_DUMP_WAVES 0	// Dump out wave tables (to plot with gnuplot)
#define DEBUG_DUMP_AMP 0	// Dump output amplitude to stdout per chunk
#define N_CH 16			// Number of channels

struct Voice {
  int typ;			// Voice type: 0 off, 1 binaural, 2 pink noise, 3 bell, 4 spin, 5 mix, 6 mixspin, 7 mixpulse, 8 isochronic, 9 white noise, 10 brown noise, -1 to -100 wave00 to wave99
  double amp;			// Amplitude level (0-4096 for 0-100%)
  double carr;			// Carrier freq (for binaural/bell/isochronic), width (for spin)
  double res;			// Resonance freq (-ve or +ve) (for binaural/spin/isochronic)
};

struct Channel {
  Voice v;			// Current voice setting (updated from current period)
  int typ;			// Current type: 0 off, 1 binaural, 2 pink noise, 3 bell, 4 spin, 5 mix, 6 mixspin, 7 mixpulse, 8 isochronic, 9 white noise, 10 brown noise, -1 to -100 wave00 to wave99
  int amp, amp2;		// Current state, according to current type
  int inc1, off1;		//  ::  (for binaural tones, offset + increment into sine 
  int inc2, off2;		//  ::   table * 65536)
};

struct Period {
  Period *nxt, *prv;		// Next/prev in chain
  int tim;			// Start time (end time is ->nxt->tim)
  Voice v0[N_CH], v1[N_CH];	// Start and end voices
  int fi, fo;			// Temporary: Fade-in, fade-out modes
};

struct NameDef {
  NameDef *nxt;
  char *name;			// Name of definition
  BlockDef *blk;		// Non-zero for block definition
  Voice vv[N_CH];		// Voice-set for it (unless a block definition)
};

struct BlockDef {
  BlockDef *nxt;		// Next in chain
  char *lin;			// StrDup'd line
};

#define ST_AMP 0x7FFFF		// Amplitude of wave in sine-table
#define NS_ADJ 12		// Noise is generated internally with amplitude ST_AMP<<NS_ADJ
#define NS_DITHER 16		// How many bits right to shift the noise for dithering
#define NS_AMP (ST_AMP<<NS_ADJ)
#define ST_SIZ 16384		// Number of elements in sine-table (power of 2)
int *sin_table;
#define AMP_DA(pc) (40.96 * (pc))	// Display value (%age) to ->amp value
#define AMP_AD(amp) ((amp) / 40.96)	// Amplitude value to display %age
int *waves[100];		// Pointers are either 0 or point to a sin_table[]-style array of int

Channel chan[N_CH];		// Current channel states
int now;			// Current time (milliseconds from midnight)
Period *per= 0;			// Current period
NameDef *nlist;			// Full list of name definitions

int *tmp_buf;			// Temporary buffer for 20-bit mix values
short *out_buf;			// Output buffer
int out_bsiz;			// Output buffer size (bytes)
int out_blen;			// Output buffer length (samples) (1.0* or 0.5* out_bsiz)
int out_bps;			// Output bytes per sample (2 or 4)
int out_buf_ms;			// Time to output a buffer-ful in ms
int out_buf_lo;			// Time to output a buffer-ful, fine-tuning in ms/0x10000
int out_fd;			// Output file descriptor
int out_rate= 44100;		// Sample rate
int out_rate_def= 1;		// Sample rate is default value, not set by user
int out_mode= 1;		// Output mode: 0 unsigned char[2], 1 short[2], 2 swapped short[2]
int out_prate= 10;		// Rate of parameter change (for file and pipe output only)
int fade_int= 60000;		// Fade interval (ms)
FILE *in;			// Input sequence file
int in_lin;			// Current input line
char buf[4096];			// Buffer for current line
char buf_copy[4096];		// Used to keep unmodified copy of line
char *lin;			// Input line (uses buf[])
char *lin_copy;			// Copy of input line
double spin_carr_max;		// Maximum 'carrier' value for spin (really max width in us)

#define NS_BIT 10
int ns_tbl[1<<NS_BIT];
int ns_off= 0;

int fast_tim0= -1;		// First time mentioned in the sequence file (for -q and -S option)
int fast_tim1= -1;		// Last time mentioned in the sequence file (for -E option)
int fast_mult= 0;		// 0 to sync to clock (adjusting as necessary), or else sync to
				//  output rate, with the multiplier indicated
S64 byte_count= -1;		// Number of bytes left to output, or -1 if unlimited
int tty_erase;			// Chars to erase from current line (for ESC[K emulation)

int opt_Q;			// Quiet mode
int opt_D;
int opt_M, opt_S, opt_E;
char *opt_o, *opt_m;
int opt_O;
int opt_W;
#ifdef ALSA_AUDIO
char *opt_d= "default";	// Output device to ALSA
#endif
int opt_L= -1;			// Length of WAV file in ms
int opt_T= -1;			// Time to start at (for -S option)
#ifdef MAC_AUDIO
int opt_B= -1;		// Buffer size override (-1 = auto)
#endif

FILE *mix_in;			// Input stream for mix sound data, or 0
int mix_cnt;			// Version number from mix filename (#<digits>), or -1
int bigendian;			// Is this platform Big-endian?
int mix_flag= 0;		// Has 'mix/*' been used in the sequence?
double *mix_amp= NULL; // Amplitude of mix sound data to use with mixspin/mixpulse. Default is 100%

int opt_c;			// Number of -c option points provided (max 16)
struct AmpAdj { 
   double freq, adj;
} ampadj[16];			// List of maximum 16 (freq,adj) pairs, freq-increasing order

char *pdir;			// Program directory (used as second place to look for -m files)

#ifdef WIN_AUDIO
 #define BUFFER_COUNT 8
 #define BUFFER_SIZE 8192*4
 HWAVEOUT aud_handle;
 WAVEHDR *aud_head[BUFFER_COUNT];
 int aud_current;		// Current header
 int aud_cnt;			// Number of headers in use
#endif

#ifdef MAC_AUDIO
 #define BUFFER_COUNT 8
 #define BUFFER_SIZE 4096*4
 char aud_buf[BUFFER_COUNT][BUFFER_SIZE];
 int aud_rd;	// Next buffer to read out of list (to send to device)
 int aud_wr;	// Next buffer to write.  aud_rd==aud_wr means empty buffer list
 static AudioDeviceID aud_dev;
#endif

//
//	Delay for a short period of time (in ms)
//

#ifdef UNIX_MISC
void 
delay(int ms) {
   struct timespec ts;
   ts.tv_sec= ms / 1000;
   ts.tv_nsec= (ms % 1000) * 1000000;
   nanosleep(&ts, 0);
}
#endif
#ifdef WIN_MISC
void 
delay(int ms) {
   Sleep(ms);
}
#endif


//
//	WAV/OGG/MP3 input data buffering
//

int *inbuf;		// Buffer for input data (as 20-bit samples)
int ib_len;		// Length of input buffer (in ints)
volatile int ib_rd;	// Read-offset in inbuf
volatile int ib_wr;	// Write-offset in inbuf
volatile int ib_eof;	// End of file flag
int ib_cycle= 100;	// Time in ms for a complete loop through the buffer
int (*ib_read)(int*,int);  // Routine to refill buffer

int 
inbuf_loop(void *vp) {
   int now= -1;
   int waited= 0;	// Used to bail out if the main thread dies for some reason
   int a;

   while (1) {
      int rv;
      int rd= ib_rd;
      int wr= ib_wr;
      int cnt= (rd-1-wr) & (ib_len-1);
      if (cnt > ib_len-wr) cnt= ib_len-wr;
      if (cnt > ib_len/8) cnt= ib_len/8;

      // Choose to only work in ib_len/8 units, although this is not
      // 100% necessary
      if (cnt < ib_len/8) {
	 // Wait a little while for the buffer to empty (minimum 1ms)
	 if (waited > 10000 + ib_cycle)
	    error("Mix stream halted for more than 10 seconds; aborting");
	 delay(a= 1+ib_cycle/4);
	 waited += a;
	 continue;
      }
      waited= 0;
      
      rv= ib_read(inbuf+wr, cnt);
      //debug("ib_read %d-%d (%d) -> %d", wr, wr+cnt-1, cnt, rv);
      if (rv != cnt) {
	 ib_eof= 1;
	 return 0;
      }

      ib_wr= (wr + rv) & (ib_len-1);

      // Whenever we roll over, recalculate 'ib_cycle'
      if (ib_wr < wr) {
	 int prev= now;
	 now= calcNow();
	 if (prev >= 0 && now > prev)
	    ib_cycle= now - prev;
	 //debug("Input buffer cycle duration is now %dms", ib_cycle);
      }
   }
   return 0;
}

//
//	Read a chunk of int data from the input buffer.  This will
//	always return enough data unless we have hit the end of the
//	file, in which case it returns a lower number or 0.  If not
//	enough data has been read by the input thread, then this
//	thread pauses until data is ready -- but this should hopefully
//	never happen.
//

int 
inbuf_read(int *dst, int dlen) {
   int rv= 0;
   int waited= 0;	// As a precaution, bail out if other thread hangs for some reason
   int a;

   while (dlen > 0) {
      int rd= ib_rd;
      int wr= ib_wr;
      int avail= (wr-rd) & (ib_len-1);
      int toend= ib_len-rd;
      if (avail > toend) avail= toend;
      if (avail > dlen) avail= dlen;

      if (avail == 0) {
	 if (ib_eof) return rv;

	 // Necessary to wait for incoming mix data.  This should
	 // never happen in normal running, though, unless we are
	 // outputting to a file
	 if (waited > 10000) 
	    error("Mix stream problem; waited more than 10 seconds for data; aborting");
	 //debug("Waiting for input thread (%d)", ib_eof);
	 delay(a= ib_cycle/4 > 100 ? 100 : 1+ib_cycle/4);
	 waited += a;
	 continue;
      }
      waited= 0;
      
      memcpy(dst, inbuf+rd, avail * sizeof(int));
      dst += avail;
      dlen -= avail;
      rv += avail;
      ib_rd= (rd + avail) & (ib_len-1);
   }
   return rv;
}

//
//	Start off the thread that fills the buffer
//

void 
inbuf_start(int(*rout)(int*,int), int len) {
   if (0 != (len & (len-1)))
      error("inbuf_start() called with length not a power of two");

   ib_read= rout;
   ib_len= len;
   inbuf= ALLOC_ARR(ib_len, int);
   ib_rd= 0;
   ib_wr= 0;
   ib_eof= 0;
   if (!opt_Q) warn("Initialising %d-sample buffer for mix stream", ib_len/2);

   // Preload 75% of the buffer -- or at least attempt to do so;
   // errors/eof/etc will be picked up in the inbuf_loop() routine
   ib_wr= ib_read(inbuf, ib_len*3/4);

   // Start the thread off
#ifdef UNIX_MISC
   {
      pthread_t thread;
      if (0 != pthread_create(&thread, NULL, (void*)&inbuf_loop, NULL))
	 error("Failed to start input buffering thread");
   }
#endif
#ifdef WIN_MISC
   {
      DWORD tmp;
      if (0 == CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&inbuf_loop, 0, 0, &tmp))
	 error("Failed to start input buffering thread");
   }
#endif
}

#ifdef ALSA_AUDIO
snd_pcm_t *alsa_handle;         // ALSA PCM handle
snd_pcm_hw_params_t *alsa_params; // ALSA hardware parameters

// Function to clean up ALSA resources
void cleanup_alsa() {
  if (alsa_handle) {
    snd_pcm_close(alsa_handle);
    alsa_handle = NULL;
  }
}
#endif

//
//	Time-keeping functions
//

#define H24 (86400000)			// 24 hours
#define H12 (43200000)			// 12 hours

int t_per24(int t0, int t1) {		// Length of period starting at t0, ending at t1.
   int td= t1 - t0;				// NB for t0==t1 this gives 24 hours, *NOT 0*
   return td > 0 ? td : td + H24;
}
int t_per0(int t0, int t1) {		// Length of period starting at t0, ending at t1.
   int td= t1 - t0;				// NB for t0==t1 this gives 0 hours
   return td >= 0 ? td : td + H24;
}
int t_mid(int t0, int t1) {		// Midpoint of period from t0 to t1
   return ((t1 < t0) ? (H24 + t0 + t1) / 2 : (t0 + t1) / 2) % H24;
}

//
//	M A I N
//

int 
main(int argc, char **argv) {
   short test= 0x1100;
   int rv;
   char *p;
   
   pdir= StrDup(argv[0]);
   p= strchr(pdir, 0);
   while (p > pdir && p[-1] != '/' && p[-1] != '\\') *--p= 0;

   argc--; argv++;
   init_sin_table();
   bigendian= ((char*)&test)[0] != 0;
   
   // Process all the options
   rv= scanOptions(&argc, &argv);
   
   if (argc < 1) usage();
   
   if (rv == 'i') {
      // Immediate mode
      readSeqImm(argc, argv);
   } else if (rv == 'p') {
      // Pre-programmed sequence
      readPreProg(argc, argv);
   } else {
      // Sequenced mode -- sequence may include options, so options
      // are not settled until below this point
      if (argc < 1) usage();
      readSeq(argc, argv);
   }
   
   if (opt_W && !opt_o && !opt_O)
      error("Use -o or -O with the -W option");
   if (opt_W && opt_L < 0) {
      if (!opt_E) {
         fprintf(stderr, "*** The length has not been specified for the -W option; enabling -E option ***\n");
         fprintf(stderr, "(WAV file will have the same duration as the sequence)\n\n");
         opt_E = 1;  // Automatically enable -E option
      }
   }
   
   mix_in= 0;
   if (opt_M || opt_m) {
      char *p;
      char tmp[4];
      int raw= 1;
      if (opt_M) {
	 mix_in= stdin; 
	 tmp[0]= 0;
      } 
      if (opt_m) {
	 // Pick up #<digits> on end of filename
	 p= strchr(opt_m, 0);
	 mix_cnt= -1;
	 if (p > opt_m && isdigit(p[-1])) {
	    mix_cnt= 0;
	    while (p > opt_m && isdigit(p[-1]))
	       mix_cnt= mix_cnt * 10 + *--p - '0';
	    if (p > opt_m && p[-1] == '#') 
	       *--p= 0;
	    else {
	       p= strchr(opt_m, 0);
	       mix_cnt= -1;
	    } 
	 }
	 // p points to end of filename (NUL)

	 // Open file
	 mix_in= fopen(opt_m, "rb");
	 if (!mix_in && opt_m[0] != '/') {
	    int len= strlen(opt_m) + strlen(pdir) + 1;
	    char *tmp= ALLOC_ARR(len, char);
	    strcpy(tmp, pdir);
	    strcat(tmp, opt_m);
	    mix_in= fopen(tmp, "rb");
	    free(tmp);
	 }
	 if (!mix_in)
	    error("Can't open -m option mix input file: %s", opt_m);

	 // Pick up extension
	 if (p-opt_m >= 4 && p[-4] == '.') {
	    tmp[0]= tolower(p[-3]);
	    tmp[1]= tolower(p[-2]);
	    tmp[2]= tolower(p[-1]);
	    tmp[3]= 0;
	 }
      }
      if (0 == strcmp(tmp, "wav"))	// Skip header on WAV files
	 find_wav_data_start(mix_in);
      if (0 == strcmp(tmp, "ogg")) {
#ifdef OGG_DECODE
	 ogg_init(); raw= 0;
#else
	 error("Sorry: Ogg support wasn't compiled into this executable");
#endif
      }
      if (0 == strcmp(tmp, "mp3")) {
#ifdef MP3_DECODE
	 mp3_init(); raw= 0;
#else
	 error("Sorry: MP3 support wasn't compiled into this executable");
#endif
      }
      // If this is a raw/wav data stream, setup a 256*1024-int
      // buffer (3s@44.1kHz)
      if (raw) inbuf_start(raw_mix_in, 256*1024);
   }
   
   loop();
   
#ifdef ALSA_AUDIO
   cleanup_alsa();
#endif

   return 0;
}

//
//	Scan options.  Returns a flag indicating what is expected to
//	interpret the rest of the arguments: 0 normal, 'i' immediate
//	(-i option), 'p' -p option.
//

int 
scanOptions(int *acp, char ***avp) {
   int argc= *acp;
   char **argv= *avp;
   int val;
   char dmy;
   int rv= 0;

   // Scan options
   while (argc > 0 && argv[0][0] == '-' && argv[0][1]) {
      char opt, *p= 1 + *argv++; argc--;
      while ((opt= *p++)) {
	 // Check options that are available on both
	 switch (opt) {
	  case 'Q': opt_Q= 1; break;
	  case 'E': opt_E= 1; break;
	  case 'm':
	     if (argc-- < 1) error("-m option expects filename");
	     // Earliest takes precedence, so command-line overrides sequence file
	     if (!opt_m) opt_m= *argv++;
	     break;
	  case 'S': opt_S= 1;
	     if (!fast_mult) fast_mult= 1; 		// Don't try to sync with real time
	     break;
	  case 'L':
	     if (argc-- < 1 || 0 == (val= readTime(*argv, &opt_L)) || 
		 1 == sscanf(*argv++ + val, " %c", &dmy)) 
		error("-L expects hh:mm or hh:mm:ss time");
	     break;
	  case 'T':
	     if (argc-- < 1 || 0 == (val= readTime(*argv, &opt_T)) || 
		 1 == sscanf(*argv++ + val, " %c", &dmy)) 
		error("-T expects hh:mm or hh:mm:ss time");
	     if (!fast_mult) fast_mult= 1;		// Don't try to sync with real time
	     break;
	  case 'F':
	     if (argc-- < 1 || 1 != sscanf(*argv++, "%d %c", &fade_int, &dmy)) 
		error("-F expects fade-time in ms");
	     break;
#ifdef MAC_AUDIO
	  case 'B':
	     if (argc-- < 1 || 1 != sscanf(*argv++, "%d %c", &opt_B, &dmy)) 
		      error("-B expects buffer size in samples");
       if (opt_B < 1024 || opt_B > BUFFER_SIZE / 2)
          error("Buffer size must be between 1024 and %d samples.", BUFFER_SIZE / 2);
       if ((opt_B & (opt_B - 1)) != 0)
          error("Buffer size must be a power of 2. (e.g. 1024, 2048, 4096, etc.)");
       opt_B *= 2;
	     break;
#endif
	  case 'c':
	     if (argc-- < 1) error("-c expects argument");
	     setupOptC(*argv++);
	     break;
	  case 'i': rv= 'i'; break;
	  case 'p': rv= 'p'; break;
	  case 'h': help(); break;
	  case 'D': opt_D= 1; break;
	  case 'M': opt_M= 1; break;
	  case 'O': opt_O= 1;
	     if (!fast_mult) fast_mult= 1; 		// Don't try to sync with real time
	     break;
	  case 'W': opt_W= 1;
	     if (!fast_mult) fast_mult= 1; 		// Don't try to sync with real time
	     break;
	  case 'q': 
	     opt_S= 1;
	     if (argc-- < 1 || 1 != sscanf(*argv++, "%d %c", &fast_mult, &dmy)) 
		error("Expecting an integer after -q");
	     if (fast_mult < 1) fast_mult= 1;
	     break;
	  case 'r':
	     if (argc-- < 1 || 1 != sscanf(*argv++, "%d %c", &out_rate, &dmy))
		error("Expecting an integer after -r");
	     out_rate_def= 0;
	     break;
#ifndef MAC_AUDIO
	  case 'b':
	     if (argc-- < 1 || 
		 1 != sscanf(*argv++, "%d %c", &val, &dmy) ||
		 !(val == 8 || val == 16))
		error("Expecting -b 8 or -b 16");
	     out_mode= (val == 8) ? 0 : 1;
	     break;
#endif
	  case 'o':
	     if (argc-- < 1) error("Expecting filename after -o");
	     opt_o= *argv++;
	     if (!fast_mult) fast_mult= 1;		// Don't try to sync with real time
	     break;
#ifdef ALSA_AUDIO
	  case 'd':
	     if (argc-- < 1) error("Expecting ALSA device name after -d");
	     opt_d= *argv++;
	     break;
#endif
	  case 'R':
	     if (argc-- < 1 || 1 != sscanf(*argv++, "%d %c", &out_prate, &dmy)) 
		error("Expecting integer after -R");
	     break;
	  default:
	     error("Option -%c not known; run 'sbagen+ -h' for help", opt);
	 }
      }
   }

   *acp= argc;
   *avp= argv;
   return rv;
}

//
//	Handle an option string, breaking it into an (argc/argv) list
//	for scanOptions.
//

void 
handleOptions(char *str0) {
   // Always StrDup() string and don't bother to free(), as normal
   // argv[] strings stick around for the life of the program
   char *str= StrDup(str0);
   int const max_argc= 32;
   char *argv[max_argc+1];
   int argc= 0;

   while (*str) {
      if (argc >= max_argc)
	 error("Too many options at line: %d\n  %s", in_lin, lin_copy);
      argv[argc++]= str;
      while (*str && !isspace(*str)) str++;
      if (!*str) continue;
      *str++= 0;	// NUL-term this word
      while (isspace(*str)) str++;
   }
   argv[argc]= 0;	// Terminate argv list with a NULL

   // Process the options
   {
      char **av= argv;
      int ac= argc;
      int rv;
      
      rv= scanOptions(&ac, &av);

      if (rv == 'i') {
	 // Immediate mode
	 readSeqImm(ac, av);
      } else if (rv == 'p') {
	 // Pre-programmed sequence
	 readPreProg(ac, av);
      } else if (ac)
	 error("Trailing garbage after options at line: %d\n  %s", in_lin, lin_copy);
   }
}

//
//	Setup the ampadj[] array from the given -c spec-string
//

void 
setupOptC(char *spec) {
   char *p= spec, *q;
   int a, b;
   
   while (1) {
      while (isspace(*p) || *p == ',') p++;
      if (!*p) break;

      if (opt_c >= sizeof(ampadj) / sizeof(ampadj[0]))
	 error("Too many -c option frequencies; maxmimum is %d", 
	       sizeof(ampadj) / sizeof(ampadj[0]));

      ampadj[opt_c].freq= strtod(p, &q);
      if (p == q) goto bad;
      if (*q++ != '=') goto bad;
      ampadj[opt_c].adj= strtod(q, &p);
      if (p == q) goto bad;
      opt_c++;
   }

   // Sort the list
   for (a= 0; a<opt_c; a++)
      for (b= a+1; b<opt_c; b++) 
	 if (ampadj[a].freq > ampadj[b].freq) {
	    double tmp;
	    tmp= ampadj[a].freq; ampadj[a].freq= ampadj[b].freq; ampadj[b].freq= tmp;
	    tmp= ampadj[a].adj; ampadj[a].adj= ampadj[b].adj; ampadj[b].adj= tmp;
	 }
   return;
      
 bad:
   error("Bad -c option spec; expecting <freq>=<amp>[,<freq>=<amp>]...:\n  %s", spec);
}


//
//	If this is a WAV file we've been given, skip forward to the
//	'data' section.  Don't bother checking any of the 'fmt '
//	stuff.  If they didn't give us a valid 16-bit stereo file at
//	the right rate, then tough!
//

void 
find_wav_data_start(FILE *in) {
   unsigned char buf[16];

   if (1 != fread(buf, 12, 1, in)) goto bad;
   if (0 != memcmp(buf, "RIFF", 4)) goto bad;
   if (0 != memcmp(buf+8, "WAVE", 4)) goto bad;
   
   while (1) {
      int len;
      if (1 != fread(buf, 8, 1, in)) goto bad;
      if (0 == memcmp(buf, "data", 4)) return;		// We're in the right place!
      len= buf[4] + (buf[5]<<8) + (buf[6]<<16) + (buf[7]<<24);
      if (len & 1) len++;
      if (out_rate_def && 0 == memcmp(buf, "fmt ", 4)) {
	 // Grab the sample rate to use as the default if available
	 if (1 != fread(buf, 8, 1, in)) goto bad;
	 len -= 8;
	 out_rate= buf[4] + (buf[5]<<8) + (buf[6]<<16) + (buf[7]<<24);
	 out_rate_def= 0;
      }
      if (0 != fseek(in, len, SEEK_CUR)) goto bad;
   }

 bad:
   warn("WARNING: Not a valid WAV file, treating as RAW");
   rewind(in);
}

//
//	Input raw audio data from the 'mix_in' stream, and convert to
//	32-bit values (max 'dlen')
//

int 
raw_mix_in(int *dst, int dlen) {
   short *tmp= (short*)(dst + dlen/2);
   int a, rv;
   
   rv= fread(tmp, 2, dlen, mix_in);
   if (rv == 0) {
      if (feof(mix_in))
	 return 0;
      error("Read error on mix input:\n  %s", strerror(errno));
   }

   // Now convert 16-bit little-endian input data into 20-bit native
   // int values
   if (bigendian) {
      char *rd= (char*)tmp;
      for (a= 0; a<rv; a++) {
	 *dst++= ((rd[0]&255) + (rd[1]<<8)) << 4;
	 rd += 2;
      }
   } else {
      for (a= 0; a<rv; a++) 
	 *dst++= *tmp++ << 4;
   }

   return rv;
}

   

//
//	Update a status line
//

void 
status(char *err) {
  int a;
  int nch= N_CH;
  char *p= buf, *p0, *p1;

  if (opt_Q) return;

#ifdef ANSI_TTY
  if (tty_erase) p += sprintf(p, "\033[K");
#endif

  p0= p;		// Start of line
  *p++= ' '; *p++= ' ';
  p += sprintTime(p, now);
  while (nch > 1 && chan[nch-1].v.typ == 0) nch--;
  for (a= 0; a<nch; a++)
    p += sprintVoice(p, &chan[a].v, 0);
  if (err) p += sprintf(p, " %s", err);
  p1= p;		// End of line

#ifndef ANSI_TTY
  // Truncate line to 79 characters on Windows
  if (p1-p0 > 79) {
    p1 = p0 + 76;
    p1 += sprintf(p1, "...");
  }
#endif

#ifndef ANSI_TTY
  while (tty_erase > p-p0) *p++= ' ';
#endif

  tty_erase= p1-p0;		// Characters that will need erasing
  fprintf(stderr, "%s\r", buf);
  fflush(stderr);
}

void 				// Display current period details
dispCurrPer(FILE *fp) {
  int a;
  Voice *v0, *v1;
  char *p0, *p1;
  int len0, len1;
  int nch= N_CH;

  if (opt_Q) return;

  p0= buf;
  p1= buf_copy;
  
  p0 += sprintf(p0, "* ");
  p0 += sprintTime(p0, per->tim);
  p1 += sprintf(p1, "  ");	
  p1 += sprintTime(p1, per->nxt->tim);

  v0= per->v0; v1= per->v1;
  while (nch > 1 && v0[nch-1].typ == 0) nch--;
  for (a= 0; a<nch; a++, v0++, v1++) {
    p0 += len0= sprintVoice(p0, v0, 0);
    p1 += len1= sprintVoice(p1, v1, v0);
    while (len0 < len1) { *p0++= ' '; len0++; }
    while (len1 < len0) { *p1++= ' '; len1++; }
  }
  *p0= 0; *p1= 0;
  fprintf(fp, "%s\n%s\n", buf, buf_copy);
  fflush(fp);
}

int
sprintTime(char *p, int tim) {
  return sprintf(p, "%02d:%02d:%02d",
		 tim % 86400000 / 3600000,
		 tim % 3600000 / 60000,
		 tim % 60000 / 1000);
}

int 
sprintVoice(char *p, Voice *vp, Voice *dup) {
   switch (vp->typ) {
    case 0:
      return sprintf(p, " -");
    case 1:
      if (dup && vp->carr == dup->carr && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      if (vp->res == 0)
	return sprintf(p, " %.2f/%.2f", vp->carr, AMP_AD(vp->amp));
      return sprintf(p, " %.2f%+.2f/%.2f", vp->carr, vp->res, AMP_AD(vp->amp));
    case 2:
      if (dup && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " pink/%.2f", AMP_AD(vp->amp));
    case 9:
      if (dup && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " white/%.2f", AMP_AD(vp->amp));
    case 10:
      if (dup && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " brown/%.2f", AMP_AD(vp->amp));
    case 3:
      if (dup && vp->carr == dup->carr && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " bell%.2f/%.2f", vp->carr, AMP_AD(vp->amp));
    case 4:
      if (dup && vp->carr == dup->carr && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " spin:%.2f%+.2f/%.2f", vp->carr, vp->res, AMP_AD(vp->amp));
    case 5:
      if (dup && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " mix/%.2f", AMP_AD(vp->amp));
    case 8:  // Isochronic tones
      if (dup && vp->carr == dup->carr && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " %.2f@%.2f/%.2f", vp->carr, vp->res, AMP_AD(vp->amp));
    case 6:  // Mixspin - spinning mix stream
      if (dup && vp->carr == dup->carr && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " mixspin:%.2f%+.2f/%.2f", vp->carr, vp->res, AMP_AD(vp->amp));
    case 7:  // Mixpulse - mix stream with pulse effect
      if (dup && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " mixpulse:%.2f/%.2f", vp->res, AMP_AD(vp->amp));
    default:
      if (vp->typ < -100 || vp->typ > -1)
	return sprintf(p, " ???");
      if (dup && vp->carr == dup->carr && vp->res == dup->res && vp->amp == dup->amp)
	return sprintf(p, "  ::");
      return sprintf(p, " wave%02d:%.2f%+.2f/%.2f", -1-vp->typ, vp->carr, vp->res, AMP_AD(vp->amp));
   }
}

void 
init_sin_table() {
  int a;
  int *arr= (int*)Alloc(ST_SIZ * sizeof(int));

  for (a= 0; a<ST_SIZ; a++)
    arr[a]= (int)(ST_AMP * sin((a * 3.14159265358979323846 * 2) / ST_SIZ));

  sin_table= arr;
}

void 
error(char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
#ifdef EXIT_KEY
  fprintf(stderr, "Press <RETURN> to continue: ");
  fflush(stderr);
  getchar();
#endif
#ifdef ALSA_AUDIO
  cleanup_alsa();
#endif
  exit(1);
}

void 
debug(char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
}

void 
warn(char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
}

void *
Alloc(size_t len) {
  void *p= calloc(1, len);
  if (!p) error("Out of memory");
  return p;
}

char *
StrDup(char *str) {
  char *rv= strdup(str);
  if (!rv) error("Out of memory");
  return rv;
}

#ifdef UNIX_TIME
// Precalculate a reference timestamp to accelerate calcNow().  This
// can be any recent time.  We recalculate it every 10 minutes.  The
// only reason for doing this is to cope with clocks going forwards or
// backwards when entering or leaving summer time so that people wake
// up on time on these two dates; an hour of the sequence will be
// repeated or skipped.  The 'time_ref*' variables will be initialised
// on the first call to calcNow().

static int time_ref_epoch= 0;	// Reference time compared to UNIX epoch
static int time_ref_ms;		// Reference time in sbagen 24-hour milliseconds

void 
setupRefTime() {
  struct tm *tt;
  time_t tim= time(0);
  tt= localtime(&tim);
  time_ref_epoch= tim;
  time_ref_ms= 1000*tt->tm_sec + 60000*tt->tm_min + 3600000*tt->tm_hour;
}  

inline int  
calcNow() {
  struct timeval tv;
  if (0 != gettimeofday(&tv, 0)) error("Can't get current time");
  if (tv.tv_sec - time_ref_epoch > 600) setupRefTime();
  return (time_ref_ms + (tv.tv_sec - time_ref_epoch) * 1000 + tv.tv_usec / 1000) % H24;
}
#endif

#ifdef WIN_TIME
inline int  
calcNow() {
  SYSTEMTIME st;
  GetLocalTime(&st);
  return st.wMilliseconds + 1000*st.wSecond + 60000*st.wMinute + 3600000*st.wHour;
}
#endif

#if DEBUG_CHK_UTIME
inline int 
userTime() {
  struct tms buf;
  times(&buf);
  return buf.tms_utime;
}
#else
// Dummy to avoid complaints on MSVC
int userTime() { return 0; }
#endif

//
//	Simple random number generator.  Generates a repeating
//	sequence of 65536 odd numbers in the range -65535->65535.
//
//	Based on ZX Spectrum random number generator:
//	  seed= (seed+1) * 75 % 65537 - 1
//

#define RAND_MULT 75

static int seed= 2;

//inline int qrand() {
//  return (seed= seed * 75 % 131074) - 65535;
//}

//
//	Generate next sample for simulated pink noise, with same
//	scaling as the sin_table[].  This version uses an inlined
//	random number generator, and smooths the lower frequency bands
//	as well.
//

#define NS_BANDS 9
typedef struct Noise Noise;
struct Noise {
  int val;		// Current output value
  int inc;		// Increment
};
Noise ntbl[NS_BANDS];
int nt_off;
int noise_buf[256];
uchar noise_off= 0;

static inline int 
noise2() {
  int tot;
  int off= nt_off++;
  int cnt= 1;
  Noise *ns= ntbl;
  Noise *ns1= ntbl + NS_BANDS;

  tot= ((seed= seed * RAND_MULT % 131074) - 65535) * (NS_AMP / 65535 / (NS_BANDS + 1));

  while ((cnt & off) && ns < ns1) {
    int val= ((seed= seed * RAND_MULT % 131074) - 65535) * (NS_AMP / 65535 / (NS_BANDS + 1));
    tot += ns->val += ns->inc= (val - ns->val) / (cnt += cnt);
    ns++;
  }

  while (ns < ns1) {
    tot += (ns->val += ns->inc);
    ns++;
  }

  return noise_buf[noise_off++]= (tot >> NS_ADJ);
}

//
//	Generate next sample for white noise, with same
//	scaling as the sin_table[].
//
static inline int 
white_noise() {
  // White noise is simply a random value without filtering
  return ((seed= seed * RAND_MULT % 131074) - 65535) * (ST_AMP / 65535);
}

//
//	Generate next sample for brown noise, with same
//	scaling as the sin_table[].
//	Brown noise has more energy in lower frequencies,
//	implemented as a random walk (integration of white noise)
//
static int brown_last = 0;
static inline int 
brown_noise() {
  // Generate a random value
  int random = ((seed= seed * RAND_MULT % 131074) - 65535);
  
  // Integrate the random value with a decay factor to avoid overflow
  brown_last = (brown_last + (random / 16)) * 0.9;
  
  // Limit the value to avoid overflow
  if (brown_last > 65535) brown_last = 65535;
  if (brown_last < -65535) brown_last = -65535;
  
  // Scale to the same level as the sin_table
  return brown_last * (ST_AMP / 65535);
}

//	//
//	//	Generate next sample for simulated pink noise, scaled the same
//	//	as the sin_table[].  This version uses a library random number
//	//	generator, and no smoothing.
//	//
//	
//	inline double 
//	noise() {
//	  int tot= 0;
//	  int bit= ~0;
//	  int a;
//	  int off;
//	
//	  ns_tbl[ns_off]= (rand() - (RAND_MAX / 2)) / (NS_BIT + 1);
//	  off= ns_off;
//	  for (a= 0; a<=NS_BIT; a++, bit <<= 1) {
//	    off &= bit;
//	    tot += ns_tbl[off];
//	  }
//	  ns_off= (ns_off + 1) & ((1<<NS_BIT) - 1);
//	
//	  return tot * (ST_AMP / (RAND_MAX * 0.5));
//	}

//
//	Play loop
//

void 
loop() {	
  int c, cnt;
  int err;		// Error to add to 'now' until next cnt==0
  int fast= fast_mult != 0;
  int vfast= fast_mult > 20;		// Very fast - update status line often
  int utime= 0;
  int now_lo= 0;			// Low-order 16 bits of 'now' (fractional)
  int err_lo= 0;
  int ms_inc;

  setup_device();
  spin_carr_max= 127.0 / 1E-6 / out_rate;
  cnt= 1 + 1999 / out_buf_ms;	// Update every 2 seconds or so
  now= opt_S ? fast_tim0 : calcNow();
  if (opt_T != -1) now= opt_T;
  err= fast ? out_buf_ms * (fast_mult - 1) : 0;
  if (opt_L)
    byte_count= out_bps * (S64)(opt_L * 0.001 * out_rate);
  if (opt_E) {
    // Calculate the correct duration based on the last time in the sequence file
    int duration = t_per0(fast_tim0, fast_tim1);
    byte_count= out_bps * (S64)(duration * 0.001 * out_rate /
                (fast ? fast_mult : 1));
    if (!opt_Q) {
      printSequenceDuration();
    }
  }

  // Do byte-swapping if bigendian and outputting to a file or stream
  if ((opt_O || opt_o) &&
      out_mode == 1 && bigendian)
     out_mode= 2;

  if (opt_W)
    writeWAV();

  if (!opt_Q) fprintf(stderr, "\n");
  corrVal(0);		// Get into correct period
  dispCurrPer(stderr);	// Display
  status(0);
  
  while (1) {
    for (c= 0; c < cnt; c++) {
      corrVal(1);
      outChunk();
      ms_inc= out_buf_ms + err;
      now_lo += out_buf_lo + err_lo;
      if (now_lo >= 0x10000) { ms_inc += now_lo >> 16; now_lo &= 0xFFFF; }
      now += ms_inc;
      if (now > H24) now -= H24;
      if (vfast && (c&1)) status(0);
    }

    if (fast) {
      if (!vfast) status(0);
    }
    else {
      // Synchronize with real clock, gently over the next second or so
      char buf[32];
      int diff= calcNow() - now;
      if (abs(diff) > H12) diff= 0;
      sprintf(buf, "(%d)", diff); 

      err_lo= diff * 0x10000 / cnt;
      err= err_lo >> 16;
      err_lo &= 0xFFFF;

      if (DEBUG_CHK_UTIME) {
	int prev= utime;
	utime= userTime();
	sprintf(buf, "%d ticks", utime-prev);		// Replaces standard message
      }
      status(buf);
    }
  }
}


//
//	Output a chunk of sound (a buffer-ful), then return
//
//	Note: Optimised for 16-bit output.  Eight-bit output is
//	slower, but then it probably won't have to run at as high a
//	sample rate.
//

int rand0, rand1;

void 
outChunk() {
   int off= 0;

   if (mix_in) {
      int rv= inbuf_read(tmp_buf, out_blen);
      if (rv == 0) {
	 if (!opt_Q) warn("\nEnd of mix input audio stream");
	 exit(0);
      }
      while (rv < out_blen) tmp_buf[rv++]= 0;
   }
   
   while (off < out_blen) {
      int ns= noise2();		// Use same pink noise source for everything
      int tot1, tot2;		// Left and right channels
      int mix1, mix2;		// Incoming mix signals
      int val, a;
      Channel *ch;
      int *tab;

      mix1= tmp_buf[off];
      mix2= tmp_buf[off+1];

      // Do default mixing at 100% if no mix/* stuff is present
      if (!mix_flag) {
	 tot1= mix1 << 12;
	 tot2= mix2 << 12;
      } else {
	 tot1= tot2= 0;
      }
      
      ch= &chan[0];
      for (a= 0; a<N_CH; a++, ch++) switch (ch->typ) {
       case 0:
	  break;
       case 1:	// Binaural tones
	  ch->off1 += ch->inc1;
	  ch->off1 &= (ST_SIZ << 16) - 1;
	  tot1 += ch->amp * sin_table[ch->off1 >> 16];
	  ch->off2 += ch->inc2;
	  ch->off2 &= (ST_SIZ << 16) - 1;
	  tot2 += ch->amp2 * sin_table[ch->off2 >> 16];
	  break;
       case 2:	// Pink noise
	  val= ns * ch->amp;
	  tot1 += val;
	  tot2 += val;
	  break;
       case 9:	// White noise
	  val= white_noise() * ch->amp;
	  tot1 += val;
	  tot2 += val;
	  break;
       case 10:	// Brown noise
	  val= brown_noise() * ch->amp;
	  tot1 += val;
	  tot2 += val;
	  break;
       case 3:	// Bell
	  if (ch->off2) {
	     ch->off1 += ch->inc1;
	     ch->off1 &= (ST_SIZ << 16) - 1;
	     val= ch->off2 * sin_table[ch->off1 >> 16];
	     tot1 += val; tot2 += val;
	     if (--ch->inc2 < 0) {
		ch->inc2= out_rate/20;
		ch->off2 -= 1 + ch->off2 / 12;	// Knock off 10% each 50 ms
	     }
	  }
	  break;
       case 4:	// Spinning pink noise
	  ch->off1 += ch->inc1;
	  ch->off1 &= (ST_SIZ << 16) - 1;
	  val= (ch->inc2 * sin_table[ch->off1 >> 16]) >> 24;
	  tot1 += ch->amp * noise_buf[(uchar)(noise_off+128+val)];
	  tot2 += ch->amp * noise_buf[(uchar)(noise_off+128-val)];
	  break;
       case 5:	// Mix level
	  tot1 += mix1 * ch->amp;
	  tot2 += mix2 * ch->amp;
	  break;
       case 6:	// Mixspin - spinning mix stream
	  ch->off1 += ch->inc1;
	  ch->off1 &= (ST_SIZ << 16) - 1;
	  val= (ch->inc2 * sin_table[ch->off1 >> 16]) >> 24;
	  
	  // Mixspin intensity control
	  {
	    // Calculate intensity factor based on amplitude
	    // Amplitude varies from 0 to 4096 (0-100%)
	    // Normalize to a factor between 0.5 and 4.0
	    double intensity_factor = 0.5 + (ch->amp / 4096.0) * 3.5;

	    // Apply intensity factor to rotation value
	    int amplified_val = (int)(val * intensity_factor);
	    
	    // Limit value between -128 and 127
	    if (amplified_val > 127) amplified_val = 127;
	    if (amplified_val < -128) amplified_val = -128;
	    
	    // Use absolute value for calculations
	    int pos_val = amplified_val < 0 ? -amplified_val : amplified_val;
	    int mix_l, mix_r;
	    
	    // When val is close to 0, channels are played normally
	    // When val approaches +/-128, channels are swapped or muted
	    if (amplified_val >= 0) {
	      // Rotation to the right: left channel decreases, right channel receives part of the left channel
	      mix_l = (mix1 * (128 - pos_val)) >> 7;
	      mix_r = mix2 + ((mix1 * pos_val) >> 7);
	    } else {
	      // Rotation to the left: right channel decreases, left channel receives part of the right channel
	      mix_l = mix1 + ((mix2 * pos_val) >> 7);
	      mix_r = (mix2 * (128 - pos_val)) >> 7;
	    }

	    // Apply base volume (using 70% of amplitude for volume)
	    int base_amp = (int) (mix_amp != NULL ? *mix_amp : 4096.0) * 0.7;
	    tot1 += base_amp * mix_l;
	    tot2 += base_amp * mix_r;
	  }
	  break;
       case 7:	// Mixpulse - mix stream with pulse effect
          ch->off2 += ch->inc2;  // Modulator (pulse frequency)
          ch->off2 &= (ST_SIZ << 16) - 1;
          
          // Create the isochronic pulse effect in the audio stream
          {
            int mod_val = sin_table[ch->off2 >> 16];
            // Apply a threshold to create distinct pulses with space between them
            double mod_factor = 0.0;
            
            // Use only the positive part of the sine wave and apply a threshold
            // to create a space between pulses
            if (mod_val > ST_AMP * 0.3) {  // Threshold of 30% of the maximum value
              // Normalize from ST_AMP*0.3 to ST_AMP to 0 to 1
              mod_factor = (mod_val - (ST_AMP * 0.3)) / (double)(ST_AMP * 0.7);
              // Smooth the edges of the pulse to avoid clicks
              mod_factor = mod_factor * mod_factor * (3 - 2 * mod_factor);  // Cubic smoothing
            }
            
            // Apply the modulation factor to the audio stream
            // Use base_amp as in mixspin (70% of amplitude for volume)
            int base_amp = (int) (mix_amp != NULL ? *mix_amp : 4096.0) * 0.7;
            
            // Calculate the intensity of the effect based on amplitude (ch->amp)
            // When ch->amp is 0, there is no effect (only the original audio)
            // When ch->amp is 4096 (100%), the effect is maximum
            double effect_intensity = (ch->amp / 4096.0) * 1.5;
            
            // Apply the modulation to the audio stream with variable intensity
            // When effect_intensity is 0, only the original audio is reproduced
            // When effect_intensity is 1, the audio is fully modulated by the pulse
            double gain = (1.0 - effect_intensity) + (effect_intensity * mod_factor);
            tot1 += base_amp * mix1 * gain;
            tot2 += base_amp * mix2 * gain;
          }
          break;
       case 8:  // Isochronic tones
          ch->off1 += ch->inc1;  // Carrier (tone frequency)
          ch->off1 &= (ST_SIZ << 16) - 1;
          ch->off2 += ch->inc2;  // Modulator (pulse frequency)
          ch->off2 &= (ST_SIZ << 16) - 1;
          
          // Change the modulator to create a true isochronic tone with space between pulses
          {
            int mod_val = sin_table[ch->off2 >> 16];
            // Apply a threshold to create distinct pulses with space between them
            // Only produce sound when the modulation value is above a certain threshold
            double mod_factor = 0.0;
            
            // Use only the positive part of the sine wave and apply a threshold
            // to create a space between pulses
            if (mod_val > ST_AMP * 0.3) {  // Threshold of 30% of the maximum value
              // Normalize from ST_AMP*0.3 to ST_AMP to 0 to 1
              mod_factor = (mod_val - (ST_AMP * 0.3)) / (double)(ST_AMP * 0.7);
              // Smooth the edges of the pulse to avoid clicks
              mod_factor = mod_factor * mod_factor * (3 - 2 * mod_factor);  // Cubic smoothing
            }
            
            // Apply the modulation to the carrier
            val = ch->amp * sin_table[ch->off1 >> 16] * mod_factor;
            tot1 += val;
            tot2 += val;
          }
          break;
       default:	// Waveform-based binaural tones
	  tab= waves[-1 - ch->typ];
	  ch->off1 += ch->inc1;
	  ch->off1 &= (ST_SIZ << 16) - 1;
	  tot1 += ch->amp * tab[ch->off1 >> 16];
	  ch->off2 += ch->inc2;
	  ch->off2 &= (ST_SIZ << 16) - 1;
	  tot2 += ch->amp * tab[ch->off2 >> 16];
	  break;
      }

      // White noise dither; you could also try (rand0-rand1) for a
      // dither with more high frequencies
      rand0= rand1; 
      rand1= (rand0 * 0x660D + 0xF35F) & 0xFFFF;
      if (tot1 <= 0x7FFF0000) tot1 += rand0;
      if (tot2 <= 0x7FFF0000) tot2 += rand0;

      out_buf[off++]= tot1 >> 16;
      out_buf[off++]= tot2 >> 16;
  }

  // Generate debugging amplitude output
  if (DEBUG_DUMP_AMP) {
    short *sp= out_buf;
    short *end= out_buf + out_blen;
    int max= 0;
    while (sp < end) {
       int val= (int)sp[0] + (int)sp[1]; sp += 2;
       if (val < 0) val= -val;
       if (val > max) max= val;
    }
    max /= 328;
    while (max-- > 0) putc('#', stdout);
    printf("\n"); fflush(stdout);
  }

  // Rewrite buffer for 8-bit mode
  if (out_mode == 0) {
    short *sp= out_buf;
    short *end= out_buf + out_blen;
    char *cp= (char*)out_buf;
    while (sp < end) *cp++= (*sp++ >> 8) + 128;
  }

  // Rewrite buffer for 16-bit byte-swapping
  if (out_mode == 2) {
    char *cp= (char*)out_buf;
    char *end= (char*)(out_buf + out_blen);
    while (cp < end) { char tmp= *cp++; cp[-1]= cp[0]; *cp++= tmp; }
  }

  // Check and update the byte count if necessary
  if (byte_count > 0) {
    if (byte_count <= out_bsiz) {
      writeOut((char*)out_buf, byte_count);
#ifdef ALSA_AUDIO
      cleanup_alsa();
#endif
      exit(0);		// All done
    }
    else {
      writeOut((char*)out_buf, out_bsiz);
      byte_count -= out_bsiz;
    }
  }
  else
    writeOut((char*)out_buf, out_bsiz);
} 

void 
writeOut(char *buf, int siz) {
  int rv;

#ifdef WIN_AUDIO
  if (out_fd == -9999) {
     // Win32 output: write it to a header and send it off
     MMRESULT rv;

     //debug_win32_buffer_status();

     //while (aud_cnt == BUFFER_COUNT) {
     //while (aud_head[aud_current]->dwFlags & WHDR_INQUEUE) {
     while (!(aud_head[aud_current]->dwFlags & WHDR_DONE)) {
	//debug("SLEEP %d", out_buf_ms / 2 + 1);
	Sleep(out_buf_ms / 2 + 1);
	//debug_win32_buffer_status();
     }

     memcpy(aud_head[aud_current]->lpData, buf, siz);
     aud_head[aud_current]->dwBufferLength= (DWORD)siz;

     //debug("Output buffer %d", aud_current);
     rv= waveOutWrite(aud_handle, aud_head[aud_current], sizeof(WAVEHDR));

     if (rv != MMSYSERR_NOERROR) {
        char buf[255];
        waveOutGetErrorText(rv, buf, sizeof(buf)-1);
        error("Error writing a fragment to the audio device:\n  %s", buf);
     }
   
     aud_cnt++; 
     aud_current++;
     aud_current %= BUFFER_COUNT;

     return;
  }
#endif

#ifdef MAC_AUDIO
  if (out_fd == -9999) {
    int new_wr= (aud_wr + 1) % BUFFER_COUNT;

    // Wait until there is space
    while (new_wr == aud_rd) delay(20);

    memcpy(aud_buf[aud_wr], buf, siz);
    aud_wr= new_wr;

    return;
  }
#endif

#ifdef ALSA_AUDIO
  if (out_fd == -9998) {
    int err;
    int frames = siz / (out_mode ? 4 : 2); // Number of frames (stereo samples)
    
    // Write data to the ALSA device
    if ((err = snd_pcm_writei(alsa_handle, buf, frames)) < 0) {
      if (err == -EPIPE) {
        // Underflow occurred, try to recover
        if ((err = snd_pcm_prepare(alsa_handle)) < 0) {
          error("Unable to recover from underrun: %s", snd_strerror(err));
        }
        // Try to write again
        if ((err = snd_pcm_writei(alsa_handle, buf, frames)) < 0) {
          error("Failed to write to ALSA device after recovery: %s", snd_strerror(err));
        }
      } else {
        error("Failed to write to ALSA device: %s", snd_strerror(err));
      }
    }
    
    return;
  }
#endif

  while (-1 != (rv= write(out_fd, buf, siz))) {
    if (0 == (siz -= rv)) return;
    buf += rv;
  }
  error("Output error");
}

//
//	Calculate amplitude adjustment factor for frequency 'freq'
//

double 
ampAdjust(double freq) {
   int a;
   struct AmpAdj *p0, *p1;

   if (!opt_c) return 1.0;
   if (freq <= ampadj[0].freq) return ampadj[0].adj;
   if (freq >= ampadj[opt_c-1].freq) return ampadj[opt_c-1].adj;

   for (a= 1; a<opt_c; a++) 
      if (freq < ampadj[a].freq) 
	 break;
   
   p0= &ampadj[a-1];
   p1= &ampadj[a];
      
   return p0->adj + (p1->adj - p0->adj) * (freq - p0->freq) / (p1->freq - p0->freq);
}
   

//
//	Correct channel values and types according to current period,
//	and current time
//

void 
corrVal(int running) {
   int a;
   int t0= per->tim;
   int t1= per->nxt->tim;
   Channel *ch;
   Voice *v0, *v1, *vv;
   double rat0, rat1;
   int trigger= 0;
   
   // Move to the correct period
   while ((now >= t0) ^ (now >= t1) ^ (t1 > t0)) {
      per= per->nxt;
      t0= per->tim;
      t1= per->nxt->tim;
      if (running) {
	 if (tty_erase) {
#ifdef ANSI_TTY	
	    fprintf(stderr, "\033[K");
#else
	    fprintf(stderr, "%*s\r", tty_erase, ""); 
	    tty_erase= 0;
#endif
	 }
	 dispCurrPer(stderr); status(0);
      }
      trigger= 1;		// Trigger bells or whatever
   }
   
   // Run through to calculate voice settings for current time
   rat1= t_per0(t0, now) / (double)t_per24(t0, t1);
   rat0= 1 - rat1;
   for (a= 0; a<N_CH; a++) {
      ch= &chan[a];
      v0= &per->v0[a];
      v1= &per->v1[a];
      vv= &ch->v;

      // Pointer to the amplitude of the mix to use with mixspin/mixpulse
      if(vv->typ == 5 && mix_amp == NULL)
        mix_amp= &vv->amp;
      
      if (vv->typ != v0->typ) {
	 switch (vv->typ= ch->typ= v0->typ) {
	  case 1:
	     ch->off1= ch->off2= 0; break;
	  case 2:
	     break;
	  case 3:
	     ch->off1= ch->off2= 0; break;
	  case 4:
	     ch->off1= ch->off2= 0; break;
	  case 5:
	     break;
	  case 8:  // Isochronic tones
	     ch->off1= ch->off2= 0; break;
	  case 6:  // Mixspin
	     ch->off1= ch->off2= 0; break;
	  case 7:  // Mixpulse
	     ch->off1= ch->off2= 0; break;
	  default:
	     ch->off1= ch->off2= 0; break;
	 }
      }
      
      // Setup vv->*
      switch (vv->typ) {
       case 1:
	  vv->amp= rat0 * v0->amp + rat1 * v1->amp;
	  vv->carr= rat0 * v0->carr + rat1 * v1->carr;
	  vv->res= rat0 * v0->res + rat1 * v1->res;
	  break;
       case 2:
	  vv->amp= rat0 * v0->amp + rat1 * v1->amp;
	  break;
       case 3:
	  vv->amp= v0->amp;		// No need to slide, as bell only rings briefly
	  vv->carr= v0->carr;
	  break;
       case 4:
	  vv->amp= rat0 * v0->amp + rat1 * v1->amp;
	  vv->carr= rat0 * v0->carr + rat1 * v1->carr;
	  vv->res= rat0 * v0->res + rat1 * v1->res;
	  if (vv->carr > spin_carr_max) vv->carr= spin_carr_max; // Clipping sweep width
	  if (vv->carr < -spin_carr_max) vv->carr= -spin_carr_max;
	  break;
       case 5:
	  vv->amp= rat0 * v0->amp + rat1 * v1->amp;
	  break;
       case 8:  // Isochronic tones
          vv->amp= rat0 * v0->amp + rat1 * v1->amp;
          vv->carr= rat0 * v0->carr + rat1 * v1->carr;
          vv->res= rat0 * v0->res + rat1 * v1->res;
          break;
       case 6:  // Mixspin
          vv->amp= rat0 * v0->amp + rat1 * v1->amp;
          vv->carr= rat0 * v0->carr + rat1 * v1->carr;
          vv->res= rat0 * v0->res + rat1 * v1->res;
          if (vv->carr > spin_carr_max) vv->carr= spin_carr_max; // Clipping sweep width
          if (vv->carr < -spin_carr_max) vv->carr= -spin_carr_max;
          break;
       case 7:  // Mixpulse
          vv->amp= rat0 * v0->amp + rat1 * v1->amp;
          vv->res= rat0 * v0->res + rat1 * v1->res;
          break;
       default:		// Waveform based binaural
	  vv->amp= rat0 * v0->amp + rat1 * v1->amp;
	  vv->carr= rat0 * v0->carr + rat1 * v1->carr;
	  vv->res= rat0 * v0->res + rat1 * v1->res;
	  break;
      }
   }
   
   // Check and limit amplitudes if -c option in use
   if (opt_c) {
      double tot_beat= 0, tot_other= 0;
      for (a= 0; a<N_CH; a++) {
	 vv= &chan[a].v;
	 if (vv->typ == 1) {
	    double adj1= ampAdjust(vv->carr + vv->res/2);
	    double adj2= ampAdjust(vv->carr - vv->res/2);
	    if (adj2 > adj1) adj1= adj2;
	    tot_beat += vv->amp * adj1;
	 } else if (vv->typ) {
	    tot_other += vv->amp;
	 }
      }
      if (tot_beat + tot_other > 4096) {
	 double adj_beat= (tot_beat > 4096) ? 4096 / tot_beat : 1.0;
	 double adj_other= (4096 - tot_beat * adj_beat) / tot_other;
	 for (a= 0; a<N_CH; a++) {
	    vv= &chan[a].v;
	    if (vv->typ == 1)
	       vv->amp *= adj_beat;
	    else if (vv->typ) 	
	       vv->amp *= adj_other;
	 }
      }
   }
   
   // Setup Channel data from Voice data
   for (a= 0; a<N_CH; a++) {
      ch= &chan[a];
      vv= &ch->v;
      
      // Setup ch->* from vv->*
      switch (vv->typ) {
	 double freq1, freq2;
       case 1:
	  freq1= vv->carr + vv->res/2;
	  freq2= vv->carr - vv->res/2;
	  if (opt_c) {
	     ch->amp= vv->amp * ampAdjust(freq1);
	     ch->amp2= vv->amp * ampAdjust(freq2);
	  } else 
	     ch->amp= ch->amp2= (int)vv->amp;
	  ch->inc1= (int)(freq1 / out_rate * ST_SIZ * 65536);
	  ch->inc2= (int)(freq2 / out_rate * ST_SIZ * 65536);
	  break;
       case 2:
	  ch->amp= (int)vv->amp;
	  break;
       case 3:
	  ch->amp= (int)vv->amp;
	  ch->inc1= (int)(vv->carr / out_rate * ST_SIZ * 65536);
	  if (trigger) {		// Trigger the bell only on entering the period
	     ch->off2= ch->amp;
	     ch->inc2= out_rate/20;
	  }
	  break;
       case 4:
	  ch->amp= (int)vv->amp;
	  ch->inc1= (int)(vv->res / out_rate * ST_SIZ * 65536);
	  ch->inc2= (int)(vv->carr * 1E-6 * out_rate * (1<<24) / ST_AMP);
	  break;
       case 5:
	  ch->amp= (int)vv->amp;
	  break;
       case 8:  // Isochronic tones
          ch->amp= (int)vv->amp;
          // Carrier (tone frequency)
          ch->inc1= (int)(vv->carr / out_rate * ST_SIZ * 65536);
          // Modulator (pulse frequency)
          ch->inc2= (int)(vv->res / out_rate * ST_SIZ * 65536);
          break;
       case 6:  // Mixspin
          ch->amp= (int)vv->amp;
          ch->inc1= (int)(vv->res / out_rate * ST_SIZ * 65536);
          ch->inc2= (int)(vv->carr * 1E-6 * out_rate * (1<<24) / ST_AMP);
          break;
       case 7:  // Mixpulse
          ch->amp= (int)vv->amp;
          // Modulator (pulse frequency)
          ch->inc2= (int)(vv->res / out_rate * ST_SIZ * 65536);
          break;
       default:		// Waveform based binaural
	  ch->amp= (int)vv->amp;
	  ch->inc1= (int)((vv->carr + vv->res/2) / out_rate * ST_SIZ * 65536);
	  ch->inc2= (int)((vv->carr - vv->res/2) / out_rate * ST_SIZ * 65536);
	  if (ch->inc1 > ch->inc2) 
	     ch->inc2= -ch->inc2;
	  else 
	     ch->inc1= -ch->inc1;
	  break;
      }
   }
}       
      
//
//	Setup audio device
//

void 
setup_device(void) {

  // Handle output to files and pipes
  if (opt_O || opt_o) {
    if (opt_O)
      out_fd= 1;		// stdout
    else {
      FILE *out;		// Need to create a stream to set binary mode for DOS
      if (!(out= fopen(opt_o, "wb")))
	error("Can't open \"%s\", errno %d", opt_o, errno);
      out_fd= fileno(out);
    }
    out_blen= out_rate * 2 / out_prate;		// 10 fragments a second by default
    while (out_blen & (out_blen-1)) out_blen &= out_blen-1;		// Make power of two
    out_bsiz= out_blen * (out_mode ? 2 : 1);
    out_bps= out_mode ? 4 : 2;
    out_buf= (short*)Alloc(out_blen * sizeof(short));
    out_buf_lo= (int)(0x10000 * 1000.0 * 0.5 * out_blen / out_rate);
    out_buf_ms= out_buf_lo >> 16;
    out_buf_lo &= 0xFFFF;
    tmp_buf= (int*)Alloc(out_blen * sizeof(int));

    if (!opt_Q && !opt_W)		// Informational message for opt_W is written later
       warn("Outputting %d-bit raw audio data at %d Hz with %d-sample blocks, %d ms per block",
	    out_mode ? 16 : 8, out_rate, out_blen/2, out_buf_ms);
    return;
  }

#ifdef ALSA_AUDIO
  // ALSA audio output
  {
    int err;
    unsigned int rate = out_rate;
    snd_pcm_format_t format;
    snd_pcm_uframes_t buffer_size;
    snd_pcm_uframes_t period_size;
    
    // Open the PCM device for playback
    if ((err = snd_pcm_open(&alsa_handle, opt_d, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      error("Unable to open ALSA device %s: %s", opt_d, snd_strerror(err));
    }
    
    // Allocate hardware parameters
    snd_pcm_hw_params_alloca(&alsa_params);
    
    // Fill with default values
    if ((err = snd_pcm_hw_params_any(alsa_handle, alsa_params)) < 0) {
      error("Unable to configure default parameters: %s", snd_strerror(err));
    }
    
    // Configure access
    if ((err = snd_pcm_hw_params_set_access(alsa_handle, alsa_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      error("Unable to configure access: %s", snd_strerror(err));
    }
    
    // Configure format (16 bits or 8 bits)
    format = out_mode ? SND_PCM_FORMAT_S16 : SND_PCM_FORMAT_U8;
    if ((err = snd_pcm_hw_params_set_format(alsa_handle, alsa_params, format)) < 0) {
      error("Unable to configure format %s: %s", 
            out_mode ? "16-bit" : "8-bit", snd_strerror(err));
    }
    
    // Configure channels (stereo)
    if ((err = snd_pcm_hw_params_set_channels(alsa_handle, alsa_params, 2)) < 0) {
      error("Unable to configure stereo channels: %s", snd_strerror(err));
    }
    
    // Configure sampling rate
    if ((err = snd_pcm_hw_params_set_rate_near(alsa_handle, alsa_params, &rate, 0)) < 0) {
      error("Unable to configure sampling rate %d: %s", 
            out_rate, snd_strerror(err));
    }
    
    // Update sampling rate if it was changed
    out_rate = rate;
    
    // Configure buffer and period size
    period_size = 1024; // Initial value
    if ((err = snd_pcm_hw_params_set_period_size_near(alsa_handle, alsa_params, &period_size, 0)) < 0) {
      error("Unable to configure period size: %s", snd_strerror(err));
    }
    
    buffer_size = period_size * 4; // Buffer of 4 periods
    if ((err = snd_pcm_hw_params_set_buffer_size_near(alsa_handle, alsa_params, &buffer_size)) < 0) {
      error("Unable to configure buffer size: %s", snd_strerror(err));
    }
    
    // Apply hardware parameters
    if ((err = snd_pcm_hw_params(alsa_handle, alsa_params)) < 0) {
      error("Unable to apply hardware parameters: %s", snd_strerror(err));
    }
    
    // Get the actual period size
    snd_pcm_hw_params_get_period_size(alsa_params, &period_size, 0);
    
    // Configure output buffer size
    out_blen = period_size * 2; // Stereo
    out_bsiz = out_blen * (out_mode ? 2 : 1); // 16 bits or 8 bits
    out_bps = out_mode ? 4 : 2;
    out_buf = (short*)Alloc(out_blen * sizeof(short));
    out_buf_lo = (int)(0x10000 * 1000.0 * 0.5 * out_blen / out_rate);
    out_buf_ms = out_buf_lo >> 16;
    out_buf_lo &= 0xFFFF;
    tmp_buf = (int*)Alloc(out_blen * sizeof(int));
    
    // Mark that we are using ALSA
    out_fd = -9998; // Special value for ALSA
    
    if (!opt_Q)
      warn("ALSA audio output %d-bit at %d Hz with period of %lu samples, %d ms per period",
           out_mode ? 16 : 8, out_rate, period_size, out_buf_ms);
    return;
  }
#endif

#ifdef WIN_AUDIO
  // Output using Win32 calls
  {
     MMRESULT rv;
     WAVEFORMATEX fmt;
     int a;

     fmt.wFormatTag= WAVE_FORMAT_PCM;           
     fmt.nChannels= 2;
     fmt.nSamplesPerSec= out_rate;
     fmt.wBitsPerSample= out_mode ? 16 : 8;
     fmt.nBlockAlign= fmt.nChannels * (fmt.wBitsPerSample/8);
     fmt.nAvgBytesPerSec= fmt.nSamplesPerSec * fmt.nBlockAlign;
     fmt.cbSize= 0;
     aud_handle= NULL;

     // if (MMSYSERR_NOERROR != 
     //    waveOutOpen(&aud_handle, WAVE_MAPPER, &fmt, 0, 
     //                0L, WAVE_FORMAT_QUERY))
     //    error("Windows is rejecting our audio request (%d-bit stereo, %dHz)",
     //          out_mode ? 16 : 8, out_rate);

     if (MMSYSERR_NOERROR != 
	 (rv= waveOutOpen(&aud_handle, WAVE_MAPPER, 
			  (WAVEFORMATEX*)&fmt, (DWORD)win32_audio_callback, 
			  (DWORD)0, CALLBACK_FUNCTION))) {
	char buf[255];
	waveOutGetErrorText(rv, buf, sizeof(buf)-1);
	error("Can't open audio device (%d-bit stereo, %dHz):\n  %s",
               out_mode ? 16 : 8, out_rate, buf);
     }

     if (fmt.nChannels != 2)
	error("Can't open audio device in stereo");
     if (fmt.wBitsPerSample != (out_mode ? 16 : 8))
	error("Can't open audio device in %d-bit mode", out_mode ? 16 : 8);
     
     aud_current= 0;
     aud_cnt= 0;

     for (a= 0; a<BUFFER_COUNT; a++) {
	char *p= (char *)Alloc(sizeof(WAVEHDR) + BUFFER_SIZE);
	WAVEHDR *w= aud_head[a]= (WAVEHDR*)p;

	w->lpData= (LPSTR)p + sizeof(WAVEHDR);
	w->dwBufferLength= (DWORD)BUFFER_SIZE;
	w->dwBytesRecorded= 0L;
	w->dwUser= 0;
	w->dwFlags= 0;
	w->dwLoops= 0;
	w->lpNext= 0;
	w->reserved= 0;

	rv= waveOutPrepareHeader(aud_handle, w, sizeof(WAVEHDR));
	if (rv != MMSYSERR_NOERROR) {
	   char buf[255];
	   waveOutGetErrorText(rv, buf, sizeof(buf)-1);
	   error("Can't setup a wave header %d:\n  %s", a, buf);
	}
	w->dwFlags |= WHDR_DONE;
     }     
     
     out_rate= fmt.nSamplesPerSec;
     out_bsiz= BUFFER_SIZE;
     out_blen= out_mode ? out_bsiz/2 : out_bsiz;
     out_bps= out_mode ? 4 : 2;
     out_buf= (short*)Alloc(out_blen * sizeof(short));
     out_buf_lo= (int)(0x10000 * 1000.0 * 0.5 * out_blen / out_rate);
     out_buf_ms= out_buf_lo >> 16;
     out_buf_lo &= 0xFFFF;
     out_fd= -9999;
     tmp_buf= (int*)Alloc(out_blen * sizeof(int));

     if (!opt_Q)
	warn("Outputting %d-bit audio at %d Hz with %d %d-sample fragments, "
	     "%d ms per fragment", out_mode ? 16 : 8, 
	     out_rate, BUFFER_COUNT, out_blen/2, out_buf_ms);
  }
#endif
#ifdef MAC_AUDIO
  // Mac CoreAudio for OS X
  {
    char deviceName[256];
    OSStatus err;
    UInt32 propertySize, bufferByteCount;
    struct AudioStreamBasicDescription streamDesc;
    
    int old_out_rate= out_rate;
    int buffer_size= opt_B > 0 ? opt_B : BUFFER_SIZE;

    out_bsiz= buffer_size;
    out_blen= out_mode ? out_bsiz/2 : out_bsiz;
    out_bps= out_mode ? 4 : 2;
    out_buf= (short*)Alloc(out_blen * sizeof(short));
    out_buf_lo= (int)(0x10000 * 1000.0 * 0.5 * out_blen / out_rate);
    out_buf_ms= out_buf_lo >> 16;
    out_buf_lo &= 0xFFFF;
    tmp_buf= (int*)Alloc(out_blen * sizeof(int));

    // N.B.  Both -r and -b flags are totally ignored for CoreAudio --
    // we just use whatever the default device is set to, and feed it
    // floats.
    out_mode= 1;
    out_fd= -9999;

    // Find default device
    propertySize= sizeof(aud_dev);
    AudioObjectPropertyAddress propertyAddress = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };
    if ((err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize, &aud_dev)))
    {
        error("Get default output device failed, status = %d", (int)err);
    }
    
    if (aud_dev == kAudioDeviceUnknown)
      error("No default audio device found");
    
    // Get device name
    propertySize = sizeof(deviceName);
    propertyAddress.mSelector = kAudioDevicePropertyDeviceName;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;
    if ((err = AudioObjectGetPropertyData(aud_dev, &propertyAddress, 0, NULL, &propertySize, deviceName)))
    {
        error("Get audio device name failed, status = %d", (int)err);
    }
    
    // Get device properties
    propertySize = sizeof(streamDesc);
    propertyAddress.mSelector = kAudioDevicePropertyStreamFormat;
    propertyAddress.mScope = kAudioObjectPropertyScopeOutput; // Adjusted for output
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;
    if ((err = AudioObjectGetPropertyData(aud_dev, &propertyAddress, 0, NULL, &propertySize, &streamDesc)))
    {
        error("Get audio device properties failed, status = %d", (int)err);
    }

    out_rate= (int)streamDesc.mSampleRate;

    if (streamDesc.mChannelsPerFrame != 2) 
      error("SBaGen+ requires a stereo output device -- \n"
	    "default output has %d channels",
	    streamDesc.mChannelsPerFrame);

    if (streamDesc.mFormatID != kAudioFormatLinearPCM ||
	!(streamDesc.mFormatFlags & kLinearPCMFormatFlagIsFloat)) 
      error("Expecting a 32-bit float linear PCM output stream -- \n"
	    "default output uses another format");

    // Set buffer size
    bufferByteCount = (float) buffer_size / 2 * sizeof(float);
    propertySize = sizeof(bufferByteCount);
    propertyAddress.mSelector = kAudioDevicePropertyBufferSize;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;
    if ((err = AudioObjectSetPropertyData(aud_dev, &propertyAddress, 0, NULL, propertySize, &bufferByteCount)))
    {
        error("Set audio output buffer size failed, status = %d", (int)err);
    }

    // Setup callback and start it
    err= AudioDeviceAddIOProc(aud_dev, mac_callback, (void *)1);
    err= AudioDeviceStart(aud_dev, mac_callback);

    // Report settings      
    if (!opt_Q) {
       if (old_out_rate != out_rate && !out_rate_def) 
	  warn("*** WARNING: Non-default sampling rates not yet supported on OS X ***");
       warn("Outputting %d-bit audio at %d Hz to \"%s\",\n"
	    "  using %d %d-sample fragments, %d ms per fragment",
	    (int)streamDesc.mBitsPerChannel, out_rate, deviceName,
	    BUFFER_COUNT, out_blen/2, out_buf_ms);
    }
  }
#endif
#ifdef NO_AUDIO
  error("Direct output to soundcard not supported on this platform.\n"
	"Use -o or -O to write raw data, or -Wo or -WO to write a WAV file.");
#endif
}

//
//	Audio callback for Win32
//

#ifdef WIN_AUDIO
void CALLBACK
win32_audio_callback(HWAVEOUT hand, UINT uMsg,     
		     DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
   switch (uMsg) {
    case WOM_CLOSE:
       break;
    case WOM_OPEN:
       break;
    case WOM_DONE:
       aud_cnt--;
       //debug("Buffer done (cnt==%d)", aud_cnt);
       //debug_win32_buffer_status();
       break;
   }
}

void debug_win32_buffer_status() {
   char tmp[80];
   char *p= tmp;
   int a;
   for (a= 0; a<BUFFER_COUNT; a++) {
      *p++= (aud_head[a]->dwFlags & WHDR_INQUEUE) ? 'I' : '-';
      *p++= (aud_head[a]->dwFlags & WHDR_DONE) ? 'D' : '-';
      *p++= ' ';
   }
   p[-1]= 0;
   debug(tmp);
}
#endif

//
//	Audio callback for Mac OS X
//

#ifdef MAC_AUDIO
OSStatus mac_callback(AudioDeviceID inDevice,
		      const AudioTimeStamp *inNow,
		      const AudioBufferList *inInputData,
		      const AudioTimeStamp *inInputTime,
		      AudioBufferList *outOutputData,
		      const AudioTimeStamp *inOutputTime,
		      void *inClientData) 
{
  float *fp= outOutputData->mBuffers[0].mData;
  int cnt= opt_B > 0 ? opt_B / 2 : BUFFER_SIZE / 2;
  short *sp;

  if (aud_rd == aud_wr) {
    // Nothing in buffer list, so fill with silence
    while (cnt-- > 0) *fp++= 0.0;
  } else {
    // Consume a buffer
    sp= (short*)aud_buf[aud_rd];
    while (cnt-- > 0) *fp++= *sp++ * (1/32768.0);
    aud_rd= (aud_rd + 1) % BUFFER_COUNT;
  }
  
  return kAudioHardwareNoError;
}
#endif

//
//	Write a WAV header, and setup out_mode if byte-swapping is
//	required.  'byte_count' should have been set up by this point.
//

#define addU4(xx) { int a= xx; *p++= a; *p++= (a >>= 8); *p++= (a >>= 8); *p++= (a >>= 8); }
#define addStr(xx) { char *q= xx; *p++= *q++; *p++= *q++; *p++= *q++; *p++= *q++; }

void 
writeWAV() {
  char buf[44], *p= buf;

  if (byte_count + 36 != (int)(byte_count + 36)) {
     int tmp;
     byte_count= 0xFFFFFFF8-36;
     tmp= byte_count/out_bps/out_rate;
     warn("WARNING: Selected length is too long for the WAV format; truncating to %dh%02dm%02ds",
	  tmp/3600, tmp/60%60, tmp%60);
  }

  addStr("RIFF");
  addU4(byte_count + 36);
  addStr("WAVE");
  addStr("fmt ");
  addU4(16);
  addU4(0x00020001);
  addU4(out_rate);
  addU4(out_rate * out_bps);
  addU4(0x0004 + 0x10000*(out_bps*4));	// 2,4 -> 8,16 - always assume stereo
  addStr("data");
  addU4(byte_count);
  writeOut(buf, 44);

  if (!opt_Q)
     warn("Outputting %d-bit WAV data at %d Hz, file size %d bytes",
	  out_mode ? 16 : 8, out_rate, byte_count + 44);
}

//
//	Read a line, discarding blank lines and comments.  Rets:
//	Another line?  Comments starting with '##' are displayed on
//	stderr.
//   

int 
readLine() {
   char *p;
   lin= buf;
   
   while (1) {
      if (!fgets(lin, sizeof(buf), in)) {
	 if (feof(in)) return 0;
	 error("Read error on sequence file");
      }
      
      in_lin++;
      
      while (isspace(*lin)) lin++;
      p= strchr(lin, '#');
      if (p && p[1] == '#') fprintf(stderr, "%s", p);
      p= p ? p : strchr(lin, 0);
      while (p > lin && isspace(p[-1])) p--;
      if (p != lin) break;
   }
   *p= 0;
   lin_copy= buf_copy;
   strcpy(lin_copy, lin);
   return 1;
}

//
//	Get next word at '*lin', moving lin onwards, or return 0
//

char *
getWord() {
  char *rv, *end;
  while (isspace(*lin)) lin++;
  if (!*lin) return 0;

  rv= lin;
  while (*lin && !isspace(*lin)) lin++;
  end= lin;
  if (*lin) lin++;
  *end= 0;

  return rv;
}

//
//	Bad sequence file
//

void 
badSeq() {
  error("Bad sequence file content at line: %d\n  %s", in_lin, lin_copy);
}

// Convenience for situations where buffer is being filled by
// something other than readLine()
void 
readNameDef2() {
   lin= buf; lin_copy= buf_copy;
   strcpy(lin_copy, lin);
   readNameDef();
}
void 
readTimeLine2() {
   lin= buf; lin_copy= buf_copy;
   strcpy(lin_copy, lin);
   readTimeLine();
}

// Convenience for creating sequences on the fly
void 
formatNameDef(char *fmt, ...) {
   va_list ap;
   va_start(ap, fmt);
   vsnprintf(buf, sizeof(buf), fmt, ap);
   readNameDef2();
}
void 
formatTimeLine(int tim, char *fmt, ...) {
   va_list ap;
   char *p= buf + sprintf(buf, "%02d:%02d:%02d ", tim/3600, tim/60%60, tim%60);
   va_start(ap, fmt);
   vsnprintf(p, buf + sizeof(buf) - p, fmt, ap);
   readTimeLine2();
}

//
//	Generate a list of Period structures, based on the tone-specs
//	passed in (ac,av)
//

void 
readSeqImm(int ac, char **av) {
   char *p= buf;

   in_lin= 0;
   p += sprintf(p, "immediate:");
   while (ac-- > 0) p += sprintf(p, " %s", *av++);
   readNameDef2();
   
   strcpy(buf, "00:00 immediate");
   readTimeLine2();

   correctPeriods();
}

//
//	Read a list of sequence files, and generate a list of Period
//	structures
//

void 
readSeq(int ac, char **av) {
   // Setup a 'now' value to use for NOW in the sequence file
   now= calcNow();	

   while (ac-- > 0) {
      char *fnam= *av++;
      int start= 1;
      
      in= (0 == strcmp("-", fnam)) ? stdin : fopen(fnam, "r");
      if (!in) error("Can't open sequence file: %s", fnam);
      
      in_lin= 0;
      
      while (readLine()) {
	 char *p= lin;

	 // Blank lines
	 if (!*p) continue;
	 
	 // Look for options
	 if (*p == '-') {
	    if (!start) 
	       error("Options are only permitted at start of sequence file:\n  %s", p);
	    handleOptions(p);
	    continue;
	 }

	 // Check to see if it fits the form of <name>:<white-space>
	 start= 0;
	 if (!isalpha(*p)) 
	    p= 0;
	 else {
	    while (isalnum(*p) || *p == '_' || *p == '-') p++;
	    if (*p++ != ':' || !isspace(*p)) 
	       p= 0;
	 }
	 
	 if (p)
	    readNameDef();
	 else 
	    readTimeLine();
      }
      
      if (in != stdin) fclose(in);
   }
   
   correctPeriods();
}


//
//	Fill in all the correct information for the Periods, assuming
//	they have just been loaded using readTimeLine()
//


void 
correctPeriods() {
  // Get times all correct
  {
    Period *pp= per;
    do {
      if (pp->fi == -2) {
	pp->tim= pp->nxt->tim;
	pp->fi= -1;
      }

      pp= pp->nxt;
    } while (pp != per);
  }

  // Make sure that the transitional periods each have enough time
  {
    Period *pp= per;
    do {
      if (pp->fi == -1) {
	int per= t_per0(pp->tim, pp->nxt->tim);
	if (per < fade_int) {
	  int adj= (fade_int - per) / 2, adj0, adj1;
	  adj0= t_per0(pp->prv->tim, pp->tim);
	  adj0= (adj < adj0) ? adj : adj0;
	  adj1= t_per0(pp->nxt->tim, pp->nxt->nxt->tim);
	  adj1= (adj < adj1) ? adj : adj1;
	  pp->tim= (pp->tim - adj0 + H24) % H24;
	  pp->nxt->tim= (pp->nxt->tim + adj1) % H24;
	}
      }

      pp= pp->nxt;
    } while (pp != per);
  }

  // Fill in all the voice arrays, and sort out details of
  // transitional periods
  {
    Period *pp= per;
    do {
      if (pp->fi < 0) {
	int fo, fi;
	int a;
	int midpt= 0;

	Period *qq= (Period*)Alloc(sizeof(*qq));
	qq->prv= pp; qq->nxt= pp->nxt;
	qq->prv->nxt= qq->nxt->prv= qq;

	qq->tim= t_mid(pp->tim, qq->nxt->tim);

	memcpy(pp->v0, pp->prv->v1, sizeof(pp->v0));
	memcpy(qq->v1, qq->nxt->v0, sizeof(qq->v1));

	// Special handling for bells
	for (a= 0; a<N_CH; a++) {
	  if (pp->v0[a].typ == 3 && pp->fi != -3)
	    pp->v0[a].typ= 0;

	  if (qq->v1[a].typ == 3 && pp->fi == -3)
	    qq->v1[a].typ= 0;
	}
	      
	fo= pp->prv->fo;
	fi= qq->nxt->fi;

	// Special handling for -> slides:
	//   always slide, and stretch slide if possible
	if (pp->fi == -3) {
	  fo= fi= 2;		// Force slides for ->
	  for (a= 0; a<N_CH; a++) {
	    Voice *vp= &pp->v0[a];
	    Voice *vq= &qq->v1[a];
	    if (vp->typ == 0 && vq->typ != 0 && vq->typ != 3) {
	      memcpy(vp, vq, sizeof(*vp)); vp->amp= 0;
	    }
	    else if (vp->typ != 0 && vq->typ == 0) {
	      memcpy(vq, vp, sizeof(*vq)); vq->amp= 0;
	    }
	  }
	}

	memcpy(pp->v1, pp->v0, sizeof(pp->v1));
	memcpy(qq->v0, qq->v1, sizeof(qq->v0));

	for (a= 0; a<N_CH; a++) {
	  Voice *vp= &pp->v1[a];
	  Voice *vq= &qq->v0[a];
	  if ((fo == 0 || fi == 0) ||		// Fade in/out to silence
	      (vp->typ != vq->typ) ||		// Different types
	      ((fo == 1 || fi == 1) &&		// Fade thru, but different pitches
	       (vp->typ == 1 || vp->typ < 0) && 
	       (vp->carr != vq->carr || vp->res != vq->res))
	      ) {
	    vp->amp= vq->amp= 0;		// To silence
	    midpt= 1;				// Definitely need the mid-point

	    if (vq->typ == 3) {	 		// Special handling for bells
	      vq->amp= qq->v1[a].amp; 
	      qq->nxt->v0[a].typ= qq->nxt->v1[a].typ= 0;
	    }
	  }
	  else if (vp->typ == 3) {		// Else smooth transition - for bells not so smooth
	    qq->v0[a].typ= qq->v1[a].typ= 0;
	  }
	  else {				// Else smooth transition
	    vp->amp= vq->amp= (vp->amp + vq->amp) / 2;
	    if (vp->typ == 1 || vp->typ == 4 || vp->typ < 0) {
	      vp->carr= vq->carr= (vp->carr + vq->carr) / 2;
	      vp->res= vq->res= (vp->res + vq->res) / 2;
	    }
	  }
	}

	// If we don't really need the mid-point, then get rid of it
	if (!midpt) {
	  memcpy(pp->v1, qq->v1, sizeof(pp->v1));
	  qq->prv->nxt= qq->nxt;
	  qq->nxt->prv= qq->prv;
	  free(qq);
	}
	else pp= qq;
      }

      pp= pp->nxt;
    } while (pp != per);
  }

  // Clear out zero length sections, and duplicate sections
  {
    Period *pp;
    while (per != per->nxt) {
      pp= per;
      do {
	if (voicesEq(pp->v0, pp->v1) &&
	    voicesEq(pp->v0, pp->nxt->v0) &&
	    voicesEq(pp->v0, pp->nxt->v1))
	  pp->nxt->tim= pp->tim;

	if (pp->tim == pp->nxt->tim) {
	  if (per == pp) per= per->prv;
	  pp->prv->nxt= pp->nxt;
	  pp->nxt->prv= pp->prv;
	  free(pp);
	  pp= 0;
	  break;
	}
	pp= pp->nxt;
      } while (pp != per);
      if (pp) break;
    }
  }

  // Make sure that the total is 24 hours only (not more !)
  if (per->nxt != per) {
    int tot= 0;
    Period *pp= per;
    
    do {
      tot += t_per0(pp->tim, pp->nxt->tim);
      pp= pp->nxt;
    } while (pp != per);

    if (tot > H24) {
      warn("Total time is greater than 24 hours.  Probably two times are\n"
	   "out of order.  Suspicious intervals are:\n");
      pp= per;
      do {
	if (t_per0(pp->tim, pp->nxt->tim) >= H12) 
	   warn("  %02d:%02d:%02d -> %02d:%02d:%02d",
		pp->tim % 86400000 / 3600000,
		pp->tim % 3600000 / 60000,
		pp->tim % 60000 / 1000,
		pp->nxt->tim % 86400000 / 3600000,
		pp->nxt->tim % 3600000 / 60000,
		pp->nxt->tim % 60000 / 1000);
	pp= pp->nxt;
      } while (pp != per);
      error("\nCheck the sequence around these times and try again");
    }
  }

  // Print the whole lot out
  if (opt_D) {
    Period *pp;
    if (per->nxt != per)
      while (per->prv->tim < per->tim) per= per->nxt;

    pp= per;

    if(!opt_Q) {
      printSequenceDuration();
    }

    do {
      dispCurrPer(stdout);
      per= per->nxt;
    } while (per != pp);
    printf("\n");

    exit(0);		// All done
  }  
}

int 
voicesEq(Voice *v0, Voice *v1) {
  int a= N_CH;

  while (a-- > 0) {
    if (v0->typ != v1->typ) return 0;
    switch (v0->typ) {
     case 1:
     case 4:
     default:
       if (v0->amp != v1->amp ||
	   v0->carr != v1->carr ||
	   v0->res != v1->res)
	 return 0;
       break;
     case 2:
     case 5:
       if (v0->amp != v1->amp)
	 return 0;
       break;
     case 3:
       if (v0->amp != v1->amp ||
	   v0->carr != v1->carr)
	 return 0;
       break;
    }
    v0++; v1++;
  }
  return 1;
}

//
//	Read a name definition
//

void 
readNameDef() {
  char *p, *q;
  NameDef *nd;
  int ch;

  if (!(p= getWord())) badSeq();

  q= strchr(p, 0) - 1;
  if (*q != ':') badSeq();
  *q= 0;
  for (q= p; *q; q++) if (!isalnum(*q) && *q != '-' && *q != '_') 
    error("Bad name \"%s\" in definition, line %d:\n  %s", p, in_lin, lin_copy);

  // Waveform definition ?
  if (0 == memcmp(p, "wave", 4) && 
      isdigit(p[4]) &&
      isdigit(p[5]) &&
      !p[6]) {
     int ii= (p[4] - '0') * 10 + (p[5] - '0');
     int siz= ST_SIZ * sizeof(int);
     int *arr= (int*)Alloc(siz);
     double *dp0= (double*)arr;
     double *dp1= (double*)(siz + (char*)arr);
     double *dp= dp0;
     double dmax= 0, dmin= 1;
     int np;

     if (waves[ii])
	error("Waveform %02d already defined, line %d:\n  %s",
	      ii, in_lin, lin_copy);
     waves[ii]= arr;
     
     while ((p= getWord())) {
	double dd;
	char dmy;
	if (1 != sscanf(p, "%lf %c", &dd, &dmy)) 
	   error("Expecting floating-point numbers on this waveform "
		 "definition line, line %d:\n  %s",
		 in_lin, lin_copy);
	if (dp >= dp1)
	   error("Too many samples on line (maximum %d), line %d:\n  %s",
		 dp1-dp0, in_lin, lin_copy);
	*dp++= dd;
	if (dmax < dmin) dmin= dmax= dd;
	else {
	   if (dd > dmax) dmax= dd;
	   if (dd < dmin) dmin= dd;
	}
     }
     dp1= dp;
     np= dp1 - dp0;
     if (np < 2) 
	error("Expecting at least two samples in the waveform, line %d:\n  %s",
	      in_lin, lin_copy);

     // Adjust to range 0-1
     for (dp= dp0; dp < dp1; dp++)
	*dp= (*dp - dmin) / (dmax - dmin);

     sinc_interpolate(dp0, np, arr);
     
     if (DEBUG_DUMP_WAVES) {
	int a;
	printf("Dumping wave%02d:\n", ii);
	for (a= 0; a<ST_SIZ; a++)
	   printf("%d %g\n", a, arr[a] * 1.0 / ST_AMP);
     }
     return;
  } 

  // Must be block or tone-set, then, so put into a NameDef
  nd= (NameDef*)Alloc(sizeof(NameDef));
  nd->name= StrDup(p);

  // Block definition ?
  if (*lin == '{') {
    BlockDef *bd, **prvp;
    if (!(p= getWord()) || 
	0 != strcmp(p, "{") || 
	0 != (p= getWord()))
      badSeq();

    prvp= &nd->blk;
    
    while (readLine()) {
      if (*lin == '}') {
	if (!(p= getWord()) || 
	    0 != strcmp(p, "}") || 
	    0 != (p= getWord()))
	  badSeq();
	if (!nd->blk) error("Empty blocks not permitted, line %d:\n  %s", in_lin, lin_copy);
	nd->nxt= nlist; nlist= nd;
	return;
      }
      
      if (*lin != '+') 
	error("All lines in the block must have relative time, line %d:\n  %s",
	      in_lin, lin_copy);
      
      bd= (BlockDef*) Alloc(sizeof(*bd));
      *prvp= bd; prvp= &bd->nxt;
      bd->lin= StrDup(lin);
    }
    
    // Hit EOF before }
    error("End-of-file within block definition (missing '}')");
  }

  // Normal line-definition
  for (ch= 0; ch < N_CH && (p= getWord()); ch++) {
    char dmy;
    double amp, carr, res;
    int wave;

    // Interpret word into Voice nd->vv[ch]
    if (0 == strcmp(p, "-")) continue;
    if (1 == sscanf(p, "pink/%lf %c", &amp, &dmy)) {
       nd->vv[ch].typ= 2;
       nd->vv[ch].amp= AMP_DA(amp);
       continue;
    }
    if (1 == sscanf(p, "white/%lf %c", &amp, &dmy)) {
       nd->vv[ch].typ= 9;
       nd->vv[ch].amp= AMP_DA(amp);
       continue;
    }
    if (1 == sscanf(p, "brown/%lf %c", &amp, &dmy)) {
       nd->vv[ch].typ= 10;
       nd->vv[ch].amp= AMP_DA(amp);
       continue;
    }
    if (2 == sscanf(p, "bell%lf/%lf %c", &carr, &amp, &dmy)) {
       nd->vv[ch].typ= 3;
       nd->vv[ch].carr= carr;
       nd->vv[ch].amp= AMP_DA(amp);
       continue;
    }
    if (1 == sscanf(p, "mix/%lf %c", &amp, &dmy)) {
       nd->vv[ch].typ= 5;
       nd->vv[ch].amp= AMP_DA(amp);
       mix_flag= 1;
       continue;
    }
    if (4 == sscanf(p, "wave%d:%lf%lf/%lf %c", &wave, &carr, &res, &amp, &dmy)) {
       if (wave < 0 || wave >= 100)
	  error("Only wave00 to wave99 is permitted at line: %d\n  %s", in_lin, lin_copy);
       if (!waves[wave])
	  error("Waveform %02d has not been defined, line: %d\n  %s", wave, in_lin, lin_copy);
       nd->vv[ch].typ= -1-wave;
       nd->vv[ch].carr= carr;
       nd->vv[ch].res= res;
       nd->vv[ch].amp= AMP_DA(amp);	
       continue;
    }
    if (3 == sscanf(p, "%lf@%lf/%lf %c", &carr, &res, &amp, &dmy)) {
       nd->vv[ch].typ= 8;  // Isochronic
       nd->vv[ch].carr= carr;
       nd->vv[ch].res= res;
       nd->vv[ch].amp= AMP_DA(amp);	
       continue;
    }
    if (3 == sscanf(p, "%lf%lf/%lf %c", &carr, &res, &amp, &dmy)) {
      nd->vv[ch].typ= 1;
      nd->vv[ch].carr= carr;
      nd->vv[ch].res= res;
      nd->vv[ch].amp= AMP_DA(amp);	
      continue;
    }
    if (2 == sscanf(p, "%lf/%lf %c", &carr, &amp, &dmy)) {
      nd->vv[ch].typ= 1;
      nd->vv[ch].carr= carr;
      nd->vv[ch].res= 0;
      nd->vv[ch].amp= AMP_DA(amp);	
      continue;
    }
    if (3 == sscanf(p, "spin:%lf%lf/%lf %c", &carr, &res, &amp, &dmy)) {
      nd->vv[ch].typ= 4;
      nd->vv[ch].carr= carr;
      nd->vv[ch].res= res;
      nd->vv[ch].amp= AMP_DA(amp);	
      continue;
    }
    if (3 == sscanf(p, "mixspin:%lf%lf/%lf %c", &carr, &res, &amp, &dmy)) {
      checkMixInSequence();
      nd->vv[ch].typ= 6;
      nd->vv[ch].carr= carr;
      nd->vv[ch].res= res;
      nd->vv[ch].amp= AMP_DA(amp);	
      continue;
    }
    if (2 == sscanf(p, "mixpulse:%lf/%lf %c", &res, &amp, &dmy)) {
      checkMixInSequence();
      nd->vv[ch].typ= 7;
      nd->vv[ch].res= res;
      nd->vv[ch].amp= AMP_DA(amp);	
      continue;
    }
    badSeq();
  }
  
  // Validate total amplitude before adding to the list
  validateTotalAmplitude(nd->vv, N_CH, lin_copy, in_lin);
  
  nd->nxt= nlist; nlist= nd;
}  

//
//	Bad time
//

void 
badTime(char *tim) {
  error("Badly constructed time \"%s\", line %d:\n  %s", tim, in_lin, lin_copy);
}

//
//	Read a time-line of either type
//

void 
readTimeLine() {
  char *p, *tim_p;
  int nn;
  int fo, fi;
  Period *pp;
  NameDef *nd;
  static int last_abs_time= -1;
  int tim, rtim = 0;

  if (!(p= getWord())) badSeq();
  tim_p= p;
  
  // Read the time represented
  tim= -1;
  if (0 == memcmp(p, "NOW", 3)) {
    last_abs_time= tim= now;
    p += 3;
  }

  while (*p) {
    if (*p == '+') {
      if (tim < 0) {
	if (last_abs_time < 0) 
	  error("Relative time without previous absolute time, line %d:\n  %s", in_lin, lin_copy);
	tim= last_abs_time;
      }
      p++;
    }
    else if (tim != -1) badTime(tim_p);

    if (0 == (nn= readTime(p, &rtim))) badTime(tim_p);
    p += nn;

    if (tim == -1) 
      last_abs_time= tim= rtim;
    else 
      tim= (tim + rtim) % H24;
  }

  if (fast_tim0 < 0) fast_tim0= tim;		// First time
  fast_tim1= tim;				// Last time
      
  if (!(p= getWord())) badSeq();
      
  fi= fo= 1;
  if (!isalpha(*p)) {
    switch (p[0]) {
     case '<': fi= 0; break;
     case '-': fi= 1; break;
     case '=': fi= 2; break;
     default: badSeq();
    }
    switch (p[1]) {
     case '>': fo= 0; break;
     case '-': fo= 1; break;
     case '=': fo= 2; break;
     default: badSeq();
    }
    if (p[2]) badSeq();

    if (!(p= getWord())) badSeq();
  }
      
  for (nd= nlist; nd && 0 != strcmp(p, nd->name); nd= nd->nxt) ;
  if (!nd) error("Name \"%s\" not defined, line %d:\n  %s", p, in_lin, lin_copy);

  // Check for block name-def
  if (nd->blk) {
    char *prep= StrDup(tim_p);		// Put this at the start of each line
    BlockDef *bd= nd->blk;

    while (bd) {
      lin= buf; lin_copy= buf_copy;
      sprintf(lin, "%s%s", prep, bd->lin);
      strcpy(lin_copy, lin);
      readTimeLine();		// This may recurse, and that's why we're StrDuping the string
      bd= bd->nxt;
    }
    free(prep);
    return;
  }
      
  // Normal name-def
  pp= (Period*)Alloc(sizeof(*pp));
  pp->tim= tim;
  pp->fi= fi;
  pp->fo= fo;
      
  memcpy(pp->v0, nd->vv, N_CH * sizeof(Voice));
  memcpy(pp->v1, nd->vv, N_CH * sizeof(Voice));

  if (!per)
    per= pp->nxt= pp->prv= pp;
  else {
    pp->nxt= per; pp->prv= per->prv;
    pp->prv->nxt= pp->nxt->prv= pp;
  }

  // Automatically add a transitional period
  pp= (Period*)Alloc(sizeof(*pp));
  pp->fi= -2;		// Unspecified transition
  pp->nxt= per; pp->prv= per->prv;
  pp->prv->nxt= pp->nxt->prv= pp;

  if (0 != (p= getWord())) {
    if (0 != strcmp(p, "->")) badSeq();
    pp->fi= -3;		// Special '->' transition
    pp->tim= tim;
  }
}

int
readTime(char *p, int *timp) {		// Rets chars consumed, or 0 error
  int nn, hh, mm, ss;

  if (3 > sscanf(p, "%2d:%2d:%2d%n", &hh, &mm, &ss, &nn)) {
    ss= 0;
    if (2 > sscanf(p, "%2d:%2d%n", &hh, &mm, &nn)) return 0;
  }

  if (hh < 0 || hh >= 24 ||
      mm < 0 || mm >= 60 ||
      ss < 0 || ss >= 60) return 0;

  *timp= ((hh * 60 + mm) * 60 + ss) * 1000;
  return nn;
}

//
//	Takes a set of points and repeats them twice, inverting the
//	second set, and then interpolates them using a periodic sinc
//	function (see http://www-ccrma.stanford.edu/~jos/resample/)
//	and writes them to arr[] in the same format as the sin_table[].
//

void sinc_interpolate(double *dp, int np, int *arr) {
   double *sinc;	// Temporary sinc-table
   double *out;		// Temporary output table
   int a, b;
   double dmax, dmin;
   double adj, off;

   // Generate a modified periodic sin(x)/x function to be used for
   // each of the points.  Really this should be sin(x)/x modified
   // by the sum of an endless series.  However, this doesn't
   // converge very quickly, so to save time I'm approximating this
   // series by 1-4*t*t where t ranges from 0 to 0.5 over the first
   // half of the periodic cycle.  If you do the maths, this is at
   // most 5% out.  This will have to do - it's smooth, and I don't
   // know enough maths to make this series converge quicker.
   sinc= (double *)Alloc(ST_SIZ * sizeof(double));
   sinc[0]= 1.0;
   for (a= ST_SIZ/2; a>0; a--) {
      double tt= a * 1.0 / ST_SIZ;
      double t2= tt*tt;
      double adj= 1 - 4 * t2;
      double xx= 2 * np * 3.14159265358979323846 * tt;
      double vv= adj * sin(xx) / xx;
      sinc[a]= vv;
      sinc[ST_SIZ-a]= vv;
   }
   
   // Build waveform into buffer
   out= (double *)Alloc(ST_SIZ * sizeof(double));
   for (b= 0; b<np; b++) {
      int off= b * ST_SIZ / np / 2;
      double val= dp[b];
      for (a= 0; a<ST_SIZ; a++) {
	 out[(a + off)&(ST_SIZ-1)] += sinc[a] * val;
	 out[(a + off + ST_SIZ/2)&(ST_SIZ-1)] -= sinc[a] * val;
      }
   }

   // Look for maximum for normalization
   dmax= dmin= 0;
   for (a= 0; a<ST_SIZ; a++) {
      if (out[a] > dmax) dmax= out[a];
      if (out[a] < dmin) dmin= out[a];
   }

   // Write out to output buffer
   off= -0.5 * (dmax + dmin);
   adj= ST_AMP / ((dmax - dmin) / 2);
   for (a= 0; a<ST_SIZ; a++)
      arr[a]= (int)((out[a] + off) * adj);

   free(sinc);
   free(out);
}

//
//	Handling pre-programmed sequences
//

void 
readPreProg(int ac, char **av) {
   if (ac < 1) 
      error("Expecting a pre-programmed sequence description.  Examples:" 
	    NL "  drop 25ds+ pink/30"
	    NL "  drop 25gs+/2 mix/60"
	    );
   
   // Handle 'drop'
   if (0 == strcmp(av[0], "drop")) {
      ac--; av++;
      create_drop(ac, av);
      return;
   }

   // Handle 'slide'
   if (0 == strcmp(av[0], "slide")) {
      ac--; av++;
      create_slide(ac, av);
      return;
   }

   error("Unknown pre-programmed sequence type: %s", av[0]);
}

//
//	Error for bad p-drop args
//

void 
bad_drop() {
   error("Bad arguments: expecting -p drop [<time-spec>] <drop-spec> [<tone-specs...>]"
	 NL "<drop-spec> is <digit><digit>[.<digit>...]<a-l>[s|k][+][^][/<amp>]"
	 NL "The optional <time-spec> is t<drop-time>,<hold-time>,<wake-time>, all times"
	 NL "  in minutes (the default is equivalent to 't30,30,3')."
	 NL "The optional <tone-specs...> let you mix other stuff with the drop"
	 NL "  sequence like pink noise or a mix soundtrack, e.g 'pink/20' or 'mix/60'");
}

//
//	Generate a p-drop sequence
//
//	Credits: Jonathan Bisson created the first version of this C
//	code.  This is a rewrite to make it fit with the rest of the
//	code better.
//

void 
create_drop(int ac, char **av) {
   char *fmt;
   char *p, *q;
   char signal;
   int a;
   int slide, n_step, islong, wakeup, isisochronic;
   double carr, amp, c0, c2;
   double beat_target;
   double beat[40];
   static double beat_targets[]= { 
      4.4, 3.7, 3.1, 2.5, 2.0, 1.5, 1.2, 0.9, 0.7, 0.5, 0.4, 0.3
   };
   char extra[256];
   int len, len0= 1800, len1= 1800, len2= 180;
   int steplen, end;
   
#define BAD bad_drop()

   // Pick up optional time-spec
   if (ac < 1) BAD;
   if (av[0][0] == 't') {
      double v0, v1, v2;
      char dmy;
      if (3 != sscanf(av[0]+1, "%lf,%lf,%lf %c", &v0, &v1, &v2, &dmy)) BAD;
      len0= 60 * (int)v0;	// Whole minutes only
      len1= 60 * (int)v1;
      len2= 60 * (int)v2;
      ac--; av++;
   }

   // Handle argument list
   if (ac < 1) BAD;
   fmt= *av++; ac--;
   p= extra; *p= 0;
   while (ac > 0) {
      if (p + strlen(av[0]) + 2 > extra + sizeof(extra))
	 error("Too many extra tone-specs after -p drop");
      p += sprintf(p, " %s", av[0]);
      ac--; av++;
   }
   
   // Scan the format
   carr= 200 - 2 * strtod(fmt, &p);
   if (p == fmt || carr < 0) BAD;

   a= tolower(*p) - 'a'; p++;
   if (a < 0 || a >= sizeof(beat_targets) / sizeof(beat_targets[0])) BAD;
   beat_target= beat_targets[a];

   slide= 0;
   steplen= 180;
   if (*p == 's') { p++; slide= 1; steplen= 60; }
   else if (*p == 'k') { p++; steplen= 60; }
   n_step= 1 + (len0-1) / steplen;	// Round up
   len0= n_step * steplen;
   if (!slide) len1= (1 + (len1-1) / steplen) * steplen;

   islong= 0;
   if (*p == '+') { islong= 1; p++; }

   wakeup= 0;
   if (*p == '^') { wakeup= 1; p++; }

   isisochronic= 0;
   if (*p == '@') { isisochronic= 1; p++; }

   amp= 1.0;
   if (*p == '/') {
      p++; q= p;
      amp= strtod(p, &p);
      if (p == q) BAD;
   }

   while (isspace(*p)) p++;
   if (*p) error("Trailing rubbish after -p drop spec: \"%s\"", p);

#undef BAD
      
   // Sort out carriers
   len= islong ? len0 + len1 : len0;
   c0= carr + 5.0;
   c2= carr;

   // Calculate beats
   for (a= 0; a<n_step; a++)
      beat[a]= 10 * exp(log(beat_target/10) * a / (n_step-1));

   // Display summary
   warn("DROP summary:");
   if (slide) {
      warn(" Carrier slides from %gHz to %gHz over %d minutes", 
	   c0, c2, len/60);
      warn(" %s frequency slides from %gHz to %gHz over %d minutes", 
	   isisochronic ? "Pulse" : "Beat", beat[0], beat[n_step-1], len0/60);
   } else {
      warn(" Carrier steps from %gHz to %gHz over %d minutes", 
	   c0, c2, len/60);
      warn(" %s frequency steps from %gHz to %gHz over %d minutes:", 
	   isisochronic ? "Pulse" : "Beat", beat[0], beat[n_step-1], len0/60);
      fprintf(stderr, "   ");
      for (a= 0; a<n_step; a++) fprintf(stderr, " %.2f", beat[a]);
      fprintf(stderr, "\n");
   }
   if (wakeup) {
      warn(" Final wake-up of %d minutes, to return to initial frequencies", len2/60);
   }

   // Start generating sequence
   handleOptions("-SE");
   in_lin= 0;

   formatNameDef("off: -");
   formatTimeLine(86395, "== off ->");		// 23:59:55
   
   signal= isisochronic ? '@' : '+';
   if (slide) {
      // Slide version
      for (a= 0; a<n_step; a++) {
	 int tim= a * len0 / (n_step-1);
	 formatNameDef("ts%02d: %g%c%g/%g %s", a, 
		       c0 + (c2-c0) * tim * 1.0 / len,
		       signal, beat[a], amp, extra);
	 formatTimeLine(tim, "== ts%02d ->", a);
      }

      if (islong) {
	 formatNameDef("tsend: %g%c%g/%g %s",
		       c2, signal, beat[n_step-1], amp, extra);
	 formatTimeLine(len, "== tsend ->");
      }
      end= len;
   } else {
      // Step version
      int lim= len / steplen;
      int stepslide= steplen < 90 ? 5 : 10;    // Seconds slide between steps
      for (a= 0; a<lim; a++) {
	 int tim0= a * steplen;
	 int tim1= (a+1) * steplen;
	 formatNameDef("ts%02d: %g%c%g/%g %s", a,
		       c0 + (c2-c0) * tim1/len,
		       signal, beat[(a>=n_step) ? n_step-1 : a], 
		       amp, extra);
	 formatTimeLine(tim0, "== ts%02d ->", a);
	 formatTimeLine(tim1-stepslide, "== ts%02d ->", a);
      }
      end= len-stepslide;
   }
   
   // Wake-up and ending
   if (wakeup) {
      formatNameDef("tswake: %g%c%g/%g %s",
		    c0, signal, beat[0], amp, extra);
      formatTimeLine(end+len2, "== tswake ->");
      end += len2;
   } 
   formatTimeLine(end+10, "== off");

   correctPeriods();
}

//
//	Generate a -p slide sequence
//
//	The idea of this is to hold the beat frequency constant, but
//	to slide down through the carrier frequencies from about 200Hz.
//
//	-p slide [t<duration-minutes>] <carr>+<beat>/<amp> [extra tone-sets]

void 
bad_slide() {
   error("Bad arguments: expecting -p slide [<time-spec>] <slide-spec> [<tone-specs...>]"
	 NL "<slide-spec> is just like a tone-spec: <carrier><sign><beat>/<amp>"
	 NL "The optional <time-spec> is t<slide-time>, giving length of session in"
	 NL "  minutes (the default is equivalent to 't30')."
	 NL "The optional <tone-specs...> let you mix other stuff with the drop"
	 NL "  sequence like white/pink/brown noise or a mix soundtrack, e.g 'pink/20', 'white/20', 'brown/20', or 'mix/60'");
}

void 
create_slide(int ac, char **av) {
   int len= 1800;
   char *p, dmy, signal;
   double val, c0, c1, beat, amp;
   char extra[256];

#define BAD bad_slide()

   // Handle arguments
   if (ac < 1) BAD;
   if (av[0][0] == 't') {
      val= strtod(av[0]+1, &p);
      if (p == av[0] + 1 || *p) BAD;
      len= 60.0 * val;
      ac--; av++;
   }

   if (ac < 1) BAD;
   if (4 != sscanf(av[0], "%lf%c%lf/%lf %c", &c0, &signal, &beat, &amp, &dmy)) BAD;

   if (signal != '+' && signal != '-' && signal != '@') BAD;

   c1= beat/2;
   ac--; av++;

#undef BAD

   // Gather 'extra'
   p= extra; *p= 0;
   while (ac > 0) {
      if (p + strlen(av[0]) + 2 > extra + sizeof(extra))
	 error("Too many extra tone-specs after -p slide");
      p += sprintf(p, " %s", av[0]);
      ac--; av++;
   }

   // Summary
   warn("SLIDE summary:");
   warn(" Sliding carrier from %gHz to %gHz over %g minutes",
	c0, c1, len/60.0);
   warn(" Holding %s constant at %gHz", signal == '@' ? "pulse" : "beat", beat);

   // Generate sequence
   handleOptions("-SE");
   in_lin= 0;

   formatNameDef("off: -");
   formatTimeLine(86395, "== off ->");		// 23:59:55
   formatNameDef("ts0: %g%c%g/%g %s", c0, signal, beat, amp, extra);
   formatTimeLine(0, "== ts0 ->");
   formatNameDef("ts1: %g%c%g/%g %s", c1, signal, beat, amp, extra);
   formatTimeLine(len, "== ts1 ->");
   formatTimeLine(len+10, "== off");
   
   correctPeriods();
}   

// Function to validate the total amplitude of voices
void validateTotalAmplitude(Voice *voices, int numChannels, const char *line, int lineNum) {
  double totalAmplitude = 0.0;
  
  for (int ch = 0; ch < numChannels; ch++) {
    // Check if voice is mixspin or mixpulse to check if effect intensity is over 100%
    if (voices[ch].typ == 6 || voices[ch].typ == 7) {
      double ampPercentage = voices[ch].amp / 40.96;

      if (ampPercentage > 100.0) {
        error("Total intensity of mixspin/mixpulse exceeds 100%% (%.2f%%) at line %d:\n  %s\nPlease reduce intensity to prevent audio distortion.", 
              ampPercentage, lineNum, line);
      }
    }

    if (voices[ch].typ != 0 && voices[ch].typ != 6 && voices[ch].typ != 7) { // If voice is active. Mixspin and Mixpulse are not included
      // Convert from internal amplitude format back to percentage
      double ampPercentage = voices[ch].amp / 40.96;
      totalAmplitude += ampPercentage;
    }
  }
  
  if (totalAmplitude > 100.0) {
    error("Total amplitude exceeds 100%% (%.2f%%) at line %d:\n  %s\nPlease reduce amplitudes to prevent audio distortion.", 
          totalAmplitude, lineNum, line);
  }
}

void printSequenceDuration() {
  int duration = t_per0(fast_tim0, fast_tim1);
  fprintf(stdout, "\n*** Sequence duration: %02d:%02d:%02d (hh:mm:ss) ***\n\n", 
          duration/3600000, (duration/60000)%60, (duration/1000)%60);
}

void checkMixInSequence() {
  char *ln= lin_copy;
  int mix_exists= 0;

  while((ln= strstr(ln, "mix/")) != NULL) {
    if(isdigit(ln[4])) {
      mix_exists= 1;
      break;
    }
    ln++;
  }

  if(!mix_exists)
    error("mixspin/mixpulse without mix/<amp> specified, line %d:\n  %s", in_lin, lin_copy);
}

// END //
