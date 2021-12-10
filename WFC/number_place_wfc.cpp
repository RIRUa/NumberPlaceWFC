//
//  number_place_wfc.cpp
//  WFC
//

#include "number_place_wfc.hpp"


NumberPlaceWFC::NumberPlaceWFC(){
    this->init();
    this->mode = NumberPlace::outputMode::NoOutput;
}

NumberPlaceWFC::~NumberPlaceWFC(){
}

bool NumberPlaceWFC::init(){
    
    if (this->mode == NumberPlace::outputMode::ProcessToFile || this->mode == NumberPlace::outputMode::OnlyResultToFile) {
        std::ofstream writing_file;
        // 出力ファイルの初期化
        writing_file.open(this->file_name);
        if (writing_file.fail()) {
            return false;
        }
        writing_file << "var results = [" << std::endl;
        writing_file.close();
    }
    
    // 変数の初期化
    for (std::array<NumberPlace::square, NumberPlace::COLUMN> &row: this->num) {
        for (NumberPlace::square &square: row) {
            
            for (bool &elem: square.wave) {
                elem = true;
            }
            square.entropy = 0.0;
            square.isPropagated = false;
        }
    }
    
    return true;
}


void NumberPlaceWFC::process(){
    
    std::ofstream writing_file;
    
    //成功したか確認
    bool isClear = false;
    
    int cont = 0;
    // ループ用
    size_t r,c;
    // 最小の波動要素のマスの数字
    int num = 0;
    // 伝播していない要素を見つけたらtrue
    bool isFoundUnpropagated = false;
    
    bool isCalled = false;
    
    while (!isClear) {
        // 初期化
        if (!this->init()) {
            exit(323);
        }
        
        if (this->mode == NumberPlace::outputMode::ProcessToFile || this->mode == NumberPlace::outputMode::OnlyResultToFile) {
            writing_file.open(this->file_name, std::ios::app);
            if (writing_file.fail()) {
                return;
            }
        }
        // 一回の試行（アルゴリズムの４改良版）
        for (cont = 0; cont < 100; cont++) {
            this->calcEntoropy();
            std::tie(r,c,num) = this->observeOwnStatus();
            if (this->mode == NumberPlace::outputMode::ProcessToFile || this->mode == NumberPlace::outputMode::OnlyResultToFile) {
                this->printJsFile();
            }
            this->propagateOwnStatus(r, c, num);
            
            while (true){
                std::tie(r,c,num,isFoundUnpropagated) = this->findUnpropagatedElement();
                if (!isFoundUnpropagated) {
                    break;
                }
                isCalled = true;
                this->propagateOwnStatus(r, c, num);
            };
            
            if (this->mode == NumberPlace::outputMode::ProcessToFile || this->mode == NumberPlace::outputMode::OnlyResultToFile) {
                if (isCalled) {
                    this->printJsFile();
                    isCalled = false;
                }
            }
            
            
            if (this->verifyAllFalse()) {
                printf("失敗\n");
                break;
            }
            
            if (this->verifyOnlyOneTrue()) {
                printf("成功\n");
                isClear = true;
                break;
            }
            
        }
        if (this->mode == NumberPlace::outputMode::ProcessToFile || this->mode == NumberPlace::outputMode::OnlyResultToFile) {
            writing_file << "];" << std::endl;
            writing_file.close();
        }
    }
    
}

void NumberPlaceWFC::setOutputMode(NumberPlace::outputMode mode) {
    this->mode = mode;
}

void NumberPlaceWFC::printCUI() {
    
    size_t r,c,p;
    size_t posi = 0;
    int trueCount=0;
    
    std::cout << "[" << std::endl;
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        
        std::cout << "\t[";
        
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            trueCount = 0;
            
            for (p = 0; p<NumberPlace::POSSIBILITY; ++p) {
                // 文字の剪定
                if ( (this->num[r][c]).wave[p] ) {
                    posi = p;
                    trueCount++;
                }
            }
            
            if (trueCount == 1) {
                std::cout << (posi+1);
            }else{
                std::cout << "?";
            }
            
            if (c!=8) {
                std::cout << ",";
            }
        }
        
        std::cout << "]," << std::endl;
    }
    
    std::cout << "]" << std::endl;
}


void NumberPlaceWFC::printJsFile() {
    
    std::ofstream writing_file;
    writing_file.open(this->file_name, std::ios::app);
    
    if (writing_file.fail()) {
        return;
    }
    
    size_t r,c,p;
    size_t posi = 0;
    int trueCount=0;
    
    writing_file << "\t[" << std::endl;
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        
        writing_file << "\t\t[";
        
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            trueCount = 0;
            
            for (p = 0; p<NumberPlace::POSSIBILITY; ++p) {
                // 文字の剪定
                if ( (this->num[r][c]).wave[p] ) {
                    posi = p;
                    trueCount++;
                }
            }
            
            if (trueCount == 1) {
                writing_file << "\"" << (posi+1) << "\"";
            }else{
                writing_file << "\"?\"";
            }
            
            if (c!=8) {
                writing_file << ",";
            }
        }
        
        writing_file << "]," << std::endl;
    }
    
    writing_file << "\t]," << std::endl;
    
    writing_file.close();
}


