#include <string>
using namespace std;

class CBase64
{
public:
    CBase64(){}
    ~CBase64(){}

    /*����
    DataByte
        [in]��������ݳ���,���ֽ�Ϊ��λ
    */
    string Encode(const char* Data,int DataByte);
    /*����
    DataByte
        [in]��������ݳ���,���ֽ�Ϊ��λ
    OutByte
        [out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
        ������ݵĳ���
    */
    char * Decode(const char* Data,int DataByte,int& OutByte)  throw(std::string); 
};