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
parser.add_argument('--namespace', metavar='namespace', type=str, nargs=1, help='namespace for the shader variable in the header file')
parser.add_argument('--no-namespace', dest='no_namespace', action='store_true', help='do not use a namespace for the shader variable in the header file')
parser.add_argument('--release', dest='release', action='store_true', help='strip comments and whitespace from the shader')

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

# Write namespace
if args.namespace and not args.no_namespace:
    print('namespace ' + args.namespace[0] + ' {')

if args.name:
    print("inline std::string& "+ args.name[0] +"() {")
else:
    print("inline std::string& "+  os.path.splitext(os.path.basename(args.input[0]))[0] +"() {")

if args.release:
    # Strip comments and whitespace
    contents = re.sub(r'//.*', '', input_file_contents)
    # Remove /* */ comments
    contents = re.sub(r'/\*.*\*/', '', contents, flags=re.DOTALL)
    # remove empty lines
    contents = os.linesep.join([
        s.strip() for s in contents.splitlines() if s
    ])
else:
    contents = input_file_contents

var_decl = '    static std::string source = ' + 'R"(' + contents + ')";'
print(var_decl)
print("    return source;")
print("}")


# Write namespace end
if args.namespace and not args.no_namespace:
    print('}')
