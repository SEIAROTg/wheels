import copy

class Output:
	def __init__(self, config):
		self.config = copy.deepcopy(config)
		if self.config.encoding == None:
			self.config.encoding = 'utf-8'
		if self.config.indent == None:
			self.config.indent = 4
		self.bold = 0
		self.underline = 0
		self.heading = 0
		self.buffer = b''
		self.position = 0
		self.indent = 0
		self.indent_pending = False

	def append(self, str):
		for c in str:
			self.__append_char(c)

	def cut(self):
		raise NotImplementedError('cut not implemented')

	def set_bold(self, bold):
		raise NotImplementedError('set_bold not implemented')

	def set_underline(self, underline):
		raise NotImplementedError('set_underline not implemented')

	def set_heading(self, heading):
		raise NotImplementedError('set_heading not implemented')

	def set_indent(self, level):
		max_width = self.config.width - 4
		if level * self.config.indent  < max_width:
			self.indent = level * self.config.indent
		else:
			self.indent = max_width - max_width % self.config.indent - 1
		self.indent_pending = True

	def flush(self):
		raise NotImplementedError('flush not implemented')

	def __append_raw(self, data):
		self.buffer += data

	def __commit_indent(self):
		if self.indent_pending:
			self.__append_raw(' ' * self.indent)
			self.position = self.indent
			self.indent_pending = False

	def __append_char(self, char):
		if char == '\n':
			self.__append_newline()
		else:
			self.__commit_indent()
			encoded = char.encode(self.config.encoding)
			self.__append_raw(encoded)
			self.position += len(encoded)
			if self.position == self.config.width:
				self.flush()
				self.position = 0
				self.indent_pending = True

	def __append_newline(self):
		self.__append_raw('\n')
		self.flush()
		self.position = 0
		self.indent_pending = True
