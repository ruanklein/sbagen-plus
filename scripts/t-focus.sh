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

