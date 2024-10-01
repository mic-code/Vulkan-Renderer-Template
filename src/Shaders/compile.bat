@echo off
setlocal EnableExtensions EnableDelayedExpansion
set CompilerExe="%VULKAN_SDK%\Bin\glslangValidator.exe"
set OptimizerExe="%VULKAN_SDK%\Bin\spirv.exe"
set OptimizerConfig="OptimizerConfig.cfg"

for /r glsl/ %%I in (*.vert) do (
    set outname=%%I
    set outname=!outname:\glsl\=\spirv\!
    @echo compiling %%I
    @echo To !outname!
    
   %CompilerExe% -V "%%I" -l --target-env vulkan1.2 -o "!outname!".spv
)
for /r glsl/ %%I in (*.frag) do (
    set outname=%%I
    set outname=!outname:\glsl\=\spirv\!
    @echo compiling %%I
    @echo To !outname!
    
   %CompilerExe% -V "%%I" -l --target-env vulkan1.2 -o "!outname!".spv
)
for /r glsl/ %%I in (*.comp) do (
    set outname=%%I
    set outname=!outname:\glsl\=\spirv\!
    @echo compiling %%I
    @echo To !outname!
    
   %CompilerExe% -V "%%I" -l --target-env vulkan1.2 -o "!outname!".spv
)
pause
