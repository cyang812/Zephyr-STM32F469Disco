#!/usr/bin/env python3
import sys

def parse_config(path):
    cfg = {}
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "=" in line:
                k, v = line.split("=", 1)
                cfg[k] = v
    return cfg

if len(sys.argv) != 3:
    print("usage: diff2prj <.config.old> <.config>")
    sys.exit(1)

old_cfg = parse_config(sys.argv[1])
new_cfg = parse_config(sys.argv[2])

prj_lines = []

for k, new_val in new_cfg.items():
    old_val = old_cfg.get(k)
    if new_val != old_val:
        # strip leading CONFIG_ if present
        key = k
        if key.startswith("CONFIG_"):
            key = key[7:]
        if new_val == "y":
            prj_lines.append(f"CONFIG_{key}=y")
        elif new_val == "n":
            prj_lines.append(f"# CONFIG_{key} is not set")
        else:
            prj_lines.append(f"CONFIG_{key}={new_val}")

if prj_lines:
    print("\n".join(sorted(prj_lines)))
else:
    print("# no differences")
