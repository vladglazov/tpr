dotnet publish -c Release -r ubuntu.19.10-x64
mv ./bin/Release/netcoreapp3.0/ubuntu.19.10-x64/publish/Simplex ./bin/Release/netcoreapp3.0/SimplexUbuntu
dotnet publish -c Release -r win-x64
mv ./bin/Release/netcoreapp3.0/win-x64/publish/Simplex.exe ./bin/Release/netcoreapp3.0/SimplexWin.exe
dotnet publish -c Release -r osx.10.10-x64
mv ./bin/Release/netcoreapp3.0/osx.10.10-x64/publish/Simplex ./bin/Release/netcoreapp3.0/SimplexOsx
