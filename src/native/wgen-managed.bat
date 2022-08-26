cd ..
cd ..
rmdir /s /q "C:\Users\user\Documents\GitHub\MonoVMEmbed\out"
dotnet publish "C:\Users\user\Documents\GitHub\MonoVMEmbed\src\CsharpSample\CsharpSample.csproj" -r win-x86 -f net6.0 -p:TargetFramework=net6.0 --self-contained 
dotnet publish "C:\Users\user\Documents\GitHub\MonoVMEmbed\src\GetRuntimePack\GetRuntimePack.csproj" -r win-x86 -f net6.0 -p:TargetFramework=net6.0 --self-contained 
cd "C:\Users\user\Documents\GitHub\MonoVMEmbed\out"
mkdir "C:\Users\user\Documents\GitHub\MonoVMEmbed\out\native"
cd "C:\Users\user\Documents\GitHub\MonoVMEmbed\out\native"
mkdir "C:\Users\user\Documents\GitHub\MonoVMEmbed\out\native\generated"
runas sh "C:\Users\user\Documents\GitHub\MonoVMEmbed\src\native\gen-managed.sh" "C:\Users\user\Documents\GitHub\MonoVMEmbed\out\CsharpSample\bin\Debug\net6.0\win-x86\publish" 1> "C:\Users\user\Documents\GitHub\MonoVMEmbed\out\native\generated\tpa.h"   
 

         