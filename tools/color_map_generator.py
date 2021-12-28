import sys
import os
import math

input_filename = ""
output_filename = ""

def parse_args() -> bool:

    global input_filename
    global output_filename

    argv_len = len(sys.argv)

    if argv_len < 2:
        print("Must define input file!\n")
        return False

    input_filename = sys.argv[1]

    if not os.path.exists(input_filename):
        print("Input file not found!\n")
        return False

    output_filename = input_filename + ".data"

    return True

def show_help():
    print("Usage: color_map_generator [FILE]\n\nGenerates a header file with a RGB to indexed color map based on the indexed colors detected on the input file\nThe input file must be in the format <red>, <green>, <blue> the index assumed for each entry is the line number\nExample:\n0, 0, 0\n128, 0, 0\n 0, 128, 0\nCorresponds to a palette with 3 colors, with black as the first index (0)")

def parsing_error(line_num):
    print(f"Error parsing input file on line {line_num}")
    sys.exit(1)

if not parse_args():
    show_help()
    sys.exit(1)

class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def get_hex(self) -> int:
        return self.r << 16 | self.g << 8 | self.b

    def get_distance(self, color) -> float:
        red = abs(color.r - self.r)
        green = abs(color.g - self.g)
        blue = abs(color.b - self.b)

        return math.sqrt(red * red + green * green + blue * blue)


input_file = open(os.path.join(sys.path[0], input_filename), 'r')
input_lines = input_file.readlines()

indexed_colors = []
indexed_colors_len = 0

line_num = 0
for line in input_lines:
    components = line.split(", ")

    if len(components) < 3:
        parsing_error(line_num)

    r = components[0].rstrip("\n")
    g = components[1].rstrip("\n")
    b = components[2].rstrip("\n")

    if not r.isnumeric() or not g.isnumeric() or not b.isnumeric():
        parsing_error(line_num)
    
    indexed_colors.append(Color(int(r), int(g), int(b)))

    line_num += 1

print("Parsed input file successfully\nGenerating map",end="")

output_file = open(os.path.join(sys.path[0], output_filename), 'w')

color_map_dict = {}
for b in range(0, 256):

    if (b % 10) == 0:
        print(f" ... {str(int(100*b/256))}%", end="")
        sys.stdout.flush()

    for g in range(256):
        for r in range(256):

            rgbspace_color = Color(int(r), int(g), int(b))

            closest_dist = 999
            closest_idx = 0

            for idx, indexedspace_color in enumerate(indexed_colors):
                
                dist = rgbspace_color.get_distance(indexedspace_color)

                if dist < closest_dist:
                    closest_dist = dist
                    closest_idx = idx

                    if dist == 0:
                        break
    
            color_map_dict[rgbspace_color.get_hex()] = closest_idx

print("\nGenerated map succesfully, writing output file")

color_map_expected_size = 256*256*256
color_map_size = len(color_map_dict)

file_contents = ""
last_key = 0
for key in sorted(color_map_dict):
    last_key = key
    file_contents += f"{color_map_dict[key]},\n"

    if (key % 1000) == 0:
        output_file.write(file_contents)
        file_contents = ""

    if (key % 1000000) == 0:
        print(f" ... {str(int(100*key/color_map_size))}%", end="")
        sys.stdout.flush()

if (last_key % 1000) != 0:
    output_file.write(file_contents)

output_file.close()
print("\nGenerated output file succesfully!")