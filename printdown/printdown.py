#!/usr/bin/env python

import codecs
import argparse
import time
import sys

from cmdargs import get_cmdargs
from renderer import Renderer

if __name__ == '__main__':

	args = get_cmdargs()

	with codecs.open(args.input, 'r', 'utf-8') as f:
		document = f.read()

	renderer = Renderer(args)
	renderer.render_document(document)
	if args.cut:
		renderer.output.cut()
