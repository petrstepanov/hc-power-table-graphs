mkdir _vsbuild
cd _vsbuild
cmake -DPORTABLE_BUILD=TRUE ../
msbuild PowerGraphs.sln /p:Configuration=Release
msbuild INSTALL.vcxproj /p:Configuration=Release