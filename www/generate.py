import sys
import os.path

print("#ifndef __DATA_H__")
print("#define __DATA_H__")

for file in sys.argv[1:]:
    basename = os.path.basename(file)
    sanitized = basename.replace('.', '_')
    print("const char *{} = R\"{}(".format(sanitized, sanitized.upper()))
    with open(file, 'r') as f:
        print(f.read())
    print("){}\";".format(sanitized.upper()))

print("#endif // __DATA_H__")