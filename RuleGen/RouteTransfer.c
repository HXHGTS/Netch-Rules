// 引入头文件
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
using namespace std;

// 定义一个函数，将子网掩码转换成CIDR的位数
int mask2cidr(string mask) {
    // 将子网掩码按照点号分割成四个字节
    vector<string> mask_bytes;
    stringstream ss(mask);
    string byte;
    while (getline(ss, byte, '.')) {
        mask_bytes.push_back(byte);
    }
    // 初始化CIDR的位数为0
    int cidr = 0;
    // 遍历每个字节
    for (auto byte : mask_bytes) {
        // 将每个字节转换成二进制
        string binary = bitset<8>(stoi(byte)).to_string();
        // 统计每个字节中1的个数
        int ones = count(binary.begin(), binary.end(), '1');
        // 累加到CIDR的位数中
        cidr += ones;
    }
    // 返回CIDR的位数
    return cidr;
}

// 定义一个函数，将windows路由表转换成“ip地址/子网掩码”的格式
void convert(string output_file) {
    // 打开输出文件
    ofstream fout(output_file);
    // 判断文件是否打开成功
    if (!fout) {
        cout << "文件打开失败" << endl;
        return;
    }
    // 用_popen函数，执行route print命令，并获取其输出
    FILE* fp = _popen("route print -4 | find \".\" | find /v \"..\"", "r");
    if (fp == NULL) { // 如果执行失败，就返回
        cout << "命令执行失败" << endl;
        return;
    }
    // 读取命令输出的每一行
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // 将每一行按照空格分割成五个字段
        vector<string> fields;
        stringstream ss(line);
        string field;
        while (ss >> field) {
            fields.push_back(field);
        }
        // 判断是否是路由表的一条记录
        if (fields.size() == 5) {
            // 获取网络目标和子网掩码
            string destination = fields[0];
            string genmask = fields[1];
            // 调用函数，将子网掩码转换成CIDR的位数
            int cidr = mask2cidr(genmask);
            // 将“ip地址/子网掩码”的格式写入输出文件
            fout << destination << "/" << cidr << endl;
        }
    }
    // 关闭_popen函数和输出文件
    _pclose(fp);
    fout.close();
}

// 主函数
int main() {
    // 调用函数，将windows路由表转换成“ip地址/子网掩码”的格式，并输出到d:\route.txt文件中
    convert("d:\\route.txt");
    // 打印输出文件的内容
    system("type d:\\route.txt");
    // 暂停程序
    system("pause");
    // 返回0
    return 0;
}
