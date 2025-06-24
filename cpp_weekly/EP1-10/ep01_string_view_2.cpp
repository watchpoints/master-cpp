#include <iostream>
using namespace std;

// @brief: 路径解析
// @param: 输入一个文件完成路径  /mnt/icfs/dir/report.docx
// @ret:  返回每个目录的名字， 返回 mnt icfs dir report.docx
// @auth: +v watchpoint 2025-06-22
//@note  ceph mds 操作 27个元数据请 下面内容不看
// MDCache 元数据查找 MDCache::path_traverse 
//https://github.com/ceph/ceph/blob/main/src/mds/MDCache.cc
// Cdir->CDentry->Cinode
// MDCache的清理主要是删除一些优先级比较低的元数据，腾出一些内存空间，存放新的元数据，我们知道元数据的CDentry是使用LRU管理 
// https://krunerge.github.io/2021/03/11/MDS-metadata-op/
 //https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/comments.html#id7
//https://www.zhihu.com/question/371144076

vector<string> split_path(const string &path) 
{
  vector<string> result;
  int begin = 0; // begin 不能初始化为 -1，应该初始化为 0
  //当 begin = -1 时，find 实际上会把 -1 转换为 size_t 的一个很大的正数（比如 4294967295），
  //相当于从字符串末尾之后查找 ，结果永远找不到，直接返回 string::npos
  int end = 0;

  while(begin < path.size()) {
    int end =path.find('/',begin);
    cout << "pos is " << end << endl;
    if(end == string::npos) {
      cout << " not find /" << " begin ="<< begin << " end= "<< end << endl;
      if(begin < path.size()) {
        // 如果 begin + 1 小于 path.size()，说明还有剩余的部分
        result.push_back(path.substr(begin));
      }
      break;
    }
    if (begin > 0) {
      result.push_back(path.substr(begin,end-begin));
    }

    begin = end + 1;
  }
  
  return result; //copy
}


//反问 1：c++ 提供指针和引用方式 传递字符串避免拷贝，为什么设计 string_view
//回答 在 const 指针和 string_view 在零拷贝一样的。
//非拥有语义不管理内存, substr操作完全不一样
//0 拷贝 + 0 分配：所有 substr() 都只是构造新的 string_view，内部是 pointer + length，不分配内存

//反问 2： vector 能指针类型吗？ 需要自己管理内存，一不小心就会内存泄漏或悬空指针 
//std::vector<std::unique_ptr<T>>

//反问3: 查找 为什使用 find 函数，直接 for 循环遍历不可以吗？查找字符串怎么处理

//反问4：split_path 返回的 vecotr<string> 是拷贝吗？
//回答：是的，返回的是拷贝。vector<string> 是一个容器，当你返回它时，会发生拷贝构造，
//因此返回的 vector<string> 是原始 vector 的一个副本。 

void split_path_2(const string &path ,vector<string> &output)
{
    //data struct
    int begin = 0;
    int end = 0;
    int length = path.size();

    while (begin < length)
    {
      end = path.find('/', begin);
      if ( end == std::string::npos) {
        ///mnt/icfs/dir/report.docx"
        break;
      }
      if (begin > 0) {
        output.push_back(path.substr(begin,end-begin));
      }
      begin = end + 1;
    }
    //last dir
    if( begin < length) {  
      output.push_back(path.substr(begin));
      // substr( size_type pos = 0, size_type count = npos )
      //https://www.cppreference.com/w/cpp/string/basic_string_view/substr.html
      //View of the substring [pos, pos + rlen).
    }
}



void split_path_3(const string_view &path ,vector<string_view> &output)
{
    //data struct
    int begin = 0;
    int end = 0;
    int length = path.size();

    while (begin < length)
    {
      end = path.find('/', begin);
      if ( end == std::string::npos) {
        ///mnt/icfs/dir/report.docx"
        break;
      }
      if (begin > 0) {
        output.push_back(path.substr(begin,end-begin));
      }
      begin = end + 1;
    }
    //last dir
    if( begin < length) {  
      output.push_back(path.substr(begin));
      // substr( size_type pos = 0, size_type count = npos )
      //https://www.cppreference.com/w/cpp/string/basic_string_view/substr.html
      //View of the substring [pos, pos + rlen).
    }
}
int main()
{
    string_view path = "/mnt/icfs/dir/report.docx"; 
    vector<string_view> result;
    split_path_3(path, result);
    for (const auto &s : result) {
        cout << s << endl;
    }
    return 0;
}