#include "simulator.h"
#include "argument_parser.h"

#include <string>
#include <iostream>

std::string getUsageMessage()
{
    return {
        "\n"
        "使用方法:\n"
        "    elevator <ファイル名> <ミリ秒単位の時間>\n"
        "\n"
        "ファイル名:\n"
        "    シミュレーションの引数の値を含む入力ファイルです。\n"
        "\n"
        "    1行目: エレベーターの数、階数、最大積載荷重\n"
        "    2行目: 乗客数\n"
        "    3行目以降のx行(xは乗客数): 乗客のIDと体重\n"
        "    以降の行: 乗客がエレベータを呼び出した時刻、乗客ID、乗車階、降車階\n"
        "\n"
        "    例:\n"
        "        1 30 500\n"
        "        2\n"
        "        0 64\n"
        "        1 75\n"
        "        0 0 0 10\n"
        "        6 1 0 8\n"
        "\n"
        "    この設定では、30階建てのビルのエレベーター1台をシミュレートします。最大積載荷重は500kgです。\n"
        "    乗客は2名です。\n"
        "        乗客0の体重は64kgです。\n"
        "        乗客1の体重は75kgです。\n"
        "    時刻0のときに乗客0はエレベーターを呼び、0階から10階に昇ろうとします。\n"
        "    時刻6のときに乗客1はエレベーターを呼び、0階から8階に昇ろうとします。\n"
        "\n"
        "ミリ秒単位の時間:\n"
        "    省略可能です。シミュレーションの各ステップ間の遅延をミリ秒単位で指定します。"
        "\n"};
}

int main(int argc, char **argv)
{
    try
    {
        ArgumentParser argParser{argc, argv, getUsageMessage()};
        const auto filename{argParser.get<std::string>()};
        const auto millisecondsDelay{argParser.getOptional<int>(0)};

        Simulator simulator{millisecondsDelay};
        simulator.run(filename);
    }
    catch (const std::runtime_error &exception)
    {
        std::cout << exception.what() << std::endl;
    }
}