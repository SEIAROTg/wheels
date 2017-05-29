from collections import namedtuple
from itertools import chain, combinations
from functools import reduce
import operator

NeighborStats = namedtuple('NeighborStats', ['all', 'revealed', 'unrevealed', 'revealed_mine', 'remaining_mine_num'])

# https://docs.python.org/3/library/itertools.html
def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))

def get_neighbors(x, y, h, w, distance):
	return (
		(xx, yy)
		for yy in range(y - distance, y + distance + 1)
		for xx in range(x - distance, x + distance + 1)
		if (xx, yy) != (x, y) and 0 <= xx < w and 0 <= yy < h
	)

def iter_map(revealed_map):
	return (
		(x, y)
		for y in range(len(revealed_map))
		for x in range(len(revealed_map[y]))
	)

def filter_cells(cells, revealed_map, include_unrevealed=True, include_mine=True):
	return (
		(x, y)
		for (x, y) in cells
		if (include_unrevealed or not revealed_map[y][x] is False) and (include_mine or revealed_map[y][x] != -1)
	)

def neighbor_stats(revealed_map, x, y):
	h = len(revealed_map)
	w = len(revealed_map[0])
	neighbors = set(get_neighbors(x, y, h, w, 1))
	revealed = set((x,y) for (x,y) in neighbors if not revealed_map[y][x] is False)
	revealed_mine = set((x,y) for (x,y) in neighbors if revealed_map[y][x] == -1)
	return NeighborStats(
		all = neighbors,
		revealed = revealed,
		unrevealed = neighbors - revealed,
		revealed_mine = revealed_mine,
		remaining_mine_num = revealed_map[y][x] != -1 and revealed_map[y][x] - len(revealed_mine),
	)

# safe
# def strategy_remaining_all_mine(revealed_map):
# 	for (x, y) in filter_cells(iter_map(revealed_map), revealed_map, False, False):
# 		stats = neighbor_stats(revealed_map, x, y)
# 		unrevealed = len(stats.unrevealed)
# 		remaining_mines = revealed_map[y][x] - len(stats.revealed_mine)
# 		action = None
# 		if unrevealed:
# 			if unrevealed == remaining_mines:
# 				action = 'dig'
# 			elif unrevealed and remaining_mines == 0:
# 				action = 'reveal'
# 		if action:
# 			for neighbor in stats.all:
# 				if not neighbor in stats.revealed:
# 					return (action,) + neighbor + ('remaining_all_mine',)

# safe
def strategy_000_subset(revealed_map):
	h = len(revealed_map)
	w = len(revealed_map[0])
	stats = { (x, y): neighbor_stats(revealed_map, x, y) for (x, y) in filter_cells(iter_map(revealed_map), revealed_map, False, False) }
	for (x, y) in filter_cells(iter_map(revealed_map), revealed_map, False, False):
		unrevealed = len(stats[(x, y)].unrevealed)
		if unrevealed:
			subset_neighbors = set()
			for (xx, yy) in filter_cells(get_neighbors(x, y, h, w, 2), revealed_map, False, False):
				if stats[(xx, yy)].unrevealed < stats[(x, y)].unrevealed:
					subset_neighbors.add((xx, yy))
			for cells in powerset(subset_neighbors):
				union_cells = reduce(operator.or_, (stats[(xx, yy)].unrevealed for (xx, yy) in cells), set())
				if union_cells < stats[(x, y)].unrevealed:
					union_cell_num = len(union_cells)
					total_cell_num = sum(len(stats[(xx, yy)].unrevealed) for (xx, yy) in cells)
					total_remaining_mine_num = sum(stats[(xx, yy)].remaining_mine_num for (xx, yy) in cells)
					action = None
					if union_cell_num == total_cell_num:
						if stats[(x, y)].remaining_mine_num == total_remaining_mine_num:
							action = 'reveal'
						elif unrevealed - union_cell_num == stats[(x, y)].remaining_mine_num - total_remaining_mine_num:
							action = 'dig'
					if action:
						return (action,) + next(iter(stats[(x, y)].unrevealed - union_cells)) + ('subset',)


# unsafe
def strategy_zzz_random(revealed_map):
	for (x, y) in iter_map(revealed_map):
		if revealed_map[y][x] is False:
			return ('reveal', x, y, 'random')
