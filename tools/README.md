# Tools
In this directory are all the tools created to help the development of the Consol3 engine  
The tools are created in Python and serve only to generate data and to aid in development, no engine code is actually done in Python

### color_map_generator.py
Used to generate color mappings from full RGB to indexed colors, that are later imported by the engine to use as lookup tables for some of the different renderers  

The tool will load an input file describing the output space, this file will have all the colors in the desired output palette, ordered by index  
The input file must be in the format \<red\>, \<green\>, \<blue\> the index assumed for each entry is the line number  

**Example:**  
>0, 0, 0  
>128, 0, 0  
>0, 128, 0  

Corresponds to the output space with 3 indices, 0 corresponding to black, 1 corresponding to dark red and 2 corresponding to dark green  

After loading the output space, the tool will go over the entire RGB space and calculate the closest color in the output space for each input color (all 16 777 216 of them)  
It will then write an output file with the corresponding output index for each color, ordered by color value (first like is #000000, last line is #ffffff)  

The engine can then use this output file to initialize an array with the desired values  
When a full RGB color needs to be converted to an indexed color, the engine can just get the corresponding index at the same array location as the color's value  


To use the tool, simply use the command **python color_map_generator.py \<input file\>**  