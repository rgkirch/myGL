time nice -n 19 g++ -g -o app.exe main.cpp MyGL.cpp -I../include -std=c++14 -I/usr/include/libdrm -fopenmp -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/include/ImageMagick-6 -lMagick++-6.Q16HDRI -lMagickWand-6.Q16HDRI -lMagickCore-6.Q16HDRI -lboost_system -lboost_filesystem -lboost_program_options -pthread -fopenmp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW -lXinerama  -lXcursor
