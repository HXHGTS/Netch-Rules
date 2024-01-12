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
vector<string> convert(string input_file) {
    // 打开输入文件
    ifstream fin(input_file);
    // 判断文件是否打开成功
    if (!fin) {
        cout << "文件打开失败" << endl;
        return {};
    }
    // 定义一个字符串向量，存储转换后的结果
    vector<string> result;
    // 读取输入文件的每一行
    string line;
    while (getline(fin, line)) {
        // 将每一行按照空格分割成五个字段
        vector<string> fields;
        stringstream ss(line);
        string field;
        while (ss >> field) {
            fields.push_back(field);
        }
        // 获取网络目标和子网掩码
        string destination = fields[0];
        string genmask = fields[1];
        // 调用函数，将子网掩码转换成CIDR的位数
        int cidr = mask2cidr(genmask);
        // 将“ip地址/子网掩码”的格式添加到结果向量中
        result.push_back(destination + "/" + to_string(cidr));
    }
    // 关闭输入文件
    fin.close();
    // 返回结果向量
    return result;
}

// 定义一个函数，将“ip地址/子网掩码”的格式转换成json格式
void to_json(vector<string> ips, string output_file) {
    // 打开输出文件
    ofstream fout(output_file);
    // 判断文件是否打开成功
    if (!fout) {
        cout << "文件打开失败" << endl;
        return;
    }
    // 写入json格式的开始
    fout << "{\n";
    fout << "\t\"ip\": [\n";
    // 遍历结果向量
    for (int i = 0; i < ips.size(); i++) {
        // 写入每个ip地址，最后一个不加逗号
        fout << "\t\t\"" << ips[i] << "\"";
        if (i != ips.size() - 1) {
            fout << ",";
        }
        fout << "\n";
    }
    // 写入json格式的结束
    fout << "\t]\n";
    fout << "}\n";
    // 关闭输出文件
    fout.close();
}

// 主函数
int main() {
    // 调用函数，将windows路由表转换成“ip地址/子网掩码”的格式
    vector<string> ips = convert("d:\\route.txt");
    // 调用函数，将“ip地址/子网掩码”的格式转换成json格式
    to_json(ips, "d:\\route.json");
    // 打印输出文件的内容
    system("type d:\\route.json");
    // 暂停程序
    system("pause");
    // 返回0
    return 0;
}
