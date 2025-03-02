#!/bin/sh

cat <<EOF
Usage: t-seqall <additional-sbagen-options-if-required>

Please note: These tone-sets are based on data from a document that
appeared on USENET, from someone who appears to have measured the
frequencies used on some of the Monroe Institute tapes.  Although the
frequencies were measured, the relative amplitudes of the tones were
not, so the results may not be identical to that of the Monroe
Institute tapes.  Also I believe that the Monroe Institute uses a
guiding voice that leads the subject through the experience.

Despite that, these tone-sets (or Focus levels) may be useful for
experimenters to base their own tone-sets on.  This experimental
sequence lasts 1 hour 40 minutes, and takes the subject through the
listed tone-sets in order.  The possibilities associated with some of
the focus-levels are from a book by Ken Eagle-Feather, but these are
certainly not the last word on the effects of the focus levels.

EOF

echo -n "Press return to continue: "
read xx

cat <<'EOF'

Don't get disappointed if you don't zip off to another world on the
first attempt !  I think that the tones are intended to point the
direction - to nudge your awareness to the place where these things
are possible.  If you're too inflexible, or if you don't have enough
awareness or energy to see what there is to see when you get there (my
case a lot of the time), you're going to think nothing's happening.
You also don't have the support of the Monroe Institute belief system
when you're experimenting alone like this, and, as I say, we don't
know for certain that these tone-sets accurately reproduce the Monroe
Institute tapes.  But this is certainly fertile raw material to work
with - if you're ready to play, then there's nothing to stop you !

  10 - Brain awake, body asleep
  12 - Expanded awareness, in the mood to head out and explore
  15 - No time, 3 hours seems like 2 minutes
  21 - Alternate energy systems, bridge into worlds with non-human entities
  22 - Place where people with `loose, chaotic thoughts' go after death
  23 - Belief system territories, where dogmatic people go when dead
  24 -
  25 -
  26 -
  27 - The park, a way-station, a place of high creativity, a stepping
	stone to areas beyond.

EOF

echo -n "Press return to continue: "
read xx

pre="$*";
pink=''
#pink="pink/30"		# Uncomment this line to add pink noise

cat <<EOF |
# tN is the tone-set I'm using.  
# tNa is an alternate tone-set for the same focus level
# tNi is a tone-set I'm using as an introduction, to scramble the phases
#
t0: $pink 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10
t0a: $pink 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10
t1: $pink 50+0.25/10 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 400+10.0/10 500+10.1/10 600+4.8/10
t1a: $pink 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 400+10.0/10 500+10.1/10 600+4.8/10
t2: $pink 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 500+7.05/10 630+7.1/10 750+7.0/10
t2a: $pink 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 500+7.05/10 630+7.0/10 750+7.0/10
t3: $pink 200+4.0/10 250+5.4/10 300+5.4/10 600+16.2/10 750+16.2/10 900+16.2/10
t3a: $pink 200+4.0/10 250+4.0/10 300+4.0/10 600+16.2/10 750+15.9/10 900+16.2/10
t4: $pink 200+4.0/10 250+4.0/10 300+4.0/10 600+16.2/10 750+15.9/10 900+16.2/10
t4a: $pink 200+4.0/10 250+4.0/10 300+4.0/10 600+16.2/10 750+16.2/10 900+16.2/10
t5: $pink 400+3.9/10 503+4.0/10 600+4.0/10 750+3.9/10 900+4.0/10
t5a: $pink 400+3.9/10 503+4.0/10 600+4.0/10 750+3.9/10 900+4.0/10
t6: $pink 50+0.75/10 200+1.5/10 400+3.9/10 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10
t6a: $pink 50+0.75/10 400+3.9/10 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10
t7: $pink 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10
t7a: $pink 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10
t8: $pink 400+3.9/10 503+4.2/10 600+4.0/10 750+4.0/10 900+4.0/10
t8a: $pink 400+3.9/10 503+4.2/10 600+4.0/10 750+4.0/10 900+4.0/10
t9: $pink 50+0.80/10 400+4.0/10 503+4.2/10 600+4.0/10 750+4.0/10 900+4.0/10
t9a: $pink 400+3.9/10 503+4.2/10 600+4.0/10 750+4.0/10 900+4.0/10

t0i: $pink 100+1.5/0 200+4.0/0 250+4.03/0 300+4.07/0
t1i: $pink 50+0.25/0 100+1.5/0 200+4.0/0 250+4.03/0 300+4.07/0 400+10.0/0 500+10.1/0 600+4.8/0
t2i: $pink 100+1.5/0 200+4.0/0 250+4.03/0 300+4.07/0 500+7.05/0 630+7.1/0 750+7.0/0
t3i: $pink 200+4.0/0 250+5.4/0 300+5.45/0 600+16.2/0 750+16.23/0 900+16.27/0
t4i: $pink 200+4.0/0 250+4.03/0 300+4.07/0 600+16.2/0 750+15.9/0 900+16.25/0
t5i: $pink 400+3.9/0 503+4.0/0 600+4.03/0 750+3.95/0 900+4.07/0
t6i: $pink 50+0.75/0 200+1.5/0 400+3.9/0 503+4.0/0 600+4.03/0 750+4.05/0 900+4.08/0
t7i: $pink 503+4.0/0 600+4.03/0 750+4.05/0 900+4.08/0
t8i: $pink 400+3.9/0 503+4.2/0 600+4.0/0 750+4.03/0 900+4.07/0
t9i: $pink 50+0.80/0 400+4.0/0 503+4.2/0 600+4.03/0 750+4.05/0 900+4.08/0
off: -

0:00:00 t0i ->
0:00:20 t0 ->
0:09:50 t0 ->
0:09:59 off

0:10:00 t1i ->
0:10:20 t1 ->
0:19:50 t1 ->
0:19:59 off

0:20:00 t2i ->
0:20:20 t2 ->
0:29:50 t2 ->
0:29:59 off

0:30:00 t3i ->
0:30:20 t3 ->
0:39:50 t3 ->
0:39:59 off

0:40:00 t4i ->
0:40:20 t4 ->
0:49:50 t4 ->
0:49:59 off

0:50:00 t5i ->
0:50:20 t5 ->
0:59:50 t5 ->
0:59:59 off

1:00:00 t6i ->
1:00:20 t6 ->
1:09:50 t6 ->
1:09:59 off

1:10:00 t7i ->
1:10:20 t7 ->
1:19:50 t7 ->
1:19:59 off

1:20:00 t8i ->
1:20:20 t8 ->
1:29:50 t8 ->
1:29:59 off

1:30:00 t9i ->
1:30:20 t9 ->
1:39:50 t9 ->
1:39:59 off

EOF
sbagen $pre -S -E -

