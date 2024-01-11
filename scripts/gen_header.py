#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
# File name: gen_header.py
# Script to generate embed shader files as C++ headers
# This script reads a shader file and generates a C++ header file with the shader embedded as a string literal.

import sys
import os
import re
import argparse

# Parse arguments
parser = argparse.ArgumentParser(description='Generate C++ header file with shader embedded as a string literal.')
parser.add_argument('input', metavar='input', type=str, nargs=1, help='input shader file')
parser.add_argument('--name', metavar='name', type=str, nargs=1, help='name of the shader variable in the header file')
parser.add_argument('--no-include', dest='no_include', action='store_true', help='do not include the header file in the output')
parser.add_argument('--no-static', dest='no_static', action='store_true', help='do not use static for the shader variable in the header file')

args = parser.parse_args()

# Read input file
input_file = open(args.input[0], 'r')
input_file_contents = input_file.read()
input_file.close()

# Generate output file

# Write header
print('#pragma once')
if not args.no_include:
    print('#include <string>')
    print('')


# Write shader variable
var_decl = ''
if not args.no_static:
    var_decl += 'static '

if args.name:
    var_decl += 'const std::string ' + args.name[0] + ' = '
else:
    var_decl += 'const std::string ' + os.path.splitext(os.path.basename(args.input[0]))[0] + ' = '

print(var_decl)

# Write shader string
print('R"(')
print(input_file_contents)
print(')";')
