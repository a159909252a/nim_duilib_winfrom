#include "stdafx.h"
#include "MyGrid.h"

using namespace ui;

#define HEADER_COMBO_MARGIN_LEFT	4
#define HEADER_COMBO_MARGIN_BOTTOM	2

MyGridBody::MyGridBody(Grid *pGrid) :GridBody(pGrid){
	printf("MyGridBody::MyGridBody\n");
	m_pGrid->AttachScrollChange(nbase::Bind(&MyGridBody::OnScrollPosChanged, this, std::placeholders::_1));
}

MyGridBody::~MyGridBody(){
	m_pGrid->DetachEvent(kEventScrollChange);
}

void MyGridBody::SetColCount(int count)
{
	__super::SetColCount(count);
	GridHeader *header = GetHeader();
	int header_height = GetHeaderHeight();
	int margin_left = GetColumnWidth(0);
	for (size_t i = 1; i < header->size(); i++)
	{
		GridHeaderItem *pItem = dynamic_cast<GridHeaderItem*>(header->at(static_cast<int>(i)));
		ASSERT(pItem);
		if (!pItem)
			continue;
		int col_width = GetColumnWidth(static_cast<int>(i));
		Combo* combo = new Combo;
		combo->SetFixedWidth(ui::UiFixedInt(col_width - HEADER_COMBO_MARGIN_LEFT * 2), true, true);
		combo->SetFixedHeight(ui::UiFixedInt(header_height / 2 - HEADER_COMBO_MARGIN_BOTTOM), true);
		combo->SetMargin(ui::UiMargin(margin_left + HEADER_COMBO_MARGIN_LEFT, header_height / 2, 0, 0), true);
		combo->SetClass(L"combo2");
		combo->SetAttribute(L"cursortype", L"hand");
		combo->SetFloat(true);
		pItem->control_ = combo;
		if (AddItem(combo) == false)
		{
			assert(0);
		}
		margin_left += col_width;

		for (size_t i = 0; i < 3; i++)
		{
			ListBoxElement *combo_item = new ListBoxElement;
			combo_item->SetClass(L"listitem");
			combo_item->SetUTF8Text("item0");
			combo_item->SetFixedHeight(ui::UiFixedInt(24), true);
			combo->AddItem(combo_item);
		}
	}

}

GridHeaderItem* MyGridBody::AddCol(std::wstring text, int width)
{
	int col_index = static_cast<int>(GetHeader()->size());
	int margin_left = _SumIntList(m_hLayout);
	GridHeaderItem *item = __super::AddCol(text, width);
	if (item)
	{
		if (col_index > 0)
		{
			int col_width = GetColumnWidth(col_index);
			Combo* combo = new Combo;
			combo->SetFixedWidth(ui::UiFixedInt(col_width - HEADER_COMBO_MARGIN_LEFT * 2), true, true);
			combo->SetFixedHeight(ui::UiFixedInt(GetHeaderHeight() / 2 - HEADER_COMBO_MARGIN_BOTTOM), true);
			combo->SetMargin(ui::UiMargin(margin_left + HEADER_COMBO_MARGIN_LEFT, GetHeaderHeight() / 2, 0, 0), true);
			combo->SetClass(L"combo2");
			combo->SetAttribute(L"cursortype", L"hand");
			combo->SetFloat(true);
			item->control_ = combo;
			if (AddItem(combo) == false)
			{
				assert(0);
			}


			for (size_t i = 0; i < 3; i++)
			{
				ListBoxElement *combo_item = new ListBoxElement;
				combo_item->SetClass(L"listitem");
				combo_item->SetUTF8Text("item0");
				combo_item->SetFixedHeight(ui::UiFixedInt(24), true);
				combo->AddItem(combo_item);
			}
		}
	}

	

	return item;
}

void MyGridBody::OnColumnWidthChanged(int col_index, int width)
{
	ResetHeanderComboPos();
}

void MyGridBody::OnColumnCountChanged(int col_index, bool bRemove)
{
	ResetHeanderComboPos();
}

bool MyGridBody::OnScrollPosChanged(const ui::EventArgs& args)
{
	if (args.wParam > 0)	//y方向
	{
		ResetHeanderComboPos();
	}

	return true;
}

