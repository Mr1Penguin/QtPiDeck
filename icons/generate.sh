#!/bin/bash

svgdir="svg"
icons=`cd $svgdir && ls *.svg`
tab="    "
iconsres="../icons.qrc"


echo "<RCC>" > $iconsres
echo "${tab}<qresource prefix=\"/\">" >> $iconsres

for resolution in 16 32 64
do
    dirname="${resolution}x${resolution}"
    echo $dirname
    rm -rf $dirname
    mkdir -p $dirname
    for icon in $icons
    do
        pngpath=$dirname/${icon/svg/png}
        inkscape -w $resolution -h $resolution $svgdir/$icon --export-filename $pngpath 2>/dev/null
        echo "${tab}${tab}<file>icons/${pngpath}</file>" >> $iconsres
    done
done

echo "${tab}</qresource>" >> $iconsres
echo "</RCC>" >> $iconsres
