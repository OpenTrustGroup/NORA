#!/usr/bin/env python3
# Copyright 2026 Open Trust Group. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Write a release's locked manifest: manifest/NORA with every project at the
# release, so that a checkout of the release does not follow the branches
# as they move on.
#
#   nora/lock-manifest.py VERSION
#
# writes manifest/NORA-VERSION. This repository, each fork of upstream and
# zircon (a project on branch NORA) are pinned to their tag NORA-VERSION, by
# name: the tag is where the release is, and moves with it if it is moved; a
# project manifest/NORA already pins keeps its commit. Each tag is looked up
# on its remote first: a fork without it stops this, naming the fork.
import os
import subprocess
import sys
import xml.etree.ElementTree as ET

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def has_tag(remote, tag):
    out = subprocess.run(['git', 'ls-remote', remote, 'refs/tags/%s' % tag],
                         check=True, capture_output=True, text=True).stdout
    return bool(out.strip())


def main():
    if len(sys.argv) != 2:
        sys.exit('usage: %s VERSION' % sys.argv[0])
    version = sys.argv[1]
    tag = 'NORA-' + version
    src = os.path.join(ROOT, 'manifest', 'NORA')
    text = open(src).read()
    tree = ET.ElementTree(ET.fromstring(text))
    missing = []
    for p in tree.getroot().iter('project'):
        if p.get('path') != '.' and p.get('remotebranch') != 'NORA':
            continue  # pinned already
        # (this repository's tag is made with this file in it: not looked up)
        if p.get('path') != '.' and not has_tag(p.get('remote'), tag):
            missing.append('%s (%s)' % (p.get('name'), p.get('remote')))
            continue
        p.set('revision', 'refs/tags/' + tag)
        p.attrib.pop('remotebranch', None)
    if missing:
        sys.exit('no tag %s on: %s' % (tag, ', '.join(missing)))
    ET.indent(tree, space='  ')
    out = os.path.join(ROOT, 'manifest', tag)
    with open(out, 'w') as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write('<!-- NORA %s, locked: this repository, the forks and zircon at\n'
                '     their tag %s, the other projects at the commits\n'
                '     manifest/NORA pins. Written by nora/lock-manifest.py from\n'
                '     manifest/NORA; do not edit. -->\n' % (version, tag))
        tree.write(f, encoding='unicode')
        f.write('\n')
    print(out)


if __name__ == '__main__':
    main()
