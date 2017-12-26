GridView Theme
==============

Currently Gridview is still under extreme development and what is listed in here may
change or become broken later.  Please be sure to post any problems in this project's
issues.


Quick Overview Guide
====================
**Detailed List View MetaData is now available in gridview**

Display current selected game.
```xml
<text name="md_title"> <ALL>
```

Setup for the grid as a whole.
```xml
<text name="md_grid"> <pos> <size>             // Size and position for the whole grid.  pos origin is 0,0.
<container name="md_grid_margin"> <size>       // The margin between grid tiles.
<container name="gridRowsAndColumns"> <size>   // sets the amount of rows and columns to fit into grid space.
<image name="missing_boxart"> <path>           // Changes the missing boxart texture.  (can be done per system)
<image name="folder"> <path>                   // Changes the folder texture.  (can be done per system)
```

Gridtile modification
```xml
<image name="gridtile"> <size> <color>                  // Basic gridtile properties
<image name="gridtile_selected"> <size> <color>         // When selected
<text name="gridtile_text"> <ALL>
<container name="gridtile_textRow"> <size>              // Adjusts height of tile's text row.
<container name="gridtile_textRow_selected"> <size>	

<image name="gridtile_background"> <path> <color>
<image name="gridtile_background_selected"> <path> <color>
```


Full Use Guide
==============

Here is a quick list of all Added theme options GridView has and that do something

```xml
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
<image name="gridtile_background">
	<path> (changes gridtile's background)
	<color> (RGBA)
	<padding>
</imageh>
<image name="gridtile_background_selected">
	<color> (RGBA)
</image>

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

// Set how many rows and columns a grid has
<container name="gridRowsAndColumns">
	<size>column row</size>
</container>

// Replace the missing boxart texture
<image name="missing_boxart">
	<path>new_path</path>
</image>
```

