//
//  main.cpp
//  WFC
//

#include <chrono>

#include "number_place_wfc.hpp"

int main() {
    
    NumberPlaceWFC a;
    a.setOutputMode(NumberPlace::outputMode::ProcessToFile);

    std::chrono::system_clock::time_point  start, end; // 型は auto で可
    start = std::chrono::system_clock::now(); // 計測開始時間
    a.process();
    end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換

    std::cout << "所要時間（ミリ秒）：" << elapsed << std::endl;
    std::cout << "pythonでやった場合の所要時間予測値（ミリ秒）：" << 100*elapsed << "　〜　" << 500*elapsed << std::endl;

    a.printCUI();
    
    return 0;
}
