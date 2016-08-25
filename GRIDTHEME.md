GridView Theme
==============

Currently Gridview is still under extreme development and what is listed in here may
change or become broken later.  Please be sure to post any problems in this project's
issues.


Quick Reference Guide
=====================

Here is a quick list of all Added theme options GridView has and that do something

```
<image name="gridtile">
	<size> (sets size of gridtile and overrides default size based on boxart)
	<color> (not finished, but can be used) (can set Alpha)
</image>

// Following animates (tweens) to this when selected
<image name="gridtile_selected">  
	<size> (will add to default size if gridtile size is left unasigned)
	<color> (will set Alpha)
</image>

// Gridtile's background
<ninepatch name="gridtile_background">
	<path> (changes gridtile's background)
	<color> (RGBA)
	<padding>
</ninepatch>
<ninepatch name="gridtile_background_selected">
	<color> (RGBA)
</ninepatch>

// GameTitle in gridtile and row containers
<text name="gridtile_text">
	<ALL>
</text>
<container name="gridtile_textRow">
	<size> (only height can be modified)
</container>
<container name="gridtile_textRow_selected">
	<size> (only height can be modified)
</container>

<container name="md_grid_margin">
	<size> (sets size width and height of margin between tiles)
</container>

// Current Game selected.
<text name="md_title">
	<ALL TEXT PROPERTIES>
</text>

// The container for the whole grid (will become a container in the future)
<text name="md_grid">
	<size>
	<pos>
</text>
```

