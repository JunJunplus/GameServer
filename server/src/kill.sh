!#/bin/bash

pid=`ps aux|grep SuperServer/SuperServer|awk '{ print $2 }'`
kill -9 $pid
pid=`ps aux|grep FLServer/FLServer|awk '{ print $2 }'`
kill -9 $pid
