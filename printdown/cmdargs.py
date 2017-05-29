import argparse

def get_cmdargs():
	parser = argparse.ArgumentParser()

	parser.add_argument('input', help='markdown file')
	parser.add_argument('-o', '--output', default='ansi')
	parser.add_argument('-c', '--cut', action="store_true", help='perform cut')
	parser.add_argument('-w', '--width', help='specify line width', type=int, default=None)
	parser.add_argument('-e', '--encoding', help='specify output encoding', default=None)
	parser.add_argument('-i', '--indent', help='specify indent width', type=int, default=None)

	return parser.parse_args()
