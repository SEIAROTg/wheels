import re
import time
import collections

from output import *

class TableColumn:
	def __init__(self, **kwargs):
		self.__dict__.update(kwargs)

def get_table_columnss(option_line):
	columnss = [[]]
	for col in option_line.strip().split('|')[1:-1]:
		if col == '':
			if len(columnss[-1]):
				columnss.append([])
		else:
			# 0 = left, 1 = right, 2 = center
			align = 0
			# whether each column has border
			border = True
			match = re.match(r'\s*(:?)(-+|=+)(:?)\s*', col)
			if not match:
				raise Exception('Invalid Syntax')
			elif match.group(1) and match.group(3):
				align = 2
			elif match.group(3):
				align = 1
			else:
				align = 0
			border = match.group(2)[0] == '-'
			columnss[-1].append(TableColumn(align=align, border=border))
	return columnss

def get_table_line_border_width(columns):
	width = 0
	for i in xrange(len(columns) + 1):
		if (i != 0) and columns[i - 1].border or (i != len(columns)) and columns[i].border:
			width += 1
	return width

def calc_table_column_width(columnss, body, total_width):
	column_idx = 0
	for columns in columnss:
		border_width = get_table_line_border_width(columns)
		remaining_width = total_width - border_width
		maxs = [
			max([
				len(body[row][column_idx + col])
				for row in xrange(len(body))
			] + [1])
			for col in xrange(len(columns))
		]
		avgs = [
			sum([
				max(len(body[row][column_idx + col]), 1)
				for row in xrange(len(body))
			]) / (len(body) or 1)
			for col in xrange(len(columns))
		]
		total_maxs = sum(maxs)
		remaining_avgs = sum(avgs)
		if total_maxs <= remaining_width:
			remaining_width -= total_maxs
		else:
			remaining_width -= len(columns)
		for i in xrange(len(columns)):
			if remaining_avgs:
				extra_width = remaining_width * avgs[i] / remaining_avgs
			else:
				extra_width = 0
			remaining_avgs -= avgs[i]
			remaining_width -= extra_width
			columns[i].width = extra_width
			if total_maxs <= total_width - border_width:
				columns[i].width += maxs[i]
			else:
				columns[i].width += 1
		column_idx += len(columns)

def get_table_body(lines):
	body = []
	for line in lines:
		row = line.strip().split('|')
		if len(row) > 2 and row[0] == '':
			body.append(row[1:-1])
		else:
			break
	return body

def get_table_border_by_pos(columns, pos):
	last_border = False
	for i in xrange(len(columns)):
		if (i != 0) and columns[i - 1].border or (i != len(columns)) and columns[i].border:
			pos -= 1
			if pos < 0:
				return last_border and columns[i].border
		pos -= columns[i].width
		last_border = columns[i].border
		if pos < 0:
			return last_border
	return False

