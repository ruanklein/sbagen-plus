#!/bin/sh

cat <<EOF
Usage: t-focus-2 <additional-sbagen-options-if-required>

Please note: These tone-sets are based on data from a document
received from an anonymous source.  The data is almost identical to
the USENET-released data that appears in t-focus-1.  It is included
here because in a few tone-sets, extra tones are included, which may
be interesting or useful to certain experimenters.

No attempt has been made to sequence these tones - use ^C to abort any
tone-set and move onto the next.  Amplitudes of the tones have all
been set equal, and the user can edit these if required.

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
F03 50[1.2] 100[1.3]  288[3.6]
+++ 50+1.2/10 100+1.3/10  288+3.6/10
F10 100[1.5]  200[4.0]  250[4.0]  300[4.0]
+++ 100+1.5/10  200+4.0/10  250+4.0/10  300+4.0/10
F12 50[0.25]  100[1.5]  200[4.0]  250[4.0]  300[4.0]  400[10.0]  500[10.1] 600[4.8]
+++ 50+0.25/10  100+1.5/10  200+4.0/10  250+4.0/10  300+4.0/10  400+10.0/10  500+10.1/10 600+4.8/10
F15 100[1.5]  200[4.0]  250[4.0]  300[4.0]  500[7.05]  630[7.0]  750[7.0]
+++ 100+1.5/10  200+4.0/10  250+4.0/10  300+4.0/10  500+7.05/10  630+7.0/10  750+7.0/10
F21 200[4.0]  250[4.0]  300[4.0]  600[16.2]  750[16.2]  900[16.2]
+++ 200+4.0/10  250+4.0/10  300+4.0/10  600+16.2/10  750+16.2/10  900+16.2/10
F22 200[4.0]  250[4.0]  300[4.0]  600[16.2]  750[16.2]  900[16.2]  (same as F21)
+++ 200+4.0/10  250+4.0/10  300+4.0/10  600+16.2/10  750+16.2/10  900+16.2/10
F23 400[3.9]  503[4.0]  600[4.0]  750[3.9]  900[4.0]
+++ 400+3.9/10  503+4.0/10  600+4.0/10  750+3.9/10  900+4.0/10
F24 50[0.75]  200[1.5]  400[3.9]  503[4.0]  600[4.0]  750[4.0]  900[4.0]
+++ 50+0.75/10  200+1.5/10  400+3.9/10  503+4.0/10  600+4.0/10  750+4.0/10  900+4.0/10
F26 400[3.9]  503[4.2]  600[4.0]  750[4.0]  900[4.0]
+++ 400+3.9/10  503+4.2/10  600+4.0/10  750+4.0/10  900+4.0/10
F27 50[0.80]  400[4.0]  503[4.2]  600[4.0]  750[4.0]  900[4.0]
+++ 50+0.80/10  400+4.0/10  503+4.2/10  600+4.0/10  750+4.0/10  900+4.0/10

Sometimes F21 is as follows:
F21 200[4.0]  250[5.4]  300[5.4]  600[16.2]  750[16.2]  900[16.2]
+++ 200+4.0/10  250+5.4/10  300+5.4/10  600+16.2/10  750+16.2/10  900+16.2/10

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
