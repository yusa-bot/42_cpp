#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include <iostream>

int main()
{
    std::cout << "\n\n===== Animal配列のテスト =====" << std::endl;
    const int arraySize = 4;
    Animal* animals[arraySize];

    std::cout << "\n--- 配列への格納（前半Dog, 後半Cat） ---" << std::endl;
    for (int k = 0; k < arraySize; k++) {
        if (k < arraySize / 2) {
            animals[k] = new Dog();
        } else {
            animals[k] = new Cat();
        }
    }

    std::cout << "\n--- 配列要素の削除（Animalポインタからの直接削除） ---" << std::endl;

    for (int k = 0; k < arraySize; k++) {
        // virtual ~Animal();のため、vptrを辿り実体(cat, dog)を消す -> Animalを消す
        // 利点: 継承元の型(Animal*)で統一して処理できる。
            // (「消し方」のルールはそれぞれのクラス（Dog/Cat）に封じ込められており、実行時にvptrがvtableのアドレスを正しく呼び出す)
        delete animals[k];
    }

    std::cout << "\n\n===== ディープコピーのテスト =====" << std::endl;
    // ブロックスコープ {} を利用してディープコピーを検証します。
    // シャローコピーの場合、スコープを抜ける際のデストラクタで同じBrainのメモリが
    // 2回解放されようとし（ダブルフリー）、プログラムがクラッシュします。

    std::cout << "\n--- Dogのコピーコンストラクタのテスト ---" << std::endl;
    Dog originalDog;
    {
        Dog copyDog = originalDog; // コピーコンストラクタ
    }
    std::cout << "-> ブロックスコープ終了。シャローコピーならここでクラッシュするか、次の削除でクラッシュします。" << std::endl;

    std::cout << "\n--- Catの代入演算子のテスト ---" << std::endl;
    Cat originalCat;
    {
        Cat assignCat;
        assignCat = originalCat; // コピー代入演算子
    }
    std::cout << "-> ブロックスコープ終了。クラッシュしなければディープコピー成功です。" << std::endl;

    std::cout << "\n--- main関数の終了 ---" << std::endl;

    // originalDog と originalCat のデストラクタが最後に呼ばれる
    return 0;
}
