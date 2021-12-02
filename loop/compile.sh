clang -emit-llvm -O3 $1 -S -o ${1%.*}.ll --target=aarch64-linux-gnu -march=armv8.2-a+sve -Rpass-analysis=loop
clang ${1%.*}.ll -S -o ${1%.*}.s --target=aarch64-linux-gnu -march=armv8.2-a+sve
