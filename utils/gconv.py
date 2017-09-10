#! /usr/bin/env python3
# Geometry convertor

from pyassimp import * 
import json
import sys

def flatten_float(l):
    return [float("{:.6f}".format(item)) for sublist in l for item in sublist]

def flatten_int(l):
    return [int(item) for sublist in l for item in sublist]

source_file_path = ""
destination_file_path = ""

try:
    source_file_path = sys.argv[1]
    destination_file_path = sys.argv[2]
except IndexError:
    print("Usage: % source_file.some_3d_format destination_file.json".format(sys.argv[0]), file=sys.stderr)
    sys.exit(1)

scene = load(source_file_path) 

assert len(scene.meshes) 
mesh = scene.meshes[0] 
assert len(mesh.vertices) 

release(scene)

with open(destination_file_path, "w") as f:
    print(json.dumps({
        "vertices": flatten_float(mesh.vertices),
        "normals": flatten_float(mesh.normals),
        "indices": flatten_int(mesh.faces),
    }, sort_keys=True), file=f)