class Renderer:
	def __init__(self, config):
		if config.output == 'escpos':
			self.output = escpos.Output(config)
		else:
			self.output = ansi.Output(config)

	def render_document(self, document):
		lines = document.split('\n')
		if len(lines) and lines[-1] == u'':
			lines.pop()
		idx = 0
		while idx < len(lines):
			idx = self.__render_line(lines, idx)

	def __render_line(self, lines, idx):
		if lines[idx].startswith('#'):
			self.__render_heading(lines[idx])
			return idx + 1
		match = re.match(r'^(\t*)(.*)$', lines[idx])
		self.output.set_indent(len(match.group(1)))

		text = match.group(2)
		if (idx < len(lines) - 1 and re.match('^\s*$', lines[idx])) and re.match(r'^\s*\|', lines[idx+1]):
			lines_of_table = self.__render_table(lines[idx+1:])
			if lines_of_table:
				return idx + lines_of_table + 1
		elif idx == 0 and text.startswith('|'):
			lines_of_table = self.__render_table(lines[idx:])
			if lines_of_table:
				return idx + lines_of_table

		self.__render_simple_text(text)
		self.output.append('\n')
		return idx + 1

	def __render_heading(self, line):
		hashs = re.match(r'^#*', line).group(0)
		level = min(len(hashs), 6)
		heading = line[level:].strip()
		self.output.set_heading(level)
		self.output.append(heading)
		self.output.set_heading(0)
		self.output.append('\n')

	def __render_table(self, lines):
		# whether display heading
		headings = lines[0].split('|')[1:-1]
		has_headings = any([cell != '' for cell in headings])
		# align and border option
		try:
			columnss = get_table_columnss(lines[1])
		except:
			raise
			return 0
		# string to array
		body = get_table_body(lines[2:])
		# calculate width of each column
		if has_headings:
			calc_table_column_width(columnss, [headings] + body, self.output.config.width)
		else:
			calc_table_column_width(columnss, body, self.output.config.width)
		self.__render_table_top_border(columnss, self.output.config.width)
		if has_headings:
			self.__render_table_row(headings, columnss, True)
		for i in xrange(len(body)):
			self.__render_table_row(body[i], columnss, False, i == len(body) - 1)
		self.output.set_bold(0)
		self.output.set_underline(0)
		self.output.append('\n')
		return len(body) + 2

	def __render_table_top_border(self, columnss, total_width):
		columns = columnss[0]
		for i in xrange(total_width):
			if get_table_border_by_pos(columns, i):
				self.output.set_underline(2)
			else:
				self.output.set_underline(0)
			self.output.append(' ')
			
	def __render_table_row(self, row, columnss, is_heading=False, is_last_row=False):
		underline = False
		column_idx = 0
		for i in xrange(len(columnss)):
			columns = columnss[i]
			next_columns = columnss[i % len(columnss)]
			lines = 1
			for j in xrange(len(columns)):
				if columns[j].width:
					current_lines = -(-len(row[column_idx + j]) / columns[j].width) # round up
				else:
					current_lines = 1
				lines = max(lines, current_lines)
			for j in xrange(lines):
				is_last_text_line = j == lines - 1
				for k in xrange(len(columns)):
					# need left border
					if (k != 0) and columns[k-1].border or columns[k].border:
						self.output.set_bold(1)
						self.__update_table_underline(columnss, i, is_last_text_line, is_last_row)
						self.output.append('|')
						self.output.set_bold(0)
					cell = row[column_idx + k]
					text = cell[columns[k].width*j:columns[k].width*(j+1)]
					# add padding
					padding = columns[k].width - len(text)
					
					if columns[k].align == 2:				# middle align
						text = ' ' * (padding / 2) + text + ' ' * (padding - padding / 2)
					elif columns[k].align == 1:
						text = ' ' * padding + text			# right align
					else:
						text = text + ' ' * padding			# left align

					if is_heading:
						self.output.set_bold(1)
					for c in text:
						self.__update_table_underline(columnss, i, is_last_text_line, is_last_row)
						self.output.append(c)
					if is_heading:
						self.output.set_bold(0)


				if columns[-1].border:
					self.output.set_bold(1)
					self.__update_table_underline(columnss, i, is_last_text_line, is_last_row)
					self.output.append('|')
					self.output.set_bold(0)

			column_idx += len(columns)

	def __update_table_underline(self, columnss, columns_idx, is_last_text_line=False, is_last_row=False):
		if not is_last_text_line:
			self.output.set_underline(0)
			return
		is_last_cell_line = columns_idx == len(columnss) - 1
		next_columns = columnss[(columns_idx + 1) % len(columnss)]
		underline_current = get_table_border_by_pos(columnss[columns_idx], self.output.position)
		underline_next = not (is_last_cell_line and is_last_row) and get_table_border_by_pos(next_columns, self.output.position)
		if underline_current or underline_next:
			if columns_idx == len(columnss) - 1:
				self.output.set_underline(2)
			else:
				self.output.set_underline(1)
		else:
			self.output.set_underline(0)


	def __render_simple_text(self, text):
		pos = 0
		while pos < len(text):
			if text[pos] == '\\':
				pos += 1
				self.output.append(text[pos])
			elif text[pos:pos+2] == '**':
				if self.output.bold:
					self.output.set_bold(0)
					pos += 1
				elif re.search(r'[^\\]\*\*', text[pos+1:]):
					self.output.set_bold(1)
					pos += 1
				else:
					self.output.append(text[pos])
			elif text[pos:pos+2] == '__':
				if self.output.underline:
					self.output.set_underline(0)
					pos += 1
				elif re.search(r'[^\\]__', text[pos+1:]):
					self.output.set_underline(1)
					pos += 1
				else:
					self.output.append(text[pos])
			else:
				self.output.append(text[pos])
			pos += 1
