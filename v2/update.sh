#!/bin/sh
echo Updating $1
git add "$1"
echo Commit
read msg
git commit -m "$msg"
git push origin master 
