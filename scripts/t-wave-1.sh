#!/bin/sh

cat <<EOF
Usage: t-wave-1 <additional-sbagen-options-if-required>

Please note: These tone-sets are based on data from a document that
appeared on USENET, from someone who appears to have measured the
frequencies used on some of the Monroe Institute tapes.  Although the
frequencies were measured, the relative amplitudes of the tones were
not, so the results may not be identical to that of the Monroe
Institute tapes.  Also I believe that the Monroe Institute uses a
guiding voice that leads the subject through the experience.

Despite that, these tone-sets (or Focus levels) may be useful for
experimenters to base their own tone-sets on.  No attempt has been
made to sequence these tones - use ^C to abort any tone-set and move
onto the next.  Amplitudes of the tones have all been set equal, and
the user can edit these if required.

- Jim Peters <http://uazu.net/>

EOF

echo -n "Press return to continue: "
read xx

#trap 'echo -e "\n**ABORT**\n"' 2 
trap ':' 2 
pre="$*";
pink=''

# Uncomment the following line if you'd like pink noise as a background
#pink='pink/30'

function play {
    cmd="sbagen $pre -Q -i $* $pink"
    echo $cmd
    $cmd
}

cat <<EOF |
-----------------------------------------------------------------------
Wave I - Discovery
-----------------------------------------------------------------------

Tape 1 - Orientation (Focus 3)
Hemisync, Energy Conversion Box, Gateway Affirmation, Resonant Tuning

l/r 300/304 plus 100 Hz and 500 Hz carriers without recognizeable Delta

+++ 100+0/10 302+4/10 500+0/10

-----------------------------------------------------------------------

Tape 2 - Intro Focus 10

Focus 10, Ten Point Relaxation

100[4.0], 300[4.0], 500[4.0]

+++ 100+4.0/10 300+4.0/10 500+4.0/10

-----------------------------------------------------------------------

Tape 3 - Advanced Focus 10
Recall

-----------------------------------------------------------------------

Tape 4 - Release and Recharge (Focus 10)
Process of Energy Conversion - Daily Exercise!

l/r 100/104, 414/410, 504/500

+++ 102+4/10 412-4/10 502-4/10

-----------------------------------------------------------------------

Tape 5 - Exploration Sleep (Focus 10)
Remembering who and what you are, Rolling out, Floating upwards

l/r 100/104, 300/304, 496/500

+++ 102+4/10 302+4/10 498+4/10

-----------------------------------------------------------------------

Tape 6 - Focus 10 Free Flow for your own purpose

l/r 100/104, 300/304, 500/504
C1: 308/322, 500/515

+++ 102+4/10 302+4/10 502+4/10
+++ 315+14/10 507.5+15/10

-----------------------------------------------------------------------
EOF
(
    trap 'echo -en "\a"' 2 
    while read xx
    do
	case "$xx" in
	    +++*)
		play ${xx#+++}
		;;
	    *)
		echo "$xx"
		;;
	esac
    done
)

echo "DONE"
