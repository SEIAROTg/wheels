import copy
import usb.core, usb.util

import base

class Output(base.Output):
	def __init__(self, config):
		config_copy = copy.deepcopy(config)
		if config_copy.width == None:
			config_copy.width = 48
		if config_copy.encoding == None:
			config_copy.encoding = 'gb18030'
		base.Output.__init__(self, config_copy)
		self.printer, self.out_ep = find_printer()
		init_printer(self.printer)
		self.__append_raw(self.__ESCPOS_RESET)

	def cut(self):
		self.__append_raw(self.__ESCPOS_CUT_PARTIAL)
		self.flush()

	def set_bold(self, bold):
		if bold != self.bold:
			if bold:
				self.__append_raw(self.__ESCPOS_BOLD_ON)
			else:
				self.__append_raw(self.__ESCPOS_BOLD_OFF)
			self.bold = bold

	def set_underline(self, underline):
		if underline != self.underline:
			if underline == 1:
				self.__append_raw(self.__ESCPOS_UNDERLINE1_ON)
			elif underline == 2:
				self.__append_raw(self.__ESCPOS_UNDERLINE2_ON)
			else:
				self.__append_raw(self.__ESCPOS_UNDERLINE_OFF)
			self.underline = underline

	def set_heading(self, heading):
		if heading != self.heading:
			if self.heading:
				self.__append_raw(self.__ESCPOS_HEADING[self.heading-1][1])
			if heading:
				self.__append_raw(self.__ESCPOS_HEADING[heading-1][0])
			self.heading = heading

	def flush(self):
		self.out_ep.write(self.buffer, 0)
		self.buffer = b''

	__ESCPOS_RESET = b'\x1b@'
	__ESCPOS_CUT_PARTIAL = b'\n\n\n\n\n\n\x1dV\x01'
	__ESCPOS_NORMAL = b'\x1b!\x00'
	__ESCPOS_BOLD_ON = b'\x1b\x45\x01'
	__ESCPOS_BOLD_OFF = b'\x1b\x45\x00'
	__ESCPOS_UNDERLINE1_ON = b'\x1b\x2d\x01'
	__ESCPOS_UNDERLINE2_ON = b'\x1b\x2d\x02'
	__ESCPOS_UNDERLINE_OFF = b'\x1b\x2d\x00'
	__ESCPOS_HEADING = [
		(b'\x1d!\x55', b'\x1b!\x00'),
		(b'\x1d!\x44', b'\x1b!\x00'),
		(b'\x1d!\x33', b'\x1b!\x00'),
		(b'\x1d!\x22', b'\x1b!\x00'),
		(b'\x1b!\x30', b'\x1b!\x00'),
		(b'\x1b\x45\x01', b'\x1b\x45\x00'),
	]

def is_printer(dev):
	return dev.bDeviceClass == 0x7 or get_interface_by_dev(dev)

def get_interface_by_dev(dev):
	for cfg in dev:
		intf = usb.util.find_descriptor(cfg, bInterfaceClass=0x7)
		if intf != None:
			return intf
	return None

def is_out_ep(ep):
	return ep.bEndpointAddress & 0x80 == 0

def find_printer():
	printers = list(usb.core.find(find_all=True, custom_match=is_printer))
	if len(printers) == 0:
		print 'no printers found'
		exit()
	elif len(printers) > 1:
		# TODO: manually specify printer
		print 'multiple printers found'
		exit()
	dev = printers[0]
	intf = get_interface_by_dev(dev)
	out_eps = list(usb.util.find_descriptor(intf, find_all=True, custom_match=is_out_ep))
	if len(printers) == 0:
		print 'no output endpoints found'
		exit()
	elif len(printers) > 1:
		print 'multiple output endpoints found'
		exit()
	return (dev, out_eps[0])

def init_printer(dev):
	driver = dev.is_kernel_driver_active(0)
	if driver is None or driver:
		try:
			dev.detach_kernel_driver(0)
		except usb.core.USBError:
			if driver is not None:
				print 'Failed to detech kernel driver'
				exit()
	try:
		dev.set_configuration()
		dev.reset()
	except usb.core.USBError:
		print 'Failed to set configuration'
		exit()
