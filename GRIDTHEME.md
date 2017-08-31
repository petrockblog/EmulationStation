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
<imagegrid name="gamegrid">
    <pos>               // Size and position for the whole grid. pos origin is 0,0.
    <size>
    <margin>            // The margin between grid tiles.
    <rowsAndColumns>    // sets the amount of rows and columns to fit into grid space.
    <pathMissingBoxArt> // Changes the missing boxart texture. (can be done per system)
</imagegrid>
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

// Current Game selected.
<text name="md_title">
	<ALL TEXT PROPERTIES>
</text>

<imagegrid name="gamegrid">
	<size>
	<pos>
	<margin> //sets size width and height of margin between tiles
    <rowsAndColumns> [COLUMN] [ROW] </rowsAndColumns>   // Set how many rows and columns a grid has
    <pathMissingBoxArt> [PATH] </pathMissingBoxArt>     // Replace the missing boxart texture
</imagegrid>
```

