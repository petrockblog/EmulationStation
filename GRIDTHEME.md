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
	<color> (not finished, but can be used)
</image>

// Following animates (tweens) to this when selected
<image name="gridtile_selected">  
	<size> (will add to default size if gridtile size is left unasigned)
	<color> 
</image>

// Gridtile's background
<ninepatch name="gridtile_background">
	<path> (changes gridtile's background)
</ninepatch>

// GameTitle in gridtile.
<text name="gridtile_text">
	<ALL>
</text>

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

