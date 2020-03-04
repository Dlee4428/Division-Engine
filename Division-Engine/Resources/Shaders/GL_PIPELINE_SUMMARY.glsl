//////////////////////////////////////////////////////////////////
// SUMMARY LINE FOR TERRAIN TESSELLATION SHADER
//////////////////////////////////////////////////////////////////
// 1. The VS is executed on every vertex in a patch. 
//    The patch comprises several CPs from the vertex buffer 
//    (up to a limit defined by the driver and GPU).

// 2. The TCS takes the vertices that have been processed by the VS and generates 
//    an output patch. In addition, it generates TLs.

// 3. Based on the configured domain, the TLs it got from the TCS and the configured 
//    output topology, the PG generates domain location and their connectivity.

// 4. The TES is executed on all generated domain locations.

// 5. The primitives that were generated in step 3 continue down the pipe. 
//    The output from the TES is their data.

// 6. Processing continues either at the GS or at the rasterizer.