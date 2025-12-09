#include "pch.h"
#include "TypeDB.h"

//CTypeDB 클래스의 멤버함수 "ReadCSVFile(CString filename)" 정의부
//주요 기능 : CSV 파싱을 통해 "월인천강지곡"을 메모리로 읽어오고 장수, 글자수 계산

BOOL CTypeDB::ReadCSVFile(CString filename)
{
    CStdioFile file;

    if (!file.Open(filename, CFile::modeRead | CFile::typeText))
        return FALSE;

    CString line;
    file.ReadString(line);   // 첫 줄은 무시

    m_Chars.RemoveAll();

    while (file.ReadString(line))
    {
        SCharInfo info;

        CString temp;
        int pos = 0;

        //DB_ViewerDlg.h의 SCharInfo 구조체에 선언한 변수 순서로
        //문자 (유니코드 문자열)
        //_wtoi를 사용해서 문자열을 정수로 변환해서 CSV의 문자열을 int형 멤버 변수에 저장
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_char = temp;
        line.Delete(0, pos + 1);

        //type
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_type = _wtoi(temp);
        line.Delete(0, pos + 1);

        // sheet
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_sheet = _wtoi(temp);
        line.Delete(0, pos + 1);

        //sx
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_sx = _wtoi(temp);
        line.Delete(0, pos + 1);

        //sy
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_sy = _wtoi(temp);
        line.Delete(0, pos + 1);

        //line
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_line = _wtoi(temp);
        line.Delete(0, pos + 1);

        //order
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_order = _wtoi(temp);
        line.Delete(0, pos + 1);

        //width
        pos = line.Find(L",");
        temp = line.Left(pos);
        info.m_width = _wtoi(temp);
        line.Delete(0, pos + 1);

        //height
        info.m_height = _wtoi(line);

        m_Chars.Add(info);
    }

    //전체 글자 수
    m_nChar = (int)m_Chars.GetSize();

    //전체 sheet 장 수 계산
    int maxSheet = 0;
    for (int i = 0; i < m_Chars.GetSize(); i++)
    {
        if (m_Chars[i].m_sheet > maxSheet)
            maxSheet = m_Chars[i].m_sheet;
    }
    m_nSheet = maxSheet;

    file.Close();
    return TRUE;
}