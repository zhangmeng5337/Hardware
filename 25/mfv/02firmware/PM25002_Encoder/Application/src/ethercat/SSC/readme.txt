How to generate SSC files from ETG software

1. Open .ESP file using the latest SSC tool
2. Update project per new SSC
3. Generate .c and .h files. New code should be available under Src folder
4. Comment out the function TSYNCMAN ESCMEM * GetSyncMan( UINT8 channel ) in ecat.slv.c
5. Build project

Loading SII File.

1. Run EEPROM programmer using SLAVE STACK CODE (SSC Tool)
2. Load .xml file from project.