void MyGridBody::ResetHeanderComboPos()
{
	GridHeader* pHeader = GetHeader();
	if (pHeader)
	{
		UiSize64 szOff64 = m_pGrid->GetScrollPos();
		UiSize szOff(TruncateToInt32(szOff64.cx), TruncateToInt32(szOff64.cy));
		int posx = 0;
		for (size_t i = 0; i < pHeader->size(); i++)
		{
			if (i == 0){
				posx += m_hLayout[i];
				continue;
			}
			Control *pControl = pHeader->at(static_cast<int>(i))->GetControl();
			if (!pControl){
				posx += m_hLayout[i];
				continue;
			}
				
			UiMargin rcMargin = pControl->GetMargin();
			rcMargin.left = posx + HEADER_COMBO_MARGIN_LEFT;
			rcMargin.top = GetHeaderHeight() / 2 + szOff.cy;
			pControl->SetMargin(rcMargin, true);
			pControl->SetFixedWidth(ui::UiFixedInt(m_hLayout[i] - HEADER_COMBO_MARGIN_LEFT * 2), true, true);

			posx += m_hLayout[i];
		}
	}
}

void MyGridBody::PaintBody(IRender* pRender)
{
	UiSize64 szOff64 = m_pGrid->GetScrollPos();
	UiSize szOff(TruncateToInt32(szOff64.cx), TruncateToInt32(szOff64.cy));
	int posx = 0;
	int posy = 0;
	int row_count = GetRowCount();
	int col_count = GetColCount();
	int fixed_col_width = GetFixedColWidth();
	int fixed_row_height = GetFixedRowHeight();
	int grid_width = m_pGrid->GetWidth();
	int grid_height = m_pGrid->GetHeight();
	UiColor dwDefColor = GlobalManager::Instance().Color().GetColor(L"textdefaultcolor");

	//draw fixed col && fixed row bkcolor
	if (fixed_row_height > 0)
	{
		UiRect rcPaint = GetPos();
		rcPaint.bottom = rcPaint.top + fixed_row_height;
		rcPaint.right = rcPaint.left + (GetFixedWidth().GetInt32() - szOff.cx > grid_width ? grid_width : GetFixedWidth().GetInt32() - szOff.cx);
		pRender->FillRect(rcPaint, GlobalManager::Instance().Color().GetColor(m_strFixedBkColor), 255);
	}
	if (fixed_col_width > 0)
	{
		UiRect rcPaint = GetPos();
		rcPaint.right = rcPaint.left + fixed_col_width;
		rcPaint.bottom = rcPaint.top + (GetFixedHeight().GetInt32() - szOff.cy > grid_height ? grid_height : GetFixedHeight().GetInt32() - szOff.cy);
		pRender->FillRect(rcPaint, GlobalManager::Instance().Color().GetColor(m_strFixedBkColor), 255);
	}

	//draw fixed col && fixed row text
	for (int i = 0; i < (int)m_nFixedRow; i++)
	{
		if (m_vLayout[i] == 0)
			continue;
		posx = 0;
		GridRow *grid_row = m_vecRow[i];
		for (size_t j = 0; j < m_nFixedCol; j++)
		{
			if (m_hLayout[j] == 0)
				continue;
			UiRect rc = { posx, posy, posx + m_hLayout[j], posy + m_vLayout[i] };
			rc.Offset({ GetRect().left, GetRect().top });
			pRender->DrawString(rc, grid_row->at(static_cast<int>(j))->text, dwDefColor, m_strGridFont, m_uTextStyle);
			posx += m_hLayout[j];
		}
		posy += m_vLayout[i];
	}

	//draw fixed row text
		{
			UiRect rcClip = m_pGrid->GetPos();
			rcClip.left += GetFixedColWidth();
			rcClip.bottom = rcClip.top + GetFixedRowHeight();
			AutoClip clip(pRender, rcClip, IsClip());
			posy = 0;
			for (int i = 0; i < (int)m_nFixedRow; i++)
			{
				if (m_vLayout[i] == 0)
					continue;
				GridRow *grid_row = m_vecRow[i];
				posx = GetFixedColWidth();
				for (size_t j = m_nFixedCol; j < (size_t)col_count; j++)
				{
					if (m_hLayout[j] == 0)
						continue;
					std::wstring str = grid_row->at(static_cast<int>(j))->text;
					if (!str.empty() && posx + m_hLayout[j] - szOff.cx > fixed_col_width)		//单元格右边线没有超过fixed_col_width
					{
						UiRect rc = { posx, posy, posx + m_hLayout[j], posy + m_vLayout[i] };
						if (i == 0)
						{
							rc.bottom = posy + m_vLayout[i] / 2;
						}
						rc.Offset({ GetRect().left - szOff.cx, GetRect().top });
						pRender->DrawString(rc, str, dwDefColor, m_strGridFont, m_uTextStyle);
					}
					posx += m_hLayout[j];
					if (posx - szOff.cx > grid_width)	//超出grid宽度
						break;
				}
				posy += m_vLayout[i];
				if (posy - szOff.cy > grid_height)		//超出grid高度
					break;
			}
		}

		//draw fixed col text
		{
			UiRect rcClip = m_pGrid->GetPos();
			rcClip.top += GetFixedRowHeight();
			rcClip.right = rcClip.left + GetFixedColWidth();
			AutoClip clip(pRender, rcClip, IsClip());
			posx = 0;
			for (int i = 0; i < (int)m_nFixedCol; i++)
			{
				if (m_hLayout[i] == 0)
					continue;
				posy = GetFixedRowHeight();
				for (size_t j = m_nFixedRow; j < (size_t)row_count; j++)
				{
					if (m_vLayout[j] == 0)
						continue;
					GridRow *grid_row = m_vecRow[j];
					std::wstring str = grid_row->at(i)->text;
					if (!str.empty() && posy + m_vLayout[j] - szOff.cy > fixed_row_height)		//单元格下边线没有超过fixed_row_height
					{
						UiRect rc = { posx, posy, posx + m_hLayout[i], posy + m_vLayout[j] };
						rc.Offset({ GetRect().left, GetRect().top - szOff.cy });
						pRender->DrawString(rc, str, dwDefColor, m_strGridFont, m_uTextStyle);
					}
					posy += m_vLayout[j];
					if (posy - szOff.cy > grid_height)	//超出grid高度
						break;
				}
				posx += m_hLayout[i];
				if (posx - szOff.cx > grid_width)		//超出grid宽度
					break;
			}
		}

		//draw other GridItem
		{
			UiRect rcClip = m_pGrid->GetPos();
			rcClip.left += GetFixedColWidth();
			rcClip.top += GetFixedRowHeight();;
			AutoClip clip(pRender, rcClip, IsClip());
			posy = GetFixedRowHeight();
			for (int i = static_cast<int>(m_nFixedRow); i < row_count; i++)
			{
				if (m_vLayout[i] == 0)
					continue;
				if (posy + m_vLayout[i] - szOff.cy > fixed_row_height)		//单元格下边线没有超过fixed_row_height
				{
					GridRow *grid_row = m_vecRow[i];
					posx = GetFixedColWidth();

					for (size_t j = m_nFixedCol; j < (size_t)col_count; j++)
					{
						if (m_hLayout[j] == 0)
							continue;
						GridItem *pItem = grid_row->at(static_cast<int>(j));

						UiRect rc = { posx, posy, posx + m_hLayout[j], posy + m_vLayout[i] };
						rc.Offset({ GetRect().left - szOff.cx, GetRect().top - szOff.cy });
						rc.Deflate(1, 1, 2, 2);
						//绘制单元格背景色
						if (pItem->IsSelected())
						{
							pRender->FillRect(rc, GlobalManager::Instance().Color().GetColor(m_strSelForeColor), 255);
						}
						else if (!pItem->bk_color.empty())
						{
							pRender->FillRect(rc, GlobalManager::Instance().Color().GetColor(pItem->bk_color), 255);
						}

						//绘制text
						std::wstring str = pItem->text;
						if (!str.empty() && posx + m_hLayout[j] - szOff.cx > fixed_col_width)		//单元格右边线没有超过fixed_col_width
						{
							if (pItem->text_color.empty() && pItem->text_style == 0)
								pRender->DrawString(rc, str, dwDefColor, m_strGridFont, m_uTextStyle);
							else{
								UiColor dwColor = (pItem->text_color.empty() ? dwDefColor : 
									GlobalManager::Instance().Color().GetColor(pItem->text_color));
								UINT text_style = (pItem->text_style == 0 ? m_uTextStyle : pItem->text_style);
								pRender->DrawString(rc, str, dwColor, m_strGridFont, text_style);
							}
						}
						posx += m_hLayout[j];
						if (posx - szOff.cx > grid_width)		//超出grid宽度
							break;
					}
				}
				posy += m_vLayout[i];
				if (posy - szOff.cy > grid_height)			//超出grid高度
					break;
			}
		}
}


//----------------------------------------MyGrid------------------------------------------
MyGrid::MyGrid(){

}

MyGrid::~MyGrid(){

}

