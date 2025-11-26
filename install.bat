mkdir _vsbuild
cd _vsbuild
cmake ../
msbuild RootTest.sln /p:Configuration=Release
msbuild INSTALL.vcxproj /p:Configuration=Release