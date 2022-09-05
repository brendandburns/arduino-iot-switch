import sys
import os.path

print("#ifndef __DATA_H__")
print("#define __DATA_H__")

for file in sys.argv[1:]:
    basename = os.path.basename(file)
    sanitized = basename.replace('.', '_')
    raw = sanitized
    if len(raw) > 15:
        raw = raw[0:16]
    print("const char *{} = R\"{}(".format(sanitized, raw.upper()))
    with open(file, 'r') as f:
        print(f.read())
    print("){}\";".format(raw.upper()))

print("#endif // __DATA_H__")