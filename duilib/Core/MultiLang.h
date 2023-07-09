#ifndef UI_CORE_MULTILANG_H_
#define UI_CORE_MULTILANG_H_

#pragma once

#include "duilib/duilib_defs.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace ui 
{

/** �����Ե�֧��
*/
class UILIB_API MultiLang
{
public:
	MultiLang();
	~MultiLang();
	MultiLang(const MultiLang&) = delete;
	MultiLang& operator = (const MultiLang&) = delete;

public:
	/** �ӱ����ļ�������������ӳ���
	 * @param[in] strFilePath �����ļ�������·��
	 */
	bool LoadStringTable(const std::wstring& strFilePath);

	/** ���ڴ��м�����������ӳ���
	 * @param[in] hGlobal Ҫ���ص�����ӳ����ڴ��׵�ַ
	 */
	bool LoadStringTable(const std::vector<unsigned char>& file_data);

	/** ������������Դ
	*/
	void ClearStringTable();

public:
	/** ����ID��ȡָ�����Ե��ַ���
	 * @param[in] id ָ���ַ��� ID
	 * @return ���� ID ��Ӧ�������ַ���
	 */
	std::wstring GetStringViaID(const std::wstring& id);

private:
	/** ��������ӳ�������
	 * @param[in] list ��ȡ������ӳ��������б�
	 */
	bool AnalyzeStringTable(const std::vector<std::wstring>& list);

private:
	/** �ַ�����ID��ȡֵӳ���
	*/
	std::unordered_map<std::wstring, std::wstring> m_stringTable;
};

}
#endif //UI_CORE_MULTILANG_H_