%qt5_path%\bin\lupdate.exe ".\\" -ts modelfilterplugin_ru.ts
%qt5_path%\bin\lupdate.exe ".\\" -ts modelfilterplugin_en.ts

%qt5_path%\bin\lrelease.exe modelfilterplugin_ru.ts
%qt5_path%\bin\lrelease.exe modelfilterplugin_en.ts