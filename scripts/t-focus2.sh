#!/bin/sh

lev="$1";
shift;
pre="$*";

case "$lev" in 
3)   set  50+1.2/40  100+1.3/20  288+3.6/7;;
10)  set 100+1.5/20  200+4.0/10  250+4.0/8  300+4.0/7;;
12)  set 50+0.25/40  100+1.5/20  200+4.0/10 250+4.0/8  300+4.0/7  400+10.0/5 500+10.1/4 600+4.8/3;;
15)  set 100+1.5/20  200+4.0/10  250+4.0/8  300+4.0/7  500+7.05/4 630+7.0/3  750+7.0/3;;
21a) set 200+4.0/10  250+4.0/8   300+4.0/7  600+16.2/3 750+16.2/3 900+16.2/2;;
21b) set 200+4.0/10  250+5.4/8   300+5.4/7  600+16.2/3 750+16.2/3 900+16.2/2;;
22)  set 200+4.0/10  250+4.0/8   300+4.0/7  600+16.2/3 750+16.2/3 900+16.2/2;;
23)  set 400+3.9/5   503+4.0/4   600+4.0/3  750+3.9/3  900+4.0/2;;
24)  set 50+0.75/40  200+1.5/10  400+3.9/5  503+4.0/4  600+4.0/3  750+4.0/3  900+4.0/2;;
26)  set 400+3.9/5   503+4.2/4   600+4.0/3  750+4.0/3  900+4.0/2;;
27)  set 50+0.80/40  400+4.0/5   503+4.2/4  600+4.0/3  750+4.0/3  900+4.0/2;;
*)
	cat <<END
Usage: t-focus <focus-level> <additional-sbagen-options-if-required>

Focus levels available, with meaning from Ken E-F, if he mentioned it:
  3	
 10	Brain awake, body asleep
 12	Expanded consciousness - in the mood to head out and explore
 15	No time - 3 hours seems like 2 minutes
 21a	}  Alternate energy systems, bridge into worlds with 
 21b	}    non-human entities.
 22	Place where people with loose, chaotic thoughts go after death.
 23	
 24	Belief system territories - place where dogmatically inclined people
          go after death.
 26
 27	The park - a way-station, a place of high creativity, a stepping stone
          into areas beyond

Note that the focus level tone frequencies and beat-frequencies were
measured, but the relative amplitudes of the tones were not measured,
and may not be correct.  The original tone-sets almost certainly had
some tones stronger than others, but with this script, all have been
given approximately equal audible presence.  This means that these
tone-sets might not work at all like the original ones.  But still,
for experimentation, this is a lot better than nothing, considering
the cost of obtaining Monroe Institute courses !

END
	exit 1;
esac

echo "Focus $lev:"
echo ""
echo sbagen $pre -i $*
echo ""

sbagen $pre -i $*





############


-----------------------------------------------------------------------
Wave I - Discovery
-----------------------------------------------------------------------

Tape 1 - Orientation (Focus 3)
Hemisync, Energy Conversion Box, Gateway Affirmation, Resonant Tuning

l/r 300/304 plus 100 Hz and 500 Hz carriers without recognizeable Delta

### 100+0/ 302+4/ 500+0/

-----------------------------------------------------------------------

Tape 2 - Intro Focus 10

Focus 10, Ten Point Relaxation

### 100+4.0/ 300+4.0/ 500+4.0/

-----------------------------------------------------------------------

Tape 3 - Advanced Focus 10
Recall

-----------------------------------------------------------------------

Tape 4 - Release and Recharge (Focus 10)
Process of Energy Conversion - Daily Exercise!

### 102+4/ 412-4/ 502-4/

-----------------------------------------------------------------------

Tape 5 - Exploration Sleep (Focus 10)
Remembering who and what you are, Rolling out, Floating upwards

### 102+4 302+4 498+4

-----------------------------------------------------------------------

Tape 6 - Focus 10 Free Flow for your own purpose

l/r 100/104, 300/304, 500/504
C1: 308/322, 500/515

### 102+4 302+4 502+4
### 315+14 507.5+15

-----------------------------------------------------------------------
Wave II - Threshold
-----------------------------------------------------------------------

Tape 1 - Intro Focus 12

l/r 51/51.75, 98.75/101; Delta-Frq: 0.75 + 2.25 Hz

### 51.375+0.75 99.875+2.25

Tape 2 - Problem Solving (Focus 12)

l/r 99.5/101, 202.7/204.2

### 100.25+1.5 203.45+1.5

-----------------------------------------------------------------------
                   Focus Levels/Frequencies

F3   59[1.2]-110[1.3], 288[3.7]

F10  100[1.5], 200[4.0], 250[4.0], 300[4.0]

F12  100[1.5], 200[4.0], 250[4.0], 300[4.0], 400[10.0], 500[10.1], 600[4.8]

F15  100[1.5], 200[4.0], 250[4.0], 300[4.0], 500[7.05], 630[7.1], 750[7.0]

F21  200[4.0], 250[4.0], 300[4.0], 600[16.2], 750[15.9], 900[16.2]

F22  Same as F21

F23  400[3.9], 503[4.0], 600[4.0], 750[3.9], 900[4.0]

F24  50[0.75], 400[3.9], 503[4.0], 600[4.0], 750[4.0], 900[4.0]

F25  503[4.0], 600[4.0], 750[4.0], 900[4.0]

F26  400[3.9], 503[4.2], 600[4.0], 750[4.0], 900[4.0]

F27  Same as F26

Example: The notation 503[4.2] should be understood to
represent a beat frequency of 4.2 Hz between two carriers
equally separated from 503 Hz (i.e., 500.9 and 505.1 Hz).

Amplitudes are generally maintained about 20 dB above
background 1/f (pink) noise and/or surf.

-----------------------------------------------------------------------

 L  58.4, 109.35, 286.15
F3
 R  59.6, 110.65, 289.85

 L   99.25, 198, 248, 298
F10
 R  100.75, 202, 252, 302

 L   99.25, 198, 248, 298, 395, 494.95, 597.6
F12
 R   100.75, 202, 252, 302, 405, 505.05, 602.4

 L   99.25, 198, 248, 298, 496.475, 626.45, 746.5
F15
 R   100.75, 202, 252, 302, 503.525, 633.55, 753.5
 
 L   198, 248, 298, 591.9, 742.05, 891.9
F21-F22
 R   202, 252, 302, 608.1, 757.95, 908.1

 L   398.05, 501, 598, 748.05, 898
 F23
 R   401.95, 505, 602, 751.95, 902

 L   49.625, 398.05, 501, 598, 748, 898
 F24
 R   50.375, 401.95, 505, 602, 752, 902

 L   501, 598, 748, 898
 F25
 R   505, 602, 752, 902

 L   398.05, 500.9, 598, 748, 898
 F26-F27
 R   401.95, 505.1, 602, 752, 902



