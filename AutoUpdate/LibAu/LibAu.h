// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBAU_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBAU_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LIBAU_EXPORTS
#define LIBAU_API __declspec(dllexport)
#else
#define LIBAU_API __declspec(dllimport)
#endif

// �����Ǵ� LibAu.dll ������
class LIBAU_API CLibAu {
public:
	CLibAu(void);
	// TODO:  �ڴ�������ķ�����
	void check(const char *softname,int ver ,const char *url);
};

extern LIBAU_API int nLibAu;

LIBAU_API int fnLibAu(void);
