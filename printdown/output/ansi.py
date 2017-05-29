import sys
import copy

import base

class Output(base.Output):
	def __init__(self, config):
		config_copy = copy.deepcopy(config)
		if config_copy.width == None:
			config_copy.width = 80
		base.Output.__init__(self, config_copy)
		self.__commit_style()

	def cut(self):
		self.append('\n\n\n\n\n\n[CUT]')
		self.flush()

	def set_bold(self, bold):
		if bold != self.bold:
			self.bold = bold
			self.__commit_style()

	def set_underline(self, underline):
		if underline != self.underline:
			self.underline = underline
			self.__commit_style()

	def set_heading(self, heading):
		if heading != self.heading:
			self.heading = heading
			self.__commit_style()

	def flush(self):
		sys.stdout.write(self.buffer)
		self.buffer = b''

	def __commit_style(self):
		styles = []
		if self.bold or self.heading:
			styles.append(1)
		if self.underline:
			styles.append(4)
		self.__append_raw(b'\x1b[m')
		if len(styles):
			self.__append_raw(b'\x1b[' + b';'.join(map(str, styles)) + b'm')
