------------------------------------------------------------
ModelFilter - The Renga plugin for filtering Renga objects by different conditions
------------------------------------------------------------
**[Download](https://github.com/RengaSoftware/ModelFilter/releases)**

About
-----

This plugin allows you to filter Renga model by different conditions. 
The plugin functionality:
*   filtering objects by system object parameters (height, volume, surface area, etc.)
*   filtering objects by user properties.
*   exporting and importing filters from/into a file.

For example, using ModelFilter, you can filter all structural elements made of concrete or steel on some level or several levels or filter all rooms by category. You also can share filters with other users.

Using the filter you can hide, isolate or select objects using Filters dialog or context menu. Conditions in filters can be quite complex, however, it is easy to create and understand them.

### What the filter consists of

Each filter consists of one or more groups. Each group works (can filter) with a single object type.
A group contains search criteria for this object type, which are conditions for system object parameters 
or user defined properties. Search criteria work on the **AND** condition. 

**Group examples:** 
*   **Walls** _higher than 3 meters_ **AND** _with thickness equal to 200 cm_  **AND** _made of "Concrete"_.
*   **Beams** _longer than 2 meters_ **AND** _located on the "Ground floor"_.

Groups in a filter work on the **OR** condition, so combining groups described above in a single 
filter, after applying a filter, you will get high and thick walls made of concrete and long beams on the "Groud floor".

### How to create a filter

To add a new filter сlick the "Add" button, the "Filter adding" window will open. Here you can manage groups by using "Add", "Edit", "Delete" buttons.
In "Group adding" window you can set an object type and add a search criteria to the group.

To add the search criteria do following:
*   Select the property, the operator and the value for the criteria
*   Push the "Add" button

How to make plugin working
-----

1. Open Renga installation folder.
2. Open Plugins directory (create in case of missing).
3. Create directory ModelFilterPlugin and open it.
4. Copy plugin files to the just created directory. If you build plugin by your own all required files will be located in the project output directory.
5. Restart Renga.

How to build the plugin
-----

To build the plugin you need RengaSDK and Qt 5.8.1.
You can download the latest version of RengaSDK from "http://rengacad.com/ru/support/sdk/".
There are two environment variables (renga_sdk, qt5_path) used in plugin.
You should add these variables to the system, and specify paths to RengaSDK folder and Qt folder accordingly.
Alternatively you can set required paths in Visual Studio.

Other links
-----

- [Renga system](http://rengacad.com/)
- [ModelFilter issue tracker](https://github.com/RengaSoftware/ModelFilter/issues)


Feel free to use this plugin for work, as a basis for your own development or as an example.
