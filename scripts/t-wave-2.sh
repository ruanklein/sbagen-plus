#!/bin/sh

cat <<EOF
Usage: t-wave-2 <additional-sbagen-options-if-required>

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
Wave II - Threshold
-----------------------------------------------------------------------

Tape 1 - Intro Focus 12

l/r 51/51.75, 98.75/101; Delta-Frq: 0.75 + 2.25 Hz

+++ 51.375+0.75/10 99.875+2.25/10

Tape 2 - Problem Solving (Focus 12)

l/r 99.5/101, 202.7/204.2

+++ 100.25+1.5/10 203.45+1.5/10

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
