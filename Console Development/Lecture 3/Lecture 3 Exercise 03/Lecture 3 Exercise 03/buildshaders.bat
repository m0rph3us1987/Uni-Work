

REM Compile PPSL Vertex/Pixel/Compute Shaders to Binary Files
REM
REM orbis-psslc.exe
REM C:\Program Files (x86)\SCE\ORBIS SDKs\2.500\host_tools\bin
REM

orbis-psslc.exe "./Media/my_cs_set_uint_c.pssl" -o "./Media/my_cs_set_uint_c.sb"
orbis-psslc.exe "./Media/my_pix_clear_p.pssl" -o "./Media/my_pix_clear_p.sb"
orbis-psslc.exe "./Media/my_shader_p.pssl" -o "./Media/my_shader_p.sb"
orbis-psslc.exe "./Media/my_shader_vv.pssl" -o "./Media/my_shader_vv.sb"


