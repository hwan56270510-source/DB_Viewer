#pragma once
#include <afx.h>
#include <afxtempl.h> 
struct SCharInfo
{
    CString m_char;   // 유니코드 형태의 12자리 CString 타입
    int m_type;
    int m_sheet;
    int m_sx;
    int m_sy;
    int m_line;
    int m_order;
    int m_width;
    int m_height;

    CArray<CString> m_compList; //구성글자 리스트
};

//CTypeDB 클래스 정의부
class CTypeDB
{
public:
    int m_nSheet;   // 전체 장 수
    int m_nChar;    // 전체 글자 수

    CArray<SCharInfo, SCharInfo&> m_Chars;

    BOOL ReadCSVFile(CString filename); // 객체 구성 성공 : TRUE 리턴// 실패 : FALSE
};
