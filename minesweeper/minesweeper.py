#!/usr/bin/env python

import argparse
import random
import sys
from termcolor import colored
from time import sleep

import strategies

def cls():
    # sys.stdout.write('\x1b[2J\x1b[H')
    sys.stdout.write('\x1b[H\x1b[2K')
    sys.stdout.flush()

class Game:
    def __init__(self, width, height, mine_num):
        self.width = width
        self.height = height
        self.mine_num = mine_num
        size = width * height
        assert mine_num <= size
        self.mines = set((pos % width, pos // width) for pos in random.sample(range(size), mine_num))
        self.matrix = [[self.cell_value(x, y) for x in range(width)] for y in range(height)]
        self.status = [[False] * width for i in range(height)]

    def in_range(self, x, y):
        return 0 <= x < self.width and 0 <= y < self.height

    def cell_value(self, x, y):
        if (x, y) in self.mines:
            return -1
        return sum([
            1
            for xx in [x-1, x, x+1]
            for yy in [y-1, y, y+1]
            if self.in_range(xx, yy) and (xx, yy) in self.mines
        ])

    def can_reveal(self, x, y):
        return self.in_range(x, y) and self.matrix[y][x] != -1 and self.status[y][x] == False

    def can_dig(self, x, y):
        return self.in_range(x, y) and self.matrix[y][x] == -1 and self.status[y][x] == False

    def reveal_cell(self, x, y):
        assert self.can_reveal(x, y)
        self.status[y][x] = True
        if self.matrix[y][x] == 0:
            for xx in [x-1, x, x+1]:
                for yy in [y-1, y, y+1]:
                    if self.can_reveal(xx, yy):
                        self.reveal_cell(xx, yy)

    def dig_mine(self, x, y):
        assert self.can_dig(x, y)
        self.status[y][x] = True

    def format_cell(self, x, y):
        value = self.matrix[y][x]
        if value == -1:
            char = '💣'
        elif value == 0:
            char = ' '
        else:
            char = str(value)
        return ' {} '.format(char)

    def print(self):
        for y in range(len(self.matrix)):
            line = ''
            for x in range(len(self.status)):
                foreground = 'white'
                background = 'grey'
                attrs = []
                if not self.status[y][x]:
                    foreground, background = background, foreground
                if self.matrix[y][x] == -1:
                    foreground = 'red'
                line += colored(self.format_cell(x, y), foreground, 'on_' + background)
            print(line)

    def move(self, strategies):
        revealed_map = [[self.status[y][x] and self.matrix[y][x] for x in range(self.width)] for y in range(self.height)]
        for strategy in strategies:
            move = strategy(revealed_map)
            if move:
                break
        if not move:
            raise Exception('No Applicable Strategy')
        print(move)
        if move[0] == 'reveal':
            self.reveal_cell(move[1], move[2])
        elif move[0] == 'dig':
            self.dig_mine(move[1], move[2])
        else:
            assert False
        return move

parser = argparse.ArgumentParser(description="generate map")
parser.add_argument('width', type=int)
parser.add_argument('height', type=int)
parser.add_argument('mine_num', type=int)

args = parser.parse_args()

game = Game(args.width, args.height, args.mine_num)

all_strategies = [getattr(strategies, name) for name in dir(strategies) if name.startswith('strategy_')]

while True:
    cls()
    move = game.move(all_strategies)
    print()
    game.print()
    # sleep(0.1)
    # input()
