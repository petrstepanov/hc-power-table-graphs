mkdir _vsbuild
cd _vsbuild
cmake -DPORTABLE_INSTALL=TRUE ../
msbuild PowerGraphs.sln /p:Configuration=Release
msbuild INSTALL.vcxproj /p:Configuration=Release