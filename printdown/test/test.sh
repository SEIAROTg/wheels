#!/bin/sh

relative=$(dirname "$0")
for document in $relative/*.md; do
	$relative/../printdown.py $document "$@"
done
