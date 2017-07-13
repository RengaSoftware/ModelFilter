------------------------------------------------------------
ModelFilter - The Renga plugin for filtering Renga objects by different conditions
------------------------------------------------------------
**[Download](https://github.com/RengaSoftware/ModelFilter/releases)**

About
-----

This plugin allows you to filter Renga model by different conditions. In this version you can hide or isolate filtered objects. 
For example you can filter all structural elements made of concrete or steel on some level or several levels. 
You also can filter objects by user properties, for example you can filter all rooms by category. 
Furthermore you can export and import filters from/to a file, share them with other users. 
Conditions in filters can be quite complex, however it is easy to create and understand them.

### What the filter consists of

Each filter consists of one or more groups. Each group works (can filter) a single object type.
A group contains search criteria for this object type, which are conditions for system object parameters 
(height, volume, surface area) or user defined properties. Search criteria work on the condition **AND**. 

**Here is an example of a group:** **Walls** _higher than 3 meters_ **AND** _with thickness equal to 200 cm_  **AND** _made of "Concrete"_.

**Here is an example of another group:** **Beams** _longer than 2 meters_ **AND** _located on the "Ground floor"_.

Groups in a filter work on the condition **OR**, so, combining groups described above in a single 
filter, after applying a filter you will get high and thick walls made of concrete and long beams on the "Groud floor".

### How to create a filter

Creation of a filter is quite simple. In main dialog you can add a new filter by clicking the "Add" button - the "Filter adding" will open.
Here you can manage groups using buttons "Add", "Edit", "Delete". In "Group adding" dialog you can set an object type and 
add a search criteria to the group. To add search criteria just select the property, the operator and the value for the criteria and push the "Add" button.


How to make plugin working
-----

1. Open Renga installation folder.
2. Open Plugins directory (create in case of missing).
3. Create directory ModelFilterPlugin and open it.
4. Copy plugin files to the just created directory. If you build plugin by your own - all required files located in project output directory.
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
