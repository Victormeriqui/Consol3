# Consol3
A C++ graphics engine that executes entirely on the CPU


### Intro

###### Software Rendering
Consol3 is a 3d graphics engine that doesn't use the GPU to render any frame, instead the same calculations that would typically be made by the GPU hardware is done in software, every single vertex transformation, matrix calculation, etc is programmed on the CPU

###### Windows Console
Consol3 as it stands today is a Windows console application, it uses the console as it's output display, by taking advantage of the console API available in Windows

I have plans to add support for other terminals/platforms, but as of now they are just plans

###### Older Versions
Building this engine is a hobby of mine and I've been working on it infrequently for some years now, as such it has gone through many refactors, partial and complete rewrites, this is the latest version of the engine

###### Rules
No external dependencies will ever be used in this engine, the goal is to do everything using only what the OS already provides, that means no external math libraries, window managers, etc

### Features

###### OBJ file loading  
<img src="images/obj.png" width="200" height="200">  

###### MD2 file loading  
<img src="images/md2.png" width="200" height="200">  

###### Directional lights  
<img src="images/directionallight.png" width="200" height="200">  

###### Point lights  
<img src="images/pointlight.gif" width="200" height="200">  

###### Spot lights  
<img src="images/spotlight.png" width="200" height="200">  

###### Textures  
<img src="images/textures.gif" width="200" height="200">  

###### Shadow maps  
<img src="images/shadows.png" width="200" height="200">  

###### Shadows from multiple sources
<img src="images/multishadows.png" width="200" height="200">  

###### MD2 Animations
<img src="images/animation1.gif" width="200" height="200">  <img src="images/animation2.gif" width="200" height="200">  

###### Ico-sphere generation
<img src="images/sphere.gif" width="200" height="200">  

###### Normal maps
<img src="images/normal_map.png" width="200" height="200">  <img src="images/normal_map.gif" width="200" height="200">  

### Existing Renderers  
Different renderers for the same scene can be used, these components basically decide how each pixel should be drawn to the console

###### Greyscale  
Overrides the palette with 16 shades from black to white  
<img src="images/greyscale.png" width="200" height="200">  

###### Dithered Greyscale 
Similar to the previous one, but also takes advantage of the dithering block characters (░▒▓) to dither different combinations of the 16 shades, expands the original 16 to 80 shades  
<img src="images/ditheredgreyscale.png" width="200" height="200">  

###### Dithered  
Uses the same mechanism from the previous renderer but with the default pallette, giving more depth to the default colors (11 shades per color)  
<img src="images/dithered.png" width="200" height="200">  

###### Text Only  
Does not use any attribute change, thus the only color is white, the lightness of each pixel is controlled through the character in the cell  

Due to the way the Windows Console renders the character cells, this is currently the fastest renderer (although the one with the least detail), the current characters used are: " ·;%░≡¥▒▓█"  
<img src="images/ascii.png" width="200" height="200">  


### Shaders
Vertex and Fragment shaders can be created, they are basically classes that implement a vertex and fragment stage in the pipeline, and can pass data around via the class members

A simple shader:
```cpp
bool PlainTextureShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
{
	TransformVertexMVP(v0, mvp_mats);
	TransformVertexMVP(v1, mvp_mats);
	TransformVertexMVP(v2, mvp_mats);

	vert_v0_texture_coord = v0.GetTextureCoords();
	vert_v1_texture_coord = v1.GetTextureCoords();
	vert_v2_texture_coord = v2.GetTextureCoords();

	return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
}

HSVColor PlainShader::FragmentShader(const RGBColor& color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2)
{
	Vector2 frag_texture_coord = PerspectiveCorrectInterpolate<Vector2>(vert_v0_texture_coord, vert_v1_texture_coord, vert_v2_texture_coord, triangle, barcoord0, barcoord1, barcoord2);

	RGBColor texture_color = texture->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y);

	return HSVColor(texture_color.BlendMultiply(color));
}
```

### Planned Features  

* Faster vertex transformations with SIMD  
* ANSI Escape sequence renderer  
* Faster rasterizer with multipixel filling  
* Faster rasterizer with binning  
