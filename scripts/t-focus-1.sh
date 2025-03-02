#!/bin/sh

cat <<EOF
Usage: t-focus-1 <additional-sbagen-options-if-required>

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
                   Focus Levels/Frequencies

F3   59[1.2]-110[1.3], 288[3.7]

+++ 59+1.2/10 110+1.3/10 288+3.7/10

F10  100[1.5], 200[4.0], 250[4.0], 300[4.0]

+++ 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10

F12  100[1.5], 200[4.0], 250[4.0], 300[4.0], 400[10.0], 500[10.1], 600[4.8]

+++ 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 400+10.0/10 500+10.1/10 600+4.8/10

F15  100[1.5], 200[4.0], 250[4.0], 300[4.0], 500[7.05], 630[7.1], 750[7.0]

+++ 100+1.5/10 200+4.0/10 250+4.0/10 300+4.0/10 500+7.05/10 630+7.1/10 750+7.0/10

F21  200[4.0], 250[4.0], 300[4.0], 600[16.2], 750[15.9], 900[16.2]

+++ 200+4.0/10 250+4.0/10 300+4.0/10 600+16.2/10 750+15.9/10 900+16.2/10

F22  Same as F21

F23  400[3.9], 503[4.0], 600[4.0], 750[3.9], 900[4.0]

+++ 400+3.9/10 503+4.0/10 600+4.0/10 750+3.9/10 900+4.0/10

F24  50[0.75], 400[3.9], 503[4.0], 600[4.0], 750[4.0], 900[4.0]

+++ 50+0.75/10 400+3.9/10 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10

F25  503[4.0], 600[4.0], 750[4.0], 900[4.0]

+++ 503+4.0/10 600+4.0/10 750+4.0/10 900+4.0/10

F26  400[3.9], 503[4.2], 600[4.0], 750[4.0], 900[4.0]

+++ 400+3.9/10 503+4.2/10 600+4.0/10 750+4.0/10 900+4.0/10

F27  Same as F26

Example: The notation 503[4.2] should be understood to
represent a beat frequency of 4.2 Hz between two carriers
equally separated from 503 Hz (i.e., 500.9 and 505.1 Hz).

Amplitudes are generally maintained about 20 dB above
background 1/f (pink) noise and/or surf.

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