std::tuple<size_t, size_t, int> NumberPlaceWFC::observeOwnStatus(){
    // エントロピーが最小の波動要素を探す
    
    int r,c,p;
    size_t rPosi = 0, cPosi = 0;
    // 最小のエントロピーを探して入れる。
    double ent = 1000.00;
    
    // 探し出した最小のエントロピーの箇所の数字を代入（必要であればランダム）
    size_t pos = 0;
    
    // ランダム関数用
    std::random_device rnd;
    std::uniform_int_distribution<> uni(0,8);
    
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            if ( (this->num[r][c]).entropy < ent && (this->num[r][c]).entropy > 0) {
                ent = (this->num[r][c]).entropy;
                rPosi = r;
                cPosi = c;
            }
            
        }
    }
    
    // 数字の決定
    do {
        
        pos = uni(rnd);

    } while ( !(this->num[rPosi][cPosi]).wave[pos] );
    
    // 確定状態へ畳み込み（配列の１つだけをtrueにする）
    for (p = 0; p < NumberPlace::POSSIBILITY; ++p) {
        if (p!=pos) {
            (this->num[rPosi][cPosi]).wave[p] = false;
        }
    }
    
    
    return std::forward_as_tuple(rPosi, cPosi, pos);
}

//　伝播
void NumberPlaceWFC::propagateOwnStatus(size_t row, size_t col, int num){
    
    int i;
    size_t r,c;
    
    // 自分が伝播させたことをフラグに登録
    (this->num[row][col]).isPropagated = true;
    
    // 縦向きに伝播
    for (i = 0; i < NumberPlace::ROW; ++i) {
        
        if (i != row) {
            (this->num[i][col]).wave[num] = false;
        }
    }
    
    // 横向きに伝播
    for (i = 0; i < NumberPlace::COLUMN; ++i) {
        if (i != col) {
            (this->num[row][i]).wave[num] = false;
        }
        
    }
    
    // 四角形の伝播
    const size_t bordor[3] = {3,6,9};
    /** 四角形の位置（左上から)
     横（x軸・・・配列の０の要素）
     縦（y軸・・・配列の１の要素）
     **/
    size_t boxPosi[2] = {100,100};
    
    if (0 <= row && row < bordor[0]) {
        boxPosi[0] = 0;
    }else if (bordor[0] <= row && row < bordor[1]){
        boxPosi[0] = 1;
    }else if (bordor[1] <= row && row < bordor[2]){
        boxPosi[0] = 2;
    }
    
    if (0 <= col && col < bordor[0]) {
        boxPosi[1] = 0;
    }else if (bordor[0] <= col && col < bordor[1]){
        boxPosi[1] = 1;
    }else if (bordor[1] <= col && col < bordor[2]){
        boxPosi[1] = 2;
    }
    
    for ( r = (bordor[ boxPosi[0] ] - 3) ; r < bordor[ boxPosi[0] ] ; ++r ) {
        for (c = (bordor[ boxPosi[1] ] - 3); c < bordor[ boxPosi[1] ]; ++c) {
            
            if (r != row && c !=col ) {
                (this->num[r][c]).wave[num] = false;
            }
            
        }
    }
    
}

// エントロピーを計算
void NumberPlaceWFC::calcEntoropy(){
    
    // 確率計算用
    int count;
    // エントロピーを入れるよう変数
    double ent;
    // ループを回すよう変数
    int r,c;
    // エントロピー用のノイズ
    double noise;
    
    // 乱数生成器
    std::random_device rnd;
    std::uniform_real_distribution<double> uni(0.f,1.f);
    
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            count = 0;
            ent = 0.0;
            
            for (bool partOfWave : (this->num[r][c]).wave) {
                if (partOfWave) {
                    count++;
                }
            }
            
            for (bool partOfWave : (this->num[r][c]).wave) {
                if (partOfWave) {
                    ent += ( 1/double(count)*log2( 1/double(count) ) );
                }
            }
            // 加算する数字はランダムにするよう書き換える。0~1
            
            noise = (ent==0.0) ? 0.f : uni(rnd);
            
            (this->num[r][c]).entropy = (-1.0*ent + noise);
            
        }
    }
    
}

bool NumberPlaceWFC::verifyAllFalse() {
    
    int falseCount = 0;
    
    size_t r,c;
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            falseCount = 0;
            
            for (bool checker : (this->num[r][c]).wave) {
                if (!checker) {
                    falseCount++;
                }
                
                if (falseCount == 9) {
                    return true;
                }
            }
            
        }
    }
    
    return false;
}

bool NumberPlaceWFC::verifyOnlyOneTrue() {
    
    int trueCount;
    
    size_t r,c;
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            trueCount = 0;
            
            for (bool checker : (this->num[r][c]).wave) {
                if (checker) {
                    trueCount++;
                }
            }
            
            if (trueCount != 1) {
                return false;
            }
            
        }
    }
    
    return true;
}

std::tuple<size_t, size_t, int, bool> NumberPlaceWFC::findUnpropagatedElement(){
    int pos = 0;
    int trueCount;
    
    size_t r,c,p;
    
    for (r = 0; r<NumberPlace::ROW; ++r) {
        for (c = 0; c<NumberPlace::COLUMN; ++c) {
            
            trueCount = 0;
            
            for (p = 0; p<NumberPlace::POSSIBILITY; ++p) {
                if ((this->num[r][c]).wave[p]) {
                    trueCount++;
                    pos = int(p);
                }
            }
            
            if (trueCount == 1 && !(this->num[r][c]).isPropagated ) {
                return std::forward_as_tuple(r, c, pos, true);
            }
            
        }
    }
    
    return std::forward_as_tuple(0, 0, 0, false);
}
