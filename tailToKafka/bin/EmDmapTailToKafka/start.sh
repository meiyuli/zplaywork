#!/bin/bash

export LD_LIBRARY_PATH=/data/projects/tailToKafka/lib/usr/local/lib/:$LD_LIBRARY_PATH

EmDmapTailToKafka_cnt=`ps -ef|grep EmDmapTailToKafka|grep -v monitorclient2.0|grep -v grep |wc -l`
if [ ${EmDmapTailToKafka_cnt} -eq 0 ]
then
        date
        echo "starting EmDmapTailToKafka"
	/data/projects/tailToKafka/bin/EmDmapTailToKafka/EmDmapTailToKafka /data/projects/tailToKafka/bin/EmDmapTailToKafka/conf/engine.xml >/dev/null 2>&1 &
fi
