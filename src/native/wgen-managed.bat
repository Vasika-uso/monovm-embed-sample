@echo on
cd ..
cd ..
rmdir /s /q "./out"
dotnet publish "./src/CsharpSample/CsharpSample.csproj" -r win-x86 -f net6.0 -p:TargetFramework=net6.0 --self-contained 
dotnet publish "./src/GetRuntimePack/GetRuntimePack.csproj" -r win-x86 -f net6.0 -p:TargetFramework=net6.0 --self-contained 
cd ./out
mkdir native
cd ./native
mkdir generated
cd ..
cd ..
sh "./src/native/gen-managed.sh" "./out/CsharpSample/bin/Debug/net6.0/win-x86/publish" 1> "./out/native/generated/tpa.h"  
:: This replaces /c/ and other buggy drive letters ( just add a letter :/ )
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/c/', 'C:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/d/', 'D:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/e/', 'E:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/f/', 'F:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/g/', 'G:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"
powershell -Command "(gc ./out/native/generated/tpa.h) -replace '/h/', 'H:/' | Out-File -encoding ASCII ./out/native/generated/tpa.h"

 
 
 
 

         