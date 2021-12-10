//
//  number_place_wfc.hpp
//  WFC
//

#ifndef number_place_wfc_hpp
#define number_place_wfc_hpp

#include <iostream>
#include <fstream>
#include <array>
#include <tuple>
#include <cmath>
#include <random>


namespace NumberPlace {
    const size_t ROW = 9;
    const size_t COLUMN = 9;
    const size_t POSSIBILITY = 9;

    // 一つのマス
    typedef struct __square{
        std::array<bool, NumberPlace::POSSIBILITY> wave;
        double entropy;
        bool isPropagated;
    }square;

    // 出力モード
    enum class outputMode {
        // 何も表示しない（デフォルト）
        NoOutput,
        // HTMLに結果のみ表示
        OnlyResultToFile,
        // HTMLに結果までの過程も表示
        ProcessToFile,
        // CUIに結果のみ表示
        OnlyResultToCUI,
        // CUIに結果までの過程も表示（非推奨）
        ProcessToCUI
    };
}


class NumberPlaceWFC {
    
    const std::string file_name = "html/result.js";
    
    // 可能性を入れる変数
    std::array< std::array< NumberPlace::square, NumberPlace::COLUMN >, NumberPlace::ROW > num;
    
    // 出力モード用変数
    NumberPlace::outputMode mode;
    
public:
    // コンストラクタ
    NumberPlaceWFC();
    // デストラクタ
    ~NumberPlaceWFC();
    
    // 繰り返し処理（アルゴリズムの４）
    void process();
    
    // 出力用関数（CUIに出力）
    void printCUI();
    
    // ファイル出力用モードセット関数
    void setOutputMode( NumberPlace::outputMode mode );
    
    // 出力用関数（ファイルに出力）
    void printJsFile();
    
private:
    // 初期化関数（アルゴリズムの３）
    bool init();
    
    //（アルゴリズムの４-１）
    /** 返り値は左からrow, column, number **/
    std::tuple<size_t, size_t, int> observeOwnStatus();
    
    //（アルゴリズムの４-２）
    void propagateOwnStatus(size_t row, size_t col, int num);
    
    // エントロピー計算
    void calcEntoropy();
    
    // 全てfalseの要素がないか確認（true:全てfalseがある,false:全てfalseがない）
    bool verifyAllFalse();
    
    // 全てのマスでwaveの配列の１つの要素を残して全てfalseになっているか
    bool verifyOnlyOneTrue();
    
    // 伝播で確定した要素を探す（boolの箇所は値が存在したらtrue）
    std::tuple<size_t, size_t, int, bool> findUnpropagatedElement();
};

#endif /* number_place_wfc_hpp */
